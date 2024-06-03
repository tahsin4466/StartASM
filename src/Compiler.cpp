#include "include/Compiler.h"
#include "include/Lexer.h"
#include "include/Parser.h"
#include "include/AbstractSyntaxTree.h"
#include "include/SemanticAnalyzer.h"
#include "include/CodeGenerator.h"

#include <iostream>
#include <vector>
#include <string>
#include <omp.h>
#include <mutex>
#include <future>
#include <map>
#include <regex>
#include <utility>

using namespace std;

Compiler::Compiler(std::string& pathname, bool cmdSilent, bool cmdTimings, bool cmdTree, bool cmdIr) :
    cmd_silent(cmdSilent),
    cmd_timings(cmdTimings),
    cmd_tree(cmdTree),
    cmd_ir(cmdIr),
    m_statusMessage(),
    m_lexer(new Lexer()),
    m_parser(new Parser()),
    m_AST(new AST::AbstractSyntaxTree()),
    m_semanticAnalyzer(new SemanticAnalyzer()),
    m_codeGenerator(new CodeGenerator()),
    m_pathname(pathname) {}

Compiler::~Compiler() {
    //delete m_parser;
    delete m_lexer;
    delete m_parser;
    delete m_AST;
    delete m_semanticAnalyzer;
    delete m_codeGenerator;
}

void Compiler::cmdPrint(const std::string& message) const {
    if (!cmd_silent) {
        cout << message;
    }
}

void Compiler::cmdTimingPrint(const std::string& message) const {
    if (!cmd_silent && cmd_timings) {
        cout << message;
    }
}

bool Compiler::compileCode() {
    double start = omp_get_wtime();
    //Lex code
    cmdTimingPrint("Compiler: Lexing code\n");
    if (!m_lexer->lexFile(m_pathname, m_codeLines, m_codeTokens)) {
        m_statusMessage = "Lexing failed! Either the path was invalid or the file could not be found.";
        return false;
    }
    cmdTimingPrint("Time taken: " + to_string(omp_get_wtime()-start) + "\n\n");

    //Parse code
    cmdTimingPrint("Compiler: Parsing code\n");
    start = omp_get_wtime();
    if(!parseCode()) {
        return false;
    }
    cmdTimingPrint("Time taken: " + to_string(omp_get_wtime()-start) + "\n\n");

    //Resolve symbols
    cmdTimingPrint("Compiler: Resolving symbols\n");
    start = omp_get_wtime();
    if(!resolveSymbols()) {
        return false;
    }
    cmdTimingPrint("Time taken: " + to_string(omp_get_wtime()-start) + "\n\n");

    //Delete the lexer and build the AST concurrently
    cmdTimingPrint("Compiler: Building AST\n");
    start = omp_get_wtime();
    //Delete the lexer after PT creation is finished! It's no longer needed
    auto lexerDeletionFuture = std::async(std::launch::async, [this] {
        delete m_lexer;
        m_lexer = nullptr;
    });
    buildAST();
    lexerDeletionFuture.get();
    cmdTimingPrint("Time taken: " + to_string(omp_get_wtime()-start) + "\n\n");
    if(cmd_tree && !cmd_silent) {
        cout << endl;
        cout << "AST for '" + m_pathname + "':\n";
        m_AST->printTree();
        cout << endl;
    }

    //Check address scopes and analyze semantics while deleting the parse tree concurrently
    cmdTimingPrint("Compiler: Analyzing semantics and checking address scopes\n");
    start = omp_get_wtime();
    auto parserDeletionFuture = std::async(std::launch::async, [this] {
        delete m_parser;
        m_parser = nullptr;
    });
    auto checkAddressScopesFuture = std::async(&Compiler::checkAddressScopes, this);
    auto analyzeSemanticsFuture = std::async(&Compiler::analyzeSemantics, this);
    // Wait for all tasks to complete and retrieve function results
    bool checkAddressScopesResult = checkAddressScopesFuture.get();
    bool analyzeSemanticsResult = analyzeSemanticsFuture.get();
    parserDeletionFuture.get();
    if(!checkAddressScopesResult || !analyzeSemanticsResult) {
        return false;
    }
    cmdTimingPrint("Time taken: " + to_string(omp_get_wtime()-start) + "\n\n");

    //Generate code
    cmdTimingPrint("Compiler: Generating LLVM IR\n");
    start = omp_get_wtime();
    generateCode();
    cmdTimingPrint("Time taken: " + to_string(omp_get_wtime()-start) + "\n\n");
    if(cmd_ir && !cmd_silent) {
        cout << endl;
        cout << "LLVM IR for '" + m_pathname + "':\n";
        m_codeGenerator->printIR();
        cout << endl;
    }
    return true;
}

