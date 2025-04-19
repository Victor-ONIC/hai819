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
constexpr int CHUNK_WIDTH = 256;
constexpr int CHUNK_HEIGHT = 32;
constexpr int CHUNK_DEPTH = 256;
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

struct alignas(16) face {
    glm::vec4 vert[4];             // 64 octets
    glm::vec4 normal;              // 16 octets
    uint32_t blocktype;            // 4 octets
    uint32_t _pad0[3];             // 12 octets   -> PADDING pour aligner à 16 octets
    glm::vec2 coord_tex;           // 8 octets
    glm::vec2 _pad1;               // 8 octets    -> PADDING pour aligner à 16 octets
                                   // -> 64 + 16 + 4 + 12 + 8 + 4 == 112 octets
};

#endif // CONSTANTS_HPP
