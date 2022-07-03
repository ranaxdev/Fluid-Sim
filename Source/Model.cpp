#include "Model.h"

#include <fstream>
#include <sstream>
#include <iostream>

Model::Model(const char *path, std::string vert, std::string frag, std::pair<int, int> attribs, glm::vec3 color) {
    float x, y, z;
    std::ifstream filestream(path, std::ios::in);
    std::string line = "";
    std::stringstream ss;

    // Parse the model file
    while (!filestream.eof()) {
        std::getline(filestream, line);

        // Parse vertices
        if (line.compare(0, 2, "v ") == 0) {
            std::stringstream ss(line.erase(0, 1));
            ss >> x;
            ss >> y;
            ss >> z;
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
        // Parse normals
        if (line.compare(0, 2, "vn") == 0) {
            std::stringstream ss(line.erase(0, 2));
            ss >> x;
            ss >> y;
            ss >> z;
            normals.push_back(x);
            normals.push_back(y);
            normals.push_back(z);
        }

        // Parse faces
        if (line.compare(0, 2, "f ") == 0) {
            std::string node, vert, tex, norm;
            std::stringstream ss(line.erase(0, 2));

            for(int i=0; i<3; i++){
                std::getline(ss, node, ' ');
                std::stringstream node_ss(node);
                std::getline(node_ss, vert, '/');
                std::getline(node_ss, tex, '/');
                std::getline(node_ss, norm, '/');


                // get indices for vertices (to access them from the vectors we stored them in)
                std::size_t vert_index = ((std::size_t)(std::stoi(vert)) - 1) * 3;
                std::size_t norm_count = (std::size_t(std::stoi(norm)) - 1) * 3;

                // push vertices
                model_data.push_back(vertices[vert_index + 0]);
                model_data.push_back(vertices[vert_index + 1]);
                model_data.push_back(vertices[vert_index + 2]);

                // push color
                model_data.push_back(color.x);
                model_data.push_back(color.y);
                model_data.push_back(color.z);

                // push normals
                model_data.push_back(normals[norm_count + 0]);
                model_data.push_back(normals[norm_count + 1]);
                model_data.push_back(normals[norm_count + 2]);


            }
        }
    }

    shader = new Shader(SRC+"Shaders/"+vert+".glsl", SRC+"Shaders/"+frag+".glsl");
    this->attribs = attribs;
    transformation = glm::mat4(1.0f);

}