#pragma once

#include <Mesh.hpp>
#include <vector>
#include <Shader.hpp>

namespace gra {

    class SoftBody : public PhysicalObject {
    public:

        glm::vec3 *velocities{};
        // Mesh* mesh;
        float particalMass = 1.0f;
        std::unordered_map<uint, std::unordered_map<uint, float>> edgeConnections;

        SoftBody(const char* filename, Shader& shader);
        void initSoftBody();

        SoftBody(std::vector<Vertex>& verts, std::vector<uint>& indices) : PhysicalObject(verts, indices) {
            initSoftBody();
        }

        void simulateTimeStep(float dt) override;

        void printEdges() {
            for (int i = 0; i < numOfVerts; ++i) {
                std::cout << "Size of vertex [" << i << "]: ";
                std::unordered_map<uint, float>::iterator itr;
                for (itr = edgeConnections[i].begin(); itr != edgeConnections[i].end(); ++itr) {
                    std::cout << "{" << itr->first << ", " << itr->second << "}, ";
                }
                std::cout << "\n";
            }
        }
    };
}