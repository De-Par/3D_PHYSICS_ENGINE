#pragma once

#include <glm/vec4.hpp>
#include <glm/common.hpp>

namespace gra {

    template<typename T>
    struct Pixel {
        T r, g, b, a;
    };

    class Texture {
    private:

        int width{}, height{}, nrChannels{};
        unsigned char* data;

    public:
        unsigned int texture{};
        explicit Texture(const char* filepath);
        explicit Texture(glm::vec4 col);
    };
}