bool Compiler::parseCode() {
    //The parser relies on top-down recursive descent parsing
    //Implementing parallelization is a NIGHTMARE, and very easily involves race-conditions and stack overflows
    //Hence, parsing will remain sequential

    for (int i=0; i<m_codeTokens.size(); i++) {
        //Call validateInstruction in InstructionSet
        string error = m_parser->checkInstruction(i, m_codeTokens[i]);
        //If an error is present
        if (!error.empty()) {
            m_statusMessage += "\nInvalid syntax at line " + to_string(i + 1) + ": " + m_codeLines[i] + "\n" + error + "\n";
        }
    }

    //Concatenate the statusMessage string from the map (which should be ordered already)
    if (m_statusMessage.empty()) {
        return true; 
    }
    else {
        return false;
    }
}

bool Compiler::resolveSymbols() {
    //Variables to hold finalized invalid lines and parallelized lines map for each thread
    map<int, string> invalidLinesMap;
    string invalidLines;
    //Get the root of the parse tree
    PT::PTNode* parseTreeRoot = m_parser->getParseTree()->getRoot();
    int parseTreeSize = parseTreeRoot->getNumChildren();

    //Look for label declarations in parse tree and add to the label table
    //Iterate over all children in the loop leveraging OMP
    #pragma omp parallel for schedule(dynamic) default(none) shared(parseTreeRoot, parseTreeSize, invalidLinesMap)
    for (int i=0; i<parseTreeSize; i++) {
        //Iterate over every child in the root node
        if(parseTreeRoot->childAt(i)->getNodeValue() == "label") {
            string labelValue = parseTreeRoot->childAt(i)->childAt(0)->childAt(0)->getNodeValue();
            //Check if label is already declared in the symbol table
            auto itr = m_symbolTable.find(labelValue);
            if (itr != m_symbolTable.end()) {
                //Critical section as modifying STL container
                #pragma omp critical 
                {
                invalidLinesMap[i] = "\nLabel error at line " + to_string(i+1) + ": " +  m_codeLines[i] + "\nDuplicate label " + labelValue + " already declared at line " + to_string(itr->second.second+1) + "\n";
                }
            }
            //Critical section as modifying STL container
            #pragma omp critical
            {
            //Include the label (child of instruction child) and corresponding address (root node index + 1)
            m_symbolTable.emplace(labelValue, make_pair("i[" + to_string(i+1) +  "]", i));
            }
        }
    }

    //Re loop and resolve all found labels
    //Parallelize outer loop for greater efficiency
    #pragma omp parallel for schedule(dynamic) default(none) shared(parseTreeRoot, parseTreeSize, invalidLinesMap, invalidLines)
    for (int i=0; i<parseTreeSize; i++) {
        //Get the node pointer for the line and size (frequent access)
        PT::PTNode* lineNode = parseTreeRoot->childAt(i);
        int lineSize = lineNode->getNumChildren();
        for(int j=0; j<lineSize; j++) {
            //If the child is an operand node (L2 nodes are atomic with only one child)
            if (lineNode->childAt(j)->childAt(0)->getNodeType() == PTConstants::NodeType::OPERAND) {
                //Cast to operandNode to check type
                auto labelNode = dynamic_cast<PT::OperandNode*>(lineNode->childAt(j)->childAt(0));
                //Check if successful cast and type is a label
                if (labelNode != nullptr && labelNode->getOperandType() == PTConstants::OperandType::LABEL) {
                    //Decision logic - check if a part of symbolTable
                    auto itr = m_symbolTable.find(labelNode->getNodeValue());
                    if (itr == m_symbolTable.end()) {
                        //Throw an undefined error if not found in symbol table
                        //Modifying section - use critical
                        #pragma omp critical
                        {
                        invalidLinesMap[i] = "\nLabel error at line " + to_string(i+1) + ": " +  m_codeLines[i] + "\nUndefined label " + labelNode->getNodeValue() + "\n";
                        }
                    }
                    else {
                        //Change operand value and operand type to instruction address
                        //Tree is NOT thread safe writing - critical section
                        #pragma omp critical 
                        {
                        labelNode->setNodeValue(itr->second.first);
                        labelNode->setOperandType(PTConstants::OperandType::INSTRUCTIONADDRESS);
                        }
                    }
                }
            }
        }
    }

    //Concatenate invalidLines string from all errors accumulated out of order
    for (const auto& pair : invalidLinesMap) {
        invalidLines += pair.second;
    }

    //Return false if invalidLines not empty (errors encountered)
    if (!invalidLines.empty()) {
        m_statusMessage = invalidLines;
        return false;
    }
    else {
        return true;
    }
}

