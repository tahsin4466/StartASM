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
        ~Parser() = default;

        //Delete copy and assignment
        Parser(const Parser&) = delete;
        Parser& operator=(const Parser&) = delete;

        //Parser main method
        bool parseCode(PT::ParseTree* parseTree, const std::vector<std::string>& codeLines, const std::vector<std::vector<std::pair<std::string, LexerConstants::TokenType>>>& tokens, std::string& errorMessage);

    private:
        //Hash map containing a keyword linked to an instruction parsing function
        std::unordered_map<std::string, std::vector<std::pair<std::pair<std::string, int>, std::function<std::string(PT::ParseTree*, PT::PTNode*, std::vector<std::pair<std::string, LexerConstants::TokenType>>, std::string&, int)>>>> m_templateMap;
        //Hash map containing instructions with template info
        std::unordered_map<std::string, int> m_instructionMap;

        //LEVEL 1 - INSTRUCTION CHECKERS AND PARSERS
        std::string checkInstruction(PT::ParseTree* parseTree, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens);
        std::string parseInstruction(PT::ParseTree* parseTree, PT::PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens, std::vector<std::pair<std::pair<std::string, int>, std::function<std::string(PT::ParseTree*, PT::PTNode*, std::vector<std::pair<std::string, LexerConstants::TokenType>>, std::string&, int)>>> parsingTemplate);

        //LEVEL 2 - IMPLICIT AND EXPLICIT CONJUNCTION AND CONDITION CHECKERS
        static std::string checkImplicitConjunction(PT::ParseTree* parseTree, PT::PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens, std::string& keyword, int index);
        static std::string checkImplicitCondition(PT::ParseTree* parseTree, PT::PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens, std::string& keyword, int index);
        static std::string checkExplicitConjunction(PT::ParseTree* parseTree, PT::PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens, std::string& keyword, int index);
        static std::string checkExplicitCondition(PT::ParseTree* parseTree, PT::PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens, std::string& keyword, int index);

        //LEVEL 2 - CONJUNCTION AND CONDITION PARSERS
        static std::string parseConjunction(PT::ParseTree* parseTree, PT::PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens, std::string& keyword, int index);
        static std::string parseCondition(PT::ParseTree* parseTree, PT::PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens, std::string& keyword, int index);



        //LEVEL 3 - OPERAND AND DESCRIPTOR CHECKERS
        static bool isOperand(std::pair<std::string, LexerConstants::TokenType>& token);
        static bool isDescriptor(std::pair<std::string, LexerConstants::TokenType>& token);

        //Constants helper function
        static PTConstants::OperandType returnPTOperand(LexerConstants::TokenType tokenType);
};

#endif