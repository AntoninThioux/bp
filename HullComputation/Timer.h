/**
 * @file Timer.h
 * @author Antonin Thioux (antonin.thioux@gmail.com)
 * @brief Header file of Timer.h 
 * @date last modified at 2022-06-02
 * @version 1.0
 */

#ifndef BP_TIMER_H
#define BP_TIMER_H

#include <fstream>
#include <iostream>
#include <arrayfire.h>
#include <sstream>
#include <iomanip>
#include <chrono>

namespace HullComputation {
    class Timer {
    private:
        const char *task;
        std::chrono::steady_clock::time_point startTime, lapTime;
        int currentLap, totalLaps;
        std::string formatTime(int ms);

    public:
        void start(const char *task, int laps);
        void lap();
        void stop();
    };
}

#endif
