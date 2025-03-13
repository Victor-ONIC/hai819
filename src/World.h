#ifndef WORLD_H
#define WORLD_H

#include <unordered_map>
#include <vector>
#include "Chunk.h"

class World {
public:
    World(size_t width, size_t depth); // Taille en chunks

    // Accéder à un chunk (lecture seule)
    const Chunk& getChunk(size_t x, size_t z) const;

    // Accéder à un chunk (modification possible)
    Chunk& getChunk(size_t x, size_t z);

private:
    size_t m_width, m_depth; // Dimensions en nombre de chunks
    std::unordered_map<std::pair<int, int>, Chunk> m_chunks;
};

#endif // WORLD_H
