#include "include/Compiler.h"
#include <iostream>
#include <omp.h>
#include <string>
#include <algorithm>

using namespace std;

// Function to check if a command-line option exists
bool cmdOptionExists(char** begin, char** end, const string& option) {
    return find(begin, end, option) != end;
}

// Function to check for valid .sasm file extension
bool isValidSASMFile(const string& filename) {
    if (filename.length() >= 5) {
        return filename.substr(filename.length() - 5) == ".sasm";
    }
    return false;
}

void displayHelp() {
    cout << "StartASM Compiler Usage:" << endl;
    cout << "  startasm compile <filepath.sasm> [options]" << endl;
    cout << "Options:" << endl;
    cout << "  --help        Display this help message and exit" << endl;
    cout << "  --timings     Print out timings for each compilation step" << endl;
    cout << "  --tree        Print out the AST (Abstract Syntax Tree)" << endl;
    cout << "  --silent      Suppress output (except syntax errors)" << endl;
    cout << "  --truesilent  Suppress all output, including syntax errors" << endl;
    cout << "Note that the use of --silent or --truesilent will override output flags such as --tree and --timings." << endl;
}

int main(int argc, char* argv[]) {
    // Check for --help flag before anything else
    if (cmdOptionExists(argv, argv + argc, "--help")) {
        displayHelp();
        return 0;
    }

    // Requires at least 2 arguments: the command and the filepath
    if (argc < 3) {
        if (!cmdOptionExists(argv, argv + argc, "--truesilent")) {
            cerr << "Missing arguments." << endl;
            cerr << "For usage information: startasm --help" << endl;
        }
        return 1;
    }

    string command(argv[1]);
    if (command != "compile") {
        if (!cmdOptionExists(argv, argv + argc, "--truesilent")) {
            cerr << "Unknown command: " << command << endl;
            cerr << "For usage information: startasm --help" << endl;
        }
        return 1;
    }

    string filepath(argv[2]);
    if (!isValidSASMFile(filepath) && !cmdOptionExists(argv, argv + argc, "--truesilent")) {
        cerr << "Error: The file must have a .sasm extension." << endl;
        return 1;
    }

    // Flags
    bool timings = cmdOptionExists(argv, argv + argc, "--timings");
    bool tree = cmdOptionExists(argv, argv + argc, "--tree");
    bool silent = cmdOptionExists(argv, argv + argc, "--silent") || cmdOptionExists(argv, argv + argc, "--truesilent");
    bool truesilent = cmdOptionExists(argv, argv + argc, "--truesilent");

    // Adjust the compiler instantiation to pass the truesilent flag
    Compiler StartASMCompiler(filepath, silent, timings, tree);
    double start = omp_get_wtime();
    if (!StartASMCompiler.compileCode() && !truesilent) {
        cout << StartASMCompiler.getStatus() << endl;
    }
    double end = omp_get_wtime();

    if (timings && !silent) {
        cout << "Total time taken: " << (end - start) << " seconds\n";
    }

    if (!silent) {
        cout << StartASMCompiler.getNumLines() << " lines compiled.\n";
    }

    return 0;
}
