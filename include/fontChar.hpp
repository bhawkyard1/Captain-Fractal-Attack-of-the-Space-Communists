#ifndef FONTCHAR_HPP
#define FONTCHAR_HPP

#include <ngl/NGLInit.h>

struct fontChar
{
    int width;
    GLuint textureID;
    ngl::VertexArrayObject *vao;
};


#endif
