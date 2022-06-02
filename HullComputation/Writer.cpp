/**
 * @file Writer.cpp
 * @author Antonin Thioux (antonin.thioux@gmail.com)
 * @brief This file contains the logic for extracting the spatio-temporal hulls.
 * @date last modified at 2022-06-02
 * @version 1.0
 */

#include "Writer.h"

#define INTIAL_SIZE 300
#define EPSILON 0.001
#define COLORLESS 0.38431372549

using namespace HullComputation;
using namespace af;
using namespace std;

/**
 * @brief This helper function resizes the different vertex arrays.
 */
void Writer::resizeVertexes() {
    vertexSize *= 2;
    float *newCoords = new float[vertexSize];
    float *newNormals = new float[vertexSize];
    float *newColors = new float[vertexSize];

    for (int i = 0; i < vertexSize / 2; i++) {
        newCoords[i] = coords[i];
        newNormals[i] = normals[i];
        newColors[i] = colors[i];
    }

    delete [] coords;
    coords = newCoords;
    delete [] normals;
    normals = newNormals;
    delete [] colors;
    colors = newColors;

    for (int i = vertexSize / 2; i < vertexSize; i++)
        coords[i] = colors[i] = normals[i] = 0;
}

/**
 * @brief This helper function resizes the mesh array.
 */
void Writer::resizeMesh() {
    meshSize *= 2;
    int *newMesh = new int[meshSize];
    
    for (int i = 0; i < meshSize / 2; i++)
        newMesh[i] = mesh[i];

    delete [] mesh;
    mesh = newMesh;
    for (int i = meshSize / 2; i < meshSize; i++)
        mesh[i] = 0;
}

/**
 * @brief This helper function scales the coords such that they all fall between 0 and 1.
 * @param width The width to scale with.
 * @param height The height to scale with.
 * @param depth The depth to scale with.
 */
void Writer::scaleCoords(int width, int height, int depth) {
    for (int i = 0; i < vertexCount * 3; i += 3) {
        coords[i] /= width * 2;
        coords[i+1] /= height * 2;
        coords[i+2] /= depth * 2;
    }
}

/**
 * @brief This helper function normalizes the normal vectors.
 */
void Writer::normalizeNormals() {
    for (int i = 0; i < vertexCount * 3; i += 3) {
        float m = normals[i] * normals[i];
        m += normals[i+1] * normals[i+1];
        m += normals[i+2] * normals[i+2];
        m = sqrt(m);
        normals[i] /= m;
        normals[i+1] /= m;
        normals[i+2] /= m;
    }
}

/**
 * @brief This helper function creates a vertex map to keep track faces with the same vertex(es).
 * @param width The width of the (volume) image.
 * @param height The height of the (volume) image.
 * @param depth The depth of the volume image.
 * @return int*** The vertex map as 3D array.
 */
int *** Writer::createVertexMap(int width, int height, int depth){
    int ***map = new int**[width * 2 - 1];
    for (int x = 0; x < width * 2 - 1; x++) {
        map[x] = new int*[height * 2 - 1];
        for (int y = 0; y < height * 2 - 1; y++){
            map[x][y] = new int[depth * 2 - 1];
            for (int z = 0; z < depth * 2 - 1; z++)
                map[x][y][z] = 0;
        }
    }
    return map;
}

/**
 * @brief This helper function frees the vertex map.
 * @param map The vertex map.
 * @param width The width of the original (volume) image.
 * @param height The height of the original (volume) image.
 */
void Writer::freeVertexMap(int ***map, int width, int height){
    for (int x = 0; x < width * 2 - 1; x++) {
        for (int y = 0; y < height * 2 - 1; y++)
            delete [] map[x][y];
        delete [] map[x];
    }
    delete [] map;
}

/**
 * @brief This helper function computes the normal to a triangle given 3 points
 * @param p0 index of marching cubes point 0 in triangle.
 * @param p1 index of marching cubes point 1 in triangle.
 * @param p2 index of marching cubes point 2 in triangle.
 * @return float* array of integers size 3 representing the normal.
 */
