#ifndef WORLD_H
#define WORLD_H

#include "Chunk.h"
#include "ChunkKey.h"
#include <unordered_map>
#include <vector>

class World {
public:
    static World& getInstance() { //  Permet d'avoir une instance unique statique à la class
        static World instance;
        return instance;
    }

  // Accéder à un chunk (modification possible)
  bool hasChunk(int x, int z) const;
  void initChunk(int x, int z);
  Chunk *tryGetChunk(int x, int z);// TODO modifier pour ne pas avoir à gérer un nullptr

private:

  World();
  ~World();
    World(const World&) = delete;
    World& operator=(const World&) = delete;
  size_t m_width, m_depth; // Dimensions en nombre de chunks
  std::unordered_map<ChunkKey, Chunk> m_chunks;
};

#endif // WORLD_H
