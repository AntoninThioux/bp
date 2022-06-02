/**
 * File: Shapes.cpp
 * Author: Antonin Thioux (antonin.thioux@gmail.com)
 * Updated: 19-08-2021
 */

#ifndef BP_SAMPLER_H
#define BP_SAMPLER_H

#include <iostream>
#include <fstream>
#include <random>

#include "Shapes.h"

namespace DataGeneration {
    class Sampler {
    private:
        int height;
        int width;
        int depth;
        int frames;
        double noise;
        unsigned char (*shapeFunc)(double, double, double, double);
    public:
        Sampler(int is3D, int isNoisy);
        void run();
    };
}

#endif
