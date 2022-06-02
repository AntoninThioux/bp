/**
 * @file Calc.h
 * @author Antonin Thioux (antonin.thioux@gmail.com)
 * @brief Header file to Calc.cpp 
 * @date last modified at 2022-06-02
 * @version 1.0
 */

#ifndef BP_CALC_H
#define BP_CALC_H

#include <arrayfire.h>

#include "Parameters.h"

namespace HullComputation{
    class Calc
    {
    private:
        Parameters *params;
        af::array hulls;
        int t;
        af::array derivative(af::array M, int dim);
        af::array guassian(af::array M, int dim);
        af::array sobelD2(af::array M, int dev1, int dev2);
        af::array sobelD3(af::array M, int dev1, int dev2, int dev3);
        void flattenAndReduce(af::array spacetime);

    public:
        Calc(Parameters *params);
        void processBatch(af::array batch);
        af::array getHulls();
    };
}

#endif
