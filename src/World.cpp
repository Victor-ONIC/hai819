#include "World.h"
#include "Chunk.h"
#include "ChunkKey.h"
#include "Constants.h"
#include <unordered_map> // Pour utiliser std::unordered_map
#include <utility>       // Pour std::pair
#include <memory>  // Pour std::unique_ptr et std::shared_ptr

namespace C = Constants;

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

void World::print_nb_chunks(){
  std::size_t nb_chunks = m_chunks.size();
  std::cout << "Nombre de chunks chargés : " << nb_chunks << std::endl;
}

void World::delete_chunks(Camera cam, int chunk_radius) {
  int deleted_count = 0;
  const int max_chunks_per_tick = 1;

  glm::vec3 cam_pos = cam.get_pos();
  int cam_cx = (int)std::floor(cam_pos.x / (float)C::CHUNK_WIDTH);
  int cam_cz = (int)std::floor(cam_pos.z / (float)C::CHUNK_DEPTH);

  int radius_squared = chunk_radius * chunk_radius;

  for (auto it = m_chunks.begin(); it != m_chunks.end(); ) {
    const auto &[key, chunk_ptr] = *it;
    glm::ivec2 chunk_pos = chunk_ptr->get_xz(); // ou directement `key`

    int dx = chunk_pos.x / C::CHUNK_WIDTH - cam_cx;
    int dz = chunk_pos.y / C::CHUNK_DEPTH - cam_cz;

    if ((dx * dx + dz * dz) > radius_squared) {
      it = m_chunks.erase(it); // avance automatiquement
      glFinish();
      ++deleted_count;
      if (deleted_count >= max_chunks_per_tick) break;
    } else {
      ++it;
    }
  }
}

World::~World(){
  m_chunks.clear();
}
