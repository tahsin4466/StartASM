#include "include/Parser.h"

#include <iostream>
#include <regex>
#include <string>
#include <functional>
#include <utility>

using namespace std;

//Constructor - Initialize all data structure values
Parser::Parser() { 
    m_instructionMap.emplace("move", instructionMove);
    m_instructionMap.emplace("load", instructionLoad);
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
    m_instructionMap.emplace("label", instructionLabel);

    m_operandList.push_back(regex("r[0-9]"));
    m_operandList.push_back(regex("m<[0-9]{1,9}>"));
    m_operandList.push_back(regex("i\\[[0-9]{1,9}\\]"));
    m_operandList.push_back(regex("-?[1-9][0-9]{0,9}|0"));
    m_operandList.push_back(regex("-?\\d+\\.\\d+"));
    m_operandList.push_back(regex("true|false|1|0"));
    m_operandList.push_back(regex("."));

    m_descriptorSet.emplace("logically");
    m_descriptorSet.emplace("arithmetically");
    m_descriptorSet.emplace("greater");
    m_descriptorSet.emplace("less");
    m_descriptorSet.emplace("equal");
    m_descriptorSet.emplace("integer");
    m_descriptorSet.emplace("float");
    m_descriptorSet.emplace("boolean");
    m_descriptorSet.emplace("character");
    m_descriptorSet.emplace("address");

    m_parseTree = new PT();
}

string Parser::validateInstruction(int line, vector<string> tokens) {
    //If keyword doesn't match, return error no instruction found
    auto itr = m_instructionMap.find(tokens[0]);
    if (itr == m_instructionMap.end()) {
        return "Unknown instruction '" + tokens[0] + "'";
    }

    //If found, go to instruction method and add new child node to root
    return itr->second((m_parseTree->getRoot()->insertChild(line, (new GeneralNode(0, tokens[0], INSTRUCTION)))), tokens);
}


//LEVEL 1 - INSTRUCTION METHODS
//Parse move instruction
string Parser::instructionMove(PTNode* node, vector<string> tokens) {
    string returnString = "";
    //Parse from segment
    if (true) {
        //From node is implicit, so always exists
        //Add keyword as child
        //Rewrite returnString if L2 analysis returns an error
        returnString = conjunctionFrom(node->insertChild(1, (new GeneralNode(NULLINDEX, "from", CONJUNCTION))), tokens);
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
    else if (tokens[2] != "to") {
        return "Unknown conjunction " + tokens[2] + ". Expected 'to'";
    }
    //If passed, add to keyword as child
    //Rewrite returnString if L2 analysis returns an error
    else {
        returnString = conjunctionTo(node->insertChild(2, (new GeneralNode(2, "to", CONJUNCTION))), tokens);
        if (returnString!="") {
            return returnString;
        }
    }
    return returnString;
}



//LEVEL 2 - CONJUNCTION AND CONDITION METHODS
string Parser::conjunctionFrom(PTNode* node, vector<string> tokens) {
    string returnString;
    //Check if operand exists
    if (tokens.size()<2) {
        return "Missing first operand";
    }
    //Check if operand is valid
    else if (!isOperand(tokens[1])) {
        return "Unknown first operand " + tokens[1];
    }
    //Add child if passed
    else {
        node->insertChild(1, (new OperandNode(1, tokens[1], REGISTER)));
        return "";
    }
}

string Parser::conjunctionTo(PTNode* node, vector<string> tokens) {
    string returnString;
    int i = 0;
    while (tokens[i] != "to") {
        i++;
    }
    i++;
    if(tokens.size()<i) {
        return "Missing operand after 'to'";
    }
    else if (!isOperand(tokens[i])) {
        return "Unknown operand " + tokens[i] + " after 'to'";
    }
    else {
        node->insertChild(2, (new OperandNode(1, tokens[i], REGISTER)));
        return "";
    }
}



//LEVEL 3 - OPERANDS AND DESCRIPTORS
bool Parser::isOperand(string token) {
    //regex match every template in list
    for (int i=0; i<m_operandList.size(); i++) {
        if (regex_match(token, m_operandList[i])) {
            //Return true if match found
            return true;
        }
    }
    return false;
}

bool Parser::isDescriptor(string token) {
    //Hash search descriptor
    auto itr = m_descriptorSet.find(token);
    if (itr == m_descriptorSet.end()) {
        return false;
    }
    else {
        return true;
    }
}

