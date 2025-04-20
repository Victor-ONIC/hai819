#ifndef CHUNK_H_
#define CHUNK_H_

#include "Constants.h"
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#define GLEW_STATIC
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace C =
    Constants; //  Pour ne pas à avoir à écrire Constants:: à chaque fois

class Chunk {
public:
  Chunk(int x, int z);
  GLuint get_blocktype_buffer();
  GLuint get_faces_buffer();
  GLuint get_vao_blocktype();
  GLuint get_counter_faces();
  void reset_counter_faces();
  GLuint get_buffer_counter_faces();
  GLuint get_vao_faces();
  ~Chunk();

private:
  int m_x, m_z;
  GLuint m_buffer_blocktype, m_vao_blocktype; // SSBO et VAO reliés, 1 GLuint par bloc pour représenter son type
  GLuint m_buffer_faces, m_vao_faces; // SSBO et VAO reliés, faces visibles des blocs
  GLuint m_visibleFaceCounter, m_bufferVisibleFaceCounter; // counter face visible + id buffer
};

#endif // CHUNK_H_
