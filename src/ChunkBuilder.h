#ifndef CHUNKBUILDER_H_
#define CHUNKBUILDER_H_

#include "Chunk.h"
#include "Constants.h"
#include "Shader.h"
#include <fstream>
#include <iostream>
#include <memory> // Pour std::unique_ptr et std::shared_ptr
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

// Prend un Chunk, une pipeline de shader, et le construit
class ChunkBuilder {
public:
  ChunkBuilder();
  void addShader(const std::string &name);
  void build(Chunk* chunk);
  ~ChunkBuilder();

private:
  std::vector< std::shared_ptr<Shader>> m_shaders_pipeline;
};

#endif // CHUNKBUILDER_H_
