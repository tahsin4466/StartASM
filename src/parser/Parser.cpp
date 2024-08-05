#include "parser/Parser.h"

#include <functional>
#include <utility>

using namespace std;
using namespace PTConstants;
using namespace PT;

//Constructor and helpers - Initialize all data structure values and parsing templates
Parser::Parser() {
    // Initialize the instructionMap with expected lengths of each instruction
    m_instructionMap.reserve(26);
    m_instructionMap["move"] = 4;
    m_instructionMap["load"] = 4;
    m_instructionMap["store"] = 4;
    m_instructionMap["create"] = 5;
    m_instructionMap["cast"] = 3;
    m_instructionMap["add"] = 6;
    m_instructionMap["sub"] = 6;
    m_instructionMap["multiply"] = 6;
    m_instructionMap["divide"] = 6;
    m_instructionMap["or"] = 4;
    m_instructionMap["and"] = 4;
    m_instructionMap["not"] = 2;
    m_instructionMap["shift"] = 6;
    m_instructionMap["compare"] = 4;
    m_instructionMap["jump"] = 5;
    m_instructionMap["call"] = 3;
    m_instructionMap["push"] = 2;
    m_instructionMap["pop"] = 3;
    m_instructionMap["return"] = 1;
    m_instructionMap["stop"] = 1;
    m_instructionMap["input"] = 4;
    m_instructionMap["output"] = 2;
    m_instructionMap["print"] = 2;
    m_instructionMap["label"] = 2;
    m_instructionMap["comment"] = 2;

    // Preallocate size for the template map
        m_templateMap.reserve(26);

    // Move instruction template
        m_templateMap["move"].reserve(2);
        m_templateMap["move"].push_back({{"from", 0}, checkImplicitConjunction});
        m_templateMap["move"].push_back({{"to", 2}, checkExplicitConjunction});

    // Load instruction template
        m_templateMap["load"].reserve(2);
        m_templateMap["load"].push_back({{"from", 0}, checkImplicitConjunction});
        m_templateMap["load"].push_back({{"to", 2}, checkExplicitConjunction});

    // Store instruction template
        m_templateMap["store"].reserve(2);
        m_templateMap["store"].push_back({{"from", 0}, checkImplicitConjunction});
        m_templateMap["store"].push_back({{"to", 2}, checkExplicitConjunction});

    // Create instruction template
        m_templateMap["create"].reserve(3);
        m_templateMap["create"].push_back({{"type", 0}, checkImplicitCondition});
        m_templateMap["create"].push_back({{"from", 1}, checkImplicitConjunction});
        m_templateMap["create"].push_back({{"to", 3}, checkExplicitConjunction});

    // Cast instruction template
        m_templateMap["cast"].reserve(2);
        m_templateMap["cast"].push_back({{"type", 0}, checkImplicitCondition});
        m_templateMap["cast"].push_back({{"self", 1}, checkImplicitConjunction});

    // Add instruction template
        m_templateMap["add"].reserve(3);
        m_templateMap["add"].push_back({{"from", 0}, checkImplicitConjunction});
        m_templateMap["add"].push_back({{"with", 2}, checkExplicitConjunction});
        m_templateMap["add"].push_back({{"to", 4}, checkExplicitConjunction});

    // Sub instruction template
        m_templateMap["sub"].reserve(3);
        m_templateMap["sub"].push_back({{"from", 0}, checkImplicitConjunction});
        m_templateMap["sub"].push_back({{"with", 2}, checkExplicitConjunction});
        m_templateMap["sub"].push_back({{"to", 4}, checkExplicitConjunction});

    // Multiply instruction template
        m_templateMap["multiply"].reserve(3);
        m_templateMap["multiply"].push_back({{"from", 0}, checkImplicitConjunction});
        m_templateMap["multiply"].push_back({{"with", 2}, checkExplicitConjunction});
        m_templateMap["multiply"].push_back({{"to", 4}, checkExplicitConjunction});

    // Divide instruction template
        m_templateMap["divide"].reserve(3);
        m_templateMap["divide"].push_back({{"from", 0}, checkImplicitConjunction});
        m_templateMap["divide"].push_back({{"with", 2}, checkExplicitConjunction});
        m_templateMap["divide"].push_back({{"to", 4}, checkExplicitConjunction});

    // Or instruction template
        m_templateMap["or"].reserve(2);
        m_templateMap["or"].push_back({{"self", 0}, checkImplicitConjunction});
        m_templateMap["or"].push_back({{"with", 2}, checkExplicitConjunction});

    // And instruction template
        m_templateMap["and"].reserve(2);
        m_templateMap["and"].push_back({{"self", 0}, checkImplicitConjunction});
        m_templateMap["and"].push_back({{"with", 2}, checkExplicitConjunction});

    // Not instruction template
        m_templateMap["not"].reserve(1);
        m_templateMap["not"].push_back({{"self", 0}, checkImplicitConjunction});

    // Shift instruction template
        m_templateMap["shift"].reserve(3);
        m_templateMap["shift"].push_back({{"direction", 0}, checkImplicitCondition});
        m_templateMap["shift"].push_back({{"self", 1}, checkImplicitConjunction});
        m_templateMap["shift"].push_back({{"by", 3}, checkExplicitConjunction});

    // Compare instruction template
        m_templateMap["compare"].reserve(2);
        m_templateMap["compare"].push_back({{"self", 0}, checkImplicitConjunction});
        m_templateMap["compare"].push_back({{"with", 2}, checkExplicitConjunction});

    // Jump instruction
        m_templateMap["jump"].reserve(2);
        m_templateMap["jump"].push_back({{"if", 1}, checkExplicitCondition});
        m_templateMap["jump"].push_back({{"to", 3}, checkExplicitConjunction});

    // Call instruction
        m_templateMap["call"].reserve(1);
        m_templateMap["call"].push_back({{"to", 1}, checkExplicitConjunction});

    // Push instruction
        m_templateMap["push"].reserve(1);
        m_templateMap["push"].push_back({{"from", 0}, checkImplicitConjunction});

    // Pop instruction
        m_templateMap["pop"].reserve(1);
        m_templateMap["pop"].push_back({{"to", 1}, checkExplicitConjunction});

    // Return instruction
        m_templateMap["return"].reserve(0);  // No arguments for return

    // Stop instruction
        m_templateMap["stop"].reserve(0);  // No arguments for stop

    // Input instruction
        m_templateMap["input"].reserve(2);
        m_templateMap["input"].push_back({{"type", 0}, checkImplicitCondition});
        m_templateMap["input"].push_back({{"to", 2}, checkExplicitConjunction});

    // Output instruction template
        m_templateMap["output"].reserve(1);
        m_templateMap["output"].push_back({{"from", 0}, checkImplicitConjunction});

    // Print instruction
        m_templateMap["print"].reserve(1);
        m_templateMap["print"].push_back({{"from", 0}, checkImplicitConjunction});

    // Comment instruction
        m_templateMap["comment"].reserve(1);
        m_templateMap["comment"].push_back({{"static", 0}, checkImplicitConjunction});

    // Label instruction
        m_templateMap["label"].reserve(1);
        m_templateMap["label"].push_back({{"static", 0}, checkImplicitConjunction});
}

