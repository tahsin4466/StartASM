#ifndef PARSER_H
#define PARSER_H

#include "ParseTree.h"
#include "Lexer.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <functional>
#include <regex>

class Parser {
    public:
        //Constructor and destructor
        Parser();
        ~Parser() {delete m_parseTree;};

        //Delete copy and assignment
        Parser(const Parser&) = delete;
        Parser& operator=(const Parser&) = delete;

        //LEVEL 1 - INSTRUCTION CHECKER (PUBLIC)
        std::string checkInstruction(int line, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens);
        //Get pointer to parse tree
        PT* getParseTree() {return m_parseTree;};

    private:
        //Pointer to PT
        PT* m_parseTree;
        //Hash map containing a keyword linked to an instruction parsing function
        std::unordered_map<std::string, std::vector<std::pair<std::pair<std::string, int>, std::function<std::string(PTNode*, std::vector<std::pair<std::string, LexerConstants::TokenType>>, std::string, int)>>>> m_instructionMap;



        //LEVEL 1 - INSTRUCTION PARSERS
        static std::string parseInstruction(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens, std::vector<std::pair<std::pair<std::string, int>, std::function<std::string(PTNode*, std::vector<std::pair<std::string, LexerConstants::TokenType>>, std::string, int)>>> parsingTemplate);



        //LEVEL 2 - IMPLICIT AND EXPLICIT CONJUNCTION AND CONDITION CHECKERS
        static std::string checkImplicitConjunction(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens, std::string keyword, int index);
        static std::string checkImplicitCondition(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens, std::string keyword, int index);
        static std::string checkExplicitConjunction(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens, std::string keyword, int index);
        static std::string checkExplicitCondition(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens, std::string keyword, int index);

        //LEVEL 2 - CONJUNCTION AND CONDITION PARSERS
        static std::string parseConjunction(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens, std::string keyword, int index);
        static std::string parseCondition(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens, std::string keyword, int index);



        //LEVEL 3 - OPERAND AND DESCRIPTOR CHECKERS
        static bool isOperand(std::pair<std::string, LexerConstants::TokenType> token);
        static bool isDescriptor(std::pair<std::string, LexerConstants::TokenType> token);

        //Constants helper function
        static PTConstants::OperandType returnPTOperand(LexerConstants::TokenType tokenType);
};

#endif