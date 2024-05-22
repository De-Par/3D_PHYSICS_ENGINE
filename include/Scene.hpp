#pragma once

#include "RigidBody.hpp"

namespace gra {

    class Scene {
    public:

        std::vector<std::shared_ptr<RigidBody> > meshes;
        Scene() = default;

        void drawScene()  {
            for (const auto& it: meshes) {
                it->draw();
            }
        }

        void simulatePhysics(float dt) {
            for (const auto& it: meshes) {
                it->simulateTimeStep(dt);
            }
        }

        void addMesh(const std::shared_ptr<RigidBody>& mesh) {
            meshes.push_back(mesh);
        }
    };
}