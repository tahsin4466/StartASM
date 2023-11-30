#include "include/Compiler.h"
#include "include/Lexer.h"
#include "include/Parser.h"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <omp.h>
#include <sstream>
#include <map>
#include <regex>
#include <utility>

using namespace std;

Compiler::Compiler(std::string pathname):
    m_pathname(pathname),
    m_statusMessage(""),
    m_lineIndex(0),
    //m_parser(new Parser()),
    m_lexer(new Lexer()),
    m_parser(new Parser()) {

};

Compiler::~Compiler() {
    //delete m_parser;
    delete m_lexer;
    delete m_parser;
}

bool Compiler::compileCode() {
    if(!loadFile()) {
        return false;
    }
    lexCode();
    if(!parseCode() || !resolveSymbols()) {
        return false;
    }
    buildAST();
    if(!analyzeSemantics()) {
        return false;
    }
    generateCode();
    return true;

    
}

bool Compiler::loadFile() {
    //Set line index to 0
    m_lineIndex = 0;

    //Attempt to open codeFile
    string currentLine;
    ifstream codeFile(m_pathname);

    if (codeFile.is_open())
    {
        //While line exists
        while (getline(codeFile,currentLine)) {
            //Push back current lines (including empty)
            //This is to keep accurate track of line numners
            m_codeLines.push_back(currentLine);
        }
        codeFile.close();
 
        return true;
    }
    else {
        //Return error if path not found
        m_statusMessage = "Invalid path!";
        return false;
    }
        
}

void Compiler::lexCode() {
    //Map to temporarily sort each line through parallelization
    map<long unsigned int, vector<pair<string, LexerConstants::TokenType>>> lineMap;
    //Temporary private variable to store result of lexer when parallelized
    vector<pair<string, LexerConstants::TokenType>> tempTokens;

    //Call the lexer to tokenize and label each token
    //Leverage OpenMP for lexing as the lexer involves minimal recursion and is thread-safe
    #pragma omp parallel for private(tempTokens)
    for (long unsigned int i = 0; i < m_codeLines.size(); i++) {
        tempTokens = m_lexer->tokenizeLine(m_codeLines[i]);
        #pragma omp critical 
        {
            lineMap[i] = tempTokens;
        }
    }

    for (const auto& pair : lineMap) {
        // pair.first is the int key, pair.second is the vector<string>
        m_codeTokens.push_back(pair.second);
    }
}

bool Compiler::parseCode() {
    //The parser relies on top-down recursive descent parsing
    //Implementing parallelization is a NIGHTMARE, and very easily involves race-conditions and stack overflows
    //Hence, parsing will remain sequential

    //Create error string
    string error;
    for (int i=0; i<m_codeTokens.size(); i++) {
        //Call validateInstruction in InstructionSet
        error = m_parser->checkInstruction(i, m_codeTokens[i]);
        //If an error is present
        if (error != "") {
            m_statusMessage += "\nInvalid syntax at line " + to_string(i + 1) + ": " + m_codeLines[i] + "\n" + error + "\n";
        }
        error.clear();
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
    //Variables to hold finalized invalid lines and paralellized lines map for each thread
    map<int, string> invalidLinesMap;
    string invalidLines;
    //Get the root of the parse tree
    PTNode* parseTreeRoot = m_parser->getParseTree()->getRoot();
    int parseTreeSize = parseTreeRoot->getNumChildren();

    //Look for label declerations in parse tree and add to the label table
    //Iterate over all children in the loop leveraging OMP
    #pragma omp parallel for
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

    //Reloop and resolve all found labels
    //Parallelize outer loop for greater efficiency
    #pragma omp parallel for
    for (int i=0; i<parseTreeSize; i++) {
        //Get the node pointer for the line and size (frequent access)
        PTNode* lineNode = parseTreeRoot->childAt(i);
        int lineSize = lineNode->getNumChildren();
        for(int j=0; j<lineSize; j++) {
            //If the child is an operand node (L2 nodes are atomic with only one child)
            if (lineNode->childAt(j)->childAt(0)->getNodeType() == PTConstants::NodeType::OPERAND) {
                //Cast to operandNode to check type
                OperandNode* labelNode = dynamic_cast<OperandNode*>(lineNode->childAt(j)->childAt(0));
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
    /*//Use OMP to parallelize line reading to build the AST
    //AST will store nodes in ordered state
    #pragma omp parallel for
    for(long unsigned int i=0; i<m_codeLines.size(); i++) {
        //Declare all necessary info to build each AST node
        string instructionKeyword;
        NumOperands instructionOperands;
        SemanticType instructionSemantic;
        vector<pair<string, OperandType>> operands;

        //Grab instruction keyword (first token)
        instructionKeyword = m_codeTokens[i][0];
        //Grab operand and semantic info from instructionSet (stored as pair)
        instructionOperands = m_instructionSet->returnInstructionInfo(instructionKeyword).first;
        instructionSemantic = m_instructionSet->returnInstructionInfo(instructionKeyword).second;

        //Loop through rest of tokens to search for operands
        //Do NOT use OMP as operand order must be preserved
        for (long unsigned int j=1; j<m_codeTokens[i].size(); j++) {
            //Check token against all regex templates for operands in InstructionSet
            for(int k=0; k<m_instructionSet->getNumOperands(); k++) {
                regex operandTemplate(m_instructionSet->returnOperandInfo(k).first);
                if (regex_match(m_codeTokens[i][j], operandTemplate)) {
                    //Push back operand vector with a copy of operand and its info
                    operands.push_back(make_pair(m_codeTokens[i][j], m_instructionSet->returnOperandInfo(k).second));
                    //Terminate the loop (saves a few cycles)
                    k = m_instructionSet->getNumOperands();
                }
            }
        }

        //Critical OMP section
        //Map insertion is NOT thread safe, each thread must modify AST one at a time
        #pragma omp critical
        {
            //Insert nullary, unary, binary or ternary node into AST depending on the number of operands specified by
            //the instruction
            switch(instructionOperands) {
                //Switch statement calls overloaded constructors for nodes in AST class
                case 0:
                    m_AST->createInstructionNode(i, instructionKeyword);
                    break;
                case 1:
                    m_AST->createInstructionNode(i, instructionKeyword, operands[0].first, operands[0].second, instructionSemantic);
                    break;
                case 2:
                    m_AST->createInstructionNode(i, instructionKeyword, operands[0].first, operands[0].second, operands[1].first, operands[1].second, instructionSemantic);
                    break;
                case 3:
                    m_AST->createInstructionNode(i,instructionKeyword, operands[0].first, operands[0].second, operands[1].first, operands[1].second, operands[2].first, operands[2].second, instructionSemantic);
                    break;
                default:
                    break;
            }
        }

    } */
}

bool Compiler::analyzeSemantics() {
    return true;
}

void Compiler::generateCode() {
}