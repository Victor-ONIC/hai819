#include "World.h"
#include "Chunk.h"
#include "ChunkKey.h"
#include "Constants.h"
#include <unordered_map> // Pour utiliser std::unordered_map
#include <utility>       // Pour std::pair

World::World(){}

bool World::hasChunk(int x, int z) const {
    ChunkKey key(x, z);
    return m_chunks.find(key) != m_chunks.end();
}

//T0D0 : Changer pour ne pas à avoir à gérer un nullptr
void World::initChunk(int x, int z) {
    ChunkKey key(x, z);
    auto result = m_chunks.emplace(key, Chunk(x, z));
    if (!result.second) {
        std::cerr << "Chunk " << x << ", " << z << " already exists!" << std::endl;
    }
}

// Permet d'obtenir un chunk (x,z) si il existe, sinon renvoi un nullptr
Chunk* World::tryGetChunk(int x, int z) {
  ChunkKey key(x, z);
  auto it = m_chunks.find(key);
  return (it != m_chunks.end()) ? &it->second : nullptr;
}

World::~World(){
  m_chunks.clear();
}
