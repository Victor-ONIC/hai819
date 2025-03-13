#ifndef WORLD_H
#define WORLD_H

#include "Chunk.h"
#include "ChunkKey.h"
#include <unordered_map>
#include <vector>

class World {
public:
  World(); // Taille en chunks
  ~World();

  // Accéder à un chunk (modification possible)
  bool hasChunk(int x, int z) const;
  Chunk *findChunk(int x, int z);
  Chunk &getChunk(int x, int z);
  void initChunk(int x, int z);
  Chunk *tryGetChunk(int x, int z);

private:
  size_t m_width, m_depth; // Dimensions en nombre de chunks
  std::unordered_map<ChunkKey, Chunk> m_chunks;
};

#endif // WORLD_H