float *Writer::getNormal(int p0, int p1, int p2){
    int p0x = MarchingCubes::coords[p0][0], p0y = MarchingCubes::coords[p0][1], p0z = MarchingCubes::coords[p0][2];
    int p1x = MarchingCubes::coords[p1][0], p1y = MarchingCubes::coords[p1][1], p1z = MarchingCubes::coords[p1][2];
    int p2x = MarchingCubes::coords[p2][0], p2y = MarchingCubes::coords[p2][1], p2z = MarchingCubes::coords[p2][2];

    // N = P0P1 x P0P2
    float *normal = new float[3];
    normal[0] = (p1y-p0y) * (p2z-p0z) - (p1z-p0z) * (p2y-p0y);
    normal[1] = (p1z-p0z) * (p2x-p0x) - (p1x-p0x) * (p2z-p0z);
    normal[2] = (p1x-p0x) * (p2y-p0y) - (p1y-p0y) * (p2x-p0x);

    float m = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
    normals[0] /= m;
    normals[1] /= m;
    normals[2] /= m;

    return normal;
}

/**
 * @brief This function processes a cube case in marching cubes algorithm.
 * @param vmap The vertex map.
 * @param cases The case at each x y z coord.
 * @param vals The value at each x y z coord (used for coloring).
 * @param x The x coord.
 * @param y The y coord.
 * @param z The z coord.
 * @param isColored If true colors according to value else grey.
 */
void Writer::cubeCase(int ***vmap, int ***cases, float ***vals, int x, int y, int z, int isColored) {
    const char *faces = MarchingCubes::lookup[cases[x][y][z]];

    // iterate over triangle
    for (int i = 0; i < 15; i += 3) {
        if (faces[i] == -1) break;

        if ((++faceCount) * 3 > meshSize)
                resizeMesh();

        float *normal = getNormal(faces[i], faces[i + 1], faces[i + 2]);

        // iterate over all vetecies of triangle
        for (int j = 0; j < 3; j++) {
            int v;
            int vx = MarchingCubes::coords[faces[i + j]][0] + 2 * x;
            int vy = MarchingCubes::coords[faces[i + j]][1] + 2 * y;
            int vz = MarchingCubes::coords[faces[i + j]][2] + 2 * z;
        
            // new vertex?
            if (!(v = vmap[vx][vy][vz])) {
                vmap[vx][vy][vz] = v = ++vertexCount;
                if (vertexCount * 3 > vertexSize)
                    resizeVertexes();
                
                // add coords
                coords[3*v - 3] = vx;
                coords[3*v - 2] = vy;
                coords[3*v - 1] = vz;

                // add color
                if (isColored) {
                    float t = vals[vx][vy][vz];
                    colors[3*v - 3] = 1 - t / 0xFF;
                    colors[3*v - 2] = 0.0;
                    colors[3*v - 1] = t / 0xFF;
                } else
                    colors[3*v - 3] = colors[3*v - 2] = colors[3*v - 1] = COLORLESS;
            }

            // add normal
            normals[3*v - 3] += normal[0];
            normals[3*v - 2] += normal[1];
            normals[3*v - 1] += normal[2];

            // add face vertex
            mesh[3*faceCount - 3 + j] = v;
        }
        
        delete [] normal;
    }
}

/**
 * @brief The marching cubes algorithm.
 * @param M arrayfire matrix of data to use in marching cubes.
 * @param isColored Whether or not to color the vertexes based on values.
 */
