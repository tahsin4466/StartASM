#ifndef PARSER_H
#define PARSER_H

#include "ParseTree.h"

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
        std::string validateInstruction(int line, std::vector<std::string> tokens);
        //Get pointer to parse tree
        PT* getParseTree() {return m_parseTree;};

    private:
        //Pointer to PT
        PT* m_parseTree;
        //Hash map containing a keyword linked to an instruction parsing function
        std::unordered_map<std::string, std::function<std::string(PTNode*, std::vector<std::string>)>> m_instructionMap;
        //Vector containing operand regex templates
        std::vector<std::regex> m_operandList;
        //Hash set containing descriptors
        std::unordered_set<std::string> m_descriptorSet;


        //LEVEL 1 - INSTRUCTION PARSERS
        //Instruction Parsing Functions
        std::string instructionMove(PTNode* node, std::vector<std::string> tokens);
        std::string instructionLoad(PTNode* node, std::vector<std::string> tokens);
        std::string instructionStore(PTNode* node, std::vector<std::string> tokens);
        std::string instructionCreate(PTNode* node, std::vector<std::string> tokens);
        std::string instructionCast(PTNode* node, std::vector<std::string> tokens);
        std::string instructionAdd(PTNode* node, std::vector<std::string> tokens);
        std::string instructionSub(PTNode* node, std::vector<std::string> tokens);
        std::string instructionMultiply(PTNode* node, std::vector<std::string> tokens);
        std::string instructionDivide(PTNode* node, std::vector<std::string> tokens);
        std::string instructionOr(PTNode* node, std::vector<std::string> tokens);
        std::string instructionAnd(PTNode* node, std::vector<std::string> tokens);
        std::string instructionNot(PTNode* node, std::vector<std::string> tokens);
        std::string instructionShift(PTNode* node, std::vector<std::string> tokens);
        std::string instructionCompare(PTNode* node, std::vector<std::string> tokens); 
        std::string instructionJump(PTNode* node, std::vector<std::string> tokens);
        std::string instructionCall(PTNode* node, std::vector<std::string> tokens);
        std::string instructionPush(PTNode* node, std::vector<std::string> tokens);
        std::string instructionPop(PTNode* node, std::vector<std::string> tokens);
        std::string instructionReturn(PTNode* node, std::vector<std::string> tokens);
        std::string instructionStop(PTNode* node, std::vector<std::string> tokens);
        std::string instructionLabel(PTNode* node, std::vector<std::string> tokens);
        std::string instructionComment(PTNode* node, std::vector<std::string> tokens);


        //LEVEL 2 - CONJUNCTION AND CONDITION PARSERS
        //Conjunction Parsing Functions
        std::string conjunctionFrom(PTNode* node, std::vector<std::string> tokens);
        std::string conjunctionWith(PTNode* node, std::vector<std::string> tokens);
        std::string conjunctionSelf(PTNode* node, std::vector<std::string> tokens);
        std::string conjunctionTo(PTNode* node, std::vector<std::string> tokens);
        std::string conjunctionBy(PTNode* node, std::vector<std::string> tokens);

        //Condition Parsing Functions
        std::string conditionLeft(PTNode* node, std::string tokens);
        std::string conditionRight(PTNode* node, std::string tokens);
        std::string conditionIf(PTNode* node, std::string tokens);
        std::string conditionExcept(PTNode* node, std::string tokens);


        //LEVEL 3 - OPERAND AND DESCRIPTOR CHECKERS
        bool isOperand(std::string token);
        bool isDescriptor(std::string token);


};

#endif