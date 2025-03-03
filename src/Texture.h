#ifndef TEXTURE_
#define TEXTURE_

#include <string>
#include <iostream>

#define GLEW_STATIC
#include "GL/glew.h"

class Texture
{
    unsigned int ID;

public:
    Texture(const std::string& path);
    ~Texture();

    void bind(unsigned int slot);
    void unbind();
};

#endif
