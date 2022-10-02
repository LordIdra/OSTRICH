#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "glad/glad.h"



Texture::Texture(const char *path) {
    // Load image
    unsigned char *data = stbi_load(path, &width, &height, &channels, 0); 
    
    // Generate texture
    glGenTextures(1, &id);
    Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Free memory
    stbi_image_free(data);
}

auto Texture::Bind() -> void {
    glBindTexture(GL_TEXTURE_2D, id);
}