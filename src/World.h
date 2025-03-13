#ifndef WORLD_H
#define WORLD_H

#include "Chunk.h"
#include "ChunkKey.h"
#include <unordered_map>
#include <vector>

class World {
public:
  World(size_t width, size_t depth); // Taille en chunks
  ~World();

  // Accéder à un chunk (modification possible)
  void getChunk(int x, int z);
  void initChunk(int x, int z);

private:
  size_t m_width, m_depth; // Dimensions en nombre de chunks
  std::unordered_map<ChunkKey, Chunk> m_chunks;
};

#endif // WORLD_H
