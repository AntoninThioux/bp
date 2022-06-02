/**
 * @file main.cpp
 * @author Antonin Thioux (antonin.thioux@gmail.com)
 * @brief This file contains the logic for the main pipeline for computing and extracting Spatio-Temporal hulls.
 * @date last modified at 2022-06-02
 * @version 1.0
 */

#include <iostream>
#include <arrayfire.h>

#include "Parameters.h"
#include "Timer.h"
#include "Reader.h"
#include "Viewer.h"
#include "Calc.h"
#include "Writer.h"

using namespace std;
using namespace HullComputation;

void debugWriter(Parameters *params);

/**
 * @brief This function runs the hull computation and extraction pipeline.
 * @param params Parameters object.
 */
void pipeline(Parameters *params) {
    Timer timer;
    Reader reader(params);
    Calc calc(params);
    Writer writer(params);

    if (params->isTimed) timer.start("Computing", params->batches);
    for (int _ = 0; _ < params->batches; _++) {
        af::array batch = reader.getNextBatch();
        calc.processBatch(batch);
        if (params->isTimed) timer.lap();
    }
    af::array hulls = calc.getHulls();
    if (params->isTimed) timer.stop();

    if (params->isTimed) timer.start("Extracting", 1);
    writer.extract(hulls);
    if (params->isTimed) timer.stop();   

    if (params->isViewed){
        Viewer viewer(params);
        viewer.show(hulls, reader.getAnimation());
    }
}

/**
 * This is the main function for the compute program.
 * @param argc The number of arguments.
 * @param argv The array of arguments.
 * @return Exit success when computation is complete.
 */
int main(int argc, char *argv[]) {
    Parameters params(argc, argv);
    pipeline(&params);
    return 0;
}





/**
 * @brief Dummy function to debug writer.
 * @param params Parameters object.
 */
void debugWriter(Parameters *params) {
    Writer writer(params);

    af::array hulls = af::constant(0, 3, 3);
    hulls(1, 1) = 0x62;

    writer.extract(hulls);
}

/**
 * @brief Dummy function to debug parameters.
 * @param params Parameters object.
 */
void debugParams(Parameters *params){
    cout << "width = " << params->height << ", height = " << params->height << ", depth = " << params->depth << ", duration = " << params->duration << endl;
    cout << "is4D = " << params->is4D << endl;
    for (int i = 0; i < params->duration; i++) cout << "datafiles[" << i << "] = " << params->datafiles[i].c_str() << endl;
    cout << "isViewed = " << params->isViewed << ", viewSlice = " << params->viewSlice << endl;
    cout << "isTimed = " << params->isTimed << ", batches = " << params->batches << endl;
    cout << "kx = " << params->kx << ", ky = " << params->ky << ", kz = " << params->kz << ", kt = " << params->kt << ", threshold = " << params->threshold << endl;
}

/**
 * @brief Dummy function to test batching.
 * @param params Parameters object.
 */
void testBatching(Parameters *params){
    Reader reader(params);
    af::array batch;
    for (int i = 0; i < params->batches; i++){
        batch = reader.getNextBatch();
        cout << "batch: " << i << ", size: " << batch.dims(0) << ", type: " << batch.type() << endl;
    }
    cout << "finished tests" << endl;

}
