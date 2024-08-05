#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <regex>

namespace LexerConstants {
    enum TokenType {INSTRUCTION, CONJUNCTION, JUMPCONDITION, TYPECONDITION, SHIFTCONDITION, REGISTER, INSTRUCTIONADDRESS, MEMORYADDRESS, INTEGER, FLOAT, BOOLEAN, CHARACTER, LABEL, STRING, BLANK, NEWLINE, UNKNOWN};
}

class Lexer {
    public:
        Lexer();
        ~Lexer() = default;
        //Delete copy and assignment
        Lexer(const Lexer&) = delete;
        Lexer& operator=(const Lexer&) = delete;

        //Lexer method
        bool lexFile(const std::string&, std::vector<std::string>&, std::vector<std::vector<std::pair<std::string, LexerConstants::TokenType>>>&);

    private:
        //File reader function
        bool readFile(const std::string&, std::vector<std::string>&);
        //File tokenizer function
        void tokenizeFile(std::vector<std::string>&, std::vector<std::vector<std::pair<std::string, LexerConstants::TokenType>>>&);
        //Line tokenizer helper function
        std::vector<std::pair<std::string, LexerConstants::TokenType>> tokenizeLine(std::string&);
        //Dictionary of tokens
        std::unordered_map<std::string, LexerConstants::TokenType> m_tokenDictionary;
        std::vector<std::pair<std::regex, LexerConstants::TokenType>> m_operandDictionary;
        std::regex m_stringTemplate;
};

#endif