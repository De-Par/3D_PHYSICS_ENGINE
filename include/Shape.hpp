#pragma once

#include <vector>
#include <glad/glad.h>
#include <unordered_map>
#include <utility>
#include <bits/stdc++.h>
#include <glm/vec3.hpp>
#include <Shader.hpp>

namespace gra {
    // enum ShapeType
    // {
    //     General, Point, Trian, Plane, Box, Pyramid, Cylinder, Sphere, Lane
    // };
    class Shape {
    public:

        std::vector<Vertex> verts;

        explicit Shape(std::vector<Vertex>& verts) {
            this->verts = verts;
        }

        glm::vec3 support(glm::vec3 direction) {
            glm::vec3 v = verts[0].pos;
            float maxDot = glm::dot(v, direction);
            for (auto & vert : verts) {
                float d = glm::dot(vert.pos, direction);
                if(d >= maxDot){
                    v = vert.pos;
                    maxDot = d;
                }
            }
            return v;
        }
    };
}