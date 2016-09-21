#ifndef FRAME_HPP
#define FRAME_HPP

#include <ngl/NGLInit.h>

class frame
{
public:
    frame(int _width, int _height);

    void addDiffuseComponent();
    void addNormalComponent();
    void addPositionComponent();
    void addDepthComponent();
    void addUVComponent();

    size_t size() const {return m_size;}
private:
    int m_width;
    int m_height;
    GLuint m_diffuse;
    GLuint m_normal;
    GLuint m_position;
    GLuint m_depth;
    GLuint m_uv;
    size_t m_size;

    GLuint genTexture(GLint _format);
};

#endif
