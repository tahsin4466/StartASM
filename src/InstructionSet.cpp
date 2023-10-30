#include "include/InstructionSet.h"

#include <iostream>
#include <regex>
#include <string>
#include <sstream>

using namespace std;

InstructionSet::InstructionSet() {
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
    m_templateMap.emplace("shift", vector<string>{"shift left arithmetically r[0-9] by [0-9]", "shift left logically r[0-9] by [0-9]", "shift right arithmetically r[0-9] by [0-9]", "shift right logically r[0-9] by [0-9]"});
    m_templateMap.emplace("jump", vector<string>{"jump to i[0-9]+", "jump if greater to i[0-9]+", "jump if less to i[0-9]+", "jump if equal to i[0-9]+", "jump if not equal to i[0-9]+", "jump if zero to i[0-9]+", "jump if not zero to i[0-9]+"});
    m_templateMap.emplace("push", vector<string>{"push r[0-9]"});
    m_templateMap.emplace("pop", vector<string>{"pop to r[0-9]"});
    m_templateMap.emplace("stop", vector<string>{"stop"});
    m_templateMap.emplace("comment", vector<string>{"comment \"[^\"]*\""});  
}

bool InstructionSet::isValidInstruction(string line) {
    // Extract the keyword from the line
    std::istringstream iss(line);
    std::string keyword;
    iss >> keyword;

    auto itr = m_templateMap.find(keyword);
    if (itr == m_templateMap.end()) {
        return false;
    }

    vector<string> templates = itr->second;
    for (string& templateStr : templates) {
        regex regexTemplate(templateStr);
        if (regex_match(line, regexTemplate)) {
            return true;
        }
    }

    return false;

}