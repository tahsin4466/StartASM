#include "include/Lexer.h"

#include <iostream>
#include <regex>
#include <string>
#include <functional>
#include <utility>
#include <sstream>

using namespace std;
using namespace LexerConstants;

//Constructor
Lexer::Lexer() {
    //Add instructions to dictionary
    m_tokenDictionary.emplace("move", INSTRUCTION);
    m_tokenDictionary.emplace("load", INSTRUCTION);
    m_tokenDictionary.emplace("store", INSTRUCTION);
    m_tokenDictionary.emplace("create", INSTRUCTION);
    m_tokenDictionary.emplace("cast", INSTRUCTION);
    m_tokenDictionary.emplace("add", INSTRUCTION);
    m_tokenDictionary.emplace("sub", INSTRUCTION);
    m_tokenDictionary.emplace("multiply", INSTRUCTION);
    m_tokenDictionary.emplace("divide", INSTRUCTION);
    m_tokenDictionary.emplace("or", INSTRUCTION);
    m_tokenDictionary.emplace("and", INSTRUCTION);
    m_tokenDictionary.emplace("not", INSTRUCTION);
    m_tokenDictionary.emplace("shift", INSTRUCTION);
    m_tokenDictionary.emplace("compare", INSTRUCTION);
    m_tokenDictionary.emplace("jump", INSTRUCTION);
    m_tokenDictionary.emplace("push", INSTRUCTION);
    m_tokenDictionary.emplace("pop", INSTRUCTION);
    m_tokenDictionary.emplace("return", INSTRUCTION);
    m_tokenDictionary.emplace("stop", INSTRUCTION);
    m_tokenDictionary.emplace("label", INSTRUCTION);
    m_tokenDictionary.emplace("comment", INSTRUCTION);
    //Add conjunctions to dictionary
    m_tokenDictionary.emplace("from", CONJUNCTION);
    m_tokenDictionary.emplace("with", CONJUNCTION);
    m_tokenDictionary.emplace("self", CONJUNCTION);
    m_tokenDictionary.emplace("to", CONJUNCTION);
    m_tokenDictionary.emplace("by", CONJUNCTION);
    //Add conditions to dictionary
    m_tokenDictionary.emplace("left", CONDITION);
    m_tokenDictionary.emplace("right", CONDITION);
    m_tokenDictionary.emplace("if", CONDITION);
    m_tokenDictionary.emplace("except", CONDITION);
    //Add descriptors to dictionary
    m_tokenDictionary.emplace("logically", DESCRIPTOR);
    m_tokenDictionary.emplace("arithmetically", DESCRIPTOR);
    m_tokenDictionary.emplace("greater", DESCRIPTOR);
    m_tokenDictionary.emplace("less", DESCRIPTOR);
    m_tokenDictionary.emplace("equal", DESCRIPTOR);
    m_tokenDictionary.emplace("integer", DESCRIPTOR);
    m_tokenDictionary.emplace("float", DESCRIPTOR);
    m_tokenDictionary.emplace("boolean", DESCRIPTOR);
    m_tokenDictionary.emplace("character", DESCRIPTOR);
    m_tokenDictionary.emplace("address", DESCRIPTOR);

    //Add operand regex templates to vector
    m_operandDictionary.push_back(make_pair(regex("r[0-9]"), REGISTER));
    m_operandDictionary.push_back(make_pair(regex("m<[0-9]{1,9}>"), MEMORYADDRESS));
    m_operandDictionary.push_back(make_pair(regex("i\\[[0-9]{1,9}\\]"), INSTRUCTIONADDRESS));
    m_operandDictionary.push_back(make_pair(regex("-?[1-9][0-9]{0,9}|0"), INTEGER));
    m_operandDictionary.push_back(make_pair(regex("-?\\d+\\.\\d+"), FLOAT));
    m_operandDictionary.push_back(make_pair(regex("true|false|1|0"), BOOLEAN));
    m_operandDictionary.push_back(make_pair(regex("."), CHARACTER));
    m_operandDictionary.push_back(make_pair(regex("'([^']+)'"), LABEL));
    m_operandDictionary.push_back(make_pair(regex(R"(comment ".+")"), COMMENT));
}

vector<pair<string, TokenType>> Lexer::tokenizeLine(string line) {
    stringstream ss(line);
    string token;
    vector<pair<string, TokenType>> tokenizedLine;

    while (ss >> token) {
        auto itr = m_tokenDictionary.find(token);
        if (itr != m_tokenDictionary.end()) {
            tokenizedLine.push_back(make_pair(token, itr->second));
        }
        else {
            bool matched = false;
            for (int i=0; i<m_operandDictionary.size(); i++) {
                if (regex_match(token, m_operandDictionary[i].first)) {
                    tokenizedLine.push_back(make_pair(token, m_operandDictionary[i].second));
                    i = m_operandDictionary.size();
                    matched = true;
                }
            }
            if (!matched) {
                tokenizedLine.push_back(make_pair(token, UNKNOWN));
            }
        }
    }

    return tokenizedLine;
}