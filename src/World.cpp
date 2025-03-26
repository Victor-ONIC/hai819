#include "World.h"
#include "Chunk.h"
#include "ChunkKey.h"
#include "Constants.h"
#include <unordered_map> // Pour utiliser std::unordered_map
#include <utility>       // Pour std::pair
#include <memory>  // Pour std::unique_ptr et std::shared_ptr

World::World(){}

//T0D0 : Changer pour ne pas à avoir à gérer un nullptr
void World::initChunk(int x, int z) {
    ChunkKey key(x, z);
    auto result = m_chunks.emplace(key, std::make_unique<Chunk>(x, z));
    if (!result.second) {
        std::cerr << "Chunk " << x << ", " << z << " already exists!" << std::endl;
    }
}

Chunk* World::tryGetChunk(int x, int z) {
    auto it = m_chunks.find(ChunkKey(x, z));
    return (it != m_chunks.end()) ? it->second.get() : nullptr;
}

bool World::hasChunk(int x, int z) const {
    ChunkKey key(x, z);
    return m_chunks.find(key) != m_chunks.end();
}

World::~World(){
  m_chunks.clear();
}
