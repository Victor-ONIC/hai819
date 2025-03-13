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

Chunk* World::findChunk(int x, int z) {
    ChunkKey key(x, z);
    auto it = m_chunks.find(key);
    return (it != m_chunks.end()) ? &it->second : nullptr;
}

Chunk& World::getChunk(int x, int z) {
    Chunk* chunk = findChunk(x, z);
    if (!chunk) {
        throw std::runtime_error("Chunk " + std::to_string(x) + ", " + std::to_string(z) + " doesn't exist!");
    }
    else{ std::cout << "getchunk finded" << std::endl;}
    return *chunk;
}

void World::initChunk(int x, int z) {
    ChunkKey key(x, z);
    auto result = m_chunks.emplace(key, Chunk(x, z));
    if (!result.second) {
        std::cerr << "Chunk " << x << ", " << z << " already exists!" << std::endl;
    }
}

Chunk* World::tryGetChunk(int x, int z) {
  ChunkKey key(x, z);
  auto it = m_chunks.find(key);
  return (it != m_chunks.end()) ? &it->second : nullptr;
}

World::~World(){
  m_chunks.clear();
}