bool Parser::parseCode(PT::ParseTree* parseTree, const std::vector<std::string>& codeLines, const std::vector<std::vector<std::pair<std::string, LexerConstants::TokenType>>>& tokens, std::string& errorMessage) {
    //The parser relies on top-down recursive descent parsing
    //Preallocate L1 based on codeLines
    int numTokens = tokens.size();
    parseTree->getRoot()->reserveChildren(numTokens);
    for (int i=0; i<numTokens; i++) {
        //Call validateInstruction in InstructionSet
        string error = checkInstruction(parseTree, tokens[i]);
        //If an error is present
        if (!error.empty()) {
            errorMessage += "\nInvalid syntax at line " + to_string(i + 1) + ": " + codeLines[i] + "\n" + error + "\n";
        }
    }
    //Concatenate the statusMessage string from the map (which should be ordered already)
    if (errorMessage.empty()) {
        return true;
    }
    else {
        return false;
    }
}

//LEVEL 1 - INSTRUCTION PARSER AND CHECKER
string Parser::checkInstruction(ParseTree* parseTree, vector<pair<string, LexerConstants::TokenType>> tokens) {
    //Zero case, return instantly with valid syntax and no AST construction
    if (tokens[0].second == LexerConstants::TokenType::BLANK) {
        parseTree->getRoot()->insertChild((new GeneralNode(0, "", BLANK)));
        return "";
    }
    //If keyword doesn't match, return error no instruction found
    if (tokens[0].second != LexerConstants::TokenType::INSTRUCTION) {
        return "Unknown instruction '" + tokens[0].first + "'";
    }
    auto itr = m_templateMap.find(tokens[0].first);
    //If found, go to parse instruction method creating a new instruction node
    if (itr!= m_templateMap.end()) {
        return parseInstruction(parseTree, (parseTree->getRoot()->insertChild((new GeneralNode(0, tokens[0].first, INSTRUCTION)))), tokens, itr->second);
    }
    else {
        //Edge case, valid instruction with no method implemented (debug)
        return "Compiler error for '" + tokens[0].first + "'. Could not find instruction parsing method.";
    }
}

