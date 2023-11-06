#include "include/InstructionSet.h"

#include <iostream>
#include <regex>
#include <string>
#include <sstream>
#include <functional>

using namespace std;

InstructionSet::InstructionSet() { /*
    m_templateMap.emplace("move", vector<string>{"move r[0-9] to r[0-9]"});
    m_templateMap.emplace("load", vector<string>{"load [0-9]+ to r[0-9]", "load m[0-9]+ to r[0-9]"});
    m_templateMap.emplace("store", vector<string>{"store r[0-9] to m[0-9]+"});
    m_templateMap.emplace("add", vector<string>{"add r[0-9] with r[0-9] to r[0-9]"});
    m_templateMap.emplace("sub", vector<string>{"sub r[0-9] with r[0-9] to r[0-9]"});
    m_templateMap.emplace("multiply", vector<string>{"multiply r[0-9] with r[0-9] to r[0-9]"});
    m_templateMap.emplace("divide", vector<string>{"divide r[0-9] with r[0-9] to r[0-9]"});
    m_templateMap.emplace("or", vector<string>{"or r[0-9] with r[0-9]"});
    m_templateMap.emplace("nor", vector<string>{"nor r[0-9] with r[0-9]"});
    m_templateMap.emplace("xor", vector<string>{"xor r[0-9] with r[0-9]"});
    m_templateMap.emplace("and", vector<string>{"and r[0-9] with r[0-9]"});
    m_templateMap.emplace("not", vector<string>{"not r[0-9]"});
    m_templateMap.emplace("shift", vector<string>{"shift left arithmetically r[0-9] by r[0-9]", "shift left logically r[0-9] by r[0-9]", "shift right arithmetically r[0-9] by r[0-9]", "shift right logically r[0-9] by r[0-9]"});
    m_templateMap.emplace("jump", vector<string>{"jump unconditionally to i[0-9]+", "jump if greater to i[0-9]+", "jump if less to i[0-9]+", "jump if equal to i[0-9]+", "jump if not equal to i[0-9]+", "jump if zero to i[0-9]+", "jump if not zero to i[0-9]+"});
    m_templateMap.emplace("push", vector<string>{"push r[0-9]"});
    m_templateMap.emplace("pop", vector<string>{"pop to r[0-9]"});
    m_templateMap.emplace("stop", vector<string>{"stop"});
    m_templateMap.emplace("comment", vector<string>{"comment \"[^\"]*\""});  
    */

    m_parsingMap.emplace("move", parseMove);
    m_parsingMap.emplace("load", parseLoad);
    m_parsingMap.emplace("store", parseStore);
    m_parsingMap.emplace("add", parseAdd);
    m_parsingMap.emplace("sub", parseSub);
    m_parsingMap.emplace("multiply", parseMultiply);
    m_parsingMap.emplace("divide", parseDivide);
    m_parsingMap.emplace("or", parseOr);
    m_parsingMap.emplace("nor", parseNor);
    m_parsingMap.emplace("xor", parseXor);
    m_parsingMap.emplace("and", parseAnd);
    m_parsingMap.emplace("not", parseNot);
    m_parsingMap.emplace("shift", parseShift);
    m_parsingMap.emplace("jump", parseJump);
    m_parsingMap.emplace("push", parsePush);
    m_parsingMap.emplace("pop", parsePop);
    m_parsingMap.emplace("stop", parseStop);
    m_parsingMap.emplace("comment", parseComment);
}

string InstructionSet::validateInstruction(string line, vector<string> tokens) {
    // Extract the keyword from the line
    istringstream iss(line);
    string keyword;
    iss >> keyword;

    //If keyword doesn't match, return error
    auto itr = m_parsingMap.find(keyword);
    if (itr == m_parsingMap.end()) {
        return "Unknown instruction '" + keyword + "'";
    }

    return itr->second(line, tokens);
}

string InstructionSet::parseMove(string line, vector<string> tokens) {
    regex lineTemplate("move r[0-9] to r[0-9]");
    regex registerTemplate("r[0-9]");

    if (regex_match(line, lineTemplate)) {
        return ("");
    }
    else {
        if((tokens.size() != 4)) {
            return "Unknown number of tokens";
        }
        else if(!regex_match(tokens[1], registerTemplate)) {
            return "Unknown first operand '" + tokens[1] + "'. Must be register r0-r9";
        }
        else if(tokens[2]!="to") {
            return "Unknown conjunction '" + tokens[2] + "'. Expected 'to'";
        }
        else if(!regex_match(tokens[3], registerTemplate)) {
            return "Unknown second operand '" + tokens[3] + "'. Must be register r0-r9";
        }
        else {
            return "Incorrect syntax for 'move'";
        }
    }
}

string InstructionSet::parseLoad(string line, vector<string> tokens) {
    return "";
}

string InstructionSet::parseStore(string line, vector<string> tokens) {
    regex lineTemplate("store r[0-9] to m[0-9]+");
    regex registerTemplate("r[0-9]");
    regex memoryTemplate("m[0-9]+");

    if (regex_match(line, lineTemplate)) {
        return "";
    } else {
        if (tokens.size() != 4) {
            return "Incorrect number of tokens";
        } 
        else if (!regex_match(tokens[1], registerTemplate)) {
            return "Invalid register '" + tokens[1] + "'. Must be register r0-r9";
        } 
        else if (tokens[2] != "to") {
            return "Invalid syntax '" + tokens[2] + "'. Expected 'to'";
        } 
        else if (!regex_match(tokens[3], memoryTemplate)) {
            return "Invalid memory address '" + tokens[3] + "'. Must follow the pattern m<number>";
        } 
        else {
            return "Incorrect syntax for 'store'";
        }
    }
}

string InstructionSet::parseAdd(string line, vector<string> tokens) {
    return "";
}

string InstructionSet::parseSub(string line, vector<string> tokens) {
    return "";
}

string InstructionSet::parseMultiply(string line, vector<string> tokens) {
    return "";
}

string InstructionSet::parseDivide(string line, vector<string> tokens) {
    return "";
}

string InstructionSet::parseOr(string line, vector<string> tokens) {
    return "";
}

string InstructionSet::parseNor(string line, vector<string> tokens) {
    return "";
}

string InstructionSet::parseXor(string line, vector<string> tokens) {
    return "";
}

string InstructionSet::parseAnd(string line, vector<string> tokens) {
    return "";
}

string InstructionSet::parseNot(string line, vector<string> tokens) {
    return "";
}

string InstructionSet::parseShift(string line, vector<string> tokens) {
    return "";
}

string InstructionSet::parseJump(string line, vector<string> tokens) {
    return "";
}

string InstructionSet::parsePush(string line, vector<string> tokens) {
    return "";
}

string InstructionSet::parsePop(string line, vector<string> tokens) {
    return "";
}

string InstructionSet::parseStop(string line, vector<string> tokens) {
    return "";
}

string InstructionSet::parseComment(string line, vector<string> tokens) {
    return "";
}