void Writer::marchingCubes(array M, int isColored) {
    int width = M.dims(2), height = M.dims(1), depth = M.dims(0);
    int ***vmap = createVertexMap(width, height, depth);

    // build case matrix
    array B = (M > EPSILON), C;
    C = 128*B(seq(1,-1),seq(1,-1),seq(0,-2)) + 64*B(seq(1,-1),seq(1,-1),seq(1,-1));
    C += 32*B(seq(0,-2),seq(1,-1),seq(1,-1)) + 16*B(seq(0,-2),seq(1,-1),seq(0,-2));
    C +=  8*B(seq(1,-1),seq(0,-2),seq(0,-2)) +  4*B(seq(1,-1),seq(0,-2),seq(1,-1));
    C +=  2*B(seq(0,-2),seq(0,-2),seq(1,-1)) +  1*B(seq(0,-2),seq(0,-2),seq(0,-2));
    int ***cases = new int**[width - 1];
    for (int i = 0; i < width - 1; i++) {
        cases[i] = new int*[height - 1];
        for (int j = 0; j < height - 1; j++) {
            cases[i][j] = new int[depth - 1];
            C(span, j, i).host(cases[i][j]);
        }    
    }

    array V = constant(0, depth * 2 - 1, height * 2 - 1, width * 2 - 1);
    V(seq(0,-1,2),seq(0,-1,2),seq(0,-1,2)) = M;
    V(seq(1,-2,2),seq(0,-1,2),seq(0,-1,2)) = af::max(M(seq(0,-2),span,span), M(seq(1,-1),span,span)); 
    V(seq(0,-1,2),seq(1,-2,2),seq(0,-1,2)) = af::max(M(span,seq(0,-2),span), M(span,seq(1,-1),span)); 
    V(seq(0,-1,2),seq(0,-1,2),seq(1,-2,2)) = af::max(M(span,span,seq(0,-2)), M(span,span,seq(1,-1))); 
    float ***vals = new float**[width * 2 - 1];
    for (int i = 0; i < width * 2 - 1; i++){
        vals[i] = new float*[height * 2 - 1];
        for (int j = 0; j < height * 2 - 1; j++) {
            vals[i][j] = new float[depth * 2 - 1];
            V(span, j, i).host(vals[i][j]);
        }    
    }

    // start marching
    for (int x = 0; x < width - 1; x++)
        for (int y = 0; y < height - 1; y++)
            for (int z = 0; z < depth - 1; z++)
                cubeCase(vmap, cases, vals, x, y, z, isColored);

    for (int i = 0; i < width - 1; i++) {
        for (int j = 0; j < height - 1; j++)
            delete [] cases[i][j];
        delete [] cases[i];
    }
    delete [] cases;

    for (int i = 0; i < width * 2 - 1; i++) {
        for (int j = 0; j < height * 2 - 1; j++)
            delete [] vals[i][j];
        delete [] vals[i];
    }
    delete [] vals;
    
    normalizeNormals();
    scaleCoords(width, height, depth);
    freeVertexMap(vmap, width, height);
}

/**
 * @brief This function processes a square case in marching squares algorithm.
 * @param vmap The vertex map.
 * @param cases The case at each x y coord.
 * @param vals The value at each x y coord (used for coloring).
 * @param x The y coord.
 * @param y The x coord.
 * @param isColored If true colors according to value else grey.
 */
void Writer::squareCase(int ***vmap, int **cases, float **vals, int x, int y, int isColored){
    const char *faces = MarchingSquares::lookup[cases[x][y]];
    
    // iterate over triangles
    for (int i = 0; i < 9; i += 3) {
        if (faces[i] == -1) break;

        if ((++faceCount) * 3 > meshSize)
            resizeMesh();
        
        // iterate over vetecies of triangle
        for (int j = 0; j < 3; j++) {
            int v;
            int vx = MarchingSquares::coords[faces[i + j]][0] + 2 * x;
            int vy = MarchingSquares::coords[faces[i + j]][1] + 2 * y;

            // new vertex?
            if (!(v = vmap[vx][vy][0])) {
                vmap[vx][vy][0] = v = ++vertexCount;
                if (vertexCount * 3 > vertexSize) 
                    resizeVertexes();
                
                // add coords
                coords[3*v - 3] = vx;
                coords[3*v - 2] = vy;
                
                // add normal
                normals[3*v - 1] = 1;

                // add color
                if (isColored) {
                    float t = vals[vx][vy];
                    colors[3*v - 3] = 1.0 - t / 0xFF;
                    colors[3*v - 2] = 0.0;
                    colors[3*v - 1] = t / 0xFF;
                } else
                    colors[3*v - 3] = colors[3*v - 2] = colors [3*v - 1] = COLORLESS;
            }
            
            // add face vertex
            mesh[3*faceCount - 3 + j] = v;
        }
    }
}

/**
 * @brief The marching square algorithm.
 * @param M arrayfire matrix of data to use in marching squares.
 * @param isColored Whether or not to color the vertexes based on values.
 */
