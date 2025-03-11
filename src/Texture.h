#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <iostream>

#define GLEW_STATIC
#include "GL/glew.h"

class Texture
{
    unsigned int ID;

public:
    Texture(uint8_t * buffer, size_t width, size_t height);
    Texture(const std::string& path);
    ~Texture();

    void bind(unsigned int slot);
    void unbind();
};

#endif
