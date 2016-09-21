#include <iostream>

#include "framebuffer.hpp"

framebuffer::~framebuffer()
{    
    if(m_colourTargets.size() > 0) glDeleteTextures(m_colourTargets.size(), &m_colourTargets[0]);
    if(m_normalTargets.size() > 0) glDeleteTextures(m_normalTargets.size(), &m_normalTargets[0]);

    if(m_depthTargets.size() > 0) glDeleteFramebuffers(m_depthTargets.size(), &m_depthTargets[0]);
}

void framebuffer::init()
{
    glGenFramebuffers(1, &m_framebuffer);
}

void framebuffer::addTexture(int _width, int _height, bool _colour, bool _normal, bool _position, bool _depth)
{
    glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER, m_framebuffer);

    GLuint diffuseTex = 0;
    GLuint normalTex = 0;
    GLuint positionTex = 0;
    GLuint depthTex = 0;

    //Colour texture will be bound at index, normals at index + 1, position at index + 2 and depth at index + 3.
    size_t index = size() * 4;

    if(_colour)
    {
        diffuseTex = genTexture(_width, _height, GL_RGBA);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, diffuseTex, 0);
    }

    if(_normal)
    {
        normalTex = genTexture(_width, _height, GL_RGB);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index + 1, GL_TEXTURE_2D, normalTex, 0);
    }

    if(_position)
    {
        positionTex = genTexture(_width, _height, GL_RGB);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index + 2, GL_TEXTURE_2D, positionTex, 0);
    }

    if(_depth)
    {
        glGenTextures(1, &depthTex);
        glBindTexture(GL_TEXTURE_2D, depthTex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT,0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex, 0);
    }

    //Tell the framebuffers which attachments we will use for colour.
    std::vector<GLenum> drawBuffers;
    for(int i = 0; i < m_colourTargets.size(); ++i)
    {
        int i4 = i * 4;
        drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i4);
        drawBuffers.push_back(GL_COLOR_ATTACHMENT1 + i4);
        drawBuffers.push_back(GL_COLOR_ATTACHMENT2 + i4);
        drawBuffers.push_back(GL_COLOR_ATTACHMENT3 + i4);
    }

    glDrawBuffers(drawBuffers.size(), &drawBuffers[0]);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {

        std::cerr << "Error! Framebuffer add target failed!\n";
        exit(EXIT_FAILURE);
    }

    m_colourTargets.push_back(diffuseTex);
    m_normalTargets.push_back(normalTex);
    m_positionTargets.push_back(positionTex);
    m_depthTargets.push_back(depthTex);

    glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER, 0);
}

GLuint framebuffer::genTexture(int _width, int _height, GLint _format)
{
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexImage2D(GL_TEXTURE_2D, 0, _format, _width, _height, 0, _format, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    return tex;
}