void Writer::marchingSquares(array M, int isColored) {
    int width = M.dims(1), height = M.dims(0);
    int ***vmap = createVertexMap(width, height);    

    // build case matrix
    array B = (M > EPSILON), C;
    C  = 8*B(seq(0,-2), seq(0,-2)) + 4*B(seq(1,-1), seq(0,-2));
    C += 1*B(seq(0,-2), seq(1,-1)) + 2*B(seq(1,-1), seq(1,-1));
    int **cases = new int*[width - 1];
    for (int i = 0; i < width - 1; i++) {
        cases[i] = new int[height - 1];
        C(span, i).host(cases[i]);
    }

    array V = constant(0, height * 2 - 1, width * 2 - 1);
    V(seq(0 , -1, 2), seq(0, -1, 2)) = M;
    V(seq(1, -2, 2), seq(0, -1, 2)) = af::max(M(seq(0,-2), span), M(seq(1,-1), span)); 
    V(seq(0, -1, 2), seq(1, -2, 2)) = af::max(M(span, seq(0,-2)), M(span, seq(1,-1))); 
    float **vals = new float*[width * 2 - 1];
    for (int i = 0; i < width * 2 - 1; i++){
        vals[i] = new float[height * 2 - 1];
        V(span, i).host(vals[i]);
    }

    // start marching
    for (int x = 0; x < width - 1; x++) 
        for (int y = 0; y < height - 1; y++) 
            squareCase(vmap, cases, vals, x, y, isColored);
        
    for (int i = 0; i < width - 1; i++)
        delete [] cases[i];
    delete [] cases;

    for (int i = 0; i < width * 2 - 1; i++)
        delete [] vals[i];
    delete [] vals;

    normalizeNormals();
    scaleCoords(width, height);
    freeVertexMap(vmap, width, height);
}

/**
 * @brief This function extracts animations one frame at a time.
 */
void Writer::extractAnimation(){
    Reader reader(params);
    for (int i = 0; i < params->duration; i++) {
        array M = reader.readFile(params->datafiles[i]);
        M(M < 0xE0) = 0;
        vertexCount = 0;
        faceCount = 0;
        if (params->is4D) 
            marchingCubes(reorder(M, 1, 2, 3, 0), 0);
        else 
            marchingSquares(reorder(M, 2, 3, 0, 1), 0);
        ostringstream ss;
        ss << "animation_" << i << ".obj";
        output(ss.str());
    }
}

/**
 * @brief This function starts the extraction of the hulls in the pipeline.
 * @param hulls Arrayfire matrix of hulls to extract.
 */
void Writer::extract(array hulls){
    if (params->exportAnimation)
        extractAnimation();
    
    vertexCount = 0;
    faceCount = 0;
    if (params->is4D) 
        marchingCubes(hulls, 1);
    else 
        marchingSquares(hulls, 1);
    
    output("hulls.obj");
}

/**
 * @brief Construct a new Writer:: Writer object
 * @param params The parameters object.
 */
Writer::Writer(Parameters *params)
:params(params),meshSize(INTIAL_SIZE),vertexSize(INTIAL_SIZE) {
    mesh = new int[meshSize];
    for (int i = 0; i < meshSize; i++) 
        mesh[i] = 0;

    coords = new float[vertexSize];
    colors = new float[vertexSize];
    normals = new float[vertexSize];
    for (int i = 0; i < vertexSize; i++)
        coords[i] = colors[i] = normals[i] = 0;
}

/**
 * @brief This function prints a list of object file commands to a file.
 * @param filename file to output the object file commands to. 
 */
void Writer::output(string filename) {
    ofstream file;
    file.open(filename);
    file << "####" << endl << "#" << endl;
    file << "#\t.OBJ file generated through a Spatio-Temporal Hull computation program." << endl;
    file << "#" << endl << "####" << endl;

    // print vertexes
    file << "# all 'v' commands are listed" << endl;
    for (int i = 0; i < vertexCount * 3; i += 3) {
        file << "v " << coords[i] << " " << coords[i+1] << " " << coords[i+2];
        file << " " << colors[i] << " " << colors[i+1] << " " << colors[i+2] << endl;
    }

    // print normals
    file << "# all 'vn' commands are listed" << endl;
    for (int i = 0; i < vertexCount * 3; i += 3)
        file << "vn " << normals[i] << " " << normals[i+1] << " " << normals[i+2] << endl;

    // print faces
    file << "# all 'f' commands are listed" << endl;
    for (int i = 0; i < faceCount * 3; i += 3) {
        file << "f " << mesh[i] << "//" << mesh[i];
        file << " " << mesh[i+1] << "//" << mesh[i+1];
        file << " " << mesh[i+2] << "//" << mesh[i+2] << endl;
    }

    file.close();    
}

/**
 * @brief Destroy the Writer:: Writer object
 */
Writer::~Writer(){
    delete [] mesh;
    delete [] coords;
    delete [] colors;
    delete [] normals;
}
