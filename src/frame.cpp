#include "frame.hpp"

frame::frame(int _width, int _height)
{
    m_width = _width;
    m_height = _height;

    m_size = 0;

    m_diffuse = 0;
    m_normal = 0;
    m_position = 0;
    m_depth = 0;
    m_uv = 0;
}

void frame::addDepthComponent()
{

}

void frame::addDiffuseComponent()
{

}

void frame::addNormalComponent()
{

}

void frame::addPositionComponent()
{

}

void frame::addUVComponent()
{

}

GLuint frame::genTexture(GLint _format)
{
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexImage2D(GL_TEXTURE_2D, 0, _format, m_width, m_height, 0, _format, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    return tex;
}
