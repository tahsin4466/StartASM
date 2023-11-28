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
    m_instructionMap.emplace("move", instructionMove);
    /*m_instructionMap.emplace("load", instructionLoad);
    m_instructionMap.emplace("store", instructionStore);
    m_instructionMap.emplace("create", instructionCreate);
    m_instructionMap.emplace("cast", instructionCast);
    m_instructionMap.emplace("add", instructionAdd);
    m_instructionMap.emplace("sub", instructionSub);
    m_instructionMap.emplace("multiply", instructionMultiply);
    m_instructionMap.emplace("divide", instructionDivide);
    m_instructionMap.emplace("or", instructionOr);
    m_instructionMap.emplace("and", instructionAnd);
    m_instructionMap.emplace("not", instructionNot);
    m_instructionMap.emplace("shift", instructionShift);
    m_instructionMap.emplace("compare", instructionCompare);
    m_instructionMap.emplace("jump", instructionJump);
    m_instructionMap.emplace("call", instructionCall);
    m_instructionMap.emplace("push", instructionPush);
    m_instructionMap.emplace("pop", instructionPop);
    m_instructionMap.emplace("return", instructionReturn);
    m_instructionMap.emplace("stop", instructionStop);
    m_instructionMap.emplace("comment", instructionComment);
    m_instructionMap.emplace("label", instructionLabel);*/

    m_parseTree = new PT();
}

string Parser::validateInstruction(int line, vector<pair<string, LexerConstants::TokenType>> tokens) {
    //If keyword doesn't match, return error no instruction found
    if (tokens[0].second != LexerConstants::INSTRUCTION) {
        return "Unknown instruction '" + tokens[0].first + "'";
    }
    auto itr = m_instructionMap.find(tokens[0].first);
    //If found, go to instruction method and add new child node to root
    if (itr!= m_instructionMap.end()) {
        return itr->second((m_parseTree->getRoot()->insertChild(line, (new GeneralNode(0, tokens[0].first, INSTRUCTION)))), tokens);
    }
    else {
        return "Compiler error for '" + tokens[0].first + "'. Could not find instruction parsing method.";
    }
}


//LEVEL 1 - INSTRUCTION METHODS
//Parse move instruction
string Parser::instructionMove(PTNode* node, vector<pair<string, LexerConstants::TokenType>> tokens) {
    string returnString = "";
    //Check first if line is too long
    if (tokens.size()>4) {
        return "Too many tokens. Expected template: move (register) to (register)";
    }

    //Parse from segment
    if (true) {
        //From node is implicit, so always exists
        //Add keyword as child
        //Rewrite returnString if L2 analysis returns an error
        returnString = parseImplicit(node->insertChild(1, (new GeneralNode(Constants::NULL_INDEX, "from", CONJUNCTION))), tokens, "from");
        if (returnString!="") {
                return returnString;
        }
    }
    //Parse to segment
    //Check if to keyword exists
    if (tokens.size()<3) {
        return "Missing conjunction. Expected 'to'";
    }
    //Check if to keyword is valid
    else if (tokens[2].first != "to") {
        return "Unknown conjunction '" + tokens[2].first + "'. Expected 'to'";
    }
    //If passed, add to keyword as child
    //Rewrite returnString if L2 analysis returns an error
    else {
        returnString = parseConjunction(node->insertChild(2, (new GeneralNode(2, "to", CONJUNCTION))), tokens, "to");
        if (returnString!="") {
            return returnString;
        }
    }
    return returnString;
}



//LEVEL 2 - CONJUNCTION AND CONDITION METHODS
string Parser::parseImplicit(PTNode* node, vector<pair<string, LexerConstants::TokenType>> tokens, string keyword) {
    //Create a temporary return string to return errors
    string returnString;
    //Check if the first operand exissts
    if (tokens.size()<FIRST_OPERAND_POSITION+1) {
        return "Missing first operand";
    }
    //Check if the token is an operand
    else if (!isOperand(tokens[FIRST_OPERAND_POSITION])) {
        return "Unknown first operand " + tokens[FIRST_OPERAND_POSITION].first;
    }
    else {
        //Insert a new child as the operand
        node->insertChild(1, (new OperandNode(1, tokens[1].first, returnPTOperand(tokens[1].second))));
        return "";
    }
}

string Parser::parseConjunction(PTNode* node, vector<pair<string, LexerConstants::TokenType>> tokens, string keyword) {
    //Create temporary return string
    string returnString;
    //Iterate over the vector to find the keyword position (guaranteed in tokens)
    int i = 0;
    while (tokens[i].first != keyword) {
        i++;
    }
    i++;
    //If the operand does not exist after the keyword, return an error
    if(tokens.size()<i) {
        return "Missing operand after '" + keyword + "'";
    }
    //If the token in the operand position is not an operad, return an error
    else if (!isOperand(tokens[i])) {
        return "Unknown operand " + tokens[i].first + " after '" + keyword + "'";
    }
    else {
        //Insert a new child as the operand
        node->insertChild(i, (new OperandNode(i, tokens[i].first, returnPTOperand(tokens[i].second))));
        return "";
    }
}

string Parser::parseCondition(PTNode* node, vector<pair<string, LexerConstants::TokenType>> tokens, string keyword) {
    //Create temporary return string
    string returnString;
    //Iterate over tokens until keyword is found (guaranteed in tokens)
    int i = 0;
    while (tokens[i].first != keyword) {
        i++;
    }
    i++;
    //If the operand after keyword does not exist
    if(tokens.size()<i) {
        return "Missing operand after '" + keyword + "'";
    }
    //If the token after keyword does not match as a descriptor
    else if (!isDescriptor(tokens[i])) {
        return "Unknown descriptor " + tokens[i].first + " after '" + keyword + "'";
    }
    else {
        //Insert a new child as the operand
        node->insertChild(i, (new OperandNode(i, tokens[i].first, returnPTOperand(tokens[i].second))));
        return "";
    } 
}

//LEVEL 3 - OPERANDS AND DESCRIPTORS
bool Parser::isOperand(pair<string, LexerConstants::TokenType> token) {
    cout << "Called is operand" << endl;
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
    cout << "Called is descriptor" << endl;
    if (token.second == LexerConstants::TokenType::DESCRIPTOR) {
        return true;
    }
    else {
        return false;
    }
}

//Helper function converting constants
PTConstants::OperandType Parser::returnPTOperand(LexerConstants::TokenType tokenType) {
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

