#ifndef CHUNK_H_
#define CHUNK_H_

#include "Constants.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#define GLEW_STATIC
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace C = Constants; //  Pour ne pas à avoir à écrire Constants:: à chaque fois

class Chunk {
public:
  Chunk(int x, int z);
  ~Chunk();
    void bind();
    GLuint get_buffer();
    GLuint get_vao();

private:
  int m_x, m_z;
  GLuint m_buffer, m_vao;
  std::vector<GLuint> m_data{C::BLOCKS_PER_CHUNK, 0};
};

#endif // CHUNK_H_
