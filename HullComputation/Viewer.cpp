/**
 * @file Viewer.cpp
 * @author Antonin Thioux (antonin.thioux@gmail.com)
 * @brief This file contains the logic for viewing spatio-temporal hulls along side animations.
 * @date last modified at 2022-06-02
 * @version 1.0
 */

#include "Viewer.h"

#define WIDTH 1024
#define HEIGHT 512
#define FPS 30

using namespace af;
using namespace HullComputation;

/**
 * @brief This function updates the display animation.
 * @param animation The animation of the Spatio-Temporal data.
 * @param t The index of the animation to update.
 */
void Viewer::updateDisplay(array animation){
    std::chrono::steady_clock::time_point t = std::chrono::steady_clock::now();
    long ms = std::chrono::duration_cast<std::chrono::microseconds>(t - start).count();
    int frame = (ms * FPS / 1000000) % params->duration;
    display(span, seq(params->width, -1)) = animation(span, span, frame);
}

/**
 * @brief This function creates a window where the hulls and animation can be view side by side.
 * @param hulls The hulls that have been computed.
 * @param animation The animation of the data.
 */
void Viewer::show(array hulls, array animation){
    const static int width = WIDTH, height = HEIGHT;
    Window window(width, height, "Spatio-Temporal Hulls + Animation");
    int bw = params->kx / 2, bh = params->ky / 2;     // Size of boarders

    if (params->is4D) {
        array slice = flip(reorder(hulls(params->viewSlice, span, span), 1, 2, 0), 0);
        display(seq(bh - !(params->ky % 2), -1 - bh), seq(bw - !(params->kx % 2), -1 - bw - params->width)) = slice;
    } else {
        array dest = display(seq(bh - !(params->ky % 2), -1 - bh), seq(bw - !(params->kx % 2), -1 - bw - params->width));
        display(seq(bh - !(params->ky % 2), -1 - bh), seq(bw - !(params->kx % 2), -1 - bw - params->width)) = flip(hulls, 0);
    }

    do{
        updateDisplay(animation);
        window.image(display);
    } while( !window.close() );
}

/**
 * @brief Construct a new Viewer:: Viewer object
 * @param params Parameter Object.
 */
Viewer::Viewer(Parameters *params):params(params){
    display = constant(0, params->height, params->width * 2, dtype::u8);
    start = std::chrono::steady_clock::now();
}
