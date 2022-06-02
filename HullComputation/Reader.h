/**
 * @file Reader.h
 * @author Antonin Thioux (antonin.thioux@gmail.com)
 * @brief Header file for Reader.cpp
 * @date last modified at 2022-06-02
 * @version 1.0
 */

#ifndef BP_READER_H
#define BP_READER_H

#include <arrayfire.h>
#include "Parameters.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace HullComputation{
    class Reader {
    private:
        Parameters *params;
        af::array batch;
        int batchNum, pointer;

    public:
        Reader(Parameters *params);
        af::array readFile(std::string file);
        af::array getNextBatch();
        af::array getAnimation();
    };    
}

#endif
