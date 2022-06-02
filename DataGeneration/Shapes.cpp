/**
 * File: Shapes.cpp
 * Author: Antonin Thioux (antonin.thioux@gmail.com)
 * Updated: 19-08-2021
 * About: This file contains functions defining Spatio-Temporal shapes.
 */

#include "Shapes.h"

using namespace DataGeneration;

/**
 * This function defines a circle/sphere that expands with time.
 * @param x The 1st dimension of space [0-1].
 * @param y The 2nd dimension of space [0-1].
 * @param z The 3rd dimension of space [0-1].
 * @param t The dimension of time [0-1].
 * @return 0xFF if point {x,y,z,t} is contained by the shape, otherwise 0x00.
 */
unsigned char Shapes::circle(double x, double y, double z, double t) {
    double r = 0.9;
    if (t < 0.5) r = t + 0.4;
    x = 2 * x - 1;
    y = 2 * y - 1;
    z = 2 * z - 1;
    return (r * r >= x * x + y * y + z * z) * 0xFF;
}

/**
 * This function defines a heart that beats with time.
 * @param x The 1st dimension of space [0-1].
 * @param y The 2nd dimension of space [0-1].
 * @param z The 3rd dimension of space [0-1].
 * @param t The dimension of time [0-1].
 * @return 0xFF if point {x,y,z,t} is contained by the shape, otherwise 0x00.
 */
unsigned char Shapes::heart(double x, double y, double z, double t) {
    double r;
    if (t < 0.125) r = 8 * t + 1;
    else if (t < 0.25) r = -12 * (t - 0.125) + 2;
    else if (t < 0.375) r = 8 * (t - 0.25) + 0.5;
    else if (t < 0.5) r = -4 * (t - 0.375) + 1.5;
    else r = 1;
    x = (2 * x - 1) / r * 3;
    y = (2 * y - 1) / r * 3;
    z = (2 * z - 1) / r * 3;
    return (pow(x * x + 9 * z * z / 4 + y * y - 1, 3) - x * x * y * y * y - 9 * z * z * y * y * y / 80 < 0) * 0xFF;
}

/**
 * This function defines a spinner that spins with time.
 * @param x The 1st dimension of space [0-1].
 * @param y The 2nd dimension of space [0-1].
 * @param z The 3rd dimension of space [0-1].
 * @param t The dimension of time [0-1].
 * @return 0xFF if point {x,y,z,t} is contained by the shape, otherwise 0x00.
 */
unsigned char Shapes::spinner(double x, double y, double z, double t) {
    double p = M_PI * t;
    x = 2 * x - 1;
    y = 2 * y - 1;
    z = 2 * z - 1;
    return (sqrt(x * x + y * y + z * z) < sin(3 * (atan2(y, x) + p))) * 0xFF;
}

/**
 * This function defines a square that slides with time.
 * @param x The 1st dimension of space [0-1].
 * @param y The 2nd dimension of space [0-1].
 * @param z The 3rd dimension of space [0-1].
 * @param t The dimension of time [0-1].
 * @return 0xFF if point {x,y,z,t} is contained by the shape, otherwise 0x00.
 */
unsigned char Shapes::square(double x, double y, double z, double t) {
    if (0.0000 < t && t < 0.1667) {
        x = 2 * x - 1.6667;
        y = 2 * y - 1.6667;
        z = 2 * z - 1.6667 + 4 * t;
    }
    else if (0.1667 < t && t < 0.5000) {
        x = 2 * x - 1.6667 + 4 * (t - 0.1667);
        y = 2 * y - 1.6667;
        z = 2 * z - 1.0000;
    }
    else if (0.5000 < t && t < 0.8333) {
        x = 2 * x - 0.3333;
        y = 2 * y - 1.6667 + 4 * (t - 0.5000);
        z = 2 * z - 1.0000;
    }
    else {
        x = 2 * x - 0.3333;
        y = 2 * y - 0.3333;
        z = 2 * z - 1.0000 + 4 * (t - 0.8333);
    }
    return (fabs(x) < 0.1 && fabs(y) < 0.1 && fabs(z) < 0.1) * 0xFF;
}

/**
 * This function defines a star that twinkles with time.
 * @param x The 1st dimension of space [0-1].
 * @param y The 2nd dimension of space [0-1].
 * @param z The 3rd dimension of space [0-1].
 * @param t The dimension of time [0-1].
 * @return 0xFF if point {x,y,z,t} is contained by the shape, otherwise 0x00.
 */
unsigned char Shapes::star(double x, double y, double z, double t) {
    double r = (t < 0.5) ? 2 * t : 2 * (1 - t);
    x = 2 * x - 1;
    y = 2 * y - 1;
    z = 2 * z - 1;
    return (sqrt(fabs(r)) > sqrt(fabs(x)) + sqrt(fabs(y)) + sqrt(fabs(z))) * 0xFF;
}
