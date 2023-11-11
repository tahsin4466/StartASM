#include "include/InstructionSet.h"

#include <iostream>
#include <regex>
#include <string>
#include <functional>

using namespace std;

InstructionSet::InstructionSet() { 
    m_parsingMap.emplace("move", parseMove);
    m_parsingMap.emplace("load", parseLoad);
    m_parsingMap.emplace("store", parseStore);
    m_parsingMap.emplace("add", parseAdd);
    m_parsingMap.emplace("sub", parseSub);
    m_parsingMap.emplace("multiply", parseMultiply);
    m_parsingMap.emplace("divide", parseDivide);
    m_parsingMap.emplace("or", parseOr);
    m_parsingMap.emplace("and", parseAnd);
    m_parsingMap.emplace("not", parseNot);
    m_parsingMap.emplace("shift", parseShift);
    m_parsingMap.emplace("compare", parseCompare);
    m_parsingMap.emplace("jump", parseJump);
    m_parsingMap.emplace("call", parseCall);
    m_parsingMap.emplace("push", parsePush);
    m_parsingMap.emplace("pop", parsePop);
    m_parsingMap.emplace("return", parseReturn);
    m_parsingMap.emplace("stop", parseStop);
    m_parsingMap.emplace("comment", parseComment);
    m_parsingMap.emplace("label", parseLabel);

}

string InstructionSet::validateInstruction(string line, vector<string> tokens) {
    //If keyword doesn't match, return error
    auto itr = m_parsingMap.find(tokens[0]);
    if (itr == m_parsingMap.end()) {
        return "Unknown instruction '" + tokens[0] + "'";
    }

    return itr->second(line, tokens);
}

string InstructionSet::parseMove(string line, vector<string> tokens) {
    //Create regex templates for the whole line and for registers
    regex lineTemplate("move r[0-9] to r[0-9]");
    regex registerTemplate("r[0-9]");

    //Return quickly if syntax directly matches
    if (regex_match(line, lineTemplate)) {
        return ("");
    }
    //Begin error reporting process otherwise
    else {
        //Invalid token size
        if((tokens.size() != 4)) {
            return "Incomplete syntax for move instruction";
        }
        //Invalid operand format
        else if(!regex_match(tokens[1], registerTemplate)) {
            return "Unknown source '" + tokens[1] + "'. Expected register r0-r9";
        }
        //Invalid conjunction
        else if(tokens[2]!="to") {
            return "Unknown conjunction '" + tokens[2] + "'. Expected to";
        }
        else if(!regex_match(tokens[3], registerTemplate)) {
            return "Unknown destination '" + tokens[3] + "'. Expected register r0-r9";
        }
        else {
            return "Compiler exception encountered for move instruction";
        }
    }
}

string InstructionSet::parseLoad(string line, vector<string> tokens) {
    regex lineTemplate("load (m<[0-9]{1,7}>|\\d+) to r[0-9]");
    regex registerTemplate("r[0-9]");
    regex memoryTemplate("m<[0-9]{1,7}>");
    regex valueTemplate("\\d+");

    if (regex_match(line, lineTemplate)) {
        return ("");
    }
    else {
        if((tokens.size() != 4)) {
            return "Incomplete syntax for load instruction";
        }
        else if(!regex_match(tokens[1], memoryTemplate) && !regex_match(tokens[1], valueTemplate)) {
            return "Unknown source '" + tokens[1] + "'. Expected memory address m<000000-999999> or integer value";
        }
        else if(tokens[2]!="to") {
            return "Unknown conjunction '" + tokens[2] + "'. Expected to";
        }
        else if(!regex_match(tokens[3], registerTemplate)) {
            return "Unknown destination '" + tokens[3] + "'. Expected register r0-r9";
        }
        else {
            return "Compiler exception encountered for load instruction";
        }
    }
}

string InstructionSet::parseStore(string line, vector<string> tokens) {
    regex lineTemplate("store r[0-9] to m<[0-9]{1,7}>");
    regex registerTemplate("r[0-9]");
    regex memoryTemplate("m<[0-9]{1,7}>");

    if (regex_match(line, lineTemplate)) {
        return "";
    } else {
        if (tokens.size() != 4) {
            return "Incomplete syntax for store instruction";
        } 
        else if (!regex_match(tokens[1], registerTemplate)) {
            return "Unknown source '" + tokens[1] + "'. Expected register r0-r9";
        } 
        else if (tokens[2] != "to") {
            return "Unknown conjunction '" + tokens[2] + "'. Expected to";
        } 
        else if (!regex_match(tokens[3], memoryTemplate)) {
            return "Unknown destination '" + tokens[3] + "'. Expected memory address m<000000-999999>";
        } 
        else {
            return "Compiler exception encountered for store instruction";
        }
    }
}

