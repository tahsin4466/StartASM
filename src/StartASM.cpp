#include "include/Compiler.h"

#include <iostream>
#include <omp.h>
#include <string>
using namespace std;

int main() {
    string pathname;
    cout << "Enter name of StartASM file: ";
    getline(cin, pathname);

    double start = omp_get_wtime();
    double end;
    Compiler StartASMCompiler(pathname);

    if(!StartASMCompiler.compileCode()) {
        cout << StartASMCompiler.getStatus() << endl;
    }

    end = omp_get_wtime();
    cout << StartASMCompiler.getNumLines() << " lines" << endl;
    cout << "Time taken: " << (end - start) << " seconds" << endl;
    return 0;
}