string Parser::parseInstruction(ParseTree* parseTree, PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens, std::vector<std::pair<std::pair<std::string, int>, std::function<std::string(ParseTree*, PTNode*, std::vector<std::pair<std::string, LexerConstants::TokenType>>, std::string&, int)>>> parsingTemplate) {
    //Temporary return string
    string returnString;
    //Loop through all templates
    //NOTE - if the instruction is a no operand (i.e. empty parsingTemplate) loop will not run and will go straight to final check
    for (auto& templateElement : parsingTemplate) {
        //Access the parsing function, passing the index expected in the token sequence
        returnString = templateElement.second(parseTree, node, tokens, templateElement.first.first, templateElement.first.second);
        //If an error arises, return instantly
        if (!returnString.empty()) {
            return returnString;
        }
        //Clear return string on every iteration
        returnString.clear();
    }

    //Final check - syntax correct but there's excess tokens present
    //Find size of template expected through instructionMap lookup
    auto itr = m_instructionMap.find(tokens[0].first);
    if(itr!=m_instructionMap.end()) {
        //If tokens exceed expected size
        if (tokens.size() > itr->second) {
            return "Excess tokens at and past '" + tokens[tokens.size()].first + "' found.";
        }
        else {
            //Correct syntax
            return "";
        }
    }
    else {
        //Compiler error - edge case with valid instruction not in hash table
        return "Compiler error for '" + tokens[0].first + "'. Could not find instruction information.";
    }

}



//LEVEL 2 - CONJUNCTION AND CONDITION CHECKERS / PARSER HELPERS
string Parser::checkImplicitConjunction(ParseTree* parseTree, PTNode* node, vector<pair<string, LexerConstants::TokenType>> tokens, string& keyword, int index) {
    string returnString;
    //Implicit node is implicit, so always exists
    //Add keyword as child
    //Rewrite returnString if L2 analysis returns an error
    returnString = parseConjunction(parseTree, node->insertChild((new GeneralNode(Constants::IMPLICIT_INDEX, keyword, CONJUNCTION))), tokens, keyword, index);
    return returnString;
}

string Parser::checkImplicitCondition(ParseTree* parseTree, PTNode* node, vector<pair<string, LexerConstants::TokenType>> tokens, string& keyword, int index) {
    string returnString;
    //Implicit node is implicit, so always exists
    //Add keyword as child
    //Rewrite returnString if L2 analysis returns an error
    returnString = parseCondition(parseTree, node->insertChild((new GeneralNode(Constants::IMPLICIT_INDEX, keyword, CONJUNCTION))), tokens, keyword, index);
    return returnString;
}

string Parser::checkExplicitConjunction(ParseTree* parseTree, PTNode* node, vector<pair<string, LexerConstants::TokenType>> tokens, string& keyword, int index) {
    string returnString;
    //Check if a conjunction exists by comparing size
    if (tokens.size()<=index) {
        return "Missing conjunction. Expected '" + keyword + "'";
    }
    //Check if to keyword is valid
    else if (tokens[index].first != keyword) {
        return "Unknown conjunction '" + tokens[index].first + "'. Expected '" + keyword + "'";
    }
    //If passed, add to keyword as child
    //Rewrite returnString if L2 analysis returns an error
    else {
        returnString = parseConjunction(parseTree, node->insertChild((new GeneralNode(index, keyword, CONJUNCTION))), tokens, keyword, index);
        return returnString;
    }
}

string Parser::checkExplicitCondition(ParseTree* parseTree, PTNode* node, vector<pair<string, LexerConstants::TokenType>> tokens, string& keyword, int index) {
    string returnString;
    //Check if a condition exists by comparing size
    if (tokens.size()<=index) {
        return "Missing condition. Expected '" + keyword + "'";
    }
    //Check if to keyword is valid
    else if (tokens[index].first != keyword) {
        return "Unknown condition '" + tokens[index].first + "'. Expected '" + keyword + "'";
    }
    //If passed, add to keyword as child
    //Rewrite returnString if L2 analysis returns an error
    else {
        returnString = parseCondition(parseTree, node->insertChild((new GeneralNode(index, keyword, CONJUNCTION))), tokens, keyword, index);
        return returnString;
    }
}