string InstructionSet::parseAdd(string line, vector<string> tokens) {
    regex lineTemplate("add r[0-9] with r[0-9] to r[0-9]");
    regex registerTemplate("r[0-9]");

    if (regex_match(line, lineTemplate)) {
        return ("");
    }
    else {
        if((tokens.size() != 6)) {
            return "Incomplete syntax for add instruction";
        }
        else if(!regex_match(tokens[1], registerTemplate)) {
            return "Unknown source '" + tokens[1] + "'. Expected register r0-r9";
        }
        else if(tokens[2]!="with") {
            return "Unknown conjunction '" + tokens[2] + "'. Expected with";
        }
        else if(!regex_match(tokens[3], registerTemplate)) {
            return "Unknown addition argument '" + tokens[3] + "'. Expected register r0-r9";
        }
        else if(tokens[4]!="to") {
            return "Unknown conjunction '" + tokens[2] + "'. Expected to";
        }
        else if(!regex_match(tokens[5], registerTemplate)) {
            return "Unknown destination '" + tokens[5] + "'. Expected register r0-r9";
        }
        else {
            return "Compiler exception encountered for add instruction";
        }
    }
}

string InstructionSet::parseSub(string line, vector<string> tokens) {
    regex lineTemplate("sub r[0-9] with r[0-9] to r[0-9]");
    regex registerTemplate("r[0-9]");

    if (regex_match(line, lineTemplate)) {
        return ("");
    }
    else {
        if((tokens.size() != 6)) {
            return "Incomplete syntax for sub instruction";
        }
        else if(!regex_match(tokens[1], registerTemplate)) {
            return "Unknown source '" + tokens[1] + "'. Expected register r0-r9";
        }
        else if(tokens[2]!="with") {
            return "Unknown conjunction '" + tokens[2] + "'. Expected with";
        }
        else if(!regex_match(tokens[3], registerTemplate)) {
            return "Unknown subtraction argument '" + tokens[3] + "'. Expected register r0-r9";
        }
        else if(tokens[4]!="to") {
            return "Unknown conjunction '" + tokens[2] + "'. Expected to";
        }
        else if(!regex_match(tokens[5], registerTemplate)) {
            return "Unknown destination '" + tokens[5] + "'. Expected register r0-r9";
        }
        else {
            return "Compiler exception encountered for sub instruction";
        }
    }
}

string InstructionSet::parseMultiply(string line, vector<string> tokens) {
    regex lineTemplate("multiply r[0-9] with r[0-9] to r[0-9]");
    regex registerTemplate("r[0-9]");

    if (regex_match(line, lineTemplate)) {
        return ("");
    }
    else {
        if((tokens.size() != 6)) {
            return "Incomplete syntax for multiply instruction";
        }
        else if(!regex_match(tokens[1], registerTemplate)) {
            return "Unknown source '" + tokens[1] + "'. Expected register r0-r9";
        }
        else if(tokens[2]!="with") {
            return "Unknown conjunction '" + tokens[2] + "'. Expected with";
        }
        else if(!regex_match(tokens[3], registerTemplate)) {
            return "Unknown multiplication argument '" + tokens[3] + "'. Expected register r0-r9";
        }
        else if(tokens[4]!="to") {
            return "Unknown conjunction '" + tokens[2] + "'. Expected to";
        }
        else if(!regex_match(tokens[5], registerTemplate)) {
            return "Unknown destination '" + tokens[5] + "'. Expected register r0-r9";
        }
        else {
            return "Compiler exception encountered for multiply instruction";
        }
    }
}

string InstructionSet::parseDivide(string line, vector<string> tokens) {
    regex lineTemplate("divide r[0-9] with r[0-9] to r[0-9]");
    regex registerTemplate("r[0-9]");

    if (regex_match(line, lineTemplate)) {
        return ("");
    }
    else {
        if((tokens.size() != 6)) {
            return "Incomplete syntax for divide instruction";
        }
        else if(!regex_match(tokens[1], registerTemplate)) {
            return "Unknown source '" + tokens[1] + "'. Expected register r0-r9";
        }
        else if(tokens[2]!="with") {
            return "Unknown conjunction '" + tokens[2] + "'. Expected with";
        }
        else if(!regex_match(tokens[3], registerTemplate)) {
            return "Unknown division argument '" + tokens[3] + "'. Expected register r0-r9";
        }
        else if(tokens[4]!="to") {
            return "Unknown conjunction '" + tokens[2] + "'. Expected to";
        }
        else if(!regex_match(tokens[5], registerTemplate)) {
            return "Unknown destination '" + tokens[5] + "'. Expected register r0-r9";
        }
        else {
            return "Compiler exception encountered for divide instruction";
        }
    }
}

