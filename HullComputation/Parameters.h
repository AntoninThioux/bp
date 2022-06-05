/**
 * @file Parameters.h
 * @author Antonin Thioux (antonin.thioux@gmail.com)
 * @brief Header file of Parameters.cpp
 * @date last modified at 2022-06-02
 * @version 1.0
 */

#ifndef BP_PARAMETERS_H
#define BP_PARAMETERS_H

#include <fstream>
#include <iostream>
#include <cstring>
#include <sstream>

namespace HullComputation{
    class Parameters {
    private:
        std::ifstream file;
        void parseFile(std::string filepath);
        void parseOptions(int n, char **options);
        void checkParameters();
        void printHelp();
        void printError(const char *error);
        
    public:
        Parameters(int argc, char *argv[]);
        ~Parameters();
        int isViewed, viewSlice;
        int isTimed, batches;
        int kx, ky, kz, kt, threshold, special;
        int width, height, depth, duration, is4D;
        int exportAnimation;
        int x, y;
        std::string *datafiles;
    };
}

#endif
