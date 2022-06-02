/**
 * File: Shapes.h
 * Author: Antonin Thioux (s3791378)
 * Updated: 19-08-2021
 */

#ifndef BP_SHAPES_H
#define BP_SHAPES_H

#include <cmath>

namespace DataGeneration {
    class Shapes {
    public:
        static unsigned char circle(double x, double y, double z, double t);
        static unsigned char heart(double x, double y, double z, double t);
        static unsigned char spinner(double x, double y, double z, double t);
        static unsigned char square(double x, double y, double z, double t);
        static unsigned char star(double x, double y, double z, double t);
    };
}


#endif
