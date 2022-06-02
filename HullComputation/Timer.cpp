/**
 * @file Timer.cpp
 * @author Antonin Thioux (antonin.thioux@gmail.com)
 * @brief This file contains the logic for the timing IO of this program.
 * @date last modified at 2022-06-02
 * @version 1.0
 */

#include "Timer.h"

using namespace HullComputation;
using namespace std;

/**
 * @brief This funciton formats time from microseconds to string (Xh XXm XX.XXXXXXs).
 * @param ms The microseconds that have passed.
 * @return string The time that has pass as a string.
 */
string Timer::formatTime(int ms) {
    ostringstream ss;
    if (ms / 360000000 > 0)
        ss << ms / 360000000 << "h " << setw(2) << setfill('0');

    if (ms / 60000000 > 0)
        ss << (ms / 60000000) % 60 << "m " << setw(2) << setfill('0');

    ss << (ms / 1000000) % 60 << "." << setw(6) << setfill('0');
    ss << ms % 1000000 << "s";

    return string(ss.str());
}

/**
 * @brief This function starts the timer for a task.
 * @param task The name of the task that should be used.
 * @param laps The number of subtasks to expect.
 */
void Timer::start(const char *task, int laps){
    af::sync();
    this->task = task;
    this->startTime = chrono::steady_clock::now();

    this->totalLaps = laps;
    this->currentLap = 1;
    this->lapTime = chrono::steady_clock::now();

    if (laps == 1)
        cout << task << "..." << flush;
    else 
        cout << task << " with " << laps << " batches" << endl;
}

/**
 * @brief This function starts timing the next lap.
 */
void Timer::lap() {
    if (this->totalLaps == 1)
        return ;
    
    af::sync();
    chrono::steady_clock::time_point endTime = chrono::steady_clock::now();
    string time = formatTime(chrono::duration_cast<chrono::microseconds>(endTime - this->lapTime).count());
    this->lapTime = endTime;

    cout << "\t[" << this->currentLap++ << "/" << this->totalLaps << "]\t\tcomplete, time = " << time << endl;
}

/**
 * @brief This function stops timing the task.
 */
void Timer::stop(){
    af::sync();
    chrono::steady_clock::time_point endTime = chrono::steady_clock::now();
    string time = formatTime(chrono::duration_cast<chrono::microseconds>(endTime - this->startTime).count());

    if (this->totalLaps == 1)
        cout << "\t done! Time = " << time << endl;
    else 
        cout << this->task << " is done! Total time = " << time << endl;
}
