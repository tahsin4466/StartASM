#include "include/Parser.h"

#include <iostream>
#include <regex>
#include <string>
#include <functional>
#include <utility>

using namespace std;
using namespace PTConstants;

const int FIRST_OPERAND_POSITION = 1;

//Constructor - Initialize all data structure values
Parser::Parser() { 
    //Initialise the root pointer for the parse tree
    m_parseTree = new PT();

    m_instructionMap["move"].push_back(make_pair("from", checkImplicit));
    m_instructionMap["move"].push_back(make_pair("to", checkConjunction));
    
}



//LEVEL 1 - INSTRUCTION PARSER AND CHECKER
string Parser::checkInstruction(int line, vector<pair<string, LexerConstants::TokenType>> tokens) {
    //If keyword doesn't match, return error no instruction found
    if (tokens[0].second != LexerConstants::INSTRUCTION) {
        return "Unknown instruction '" + tokens[0].first + "'";
    }
    auto itr = m_instructionMap.find(tokens[0].first);
    //If found, go to parse instruction method creating a new instruction node
    if (itr!= m_instructionMap.end()) {
        return parseInstruction((m_parseTree->getRoot()->insertChild(line, (new GeneralNode(0, tokens[0].first, INSTRUCTION)))), tokens, itr->second);
    }
    else {
        //Edge case, valid instruction with no method implemented (debug)
        return "Compiler error for '" + tokens[0].first + "'. Could not find instruction parsing method.";
    }
}

string Parser::parseInstruction(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens, std::vector<std::pair<std::string, std::function<std::string(PTNode*, std::vector<std::pair<std::string, LexerConstants::TokenType>>, std::string, int)>>> parsingTemplate) {
    //Temporary return string
    string returnString;
    //Loop through all templates
    //NOTE - if the instruction is a no operand (i.e. empty parsingTemplate) loop will not run and will go straight to final check
    for (int i=0; i<parsingTemplate.size(); i++) {
        //Access the parsing function, passing the index expected in the token sequence
        returnString = parsingTemplate[i].second(node, tokens, parsingTemplate[i].first, (i*2));
        //If an error arises, return instantly
        if (returnString != "") {
            return returnString;
        }
        //Clear return string on every iteration
        returnString.clear();
    }

    //Final check - syntax correct but there's excess tokens present
    //Special case - no operands
    if (parsingTemplate.size() == 0) {
        //If there exists more than just the instruction, return excess tokens
        if (tokens.size()>1) {
            return "Excess tokens at and past '" + tokens[1].first + "' found.";
        }
        else {
            return "";
        }
    }
    //General case
    else {
        if (tokens.size()>(parsingTemplate.size()*2)) {
        return "Excess token at and past'" + tokens[parsingTemplate.size()*2].first + "' found.";
        }
        //Return empty (correct syntax) if passing all tests
        else {
            return "";
        }
    }
};



//LEVEL 2 - CONJUNCTION AND CONDITION CHECKERS / PARSER HELPERS
string Parser::checkImplicit(PTNode* node, vector<pair<string, LexerConstants::TokenType>> tokens, string keyword, int index) {
    string returnString;
    //Implicit node is implicit, so always exists
    //Add keyword as child
    //Rewrite returnString if L2 analysis returns an error
    returnString = parseImplicit(node->insertChild(NULL_INDEX, (new GeneralNode(Constants::NULL_INDEX, keyword, CONJUNCTION))), tokens, keyword, index);
    return returnString;
}

string Parser::checkConjunction(PTNode* node, vector<pair<string, LexerConstants::TokenType>> tokens, string keyword, int index) {
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
        returnString = parseConjunction(node->insertChild(index, (new GeneralNode(index, keyword, CONJUNCTION))), tokens, keyword, index);
        return returnString;
    }
}

string Parser::checkCondition(PTNode* node, vector<pair<string, LexerConstants::TokenType>> tokens, string keyword, int index) {
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
        returnString = parseCondition(node->insertChild(index, (new GeneralNode(index, keyword, CONJUNCTION))), tokens, keyword, index);
        return returnString;
    }
}

