#include "ChunkBuilder.h"
#include "Chunk.h"
#include "Constants.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "Noise.h"
#include "Texture.h"

ChunkBuilder::ChunkBuilder() {
    addShader("mapCompute");
    addShader("waterCompute");
}

//Permet d'ajouter un programme GPU à la pipeline de build
void ChunkBuilder::addShader(const std::string &name) {
  ShaderManager &shader_manager = ShaderManager::getInstance();
  m_shaders_pipeline.push_back(shader_manager.getShader(name));
}

// Prend un Chunk et exécute la pipeline pour le build
void ChunkBuilder::build(Chunk *chunk) {
  ShaderManager &shader_manager = ShaderManager::getInstance();
  Noise noise = Noise(256);//TODO
  Texture tex = Texture(noise.m_buffer, 256, 256);//TODO
  tex.bind(0);//TODO
  for (const auto &shader : m_shaders_pipeline) { // Référence constante, pas de copie
    shader->use();
    shader->set_uniform("permTexture", 0);//TODO
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, chunk->get_buffer());
    shader->set_all_uniforms();
    glDispatchCompute(C::BLOCKS_PER_CHUNK / 1024, 1, 1);
    shader->stop();
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
  }
  tex.unbind();
}

ChunkBuilder::~ChunkBuilder() {
}
