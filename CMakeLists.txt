cmake_minimum_required(VERSION 3.16)
project(bp)

set(CMAKE_CXX_STANDARD 14)

find_package(ArrayFire)
find_package(OpenGL)
find_package(GLEW)


add_executable(sample DataGeneration/main.cpp
        DataGeneration/Shapes.cpp
        DataGeneration/Sampler.cpp
)

add_executable(compute HullComputation/main.cpp
        HullComputation/Parameters.cpp
        HullComputation/Timer.cpp
        HullComputation/Reader.cpp
        HullComputation/Viewer.cpp
        HullComputation/Calc.cpp
        HullComputation/Writer.cpp
)

target_link_libraries(compute ArrayFire::afcpu)

add_executable(render HullRendering/main.cpp
        HullRendering/Render.cpp
        HullRendering/Controller.cpp
        HullRendering/Loader.cpp
)

target_link_libraries(render OpenGL::GL)
target_link_libraries(render glfw)
target_link_libraries(render GLEW)
