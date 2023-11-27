#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <regex>

namespace LexerConstants {
    enum TokenType {INSTRUCTION, CONJUNCTION, CONDITION, DESCRIPTOR, REGISTER, INSTRUCTIONADDRESS, MEMORYADDRESS, INTEGER, FLOAT, BOOLEAN, CHARACTER, LABEL, COMMENT, UNKNOWN};
};

class Lexer {
    public:
        Lexer();
        ~Lexer() {};
        //Delete copy and assignment
        Lexer(const Lexer&) = delete;
        Lexer& operator=(const Lexer&) = delete;

        //Lexer method
        std::vector<std::pair<std::string, LexerConstants::TokenType>> tokenizeLine(std::string);

    private:
        std::unordered_map<std::string, LexerConstants::TokenType> m_tokenDictionary;
        std::vector<std::pair<std::regex, LexerConstants::TokenType>> m_operandDictionary;
};

#endif