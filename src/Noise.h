#ifndef NOISE_H_
#define NOISE_H_

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#define GLEW_STATIC
#include "GL/glew.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

class Noise{
public:
    size_t m_height;
    uint8_t * m_buffer;

    Noise(size_t height);

private:
};

#endif
