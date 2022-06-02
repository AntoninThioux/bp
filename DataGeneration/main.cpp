/**
 * File: main.cpp
 * Author: Antonin Thioux (antonin.thioux@gmail.com)
 * Updated: 19-08-2021
 * About: This file processes the flags for the sample program that allows users to sample Spatio-Temporal data.
 */

#include <iostream>
#include <cstring>

#include "Sampler.h"

using namespace std;
using namespace DataGeneration;

/**
 * This function prints the help display to stdout.
 */
void printHelp() {
    cout << "Usage: sample [OPTIONS]" << endl;
    cout << "OPTIONS are:" << endl;
    cout << "\t -2, --2D \t\t generates in 2 dimensions of space instead of 3" << endl;
    cout << "\t -n, --noise \t Adds noise to the Spatio-Temporal Data" << endl;
    exit(0);
}

/**
 * This function prints an error message in case of an incorrect flag.
 * @param flag The flag in question.
 */
void printError(char *flag) {
    cerr << "Error: invalid COMMAND " << flag << endl;
    cerr << "\t sample -h \t\t for help" << endl;
    exit(9);
}

/**
 * This is the main function for the sample program.
 * @param argc The number of arguments.
 * @param argv The array of arguments.
 * @return Exit success when sampling is complete.
 */
int main(int argc, char *argv[]) {
    int is3D = 1, isNoisy = 0;

    for (int i = 1; i < argc; i++) {
        char *flag = argv[i];
        if (!strcmp(flag, "-h") || !strcmp(flag, "--help")) printHelp();
        else if (!strcmp(flag, "-2") || !strcmp(flag, "--2D")) is3D = 0;
        else if (!strcmp(flag, "-n") || !strcmp(flag, "--noise")) isNoisy = 1;
        else printError(flag);
    }

    Sampler sampler(is3D, isNoisy);
    sampler.run();
    return 0;
}