void Compiler::buildAST() {
    // Get the root node of the parse tree and its size (frequent access)
    PT::PTNode* PTRoot = m_parser->getParseTree()->getRoot();
    int PTSize = PTRoot->getNumChildren();
    // Get the AST root node
    AST::ASTNode* ASTRoot = m_AST->getRoot();

    // Vector to store AST instruction nodes
    std::vector<AST::InstructionNode*> instructionNodes(PTSize);

    // Iterate over all children (instructions) in the parse tree
    // Parallelize the creation of instruction nodes and their children
    #pragma omp parallel for schedule(dynamic) default(none) shared(PTRoot, PTSize, instructionNodes)
    for (int i = 0; i < PTSize; i++) {
        // Get the pointer to the instruction node from the PT
        PT::PTNode* PTInstructionNode = PTRoot->childAt(i);

        // Initialize a new AST instruction node, using built-in conversion methods found in the AST class
        auto ASTInstructionNode = new AST::InstructionNode(
            PTInstructionNode->getNodeValue(),
            m_AST->getInstructionType(PTInstructionNode->getNodeValue()),
            m_AST->getNumOperands(PTInstructionNode->getNumChildren()),
            i + 1
        );

        // Store the instruction node in the vector
        instructionNodes[i] = ASTInstructionNode;

        // Add all operands from the PT for the AST
        for (int j = 0; j < PTInstructionNode->getNumChildren(); j++) {
            // Get the operand node from the PT and cast to an OperandNode (parser guarantees this)
            auto PTOperandNode = dynamic_cast<PT::OperandNode*>(PTInstructionNode->childAt(j)->childAt(0));

            // Do a check anyway to make sure dynamic cast was successful
            if (PTOperandNode != nullptr) {
                // Add a child for the instruction node in the AST, using conversion functions from the AST as necessary
                ASTInstructionNode->insertChild(new AST::OperandNode(PTOperandNode->getNodeValue(), m_AST->convertOperandType(PTOperandNode->getOperandType())));
            }
        }
    }

    // Insert instruction nodes into the AST root node (sequential part to ensure thread safety)
    for (int i = 0; i < PTSize; i++) {
        ASTRoot->insertChild(instructionNodes[i]);
    }
}

bool Compiler::analyzeSemantics() {
    //Create a map for parallelized error messages and the ASTRoot (reduce getter access)
    map<int, string> errorMap;
    AST::ASTNode* ASTRoot = m_AST->getRoot();

    //Parallelize semantic analysis as each instruction is independent and AST is immutable in this state
    #pragma omp parallel for schedule(dynamic) default(none) shared(ASTRoot, errorMap)
    //Iterate over all children of the AST (instruction nodes)
    for (int i=0; i<ASTRoot->getNumChildren(); i++) {
        //Cast the ASTNode to an instruction node
        auto instructionNode = dynamic_cast<AST::InstructionNode*>(ASTRoot->childAt(i));
        //If not nullptr and if the node isn't empty
        if (instructionNode != nullptr && !instructionNode->getNodeValue().empty()) {
            //Call the semantic analyzer and analyze the given node
            string error = m_semanticAnalyzer->analyzeSemantics(instructionNode);
            if (!error.empty()) {
                //If error is present
                //Critical section - STL manipulations are not thread safe
                #pragma omp critical
                {
                //Add error to error map at index i
                errorMap[i] = "\nInvalid syntax at line " + to_string(i + 1) + ": " + m_codeLines[i] + "\n" + error;
                }
            }
        }
    }

    //Concatenate status message string with all error messages
    for (const auto& pair : errorMap) {
        m_statusMessage += pair.second;
    }

    //Return true if no errors, false otherwise
    if (m_statusMessage.empty()) {
        return true;
    }
    else {
        return false;
    }
}

