#include "World.h"
#include <unordered_map>  // Pour utiliser std::unordered_map
#include <utility>        // Pour std::pair

// Constructeur de World : initialise la taille de la carte (en chunks)
World::World(size_t width, size_t depth)
    : m_width(width), m_depth(depth) {
    // Optionnel : Tu peux initialiser des chunks ici si nécessaire
}

// Accéder à un chunk (lecture seule)
const Chunk& World::getChunk(size_t x, size_t z) const {
    std::pair<int, int> key(x, z);  // Créer une clé unique pour ce chunk
    auto it = m_chunks.find(key);
    if (it != m_chunks.end()) {
        return it->second;  // Retourne le chunk si trouvé
    }
    // Gérer le cas où le chunk n'est pas trouvé, par exemple, en lançant une exception ou en retournant un chunk vide
    throw std::out_of_range("Chunk not found");
}

// Accéder à un chunk (modification possible)
Chunk& World::getChunk(size_t x, size_t z) {
    std::pair<int, int> key(x, z);  // Créer une clé unique pour ce chunk
    auto it = m_chunks.find(key);
    if (it == m_chunks.end()) {
        // Créer le chunk s'il n'existe pas
        m_chunks[key] = Chunk();
    }
    return m_chunks[key];  // Retourne la référence du chunk
}
