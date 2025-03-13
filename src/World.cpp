#include "World.h"
#include "Chunk.h"
#include "ChunkKey.h"
#include "Constants.h"
#include <unordered_map> // Pour utiliser std::unordered_map
#include <utility>       // Pour std::pair
namespace C =
    Constants; //  Pour ne pas à avoir à écrire Constants:: à chaque fois

World::World(size_t width, size_t depth) : m_width(width), m_depth(depth) {}

/*
void World::getChunk(int x, int z) {
    // Créer une clé unique pour le chunk avec les coordonnées (x, z)
    ChunkKey key(x, z);

    // Chercher si le chunk existe déjà
    auto it = m_chunks.find(key);

    // Si le chunk n'existe pas, le créer
    if (it == m_chunks.end()) {
        initChunk(x, z);
    }

    // Retourner la référence du chunk
    return m_chunks[key];
}
*/

void World::initChunk(int x, int z) {
    ChunkKey key(x, z);
    auto result = m_chunks.emplace(key, Chunk(x, z));
    if (!result.second) {
        std::cerr << "Chunk " << x << ", " << z << " already exists!" << std::endl;
    }
}

World::~World(){
   // Si les Chunks ont des ressources OpenGL à libérer
   for (auto& pair : m_chunks) {
       //TODO
       //pair.second.cleanup();
   }
}
