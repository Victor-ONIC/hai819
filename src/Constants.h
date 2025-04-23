#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include "glm/glm.hpp"

#include <array>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <variant>

namespace Constants {
constexpr int WINDOW_WIDTH = 1920;
constexpr int WINDOW_HEIGHT = 1080;

//constexpr int CHUNK_WIDTH = 256;
//constexpr int CHUNK_HEIGHT = 64;
//constexpr int CHUNK_DEPTH = 256;

//constexpr int CHUNK_WIDTH = 64;
//constexpr int CHUNK_HEIGHT = 256;
//constexpr int CHUNK_DEPTH = 64;

//constexpr int CHUNK_WIDTH = 128;
//constexpr int CHUNK_HEIGHT = 128;
//constexpr int CHUNK_DEPTH = 128;

//constexpr int CHUNK_WIDTH = 512;
//constexpr int CHUNK_HEIGHT = 128;
//constexpr int CHUNK_DEPTH = 512;

constexpr int CHUNK_WIDTH = 256;
constexpr int CHUNK_HEIGHT = 256;
constexpr int CHUNK_DEPTH = 256;

//constexpr int CHUNK_WIDTH = 320;
//constexpr int CHUNK_HEIGHT = 320;
//constexpr int CHUNK_DEPTH = 320;

//constexpr int CHUNK_WIDTH = 128;
//constexpr int CHUNK_HEIGHT = 64;
//constexpr int CHUNK_DEPTH = 128;

//constexpr int CHUNK_WIDTH = 512;
//constexpr int CHUNK_HEIGHT = 512;
//constexpr int CHUNK_DEPTH = 512;

constexpr uint64_t BLOCKS_PER_CHUNK = CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH;

using ValueType = std::variant<
    // Types scalaires
    int, unsigned int, float, double,
    // Vecteurs 2D
    std::pair<int, int>, std::pair<unsigned int, unsigned int>,
    std::pair<float, float>, glm::vec2,
    // Vecteurs 3D
    std::array<int, 3>, std::array<unsigned int, 3>, std::array<float, 3>,
    glm::vec3,
    // Vecteurs 4D
    std::array<int, 4>, std::array<unsigned int, 4>, std::array<float, 4>,
    glm::vec4,
    // Matrices 2x2
    glm::mat2,
    // Matrices 3x3
    glm::mat3,
    // Matrices 4x4
    glm::mat4>;
// Unordered_map utilisant ValueType déclaré ci-dessus
using UniformMap = std::unordered_map<std::string, ValueType>;
} // namespace Constants

struct face {
    uint32_t blocktype;            // 4 octets
    float _padding[3];             // 12 octets
    glm::vec4 vert[4];             // 64 octets
    glm::vec4 normal;              // 16 octets
    glm::vec2 coord_tex[4];        // 32 octets
                                   // -> 64 + 16 + 4 + 12 + 8 + 4 == 128 octets
};

/*
struct face{
    uint blockype;        // 4 octets
    vec4 vert[4];         // 64 octets
    vec4 normal;          // 16 octets
    vec2 coord_tex[4];    // 32 octets
    float _padding[3];    // 12 octets
                          // 4 + 64 + 16 + 32 + 12 == 128 octets
};
*/

#endif // CONSTANTS_HPP
