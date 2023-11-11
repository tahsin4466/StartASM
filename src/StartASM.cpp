#include "include/Compiler.h"

#include <iostream>
#include <string>
using namespace std;

int main() {
    string pathname;
    cout << "Enter name of StartASM file: ";
    getline(cin, pathname);

    Compiler StartASMCompiler = Compiler("code/" + pathname + ".txt");
    if(!StartASMCompiler.compileCode()) {
        cout << StartASMCompiler.getStatus() << endl;
        return 0;
    }

    cout << StartASMCompiler.getNumLines() << " lines" << endl;
    return 0;
}