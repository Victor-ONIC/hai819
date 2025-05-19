#include "ChunkBuilder.h"
#include "Chunk.h"
#include "Constants.h"
#include "Noise.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "Texture.h"

ChunkBuilder::ChunkBuilder() {
  addShader("mapComputeHeight");
  addShader("grassCompute");
  addShader("waterCompute");
  addShader("mapCompute3D");
  addShader("mapComputeVoronoi");
  addShader("genVertices");
}

// Permet d'ajouter un programme GPU à la pipeline de build
void ChunkBuilder::addShader(const std::string &name) {
  ShaderManager &shader_manager = ShaderManager::getInstance();
  m_shaders_pipeline.push_back(shader_manager.getShader(name));
}

// Prend un Chunk et exécute la pipeline pour le build
void ChunkBuilder::build(Chunk *chunk) {
  ShaderManager &shader_manager = ShaderManager::getInstance();
  chunk->is_being_processed = true;
  for (const auto &shader :
       m_shaders_pipeline) { // Référence constante, pas de copie
    //std::cout << "Using : " << shader->get_path() << std::endl;
    shader->use();
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0,
                     chunk->get_blocktype_buffer());
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, chunk->get_faces_buffer());
    glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 2,
                     chunk->get_buffer_counter_faces());
    shader->set_uniform("chunk_x", chunk->get_xz()[0]);
    shader->set_uniform("chunk_z", chunk->get_xz()[1]);
    shader->set_all_uniforms();
    shader->stop();
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    //glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT);

  }

  // Insère une fence pour savoir quand c’est fini
  if (chunk->m_fence) glDeleteSync(chunk->m_fence);
  //std::cout << "Fence ->       " << chunk->m_fence << std::endl;
  chunk->m_fence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
  chunk->get_counter_faces();
  chunk->print_counter_faces();
  // chunk->print_faces();
  chunk->genLowMemBuffer();
}

ChunkBuilder::~ChunkBuilder() {}
