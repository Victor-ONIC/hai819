#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <iostream>
#include <vector>

#include "GL/glew.h"

class Texture
{
    unsigned int ID;//TODO à passer en private ?

public:
    //Pour remplir une texture avec du Perlin noise
    Texture(std::vector<uint8_t> buffer, size_t width, size_t height);//TODO à rendre plus générale
    //Pour remplir une texture avec une image
    Texture(const std::string& path);
    ~Texture();

    void bind(unsigned int slot);
    void unbind();
};

#endif
