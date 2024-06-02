#include "include/Lexer.h"

#include <fstream>
#include <regex>
#include <string>
#include <functional>
#include <utility>
#include <sstream>


using namespace std;
using namespace LexerConstants;

//Constructor
Lexer::Lexer(): m_stringTemplate("^\".*\"$") {
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
    m_tokenDictionary.emplace("input", INSTRUCTION);
    m_tokenDictionary.emplace("output", INSTRUCTION);
    m_tokenDictionary.emplace("print", INSTRUCTION);
    m_tokenDictionary.emplace("label", INSTRUCTION);
    //Add conjunctions to dictionary
    m_tokenDictionary.emplace("from", CONJUNCTION);
    m_tokenDictionary.emplace("with", CONJUNCTION);
    m_tokenDictionary.emplace("self", CONJUNCTION);
    m_tokenDictionary.emplace("to", CONJUNCTION);
    m_tokenDictionary.emplace("by", CONJUNCTION);
    m_tokenDictionary.emplace("if", CONJUNCTION);
    //Add conditions to dictionary
    m_tokenDictionary.emplace("left", SHIFTCONDITION);
    m_tokenDictionary.emplace("right", SHIFTCONDITION);
    m_tokenDictionary.emplace("greater", JUMPCONDITION);
    m_tokenDictionary.emplace("less", JUMPCONDITION);
    m_tokenDictionary.emplace("equal", JUMPCONDITION);
    m_tokenDictionary.emplace("unequal", JUMPCONDITION);
    m_tokenDictionary.emplace("zero", JUMPCONDITION);
    m_tokenDictionary.emplace("nonzero", JUMPCONDITION);
    m_tokenDictionary.emplace("unconditional", JUMPCONDITION);
    m_tokenDictionary.emplace("integer", TYPECONDITION);
    m_tokenDictionary.emplace("float", TYPECONDITION);
    m_tokenDictionary.emplace("boolean", TYPECONDITION);
    m_tokenDictionary.emplace("character", TYPECONDITION);
    m_tokenDictionary.emplace("memory", TYPECONDITION);
    m_tokenDictionary.emplace("instruction", TYPECONDITION);

    //Add operand regex templates to vector
    m_operandDictionary.emplace_back(regex("r[0-9]+"), REGISTER);
    m_operandDictionary.emplace_back(regex("m<[0-9]+>"), MEMORYADDRESS);
    m_operandDictionary.emplace_back(regex("i\\[[0-9]+\\]"), INSTRUCTIONADDRESS);
    m_operandDictionary.emplace_back(regex("-?[1-9][0-9]{0,9}|0"), INTEGER);
    m_operandDictionary.emplace_back(regex("-?\\d+\\.\\d+"), FLOAT);
    m_operandDictionary.emplace_back(regex("true|false|1|0"), BOOLEAN);
    m_operandDictionary.emplace_back(regex("."), CHARACTER);
    m_operandDictionary.emplace_back(regex("'([^']+)'"), LABEL);
}

//Main lexer method
bool Lexer::lexFile(const std::string& filename, std::vector<std::string>& codeLines, std::vector<std::vector<std::pair<std::string, LexerConstants::TokenType>>>& tokenizedCode) {
    //Read the file
    if (!readFile(filename, codeLines)) {
        return false;
    }
    //Tokenize the file
    tokenizeFile(codeLines, tokenizedCode);
    return true;
}

//Read file method
bool Lexer::readFile(const string& filename, vector<string>& codeLines) {
    //Create a file stream and open the file
    ifstream file(filename);
    //If file is not open, return false
    if (!file.is_open()) {
        return false;
    }
    //Temporary string to hold the line
    string line;
    //Loop through every line in the file
    while (getline(file, line)) {
        //Push back the line into the vector
        codeLines.push_back(line);
    }
    //Close the file
    file.close();
    //Return true
    return true;
}

//Tokenize file method
void Lexer::tokenizeFile(vector<string>& codeLines, vector<vector<pair<string, TokenType>>>& tokenizedCode) {
    //Preallocate depending on the number of lines to allow sequential write
    std::vector<std::vector<std::pair<string, LexerConstants::TokenType>>> tempTokens(codeLines.size());

    // Parallelize lexing of each line
    #pragma omp parallel for schedule(auto) default(none) shared(codeLines, tempTokens)
    for (long unsigned int i = 0; i < codeLines.size(); i++) {
        // Each thread works on its own part of the vector
        tempTokens[i] = tokenizeLine(codeLines[i]);
    }

    // Flatten the results into m_codeTokens
    for (const auto& lineTokens : tempTokens) {
        tokenizedCode.push_back(lineTokens);
    }
}

//Tokenize line helper function
vector<pair<string, TokenType>> Lexer::tokenizeLine(string& line) {
    //Create a string stream for the line, a temporary token string, and the return vector
    stringstream ss(line);
    string token;
    vector<pair<string, TokenType>> tokenizedLine;

    //Zero case, return instantly
    if (line.empty()) {
        tokenizedLine.emplace_back("", BLANK);
        return tokenizedLine;
    }

    //Loop through every token
    while (ss >> token) {
        //Special case for comments and prints. These are the only instructions not separating operands by whitespace alone
        if (token == "comment" || token == "print") {
            //Push back string as an instruction
            tokenizedLine.emplace_back(token, INSTRUCTION);
            //Temporary string to hold the comment and a regex template to check for validity
            string operandString;
            //Get rest of the line
            getline(ss, operandString);
            operandString.erase(0, 1);
            //First we need to determine if the next token is a newline - special case
            if (operandString == "newline") {
                tokenizedLine.emplace_back(operandString, NEWLINE);
            }
            //If matching regex template, denote string as "string"
            else if (regex_match(operandString, m_stringTemplate)) {
                tokenizedLine.emplace_back(operandString, STRING);
            }
            //Denote string unknown otherwise
            else {
                tokenizedLine.emplace_back(operandString, UNKNOWN);
            }
            //Return vector instantly
            return tokenizedLine;
        }
        //Case for not comments and strings (everything else)
        else {
            //Attempt to find the token in the dictionary
            auto itr = m_tokenDictionary.find(token);
            //If found in the token dictionary (instructions, conjunctions, descriptors, conditions) push it back
            if (itr != m_tokenDictionary.end()) {
                tokenizedLine.emplace_back(token, itr->second);
            }
            //If not, check if it is an operand using regex
            else {
                int i;
                //Loop through vector of operand regex templates, checking every one
                for (i=0; i<m_operandDictionary.size(); i++) {
                    if (regex_match(token, m_operandDictionary[i].first)) {
                        //If matching, push with corresponding operand type
                        tokenizedLine.emplace_back(token, m_operandDictionary[i].second);
                        break;
                    }
                }
                if (i == m_operandDictionary.size()) {
                    //If not either, push as unknown
                    tokenizedLine.emplace_back(token, UNKNOWN);
                }
            }
        }
    }
    
    return tokenizedLine;
}