/**
 * @file MarchingSquares.h
 * @author Antonin Thioux (antonin.thioux@gmail.com)
 * @brief Cases for marching squares using triangles instead of line segments.
 * @date last modified at 2022-06-02
 * @version 1.0
 */

#ifndef B_MARCHING_SQUARES_H
#define B_MARCHING_SQUARES_H

namespace MarchingSquares {
    static const char coords[8][2] = {
        {2, 0}, \
        {2, 1}, \
        {2, 2}, \
        {1, 2}, \
        {0, 2}, \
        {0, 1}, \
        {0, 0}, \
        {1, 0}, \
    }; 
    static const char lookup[16][9] = { \
        {-1,-1,-1,-1,-1,-1,-1,-1,-1}, \
        { 0, 1, 7,-1,-1,-1,-1,-1,-1}, \
        { 2, 3, 1,-1,-1,-1,-1,-1,-1}, \
        { 0, 2, 3, 0, 3, 7,-1,-1,-1}, \
        { 3, 4, 5,-1,-1,-1,-1,-1,-1}, \
        { 0, 1, 7, 3, 4, 5,-1,-1,-1}, \
        { 1, 2, 4, 1, 4, 5,-1,-1,-1}, \
        { 0, 2, 4, 0, 4, 5, 0, 5, 7}, \
        { 5, 6, 7,-1,-1,-1,-1,-1,-1}, \
        { 0, 1, 5, 0, 5, 6,-1,-1,-1}, \
        { 1, 2, 3, 5, 6, 7,-1,-1,-1}, \
        { 0, 2, 6, 2, 3, 5, 2, 5, 6}, \
        { 3, 4, 6, 3, 6, 7,-1,-1,-1}, \
        { 0, 4, 6, 0, 1, 3, 0, 3, 4}, \
        { 2, 4, 6, 1, 2, 6, 1, 6, 7}, \
        { 0, 2, 4, 0, 4, 6,-1,-1,-1}  \
    };
}

#endif
