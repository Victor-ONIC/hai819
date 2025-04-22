#ifndef WORLD_H
#define WORLD_H

#include "Chunk.h"
#include "ChunkKey.h"
#include <memory>
#include <unordered_map>
#include <vector>

class World {
public:
  static World &getInstance() { //  Permet d'avoir une instance unique statique à la class
    static World instance;
    return instance;
  }

  void initChunk(int x, int z);
  Chunk * tryGetChunk(int x, int z); // TODO modifier pour ne pas avoir à gérer un nullptr
  bool hasChunk(int x, int z) const;
  std::unordered_map<ChunkKey, std::unique_ptr<Chunk>> m_chunks;

private:
  World();
  ~World();
  World(const World &) = delete;
  World &operator=(const World &) = delete;
};

#endif // WORLD_H