string Parser::parseConjunction(ParseTree* parseTree, PTNode* node, vector<pair<string, LexerConstants::TokenType>> tokens, string& keyword, int index) {
    //Create temporary return string
    string returnString;
    //Increment index by one to now point to where the operand should be
    index++;
    //If the operand does not exist after the keyword, return an error
    if(tokens.size()<=index) {
        return "Missing operand after '" + tokens[index-1].first + "'";
    }
    //If the token in the operand position is not an operand, return an error
    else if (!isOperand(tokens[index])) {
        return "Unknown operand '" + tokens[index].first + "' after '" + tokens[index-1].first + "'";
    }
    else {
        //Insert a new child as the operand
        node->insertChild((new OperandNode(index, tokens[index].first, returnPTOperand(tokens[index].second))));
        return "";
    }
}

string Parser::parseCondition(ParseTree* parseTree, PTNode* node, vector<pair<string, LexerConstants::TokenType>> tokens, string& keyword, int index) {
    //Create temporary return string
    string returnString;
    //Iterate the index to now point to where the condition should be
    index++;
    //If the descriptor after keyword does not exist
    if(tokens.size()<=index) {
        return "Missing descriptor after '" + tokens[index-1].first + "'";
    }
    //If the token after keyword does not match as a descriptor
    else if (!isDescriptor(tokens[index])) {
        return "Unknown descriptor '" + tokens[index].first + "' after '" + tokens[index-1].first + "'";
    }
    else {
        //Insert a new child as the operand
        node->insertChild((new OperandNode(index, tokens[index].first, returnPTOperand(tokens[index].second))));
        return "";
    }
}



//LEVEL 3 - OPERANDS AND DESCRIPTORS
bool Parser::isOperand(pair<string, LexerConstants::TokenType>& token) {
    //Switch statement to determine if a lexer constant constitutes an operand in the PT
    switch (token.second) {
        case LexerConstants::TokenType::REGISTER:
        case LexerConstants::TokenType::INSTRUCTIONADDRESS:
        case LexerConstants::TokenType::MEMORYADDRESS:
        case LexerConstants::TokenType::FLOAT:
        case LexerConstants::TokenType::INTEGER:
        case LexerConstants::TokenType::BOOLEAN:
        case LexerConstants::TokenType::CHARACTER:
        case LexerConstants::TokenType::LABEL:
        case LexerConstants::TokenType::STRING:
        case LexerConstants::TokenType::NEWLINE:
            return true;
        default:
            return false;
    }
}

bool Parser::isDescriptor(pair<string, LexerConstants::TokenType>& token) {
    //Check if token is a condition
    //Switch statement
    switch (token.second) {
        case LexerConstants::TokenType::JUMPCONDITION:
        case LexerConstants::TokenType::SHIFTCONDITION:
        case LexerConstants::TokenType::TYPECONDITION:
            return true;
        default:
            return false;
    }
}



//Helper function converting constants
PTConstants::OperandType Parser::returnPTOperand(LexerConstants::TokenType tokenType) {
    //Switch statement to switch namespaces of enum constants (from lexer namespace to PT namespace)
    switch (tokenType) {
        case LexerConstants::TokenType::REGISTER:
            return PTConstants::OperandType::REGISTER;
        case LexerConstants::TokenType::INSTRUCTIONADDRESS:
            return PTConstants::OperandType::INSTRUCTIONADDRESS;
        case LexerConstants::TokenType::MEMORYADDRESS:
            return PTConstants::OperandType::MEMORYADDRESS;
        case LexerConstants::TokenType::FLOAT:
            return PTConstants::OperandType::FLOAT;
        case LexerConstants::TokenType::INTEGER:
            return PTConstants::OperandType::INTEGER;
        case LexerConstants::TokenType::BOOLEAN:
            return PTConstants::OperandType::BOOLEAN;
        case LexerConstants::TokenType::CHARACTER:
            return PTConstants::OperandType::CHARACTER;
        case LexerConstants::TokenType::LABEL:
            return PTConstants::OperandType::LABEL;
        case LexerConstants::TokenType::STRING:
            return PTConstants::OperandType::STRING;
        case LexerConstants::TokenType::NEWLINE:
            return PTConstants::OperandType::NEWLINE;
        case LexerConstants::TokenType::JUMPCONDITION:
            return PTConstants::OperandType::JUMPCONDITION;
        case LexerConstants::TokenType::TYPECONDITION:
            return PTConstants::OperandType::TYPECONDITION;
        case LexerConstants::TokenType::SHIFTCONDITION:
            return PTConstants::OperandType::SHIFTCONDITION;
        default:
            return PTConstants::OperandType::UNKNOWN;
    }
}