string InstructionSet::parseOr(string line, vector<string> tokens) {
    regex lineTemplate("or r[0-9] with r[0-9]");
    regex registerTemplate("r[0-9]");

    if (regex_match(line, lineTemplate)) {
        return ("");
    }
    else {
        if((tokens.size() != 4)) {
            return "Incomplete syntax for or instruction";
        }
        else if(!regex_match(tokens[1], registerTemplate)) {
            return "Unknown source/destination '" + tokens[1] + "'. Expected register r0-r9";
        }
        else if(tokens[2]!="with") {
            return "Unknown conjunction '" + tokens[2] + "'. Expected with";
        }
        else if(!regex_match(tokens[3], registerTemplate)) {
            return "Unknown argument '" + tokens[3] + "'. Expected register r0-r9";
        }
        else {
            return "Compiler exception encountered for or instruction";
        }
    }
}

string InstructionSet::parseAnd(string line, vector<string> tokens) {
    regex lineTemplate("and r[0-9] with r[0-9]");
    regex registerTemplate("r[0-9]");

    if (regex_match(line, lineTemplate)) {
        return ("");
    }
    else {
        if((tokens.size() != 4)) {
            return "Incomplete syntax for and instruction";
        }
        else if(!regex_match(tokens[1], registerTemplate)) {
            return "Unknown source/destination '" + tokens[1] + "'. Expected register r0-r9";
        }
        else if(tokens[2]!="with") {
            return "Unknown conjunction '" + tokens[2] + "'. Expected with";
        }
        else if(!regex_match(tokens[3], registerTemplate)) {
            return "Unknown argument '" + tokens[3] + "'. Expected register r0-r9";
        }
        else {
            return "Compiler exception encountered for and instruction";
        }
    }
}

string InstructionSet::parseNot(string line, vector<string> tokens) {
    regex lineTemplate("not r[0-9]");
    regex registerTemplate("r[0-9]");

    if (regex_match(line, lineTemplate)) {
        return ("");
    }
    else {
        if((tokens.size() != 2)) {
            return "Incomplete syntax for not instruction";
        }
        else if(!regex_match(tokens[1], registerTemplate)) {
            return "Unknown source/destination '" + tokens[1] + "'. Expected register r0-r9";
        }
        else {
            return "Compiler exception encountered for not instruction";
        }
    }
}

string InstructionSet::parseShift(string line, vector<string> tokens) {
    regex lineTemplate("shift (left|right) (logically|arithmetically) r[0-9] by r[0-9]");
    regex registerTemplate("r[0-9]");

    if (regex_match(line, lineTemplate)) {
        return ("");
    }
    else {
        if((tokens.size() != 6)) {
            return "Incomplete syntax for shift instruction";
        }
        //Check for multiple tokens (either left or right)
        else if(tokens[1]!="left" && tokens[1]!="right") {
            return "Unknown shift direction '" + tokens[1] + "'. Expected 'left' or 'right''";
        }
        //Also check for multiple tokens (either logically or arithmetically)
        else if(tokens[2]!="logically" && tokens[2]!="arithmetically") {
            return "Unknown shift type '" + tokens[2] + "'. Expected 'logically' or 'arithmetically'";
        }
        else if(!regex_match(tokens[3], registerTemplate)) {
            return "Unknown source/destination '" + tokens[3] + "'. Expected register r0-r9";
        }
        else if(tokens[4]!="by") {
            return "Unknown conjunction '" + tokens[4] + "'. Expected by";
        }
        else if(!regex_match(tokens[5], registerTemplate)) {
            return "Unknown shift amount '" + tokens[5] + "'. Expected register r0-r9";
        }
        else {
            return "Compiler exception encountered for divide instruction";
        }
    }
}

string InstructionSet::parseCompare(string line, vector<string> tokens) {
    regex lineTemplate("compare r[0-9] with r[0-9]");
    regex registerTemplate("r[0-9]");

    if (regex_match(line, lineTemplate)) {
        return ("");
    }
    else {
        if((tokens.size() != 4)) {
            return "Incomplete syntax for compare instruction";
        }
        else if(!regex_match(tokens[1], registerTemplate)) {
            return "Unknown source/destination '" + tokens[1] + "'. Expected register r0-r9";
        }
        else if(tokens[2]!="with") {
            return "Unknown conjunction '" + tokens[2] + "'. Expected with";
        }
        else if(!regex_match(tokens[3], registerTemplate)) {
            return "Unknown argument '" + tokens[3] + "'. Expected register r0-r9";
        }
        else {
            return "Compiler exception encountered for compare instruction";
        }
    }
}

