/**
 * @file Loader.cpp
 * @author Antonin Thioux (antonin.thioux@gmail.com)
 * @brief This file contains the logic for loading and drawing models.
 * @date last modified at 2022-06-02
 * @version 1.0
 */

#include "Loader.h"

using namespace std;
using namespace HullRendering;
using namespace glm;

/**
 * @brief This file extracts the data from an object file
 * @param filepath The filepath to the .obj file.
 */
void Loader::readFile(string filepath) {
    ifstream file(filepath);
    string line;

    if (!file) {
        cerr << "File not found: " << filepath << "!" << endl;
        exit(8);
    }

    if (!getline(file, line)) {
        cerr << "Expected file got directory!" << endl;
        exit(8);
    }
    
    cout << "Loading model: " << filepath << "..." << endl;
    while (!file.eof()) {   
        stringstream ss;   
        string command;
        ss << line;
        ss >> command;
        if (command == "f") {
            unsigned int indexes[3], _;
            sscanf(line.c_str(), "f %d//%d %d//%d %d//%d", &indexes[0], &_, &indexes[1], &_, &indexes[2], &_);
            indicesData.push_back(--indexes[0]);
            indicesData.push_back(--indexes[1]);
            indicesData.push_back(--indexes[2]);
        } else if (command == "v") {
            vec3 vertex, color;
            ss >> vertex.x >> vertex.y >> vertex.z >> color.r >> color.g >> color.b;
            vertexData.push_back(vertex);
            colorData.push_back(color);
        } else if (command == "vn") {
            vec3 normal;
            ss >> normal.x >> normal.y >> normal.z;
            normalData.push_back(normal);
        }
        ss.clear();
        getline(file, line);
    }
    file.close();
}

/**
 * @brief This function makes the buffers for the data.
 */
void Loader::makeBuffers() {
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(glm::vec3), &vertexData[0], GL_STATIC_DRAW);

    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, normalData.size() * sizeof(glm::vec3), &normalData[0], GL_STATIC_DRAW);

    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, colorData.size() * sizeof(glm::vec3), &colorData[0], GL_STATIC_DRAW);

    glGenBuffers(1, &elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesData.size() * sizeof(unsigned int), &indicesData[0] , GL_STATIC_DRAW);

}

/**
 * @brief Construct a new Loader object
 * @param filepath The filepath to the .obj file.
 */
Loader::Loader(string filepath){
    readFile(filepath);
    makeBuffers();
}

/**
 * @brief This function draws the model that was loaded.
 */
void Loader::drawModel() {
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // 2nd attribute buffer : normals
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // 3rd attribute buffer : colors
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

    // Draw the triangles !
    glDrawElements(GL_TRIANGLES, indicesData.size(), GL_UNSIGNED_INT, (void*)0);
}

/**
 * @brief Destroy the Loader object
 */
Loader::~Loader(){
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &normalBuffer);
    glDeleteBuffers(1, &colorBuffer);
    glDeleteBuffers(1, &vertexBuffer);
}
