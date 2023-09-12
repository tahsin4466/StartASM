#ifndef INSTRUCTIONSET_H
#define INSTRUCTIONSET_H

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

class Token;

class InstructionSet {
    public:
        //Constructor and destructor
        InstructionSet() {};
        ~InstructionSet() {
            std::unordered_map<std::string, Token*>::iterator it;
            for (it=m_tokenMap.begin(); it!=m_tokenMap.end(); it++) {
                delete it->second;
            }
        };


    private:
        //Unordered map containing all possible tokens
        std::unordered_map<std::string, Token*> m_tokenMap;

};

#endif