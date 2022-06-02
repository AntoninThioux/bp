/**
 * File: Sampler.cpp
 * Author: Antonin (antonin.thioux@gmail.com)
 * Updated: 19-08-2021
 * About: This file contains the logic for the Sampler object. This object samples shapes with a certain
 *        Spatio-Temporal resolution.
 */

#include "Sampler.h"
#define SAMPLE_RATE 100

using namespace std;
using namespace DataGeneration;

/**
 * This is the constructor for the Sampler.
 * @param is3D Boolean whether or not to use 3 dimensions of space or 2.
 * @param isNoisy Whether or not to add noisy to sample.
 */
Sampler::Sampler(int is3D, int isNoisy)
: height(1), width(1), depth(1), frames(1), noise(0.0f) {
    cout << "height: ";
    cin >> height;

    cout << "width: ";
    cin >> width;

    if (is3D) cout << "depth: ";
    if (is3D) cin >> depth;

    cout << "frames: ";
    cin >> frames;

    if (isNoisy) cout << "noise: ";
    if (isNoisy) cin >> noise;

    int shape_id;
    cout << "Select a shape to sample..." << endl;
    cout << " 0 - circle\n 1 - heart\n 2 - spinner\n 3 - square\n 4 - star" << endl;
    cout << "shape: " << endl;
    cin >> shape_id;
    switch (shape_id) {
        case 0: shapeFunc = &(Shapes::circle); break;
        case 1: shapeFunc = &(Shapes::heart); break;
        case 2: shapeFunc = &(Shapes::spinner); break;
        case 3: shapeFunc = &(Shapes::square); break;
        case 4: shapeFunc = &(Shapes::star); break;
        default: cout << "ERROR: Invalid shape" << endl; exit(9);
    }
}

/**
 * This function samples the shape at given the resolution and saves a binary file for each frame.
 */
void Sampler::run() {
    random_device randomDevice;
    mt19937 gen(randomDevice());
    uniform_real_distribution<> dist(0, noise);
    uniform_real_distribution<> dist2(-0.5, 0.5);
    unsigned char data[width][height][depth];
    ofstream file;
    char filename[20];

    for (int f = 0; f < frames; f++) {
        // Collect data for frame f
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                for (int k = 0; k < depth; k++) {
                    // Sample voxel (i,j,k) 10 times
                    int sum = 0;
                    for (int _ = 0; _ < SAMPLE_RATE; _++) {
                        double x = (i + 1 + dist2(gen)) / (width + 1);
                        double y = (j + 1 + dist2(gen)) / (height + 1);
                        double z = (k + 1 + (depth != 1) * dist2(gen)) / (depth + 1);
                        double t = (f + 1 + dist2(gen)) / (frames + 1);
                        sum += shapeFunc(x, y, z, t);
                    }
                    unsigned char value = sum / SAMPLE_RATE;

                    // Add noise to voxel
                    if (value) value -= (unsigned char) (0xFF * dist(gen));
                    else value += (unsigned char) (0xFF * dist(gen));
                    data[i][j][k] = value;
                }
            }
        }

        // Save the data for frame f in file ./data/{f}.bin
        sprintf(filename, "data/%d.bin", f);
        file.open(filename);
        file.write(reinterpret_cast<const char *>(data), sizeof(char) * width * height * depth);
        file.close();
    }

    // Save dimension.txt file with the specs of the sampled data
    sprintf(filename, "dimensions.txt");
    file.open(filename);
    file << width << " " << height << " " << depth << endl;
    file << frames << endl;
    for (int f = 0; f < frames; f++)
        file << "data/" << f << ".bin" << endl;
    file.close();
}