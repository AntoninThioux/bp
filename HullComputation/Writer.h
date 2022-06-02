/**
 * @file Writer.h
 * @author Antonin Thioux (antonin.thioux@gmail.com)
 * @brief Header to Writer.cpp
 * @date last modified at 2022-06-02
 * @version 1.0
 */

#ifndef BP_WRITER_H
#define BP_WRITER_H

#include <arrayfire.h>
#include <sstream>
#include <iostream>
#include <fstream>

#include "Parameters.h"
#include "Reader.h"
#include "MarchingSquares.h"
#include "MarchingCubes.h"

namespace HullComputation{
    class Writer
    {
    private:
        Parameters *params;
        int vertexCount, vertexSize;
        float *coords, *normals, *colors;
        int *mesh, faceCount, meshSize;
        // Helper functions
        void resizeVertexes();
        void resizeMesh();
        void normalizeNormals();
        void scaleCoords(int width, int height, int depth = 1);
        int ***createVertexMap(int width, int height, int depth = 1);
        void freeVertexMap(int ***map, int width, int height);
        float *getNormal(int p0, int p1, int p2);
        // primary functions
        void extractAnimation();
        void squareCase(int ***vmap, int **cases, float **vals, int x, int y, int isColored);
        void marchingSquares(af::array M, int isColored);
        void cubeCase(int ***vmap, int ***cases, float ***vals, int x, int y, int z, int isColored);
        void marchingCubes(af::array M, int isColored);
        void output(std::string filename);

    public:
        Writer(Parameters *params);
        ~Writer();
        void extract(af::array hulls);
    };
}

#endif
