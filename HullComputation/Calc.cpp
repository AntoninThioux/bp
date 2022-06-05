/**
 * @file Calc.cpp
 * @author Antonin Thioux (antonin.thioux@gmail.com)
 * @brief This file contains the logic for computing Spatio-Temporal Hulls.  
 * @date last modified at 2022-06-02
 * @version 1.0
 */

#include "Calc.h"

using namespace af;
using namespace HullComputation;

/**
 * @brief This function estimates the first order partial derivative of a matrix across a single dimension.
 * @param M The matrix as arrayfire array.
 * @param dim The dimension to use.
 * @return array Resulting matrix as arrayfire array.
 */
array Calc::derivative(array M, int dim){
    af::index is[2][4] ={{span, span, span, span}, {span, span, span, span}}; 
    is[0][dim] = seq(0, -2);
    is[1][dim] = seq(1, -1);
    return M(is[0][0], is[0][1], is[0][2], is[0][3]) - M(is[1][0], is[1][1], is[1][2], is[1][3]);
}

/**
 * @brief This function applies a smooth kernel (approaching guassian distribution) on a matrix across a single dimension.
 * @param M The matrix as arrayfire array.
 * @param dim The dimension to use.
 * @return array Resulting matrix as arrayfire array.
 */
array Calc::guassian(array M, int dim){
    af::index is[2][4] ={{span, span, span, span}, {span, span, span, span}}; 
    is[0][dim] = seq(0, -2);
    is[1][dim] = seq(1, -1);
    return M(is[0][0], is[0][1], is[0][2], is[0][3]) + M(is[1][0], is[1][1], is[1][2], is[1][3]);
}

/**
 * @brief This function applies a sobel-like operator to a matrix to estimate the partial second derivative.
 * @param M The matrix as arrayfire array.
 * @param dev1 The first partial derivative.
 * @param dev2 The second partial derivative.
 * @return array Resulting matrix as arrayfire array.
 */
array Calc::sobelD2(array M, int dev1, int dev2){
    const int kernelSizes[4] = {params->kt, (params->is4D) ? params->kz : 1, params->ky, params->kx};

    for (int dimension = 0; dimension < 4; dimension++){
        int size = kernelSizes[dimension];

        for (int i = 0; i < size - 1; i++){
            if (i == 0 && dev1 == dimension) M = derivative(M, dimension);
            else if (i == 1 && dev2 == dimension) M = derivative(M, dimension);
            else M = guassian(M, dimension);
        }
    }

    return M;
}

/**
 * @brief This function applies a sobel-like operator to a matrix to estimate the partial third derivative.
 * Note: dont call this function with dev1 = dev2 = dev3 in the context of this programs as the defualt kernel sizes are too small!
 * @param M The matrix as arrayfire array.
 * @param dev1 The first partial derivative.
 * @param dev2 The second partial derivative.
 * @param dev3 The third partial derivative.
 * @return array Resulting matrix as arrayfire array.
 */
array Calc::sobelD3(array M, int dev1, int dev2, int dev3) {
    const int kernelSizes[4] = {params->kt, (params->is4D) ? params->kz : 1, params->ky, params->kx};

    for (int dimension = 0; dimension < 4; dimension++){
        int size = kernelSizes[dimension];
        int count = (dev1 == dimension) + (dev2 == dimension) + (dev3 == dimension);

        for (int i = 0; i < size - 1; i++){
            if (count > 0) M = derivative(M, dimension);
            else M = guassian(M, dimension);
            count--;
        }
    }

    return M;
}

/**
 * @brief This function flattens the spacetime cube to get the hulls of a batch, then reduces this batch with the previously computed spacetime cube.
 * @param spacetime The spacetime cube as arrayfire array.
 */
void Calc::flattenAndReduce(array spacetime){
    int n = spacetime.dims(0); 

    // perform time labelling
    for (int i = 0; i < n; i++) {
        array slice = spacetime(i, span, span, span);
        slice(slice > 1) = i + t + params->y;
        spacetime(i, span, span, span) = slice;
    }
    
    // perform time flattening
    array flatten = max(reorder(spacetime, 1, 2, 3, 0), 3);

    // perform reducing
    hulls = max(hulls, flatten);

    // Update t pointer for labelling
    t += n;
}

/**
 * @brief Construct a new Calc:: Calc object, creates identiy hulls.
 * @param params The Parameters object.
 */
Calc::Calc(Parameters *params):params(params),t(1){
    if (params->y != 0){
        hulls = readArray("hulls.af", "hulls");
        return ;
    }

    // Indentity hull matrix 3D & 4D cases
    if (params->is4D)
        hulls = constant(0, params->depth - params->kz + 1, params->height - params->ky + 1, params->width - params->kx + 1, dtype::f32);
    else   
        hulls = constant(0, 1, params->height - params->ky + 1, params->width - params->kx + 1, dtype::f32);
}

/**
 * @brief Processes a batch of data.
 * @param batch The batch of data as arrayfire array.
 */
void Calc::processBatch(array batch){
    // compute a spacetime cube from the data
    array spacetime;  

    // If special is 0 or 1 then add Px2 Py2 Pz2* Pt2
    // *= only in 4D case
    if (params->special == 0 || params->special == 1) {
        spacetime = pow2(sobelD2(batch, 0, 0));

        for (int d = 1; d < 4; d++) {
            if (!params->is4D && d == 1) continue;
            spacetime += pow2(sobelD2(batch, d, d));
        }
    }

    // If special is 0 then also compute the rest of the D2 matrix (PxPy PxPz* PxPt PyPz* PyPt PzPt*)
    // *= only in 4D case
    if (params->special == 0) {
        for (int d0 = 0; d0 < 4; d0++) {
            if (!params->is4D && d0 == 1) continue;
            for (int d1 = d0 + 1; d1 < 4; d1++) {
                if (!params->is4D && d1 == 1) continue;
                spacetime += 2 * pow2(sobelD2(batch, d0, d1));
            }
        }
    }

    // If special is 2 then compute PxPt2 PyPt2 PzPt2*
    // *= only in 4D case
    if (params->special == 2) {
        spacetime = pow2(sobelD3(batch, 0, 0, 3));
        spacetime += pow2(sobelD3(batch, 0, 0, 2));
        if (params->is4D) spacetime += pow2(sobelD3(batch, 0, 0, 1));
    }

    // filter everything that is below threshold^2
    spacetime(spacetime < (params->threshold * params->threshold)) = 0;

    // flatten it across time dimension
    flattenAndReduce(spacetime);
}

/**
 * @brief Getter for Spatio-Temporal hulls.
 * @return array The hulls as an arrayfire array.
 */
array Calc::getHulls(){
    af::saveArray("hulls", hulls, "hulls.af");
    if (params->is4D)
        return hulls  / (params->x - params->kt + 1) * 0xFF;
    return reorder(hulls, 1, 2, 0)  / (params->x - params->kt + 1) * 0xFF;
}