string InstructionSet::parseJump(string line, vector<string> tokens) {
    //First check if line is under three tokens to prevent access errors
    if (tokens.size() < 4) {
        return "Incomplete syntax for jump instruction";
    }
    //Call helper functions depending on the first token
    //Unconditional jumps always start with "unconditionally"
    else if (tokens[1] == "unconditionally") {
        return(parseUnconditionalJump(line, tokens));
    }
    //Check if it is a conditional
    else if (tokens[1] == "if") {
        //Call helper function for not if "not" is found
        if (tokens[2] == "not") {
            return (parseConditionalComplementJump(line, tokens));
        }
        //Regular conditional jump otherwise
        else {
            return(parseConditionalJump(line, tokens));
        }
    }
    else {
        return "Unknown jump condition. Expected 'greater', 'less', 'equal', 'not' or unconditional";
    }
}

string InstructionSet::parseCall(string line, vector<string> tokens) {
    regex lineTemplate("call to (i\\[[0-9]{1,7}\\]|'([^']+)')");
    regex instructionTemplate("i\\[[0-9]{1,7}\\]");
    regex labelTemplate("'([^']+)'");

    if (regex_match(line, lineTemplate)) {
        return ("");
    }
    else {
        if((tokens.size() != 3)) {
            return "Incomplete syntax for call instruction";
        }
        else if(tokens[1]!="to") {
            return "Unknown conjunction '" + tokens[1] + "'. Expected to";
        }
        else if(!regex_match(tokens[2], instructionTemplate) && !regex_match(tokens[2], labelTemplate)) {
            return "Unknown destination '" + tokens[2] + "'. Expected instruction address i[000000-999999] or 'label' (single word)";
        }
        else {
            return "Compiler exception encountered for call instruction";
        }
    }
}

string InstructionSet::parsePush(string line, vector<string> tokens) {
    regex lineTemplate("push r[0-9]");
    regex registerTemplate("r[0-9]");

    if (regex_match(line, lineTemplate)) {
        return ("");
    }
    else {
        if((tokens.size() != 2)) {
            return "Incomplete syntax for push instruction";
        }
        else if(!regex_match(tokens[1], registerTemplate)) {
            return "Unknown source '" + tokens[1] + "'. Expected register r0-r9";
        }
        else {
            return "Compiler exception encountered for push instruction";
        }
    }
}

string InstructionSet::parsePop(string line, vector<string> tokens) {
    regex lineTemplate("pop to r[0-9]");
    regex registerTemplate("r[0-9]");

    if (regex_match(line, lineTemplate)) {
        return ("");
    }
    else {
        if((tokens.size() != 3)) {
            return "Incomplete syntax for pop instruction";
        }
        else if(tokens[1]!="to") {
            return "Unknown conjunction '" + tokens[1] + "'. Expected to";
        }
        else if(!regex_match(tokens[2], registerTemplate)) {
            return "Unknown destination '" + tokens[2] + "'. Expected register r0-r9";
        }
        else {
            return "Compiler exception encountered for pop instruction";
        }
    }
}

string InstructionSet::parseReturn(string line, vector<string> tokens) {
    //No operands, so only line regex template
    regex lineTemplate("return");

    //This code is mostly redundant but here just in case there's a regex error. Should never
    //lead to an error if the parseReturn function is called
    if(regex_match(line, lineTemplate)) {
        return ("");
    }
    else {
        if((tokens.size() != 1)) {
            return "Incomplete syntax for return instruction";
        }
        else {
            return "Compiler exception encountered for return instruction";
        }
    }
}

string InstructionSet::parseStop(string line, vector<string> tokens) {
    regex lineTemplate("stop");

    if(regex_match(line, lineTemplate)) {
        return ("");
    }
    else {
        if((tokens.size() != 1)) {
            return "Incomplete syntax for stop instruction";
        }
        else {
            return "Compiler exception encountered for stop instruction";
        }
    }
}

string InstructionSet::parseLabel(string line, vector<string> tokens) {
    regex lineTemplate("label '([^']+)'");
    regex labelTemplate("'([^']+)'");

    if(regex_match(line, lineTemplate)) {
        return ("");
    }
    else {
        if (!regex_match(tokens[1], labelTemplate)) {
            return "Unrecognized 'label'. Must be single word in single-quotes";
        }
        else {
            return "Compiler exception encountered for label";
        }
    }
}

