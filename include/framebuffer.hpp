#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include <vector>

#include <ngl/NGLInit.h>

class framebuffer
{
public:
    ~framebuffer();
    void init();
    void addTexture(int _width, int _height, bool _colour, bool _normal, bool _position, bool _depth);
    void bindForReading();
    void bindForWriting();
    GLuint getBuffer() {return m_framebuffer;}
    size_t size() {return m_colourTargets.size();}

    GLuint getPos(size_t _i) {return m_positionTargets[_i];}
    GLuint getCol(size_t _i) {return m_colourTargets[_i];}
    GLuint getNorm(size_t _i) {return m_normalTargets[_i];}
    GLuint getDepth(size_t _i) {return m_depthTargets[_i];}
private:
    GLuint m_framebuffer;
    std::vector<GLuint> m_positionTargets;
    std::vector<GLuint> m_colourTargets;
    std::vector<GLuint> m_normalTargets;
    std::vector<GLuint> m_depthTargets;

    GLuint genTexture(int _width, int _height, GLint _format);
};

#endif
