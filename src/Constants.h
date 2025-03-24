#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <cstdint>

namespace Constants {
    constexpr int WINDOW_WIDTH = 1920;
    constexpr int WINDOW_HEIGHT = 1080;
    constexpr int CHUNK_WIDTH = 8;
    constexpr int CHUNK_HEIGHT = 8;
    constexpr int CHUNK_DEPTH = 16;
    constexpr uint64_t BLOCKS_PER_CHUNK = CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH;
}

#endif // CONSTANTS_HPP
