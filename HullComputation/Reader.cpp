/**
 * @file Reader.cpp
 * @author Antonin Thioux (antonin.thioux@gmail.com)
 * @brief This file contain the logic for reading binary data files.
 * @date last modified at 2022-06-02
 * @version 1.0
 */
#include "Reader.h"

using namespace HullComputation;
using namespace std;
using namespace af;

/**
 * @brief This function reads datafiles and returns them as 2 or 3D arrays.
 * @param filename The path to the data file as a string.
 * @return array An arrayfire array containing the data.
 */
array Reader::readFile(string filename) {
    int n = params->height * params->width * params->depth; 
    char *data = new char[n];

    ifstream datafile;
    datafile.open(filename);
    if (!datafile) {
        cerr << "file: " << filename << " not found!" << endl;
        exit(EXIT_FAILURE);
    }
    datafile.read(data, n);

    datafile.close();
    array dataMatrix = array(1, params->depth, params->height, params->width, (unsigned char *) data);
    delete [] data;
    return dataMatrix;
}

/**
 * @brief This function returns the next batch of Spatio-Temporal data to process.
 * @return array An arrayfire array containing the data.
 */
array Reader::getNextBatch(){
    if (batchNum != 0) {  // Case not first batch
        int border = params->kt - 1;    
        int tSize = (params->duration + (params->batches - 1) * border + batchNum) / params->batches;
        array nextBatch = array(tSize, params->depth, params->height, params->width);

        // Copy shared border
        nextBatch(seq(border), span, span, span) = batch(seq(border * -1, -1), span, span, span);
        batch = nextBatch;

        for (int i = border; i < tSize; i++)
            batch(i, span, span, span) = readFile(params->datafiles[pointer++]);
    }

    batchNum++;
    return batch / 0xFF;
}

/**
 * @brief This function returns an 3D array representing data as an animation for display purposes.
 * @return array Arrayfire array of animation.
 */
array Reader::getAnimation() {
    int h = params->height, w = params->width, frames = params->duration;
    array animation = array(h, w, frames, dtype::u8);

    for (int t = 0; t < frames; t++) {
        array frame = readFile(params->datafiles[t]);
        int i = (params->is4D) ? params->viewSlice + params->kz / 2 - 1 : 0;
        animation(span, span, t) = flip(reorder(frame(span, i, span, span), 2, 3, 0, 1), 0);

    }

    return animation;
}

/**
 * @brief Construct a new Reader:: Reader object, it also prepares the first batch of data.
 * @param params The parameters to use.
 */
Reader::Reader(Parameters *params)
:params(params),batchNum(0),pointer(0){
    // Read the first batch
    int border = params->kt - 1;    
    int tSize = (params->duration + (params->batches - 1) * border) / params->batches;
    batch = array(tSize, params->depth, params->height, params->width);
    for (int i = 0; i < tSize; i++)
        batch(i, span, span, span) = readFile(params->datafiles[pointer++]);
}
