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

        //Validate instruction method
        std::string validateInstruction(int line, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens);
        //Get pointer to parse tree
        PT* getParseTree() {return m_parseTree;};

    private:
        //Pointer to PT
        PT* m_parseTree;
        //Hash map containing a keyword linked to an instruction parsing function
        std::unordered_map<std::string, std::function<std::string(PTNode*, std::vector<std::pair<std::string, LexerConstants::TokenType>>)>> m_instructionMap;


        //LEVEL 1 - INSTRUCTION PARSERS
        //Instruction Parsing Functions
        static std::string instructionMove(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens);
       /* static std::string instructionLoad(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens);
        static std::string instructionStore(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens);
        static std::string instructionCreate(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens);
        static std::string instructionCast(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens);
        static std::string instructionAdd(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens);
        static std::string instructionSub(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens);
        static std::string instructionMultiply(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens);
        static std::string instructionDivide(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens);
        static std::string instructionOr(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens);
        static std::string instructionAnd(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens);
        static std::string instructionNot(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens);
        static std::string instructionShift(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens);
        static std::string instructionCompare(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens); 
        static std::string instructionJump(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens);
        static std::string instructionCall(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens);
        static std::string instructionPush(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens);
        static std::string instructionPop(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens);
        static std::string instructionReturn(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens);
        static std::string instructionStop(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens);
        static std::string instructionLabel(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens);
        static std::string instructionComment(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens); */


        //LEVEL 2 - CONJUNCTION AND CONDITION PARSERS
        //Conjunction Parsing Functions
        static std::string conjunctionFrom(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens);
        //static std::string conjunctionWith(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens);
        //static std::string conjunctionSelf(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens);
        static std::string conjunctionTo(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens);
        //static std::string conjunctionBy(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens);

        //Condition Parsing Functions
        /*static std::string conditionLeft(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens);
        static std::string conditionRight(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens);
        static std::string conditionIf(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens);
        static std::string conditionExcept(PTNode* node, std::vector<std::pair<std::string, LexerConstants::TokenType>> tokens); */


        //LEVEL 3 - OPERAND AND DESCRIPTOR CHECKERS
        static bool isOperand(std::pair<std::string, LexerConstants::TokenType> token);
        static bool isDescriptor(std::pair<std::string, LexerConstants::TokenType> token);

        //Conversion helper function;
        static PTConstants::OperandType returnPTOperand(LexerConstants::TokenType tokenType);
};

#endif