string InstructionSet::parseComment(string line, vector<string> tokens) {
    regex lineTemplate(R"(comment ".+")");
    

    if(regex_match(line, lineTemplate)) {
        return ("");
    }
    else {
        //If first character on token after "comment" is not a double quote
        if (tokens[1][0] != '"') {
            return "Expected starting double quotes in '" + tokens[1] + "'";
        }
        //If last character on last token is not a double quote
        else if (tokens[tokens.size()-1].back() != '"') {
            return "Expected ending double quotes in '" + tokens[tokens.size()-1] + "'";
        }
        else {
            return "Compiler exception encountered for comment";
        }
    }
}



//Jump parsing helper function
string InstructionSet::parseUnconditionalJump(string line, vector<string> tokens) {
    regex lineTemplate("jump unconditionally to (i\\[[0-9]{1,7}\\]|'([^']+)')");
    regex instructionTemplate("i\\[[0-9]{1,7}\\]");
    regex labelTemplate("'([^']+)'");

    if (regex_match(line, lineTemplate)) {
        return ("");
    }
    else {
        if((tokens.size() != 4)) {
            return "Incomplete syntax for unconditional jump instruction";
        }
        else if(tokens[1]!="unconditionally") {
            return "Unknown condition '" + tokens[1] + "'. Expected 'unconditionally'";
        }
        else if(tokens[2]!="to") {
            return "Unknown conjunction '" + tokens[2] + "'. Expected to";
        }
        else if(!regex_match(tokens[3], instructionTemplate) && !regex_match(tokens[3], labelTemplate)) {
            return "Unknown destination '" + tokens[3] + "'. Expected instruction address i[000000-999999] or 'label' (single word)";
        }
        else {
            return "Compiler exception encountered for unconditional jump instruction";
        }
    }
}

string InstructionSet::parseConditionalJump(string line, vector<string> tokens) {
    regex lineTemplate("jump if (greater|less|equal|zero) to (i\\[[0-9]{1,7}\\]|'([^']+)')");
    regex instructionTemplate("i\\[[0-9]{1,7}\\]");
    regex labelTemplate("'([^']+)'");

    if (regex_match(line, lineTemplate)) {
        return ("");
    }
    else {
        if((tokens.size() != 5)) {
            return "Incomplete syntax for conditional jump instruction";
        }
        else if(tokens[1]!="if") {
            return "Unknown conjunction '" + tokens[1] + "'. Expected if";
        }
        else if (tokens[2]!="greater" && tokens[2]!="less" && tokens[2]!="equal" && tokens[2]!="zero") {
            return "Unknown condition '" + tokens[2] + "'. Expected 'greater', 'less', 'equal' or 'zero'";
        }
        else if (tokens[3]!="to") {
            return "Unknown conjunction '" + tokens[3] + "'. Expected to";
        }
        else if(!regex_match(tokens[4], instructionTemplate) && !regex_match(tokens[4], labelTemplate)) {
            return "Unknown destination '" + tokens[4] + "'. Expected instruction address i[000000-999999] or 'label' (single word)";
        }
        else {
            return "Compiler exception encountered for conditional jump instruction";
        }
    }
}

string InstructionSet::parseConditionalComplementJump(string line, vector<string> tokens) {
    regex lineTemplate("jump if not (equal|zero) to (i\\[[0-9]{1,7}\\]|'([^']+)')");
    regex instructionTemplate("i\\[[0-9]{1,7}\\]");
    regex labelTemplate("'([^']+)'");

    if (regex_match(line, lineTemplate)) {
        return ("");
    }
    else {
        if((tokens.size() != 6)) {
            return "Incomplete syntax for complemented conditional jump instruction";
        }
        else if(tokens[1]!="if") {
            return "Unknown conjunction '" + tokens[1] + "'. Expected 'if'";
        }
        else if(tokens[2]!="not") {
            return "Unknown conjunction '" + tokens[2] + "'. Expected to";
        }
        else if (tokens[3]!="equal" && tokens[3]!="zero") {
            return "Unknown condition '" + tokens[3] + "'. Expected'equal' or 'zero'";
        }
        else if (tokens[4]!="to") {
            return "Unknown conjunction '" + tokens[4] + "'. Expected to";
        }
        else if(!regex_match(tokens[5], instructionTemplate) && !regex_match(tokens[5], labelTemplate)) {
            return "Unknown destination '" + tokens[5] + "'. Expected instruction address i[000000-999999] or 'label' (single word)";
        }
        else {
            return "Compiler exception encountered for complemented conditional jump instruction";
        }
    }
}
