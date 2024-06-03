#include "include/Parser.h"

#include <iostream>
#include <regex>
#include <string>
#include <functional>
#include <utility>

using namespace std;
using namespace PTConstants;
using namespace PT;

//Constructor - Initialize all data structure values and parsing templates
Parser::Parser() {
    //Initialise the instructionMap with expected lengths of each instruction
    m_instructionMap.emplace("move", 4);
    m_instructionMap.emplace("load", 4);
    m_instructionMap.emplace("store", 4);
    m_instructionMap.emplace("create", 5);
    m_instructionMap.emplace("cast", 3);
    m_instructionMap.emplace("add", 6);
    m_instructionMap.emplace("sub", 6);
    m_instructionMap.emplace("multiply", 6);
    m_instructionMap.emplace("divide", 6);
    m_instructionMap.emplace("or", 4);
    m_instructionMap.emplace("and", 4);
    m_instructionMap.emplace("not", 2);
    m_instructionMap.emplace("shift", 6);
    m_instructionMap.emplace("compare", 4);
    m_instructionMap.emplace("jump", 5);
    m_instructionMap.emplace("call", 3);
    m_instructionMap.emplace("push", 2);
    m_instructionMap.emplace("pop", 3);
    m_instructionMap.emplace("return", 1);
    m_instructionMap.emplace("stop", 1);
    m_instructionMap.emplace("input", 4);
    m_instructionMap.emplace("output", 2);
    m_instructionMap.emplace("print", 2);
    m_instructionMap.emplace("label", 2);
    m_instructionMap.emplace("comment", 2);


    //Move instruction template
    m_templateMap["move"].push_back(make_pair(make_pair("from", 0), checkImplicitConjunction));
    m_templateMap["move"].push_back(make_pair(make_pair("to", 2), checkExplicitConjunction));
    //Load instruction template
    m_templateMap["load"].push_back(make_pair(make_pair("from", 0), checkImplicitConjunction));
    m_templateMap["load"].push_back(make_pair(make_pair("to", 2), checkExplicitConjunction));
    //Store instruction template
    m_templateMap["store"].push_back(make_pair(make_pair("from", 0), checkImplicitConjunction));
    m_templateMap["store"].push_back(make_pair(make_pair("to", 2), checkExplicitConjunction));
    //Create instruction template
    m_templateMap["create"].push_back(make_pair(make_pair("type", 0), checkImplicitCondition));
    m_templateMap["create"].push_back(make_pair(make_pair("from", 1), checkImplicitConjunction));
    m_templateMap["create"].push_back(make_pair(make_pair("to", 3), checkExplicitConjunction));
    //Cast instruction template
    m_templateMap["cast"].push_back(make_pair(make_pair("type", 0), checkImplicitCondition));
    m_templateMap["cast"].push_back(make_pair(make_pair("self", 1), checkImplicitConjunction));
    //Add instruction template
    m_templateMap["add"].push_back(make_pair(make_pair("from", 0), checkImplicitConjunction));
    m_templateMap["add"].push_back(make_pair(make_pair("with", 2), checkExplicitConjunction));
    m_templateMap["add"].push_back(make_pair(make_pair("to", 4), checkExplicitConjunction));
    //Sub instruction template
    m_templateMap["sub"].push_back(make_pair(make_pair("from", 0), checkImplicitConjunction));
    m_templateMap["sub"].push_back(make_pair(make_pair("with", 2), checkExplicitConjunction));
    m_templateMap["sub"].push_back(make_pair(make_pair("to", 4), checkExplicitConjunction));
    //Multiply instruction template
    m_templateMap["multiply"].push_back(make_pair(make_pair("from", 0), checkImplicitConjunction));
    m_templateMap["multiply"].push_back(make_pair(make_pair("with", 2), checkExplicitConjunction));
    m_templateMap["multiply"].push_back(make_pair(make_pair("to", 4), checkExplicitConjunction));
    //Divide instruction template
    m_templateMap["divide"].push_back(make_pair(make_pair("from", 0), checkImplicitConjunction));
    m_templateMap["divide"].push_back(make_pair(make_pair("with", 2), checkExplicitConjunction));
    m_templateMap["divide"].push_back(make_pair(make_pair("to", 4), checkExplicitConjunction));
    //Or instruction template
    m_templateMap["or"].push_back(make_pair(make_pair("self", 0), checkImplicitConjunction));
    m_templateMap["or"].push_back(make_pair(make_pair("with", 2), checkExplicitConjunction));
    //And instruction template
    m_templateMap["and"].push_back(make_pair(make_pair("self", 0), checkImplicitConjunction));
    m_templateMap["and"].push_back(make_pair(make_pair("with", 2), checkExplicitConjunction));
    //Not instruction template
    m_templateMap["not"].push_back(make_pair(make_pair("self", 0), checkImplicitConjunction));
    //Shift instruction template (left)
    m_templateMap["shift"].push_back(make_pair(make_pair("direction", 0), checkImplicitCondition));
    m_templateMap["shift"].push_back(make_pair(make_pair("self", 1), checkImplicitConjunction));
    m_templateMap["shift"].push_back(make_pair(make_pair("by", 3), checkExplicitConjunction));
    //Compare instruction template
    m_templateMap["compare"].push_back(make_pair(make_pair("self", 0), checkImplicitConjunction));
    m_templateMap["compare"].push_back(make_pair(make_pair("with", 2), checkExplicitConjunction));
    //Jump instruction
    m_templateMap["jump"].push_back(make_pair(make_pair("if", 1), checkExplicitCondition));
    m_templateMap["jump"].push_back(make_pair(make_pair("to", 3), checkExplicitConjunction));
    //Call instruction
    m_templateMap["call"].push_back(make_pair(make_pair("to", 1), checkExplicitConjunction));
    //Push instruction
    m_templateMap["push"].push_back(make_pair(make_pair("from", 0), checkImplicitConjunction));
    //Pop instruction
    m_templateMap["pop"].push_back(make_pair(make_pair("to", 1), checkExplicitConjunction));
    //Return instruction
    m_templateMap["return"];
    //Stop instruction
    m_templateMap["stop"];
    //Input instruction
    m_templateMap["input"].push_back(make_pair(make_pair("type", 0), checkImplicitCondition));
    m_templateMap["input"].push_back(make_pair(make_pair("to", 2), checkExplicitConjunction));
    //Output instruction template
    m_templateMap["output"].push_back(make_pair(make_pair("from", 0), checkImplicitConjunction));
    //Print instruction
    m_templateMap["print"].push_back(make_pair(make_pair("from", 0), checkImplicitConjunction));
    //Comment instruction
    m_templateMap["comment"].push_back(make_pair(make_pair("static", 0), checkImplicitConjunction));
    //Label instruction
    m_templateMap["label"].push_back(make_pair(make_pair("static", 0), checkImplicitConjunction));

}

