#pragma once

#include <functional> // Pour std::hash

struct ChunkKey {
    int x, z;

    ChunkKey(int x, int z) : x(x), z(z) {}

    bool operator==(const ChunkKey& other) const {
        return x == other.x && z == other.z;
    }
};

// Spécialisation de std::hash pour ChunkKey
 namespace std {
    template <>
    struct hash<ChunkKey> {
        size_t operator()(const ChunkKey& key) const {
            size_t h1 = std::hash<int>{}(key.x);
            size_t h2 = std::hash<int>{}(key.z);
            return h1 ^ (h2 * 0x9e3779b9); // Constante de Fibonacci pour mieux répartir les valeurs
        }
    };
}
/*
namespace std {
    template <>
    struct hash<ChunkKey> {
        size_t operator()(const ChunkKey& key) const {
            return std::hash<int>()(key.x) ^ (std::hash<int>()(key.z) << 1);

        }
    };
}
*/
