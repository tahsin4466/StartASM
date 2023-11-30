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
    m_tokenDictionary.emplace("call", INSTRUCTION);
    m_tokenDictionary.emplace("push", INSTRUCTION);
    m_tokenDictionary.emplace("pop", INSTRUCTION);
    m_tokenDictionary.emplace("return", INSTRUCTION);
    m_tokenDictionary.emplace("stop", INSTRUCTION);
    m_tokenDictionary.emplace("label", INSTRUCTION);
    //Add conjunctions to dictionary
    m_tokenDictionary.emplace("from", CONJUNCTION);
    m_tokenDictionary.emplace("with", CONJUNCTION);
    m_tokenDictionary.emplace("self", CONJUNCTION);
    m_tokenDictionary.emplace("to", CONJUNCTION);
    m_tokenDictionary.emplace("by", CONJUNCTION);
    //Add conditions to dictionary
    m_tokenDictionary.emplace("if", CONDITION);
    //Add descriptors to dictionary
    m_tokenDictionary.emplace("left", DESCRIPTOR);
    m_tokenDictionary.emplace("right", DESCRIPTOR);
    m_tokenDictionary.emplace("greater", DESCRIPTOR);
    m_tokenDictionary.emplace("less", DESCRIPTOR);
    m_tokenDictionary.emplace("equal", DESCRIPTOR);
    m_tokenDictionary.emplace("unequal", DESCRIPTOR);
    m_tokenDictionary.emplace("zero", DESCRIPTOR);
    m_tokenDictionary.emplace("nonzero", DESCRIPTOR);
    m_tokenDictionary.emplace("unconditional", DESCRIPTOR);
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
}

vector<pair<string, TokenType>> Lexer::tokenizeLine(string line) {
    //Create a stringstream for the line, a temporary token string, and the return vector
    stringstream ss(line);
    string token;
    vector<pair<string, TokenType>> tokenizedLine;

    //Zero case, return instantly
    if (line.empty()) {
        tokenizedLine.push_back(make_pair("", BLANK));
        return tokenizedLine;
    }

    //Loop through every token
    while (ss >> token) {
        //Special case for comments. Comments are the only instructions not seperating operands by whitespace alone
        if (token == "comment") {
            //Push back comment as an instruction
            tokenizedLine.push_back(make_pair(token, INSTRUCTION));
            //Temporary string to hold the comment and a regex template to check for validity
            string comment;
            regex commentTemplate("^\".*\"$");
            //Get rest of the line
            getline(ss, comment);
            comment.erase(0, 1);
            //If matching regex template, denote string as "comment"
            if (regex_match(comment, commentTemplate)) {
                tokenizedLine.push_back(make_pair(comment, COMMENT));
            }
            //Denote string unknown otherwise
            else {
                tokenizedLine.push_back(make_pair(comment, UNKNOWN));
            }
            //Return vector instantly
            return tokenizedLine;
        }
        //Case for not comments (everything else)
        else {
            //Attempt to find the token in the dictionary
            auto itr = m_tokenDictionary.find(token);
            //If found in the token dictionary (instructions, conjunctions, descriptors, conditions) push it back
            if (itr != m_tokenDictionary.end()) {
                tokenizedLine.push_back(make_pair(token, itr->second));
            }
            //If not, check if it is an operand using regex
            else {
                int i;
                //Loop through vector of operand regex templates, checking every one
                for (i=0; i<m_operandDictionary.size(); i++) {
                    if (regex_match(token, m_operandDictionary[i].first)) {
                        //If matching, push with corresponding operand type
                        tokenizedLine.push_back(make_pair(token, m_operandDictionary[i].second));
                        break;
                    }
                }
                if (i == m_operandDictionary.size()) {
                    //If not either, push as unknown
                    tokenizedLine.push_back(make_pair(token, UNKNOWN));
                }
            }
        }
    }
    
    return tokenizedLine;
}