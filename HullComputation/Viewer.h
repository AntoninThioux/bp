/**
 * @file Viewer.h
 * @author Antonin Thioux (antonin.th@domain.com)
 * @brief Header file to Viewer.cpp
 * @date last modified at 2022-06-02
 * @version 1.0
 */

#ifndef BP_VIEWER_H
#define BP_VIEWER_H

#include <arrayfire.h>
#include "Parameters.h"
#include <chrono>

namespace HullComputation {
    class Viewer
    {
    private:
        Parameters *params;
        af::array display;
        std::chrono::steady_clock::time_point start;
        void updateDisplay(af::array animation);
    public:
        Viewer(Parameters *params);
        void show(af::array hulls, af::array animation);
    };
}

#endif
