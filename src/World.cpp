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

void World::delete_chunks(Camera cam, int unload_distance) {
  int i = 0;
  int max_chunks_per_tick = 3;
  float frange = C::CHUNK_DEPTH * (float) unload_distance;
  glm::vec3 cam_pos = cam.get_pos();
  for (auto it = m_chunks.begin(); it != m_chunks.end();) {
    const auto &[key, chunk_ptr] = *it;
    glm::ivec2 chunk_pos = chunk_ptr->get_xz(); // ou key si c'est la même chose
    glm::vec3 chunk_world_pos = glm::vec3(chunk_pos.x, C::CHUNK_HEIGHT/2, chunk_pos.y);

    float dist = glm::distance(chunk_world_pos, cam_pos);
    if (dist > frange) {
      if ((++i) > max_chunks_per_tick) break;
    //std::cout << "chunk world pos :       " << chunk_world_pos[0] << ",   " << chunk_world_pos[1] << ",    " << chunk_world_pos[2] << std::endl;
    //std::cout << "cam pos :       " << cam_pos[0] << ", " << cam_pos[1] << ", " << cam_pos[2] << std::endl;
    //std::cout << "dist chunk :       " << dist << std::endl;
      it = m_chunks.erase(it); // efface et avance l'itérateur automatiquement
    } else {
      ++it; // avance normalement
    }
  }
}

World::~World(){
  m_chunks.clear();
}
