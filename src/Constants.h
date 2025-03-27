#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include "glm/glm.hpp"

#include <cstdint>
#include <variant>
#include <unordered_map>
#include <string>

namespace Constants {
constexpr int WINDOW_WIDTH = 1920;
constexpr int WINDOW_HEIGHT = 1080;
constexpr int CHUNK_WIDTH = 256;
constexpr int CHUNK_HEIGHT = 128;
constexpr int CHUNK_DEPTH = 256;
constexpr uint64_t BLOCKS_PER_CHUNK = CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH;

    using ValueType = std::variant<
    // Types scalaires
    int, unsigned int, float, double,

    // Vecteurs 2D
    std::pair<int, int>, std::pair<unsigned int, unsigned int>,
    std::pair<float, float>,
    glm::vec2,

    // Vecteurs 3D
    std::tuple<int, int, int>,
    std::tuple<unsigned int, unsigned int, unsigned int>,
    std::tuple<float, float, float>,
    glm::vec3,

    // Vecteurs 4D
    std::tuple<int, int, int, int>,
    std::tuple<unsigned int, unsigned int, unsigned int, unsigned int>,
    std::tuple<float, float, float, float>,
    glm::vec4,

    // Matrices 2x2
    glm::mat2,
    std::array<std::array<float, 2>, 2>,

    // Matrices 3x3
    glm::mat3,
    std::array<std::array<float, 3>, 3>,

    // Matrices 4x4
    glm::mat4,
    std::array<std::array<float, 4>, 4>>;

    // Unordered_map utilisant ValueType déclaré ci-dessus
    using UniformMap = std::unordered_map<std::string, ValueType>;
} // namespace Constants

#endif // CONSTANTS_HPP
