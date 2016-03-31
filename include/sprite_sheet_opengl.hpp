#ifndef SPRITESHEETOPENGL_HPP
#define SPRITESHEETOPENGL_HPP

#include <ngl/NGLInit.h>

struct spriteSheet
{
    int width;
    GLuint textureID;
    ngl::VertexArrayObject *vao;
};


#endif
