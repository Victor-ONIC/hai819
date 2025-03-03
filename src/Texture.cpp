#include "Texture.h"

// On inclut stb_image ici car on le considère comme un .cpp.
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::string& path)
{
    // 1. Créer une texture.
    glGenTextures(1, &ID);

    // 2. Attacher la texture.
    glBindTexture(GL_TEXTURE_2D, ID);

    // 3. Modifier les propriétés de la texture.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 4. Injecter les données de cette texture.
    stbi_set_flip_vertically_on_load(true);

    int length, height, nb_channels;
    unsigned char* data = stbi_load(path.c_str(), &length, &height, &nb_channels, 4);

    if (data)
    {
        // Injecter les données de cette texture.
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, length, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        // Dire à OpenGL d'automatiquement générer les mipmaps pour cette texture.
        glGenerateMipmap(ID);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "ERREUR::TEXTURE - " << path << "\n";
    }
}

Texture::~Texture()
{
    glDeleteTextures(1, &ID);
}

void Texture::bind(unsigned int slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
