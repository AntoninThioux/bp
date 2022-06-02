# Redesign of the Spatio-Temporal Hull Visualization Pipeline
This repo is the code for my (antonin) BP project 2021/2022 at the RUG.
The goal of this code is to processing and render hull of Spatio-Temporal data.
This goal was previously achieved through deep-raycasting, however, this proved costly in rendering.
In this project prepocesses hulls instead to work around this.
The code is split into 3 directories DataGeneration, HullComputation, and HullRendering.
 - **DataGeneration** This directory is only used to generate dummy data to test the rest of the pipeline. 
 - **HullComputation** This directory contains the code to prepocess the hulls using arrayfire (on CPU or GPU).
 - **HullRendering** This directory contains the code for rendering the hulls using OpenGL.