bool Compiler::checkAddressScopes() {
    // Declare temporary variables for error reporting and finding instruction address index
    std::string errorString;
    std::mutex errorMutex; // Mutex to protect errorString
    std::string instructionIndex;
    const std::regex registerTemplate = std::regex("r[0-9]");
    const std::regex instructionTemplate = std::regex("i\\[[0-9]{1,9}\\]");
    const std::regex memoryTemplate = std::regex("m<[0-9]{1,9}>");

    // Get AST root node
    AST::ASTNode* ASTRoot = m_AST->getRoot();
    int numInstructions = ASTRoot->getNumChildren();

    // Loop through all child (instruction) nodes in AST
    // Use OpenMP to parallelize the loop
    #pragma omp parallel for schedule(dynamic) default(none) shared(ASTRoot, errorString, errorMutex, numInstructions, registerTemplate, instructionTemplate, memoryTemplate)
    for (int i = 0; i < numInstructions; i++) {
        AST::ASTNode* InstructionNode = ASTRoot->childAt(i);

        // Local variable for collecting errors in the current iteration
        std::string localErrorString;
        std::string localInstructionIndex;

        // Loop through all operands in instruction parent
        for (int j = 0; j < InstructionNode->getNumChildren(); j++) {
            // Get child node and cast dynamically to operand node
            auto operandNode = dynamic_cast<AST::OperandNode*>(InstructionNode->childAt(j));
            if (operandNode != nullptr) {
                // Switch checking based on operand type
                ASTConstants::OperandType operandType = operandNode->getOperandType();
                switch (operandType) {
                    case ASTConstants::REGISTER:
                        // Run a regex template (this one with explicit bounds) to determine if the operand is in scope
                        if (!std::regex_match(operandNode->getNodeValue(), registerTemplate)) {
                            localErrorString += "\nScope error at line " + std::to_string(i + 1) + ": " + m_codeLines[i] + "\n" + "Register '" + operandNode->getNodeValue() + "' is out of range. Max register is r9\n";
                        }
                        break;
                    case ASTConstants::MEMORYADDRESS:
                        if (!std::regex_match(operandNode->getNodeValue(), memoryTemplate)) {
                            localErrorString += "\nScope error at line " + std::to_string(i + 1) + ": " + m_codeLines[i] + "\n" + "Memory address '" + operandNode->getNodeValue() + "' is out of range. Max address is m<999999999>\n";
                        }
                        break;
                    case ASTConstants::INSTRUCTIONADDRESS:
                        // Instruction address both has to adhere to StartASM bounds (4 byte address) and the number of instructions themselves
                        // First get the actual instruction index
                        for (int k = 2; k < operandNode->getNodeValue().size() - 1; k++) {
                            localInstructionIndex += operandNode->getNodeValue()[k];
                        }
                        // If the given instruction index is greater than the number of lines
                        if ((std::stoi(localInstructionIndex) > m_codeLines.size())) {
                            localErrorString += "\nScope error at line " + std::to_string(i + 1) + ": " + m_codeLines[i] + "\n" + "Instruction address '" + operandNode->getNodeValue() + "' is out of range. Expected i[0]-i[" + std::to_string(m_codeLines.size()) + "]\n";
                        }
                        // If the instruction index is larger than the StartASM limit
                        else if (!std::regex_match(operandNode->getNodeValue(), instructionTemplate)) {
                            localErrorString += "\nScope error at line " + std::to_string(i + 1) + ": " + m_codeLines[i] + "\n" + "Instruction address '" + operandNode->getNodeValue() + "' is out of range. Max address is i[999999999]\n";
                        }
                        localInstructionIndex.clear();
                        break;
                    // Base case - break
                    default:
                        break;
                }
            }
        }

        // Lock the mutex before modifying the shared errorString
        if (!localErrorString.empty()) {
            std::lock_guard<std::mutex> lock(errorMutex);
            errorString += localErrorString;
        }
    }

    // Return false if there are errors present in the string
    if (!errorString.empty()) {
        m_statusMessage += errorString;
        return false;
    }
    else {
        return true;
    }
}

void Compiler::generateCode() {
    AST::ASTNode* ASTRoot = m_AST->getRoot();
    int numInstructions = ASTRoot->getNumChildren();

    //#pragma omp parallel for schedule(dynamic) default(none) shared(ASTRoot, numInstructions)
    for (int i = 0; i < numInstructions; i++) {
        // Cast the ASTNode to an instruction node
        auto instructionNode = dynamic_cast<AST::InstructionNode*>(ASTRoot->childAt(i));
        // If not nullptr and if the node isn't empty
        if (instructionNode != nullptr && !instructionNode->getNodeValue().empty()) {
            // Call the code generator for the given line
            m_codeGenerator->generateCode(instructionNode);
        }
    }
}