string Parser::parseImplicit(PTNode* node, vector<pair<string, LexerConstants::TokenType>> tokens, string keyword, int index) {
    //Create a temporary return string to return errors
    string returnString;
    //Check if the first operand exissts
    if (tokens.size()<FIRST_OPERAND_POSITION+1) {
        return "Missing first operand";
    }
    //Check if the token is an operand
    else if (!isOperand(tokens[FIRST_OPERAND_POSITION])) {
        return "Unknown first operand '" + tokens[FIRST_OPERAND_POSITION].first + "'";
    }
    else {
        //Insert a new child as the operand
        node->insertChild(1, (new OperandNode(FIRST_OPERAND_POSITION, tokens[FIRST_OPERAND_POSITION].first, returnPTOperand(tokens[FIRST_OPERAND_POSITION].second))));
        return "";
    }
}

string Parser::parseConjunction(PTNode* node, vector<pair<string, LexerConstants::TokenType>> tokens, string keyword, int index) {
    cout << "parsing conjunction " << endl;
    //Create temporary return string
    string returnString;
    //Increment index by one to now point to where the operand should be
    index++;
    //If the operand does not exist after the keyword, return an error
    if(tokens.size()<=index) {
        return "Missing operand after '" + keyword + "'";
    }
    //If the token in the operand position is not an operad, return an error
    else if (!isOperand(tokens[index])) {
        return "Unknown operand '" + tokens[index].first + "' after '" + keyword + "'";
    }
    else {
        //Insert a new child as the operand
        node->insertChild(index, (new OperandNode(index, tokens[index].first, returnPTOperand(tokens[index].second))));
        return "";
    }
}

string Parser::parseCondition(PTNode* node, vector<pair<string, LexerConstants::TokenType>> tokens, string keyword, int index) {
    //Create temporary return string
    string returnString;
    //Iterate the index to now point to where the condition should be
    index++;
    //If the descriptor after keyword does not exist
    if(tokens.size()<=index) {
        return "Missing descriptor after '" + keyword + "'";
    }
    //If the token after keyword does not match as a descriptor
    else if (!isDescriptor(tokens[index])) {
        return "Unknown descriptor '" + tokens[index].first + "' after '" + keyword + "'";
    }
    else {
        //Insert a new child as the operand
        node->insertChild(index, (new OperandNode(index, tokens[index].first, returnPTOperand(tokens[index].second))));
        return "";
    } 
}



//LEVEL 3 - OPERANDS AND DESCRIPTORS
bool Parser::isOperand(pair<string, LexerConstants::TokenType> token) {
    //Switch statement to determine if a lexer constant constitutes an operand in the PT
    switch (token.second) {
        case LexerConstants::TokenType::REGISTER:
            return true;
        case LexerConstants::TokenType::INSTRUCTIONADDRESS:
            return true;
        case LexerConstants::TokenType::MEMORYADDRESS:
            return true;
        case LexerConstants::TokenType::FLOAT:
            return true;
        case LexerConstants::TokenType::INTEGER:
            return true;
        case LexerConstants::TokenType::BOOLEAN:
            return true;
        case LexerConstants::TokenType::CHARACTER:
            return true;
        case LexerConstants::TokenType::LABEL:
            return true;
        case LexerConstants::TokenType::COMMENT:
            return true;
        default:
            return false;
    }
}

bool Parser::isDescriptor(pair<string, LexerConstants::TokenType> token) {
    //Check if token is a descriptor (classified by lexer)
    if (token.second == LexerConstants::TokenType::DESCRIPTOR) {
        return true;
    }
    else {
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
        case LexerConstants::TokenType::COMMENT:
            return PTConstants::OperandType::COMMENT;
        case LexerConstants::TokenType::UNKNOWN:
            return PTConstants::OperandType::UNKNOWN;
        default:
            return PTConstants::OperandType::UNKNOWN;
    }
}