bool Parser::parseCode(PT::ParseTree* parseTree, const std::vector<std::string>& codeLines, const std::vector<std::vector<std::pair<std::string, LexerConstants::TokenType>>>& tokens, std::string& errorMessage) {
    //The parser relies on top-down recursive descent parsing
    //Implementing parallelization is a NIGHTMARE, and very easily involves race-conditions and stack overflows
    //Hence, parsing will remain sequential

    for (int i=0; i<tokens.size(); i++) {
        //Call validateInstruction in InstructionSet
        string error = checkInstruction(parseTree, i, tokens[i]);
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
string Parser::checkInstruction(ParseTree* parseTree, int line, vector<pair<string, LexerConstants::TokenType>> tokens) {
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

string Parser::parseInstruction(ParseTree* parseTree, PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens, std::vector<std::pair<std::pair<std::string, int>, std::function<std::string(ParseTree*, PTNode*, std::vector<std::pair<std::string, LexerConstants::TokenType>>, std::string, int)>>> parsingTemplate) {
    //Temporary return string
    string returnString;
    //Loop through all templates
    //NOTE - if the instruction is a no operand (i.e. empty parsingTemplate) loop will not run and will go straight to final check
    for (int i=0; i<parsingTemplate.size(); i++) {
        //Access the parsing function, passing the index expected in the token sequence
        returnString = parsingTemplate[i].second(parseTree, node, tokens, parsingTemplate[i].first.first, parsingTemplate[i].first.second);
        //If an error arises, return instantly
        if (returnString != "") {
            return returnString;
        }
        //Clear return string on every iteration
        returnString.clear();
    }

    //Final check - syntax correct but there's excess tokens present
    //Find size of template expected through instrunctionMap loopup
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

};



//LEVEL 2 - CONJUNCTION AND CONDITION CHECKERS / PARSER HELPERS
string Parser::checkImplicitConjunction(ParseTree* parseTree, PTNode* node, vector<pair<string, LexerConstants::TokenType>> tokens, string keyword, int index) {
    string returnString;
    //Implicit node is implicit, so always exists
    //Add keyword as child
    //Rewrite returnString if L2 analysis returns an error
    returnString = parseConjunction(parseTree, node->insertChild((new GeneralNode(Constants::IMPLICIT_INDEX, keyword, CONJUNCTION))), tokens, keyword, index);
    return returnString;
}

string Parser::checkImplicitCondition(ParseTree* parseTree, PTNode* node, vector<pair<string, LexerConstants::TokenType>> tokens, string keyword, int index) {
    string returnString;
    //Implicit node is implicit, so always exists
    //Add keyword as child
    //Rewrite returnString if L2 analysis returns an error
    returnString = parseCondition(parseTree, node->insertChild((new GeneralNode(Constants::IMPLICIT_INDEX, keyword, CONJUNCTION))), tokens, keyword, index);
    return returnString;
}

string Parser::checkExplicitConjunction(ParseTree* parseTree, PTNode* node, vector<pair<string, LexerConstants::TokenType>> tokens, string keyword, int index) {
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

string Parser::checkExplicitCondition(ParseTree* parseTree, PTNode* node, vector<pair<string, LexerConstants::TokenType>> tokens, string keyword, int index) {
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

string Parser::parseConjunction(ParseTree* parseTree, PTNode* node, vector<pair<string, LexerConstants::TokenType>> tokens, string keyword, int index) {
    //Create temporary return string
    string returnString;
    //Increment index by one to now point to where the operand should be
    index++;
    //If the operand does not exist after the keyword, return an error
    if(tokens.size()<=index) {
        return "Missing operand after '" + tokens[index-1].first + "'";
    }
    //If the token in the operand position is not an operad, return an error
    else if (!isOperand(tokens[index])) {
        return "Unknown operand '" + tokens[index].first + "' after '" + tokens[index-1].first + "'";
    }
    else {
        //Insert a new child as the operand
        node->insertChild((new OperandNode(index, tokens[index].first, returnPTOperand(tokens[index].second))));
        return "";
    }
}

string Parser::parseCondition(ParseTree* parseTree, PTNode* node, vector<pair<string, LexerConstants::TokenType>> tokens, string keyword, int index) {
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
        case LexerConstants::TokenType::STRING:
            return true;
        case LexerConstants::TokenType::NEWLINE:
            return true;
        default:
            return false;
    }
}

bool Parser::isDescriptor(pair<string, LexerConstants::TokenType> token) {
    //Check if token is a condition
    //Switch statement
    switch (token.second) {
        case LexerConstants::TokenType::JUMPCONDITION:
            return true;
        case LexerConstants::TokenType::SHIFTCONDITION:
            return true;
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
        case LexerConstants::TokenType::UNKNOWN:
            return PTConstants::OperandType::UNKNOWN;
        default:
            return PTConstants::OperandType::UNKNOWN;
    }
}

