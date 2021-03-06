#ifndef RENDERER_OPENGL_HPP
#define RENDERER_OPENGL_HPP

#include <array>
#include <string>
#include <unordered_map>
#include <vector>

#include "assetStore.hpp"
#include "camera.hpp"
#include "common.hpp"
#include "enemy.hpp"
#include "faction.hpp"
#include "framebuffer.hpp"
#include "laser.hpp"
#include "missile.hpp"
#include "player.hpp"
#include "pointLight.hpp"
#include "sprite_sheet_opengl.hpp"
#include "ship.hpp"
#include "vectors.hpp"

#include <ngl/NGLInit.h>
#include <ngl/Transformation.h>
#include <ngl/ShaderLib.h>
#include <ngl/Obj.h>
#include <ngl/VAOPrimitives.h>

#define MAX_LIGHTS 512
#define AMBIENT_RESOLUTION_DIVIDER 256

//----------------------------------------------------------------------------------------------------------------------
/// \file renderer_opengl.hpp
/// \brief This class abstracts rendering functionality, providing the ability to draw simple shapes, and models loaded at instantiation.
/// \author Ben Hawkyard
/// \version 1.0
/// \date 11/04/16
/// Revision History :
/// This is an initial version used for the game
/// \class renderer_ngl
/// \brief Contains all the data for the window, elements, primitives to be drawn.
//----------------------------------------------------------------------------------------------------------------------

class renderer_ngl
{
public:
    //----------------------------------------------------------------------------------------------------------------------
    /// \brief ctor for the ngl renderer
    /// \param _w window width
    /// \param _h window height
    //----------------------------------------------------------------------------------------------------------------------
    renderer_ngl();

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief dtor for our NGL drawing class
    //----------------------------------------------------------------------------------------------------------------------
    ~renderer_ngl();

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Initialises SDL for use in creating the window
    //----------------------------------------------------------------------------------------------------------------------
    int init();

    void loadShips();

    void bindTextureToSampler(const GLint _shaderID, const GLuint _tex, const char * _uniform, int _target);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Creates a shader program using a vertex and fragment file, using ngl::ShaderLib
    /// \param _name name of the program,
    /// \param _vert name of the vertex file (minus .glsl)
    /// \param _frag name of the frag file (minus .glsl)
    //----------------------------------------------------------------------------------------------------------------------
    void createShaderProgram(const std::string _name, const std::string _vert, const std::string _frag);
    void createShaderProgramVGF(const std::string _name, const std::string _vert, const std::string _geo, const std::string _frag);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Updates the renderer, mostly for screen shaking
    /// \param _dt time difference since last call
    //----------------------------------------------------------------------------------------------------------------------
    void update(float _dt, base *_focus);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Sets the shader for the renderer to use
    /// \param _shader name of the shader program
    //----------------------------------------------------------------------------------------------------------------------
    void useShader(const std::string _shader) {ngl::ShaderLib * slib = ngl::ShaderLib::instance(); slib->use(_shader);}
    void activeColourAttachments(const std::vector<GLenum> _bufs);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Creates a VAO, returns the id
    /// \param _verts vector of vertices
    //----------------------------------------------------------------------------------------------------------------------
    GLuint createVAO(std::vector<ngl::Vec3> _verts);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Creates a VAO, returns the id
    /// \param _verts vector of vertices
    /// \param _cols vector of colours
    //----------------------------------------------------------------------------------------------------------------------
    GLuint createVAO(std::vector<ngl::Vec3> _verts, std::vector<ngl::Vec4> _cols);

    GLuint createVAO(std::vector<ngl::Vec3> _verts, std::vector<ngl::Vec2> _UVs);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Creates a VAO, returns the id
    /// \param _verts vector of vertices
    /// \param _cols vector of colours
    /// \param _UVs vector of uv coordinates
    //----------------------------------------------------------------------------------------------------------------------
    GLuint createVAO(std::vector<ngl::Vec3> _verts, std::vector<ngl::Vec4> _cols, std::vector<ngl::Vec2> _UVs);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Clears the vectors m_verts, m_colours and m_UVs
    /// \param _verts vector of vertices
    /// \param _cols vector of colours
    /// \param _UVs vector of uv coordinates
    //----------------------------------------------------------------------------------------------------------------------
    void clearVectors();

    void addLight(pointLight _light) {m_pointLights.push_back(_light);}
    void resetLights() {m_pointLights.clear();}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Draws the background
    /// \param _dt time difference
    /// \param _v velocity of the universe
    /// \param _p position of the universe
    /// \param _cCol colour of the universe
    //----------------------------------------------------------------------------------------------------------------------
    void drawBackground(float _dt, vec2 _v, vec2 _p, std::array<float, 3> _cCol);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Draws a button (ie glorified coloured rectangle)
    /// \param _p position of the button
    /// \param _d button dimensions
    /// \param _ang angle of the button
    /// \param _col colour of the button
    //----------------------------------------------------------------------------------------------------------------------
    void drawbutton(const vec3 _p, const vec2 _d, const float _ang, std::array<float, 4> _col);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Adds six verts to the member vertex attribute vectors. This allows me to batch draw simple objects.
    /// \param _dt time difference
    /// \param _v velocity of the universe
    /// \param _p position of the universe
    /// \param _cCol colour of the universe
    //----------------------------------------------------------------------------------------------------------------------
    void addRect(const vec3 _p, const vec2 _d, const float _ang, const std::array<float, 4> _col);
    void addPoint(const vec3 _point);
    void addColour(const std::array<float, 4> _col);
    void packExtraData(const std::array<float, 4> _data, int _num) {for(int i = 0; i < _num; ++i) m_genericData.push_back(_data);}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Takes the vert attribute vectors, treats them as rectangles, and draw them.
    /// \param _ws place the rectangles in the world (true), or on the screen (false)
    //----------------------------------------------------------------------------------------------------------------------
    void drawRects(const bool _ws);

    void drawXP(const float _xp);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Draw a single rectangle
    /// \param _p position of the rectangle
    /// \param _d dimensions
    /// \param _ang angle
    /// \param _ws place in-world (true) or on the screen (false)
    //----------------------------------------------------------------------------------------------------------------------
    void drawRect(const vec3 _p, const vec2 _d, const float _ang, const bool _ws);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Draws m_verts as rectangles, using the smoke shader
    /// \param _dt time step for the shader
    //----------------------------------------------------------------------------------------------------------------------
    void drawSmoke(const float _dt);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Draws a circle
    /// \param _p position
    /// \param _d radius
    /// \param _ws world space (true) or screen space (false)
    //----------------------------------------------------------------------------------------------------------------------
    void drawCircle(const vec3 _p, const float _d, const bool _ws);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Makes the member gl context and window active
    //----------------------------------------------------------------------------------------------------------------------
    void makeCurrent() const { SDL_GL_MakeCurrent(m_window, m_gl_context); }

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Clears the window
    //----------------------------------------------------------------------------------------------------------------------
    void clear();

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Exits the program, prints and error message
    /// \param _msg message to print
    //----------------------------------------------------------------------------------------------------------------------
    void errorExit(const std::string &_msg);

    void resetTransform() {m_transform.reset();}
    void setTransform(const vec3 _p) {m_transform.setPosition(_p.m_x, _p.m_y, _p.m_z);}
    void setTransform(const vec3 _p, const float _a) {m_transform.setPosition(_p.m_x, _p.m_y, _p.m_z); m_transform.setRotation(0.0f, 0.0f, _a);}
    void setTransform(const vec3 _p, const float _a, const vec3 _s)
    {
        m_transform.setPosition(_p.m_x, _p.m_y, _p.m_z);
        m_transform.setRotation(0.0f, 0.0f, _a);
        m_transform.setScale(_s.m_x, _s.m_y, _s.m_z);
    }
    void setTransform(const float _a) {m_transform.setRotation(0.0f, 0.0f, _a);}
    void setTransform(const vec3 _p, const vec3 _s)
    {
        m_transform.setPosition(_p.m_x, _p.m_y, _p.m_z);
        m_transform.setScale(_s.m_x, _s.m_y, _s.m_z);
    }

    void bindTextureToShader(const std::string &_shaderID, const GLuint _tex, const char *_uniform, int _target, GLenum _type = GL_TEXTURE_2D);
    ///
    /// @brief Draws a specified asset with a texture and shader.
    /// @param _model, The string id of the model, from the asset store.
    /// @param _texture, The string id of the texture, from the asset store.
    /// @param _shader, The string id of the shader.
    ///
    void drawAsset(const std::string &_model, const std::string &_texture, const std::string &_shader);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Treats the entries in the vertex attribute vectors as lines, and draws them with a laser shader
    //----------------------------------------------------------------------------------------------------------------------
    void drawLasers(const float _globalTime);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Treats the entries in the vertex attribute vectors as lines, and draws them
    /// \param _width width of the lines
    //----------------------------------------------------------------------------------------------------------------------
    void drawLines(float _width);

    void drawExplosions();

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Draws jet flames
    /// \param _width width of the lines
    //----------------------------------------------------------------------------------------------------------------------
    void drawFlames(const vec3 _pos, const vec2 _d, float _ang, std::array<float, 4> _col, const float _t, const float _speed);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Loads ascii characters using SDL_TTF, and saves them as openGL textures.
    /// \param _name the name to give the vector of
    //----------------------------------------------------------------------------------------------------------------------
    void loadFontSpriteSheet(std::string _name, std::string _path, int _size);

    GLuint loadTexture(const std::string &_path, const int _format);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Draws white text
    /// \param _text text to draw
    /// \param _font font to use
    /// \param _pos start position
    /// \param _ws world or screen space
    /// \param _scale text scale
    //----------------------------------------------------------------------------------------------------------------------
    void drawText(const std::string _text, std::string _font, vec2 _pos, const bool _ws, const float _scale);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Draws coloured text
    /// \param _text text to draw
    /// \param _font font to use
    /// \param _pos start position
    /// \param _ws world or screen space
    /// \param _scale text scale
    /// \param _col colour
    //----------------------------------------------------------------------------------------------------------------------
    void drawText(std::string _text, std::string _font, vec2 _pos, const bool _ws, const float _scale, const std::array<float, 4> _col);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Adds a line to the member vertex attribute vectors, with position, uvs and colour
    /// \param _start start of the line
    /// \param _end end of the line
    /// \param _lCol colour of the line
    //----------------------------------------------------------------------------------------------------------------------
    void addLine(const vec3 _start, const vec3 _end, const float _width, const std::array<float, 4> _lCol);

    //UI drawing
    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Draws the minimap
    /// \param _mp ref to missile vector
    /// \param _ep ref to agent vector
    /// \param _ap ref to asteroid vector
    /// \param _lp ref to laser vector
    /// \param _fp ref to faction pointer
    //----------------------------------------------------------------------------------------------------------------------
    void drawMap(std::vector<missile> * _mp, std::vector<enemy> *_ep, std::vector<ship> * _ap, std::vector<laser> * _lp, std::vector<faction> * _fp, const bool _mode);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Draws health, shield and energy bars
    /// \param _ply ref to player
    //----------------------------------------------------------------------------------------------------------------------
    void statusBars(player * _ply);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Draw the stats of the current weapon
    /// \param _ply ref to the player
    //----------------------------------------------------------------------------------------------------------------------
    void drawWeaponStats(player * _ply);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Swaps the window
    //----------------------------------------------------------------------------------------------------------------------
    void finalise(const float _t, const vec2 _vel);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Loads an image and returns a SDL_Surface
    /// \param _path path to image file
    //----------------------------------------------------------------------------------------------------------------------
    SDL_Surface * getSurface(std::string _path);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Adds camera shake
    /// \param _s shake magnitude
    //----------------------------------------------------------------------------------------------------------------------
    void addShake(float _s);
    vec3 getShake() {return m_camera.getCamPos();}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Converts an SDL_Surface to an openGL texture, returns the ID
    /// \param _s the input SDL surface
    //----------------------------------------------------------------------------------------------------------------------
    GLuint SDLSurfaceToGLTexture(SDL_Surface * _s, int _format);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Enables depth-based fragment culling
    //----------------------------------------------------------------------------------------------------------------------
    void enableDepthTesting();

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Disables depth-based fragment culling
    //----------------------------------------------------------------------------------------------------------------------
    void disableDepthTesting();

    slot getFocus() {return m_focus;}

    camera * getCamera() {return &m_camera;}

    void drawingDeferredElements();
    void drawingNonLitElements();
    void drawingUI();
private:
    //----------------------------------------------------------------------------------------------------------------------
    /// \brief The window the game will be drawn in
    //----------------------------------------------------------------------------------------------------------------------
    SDL_Window * m_window = NULL;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Window dimensions
    //----------------------------------------------------------------------------------------------------------------------
    int m_w, m_h;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Framebuffer stuff
    //----------------------------------------------------------------------------------------------------------------------
    framebuffer m_dataFB;
    framebuffer m_nonlitFB;
    std::array<framebuffer, 3>  m_postFB;
    framebuffer m_smallBackgroundFB;
    framebuffer m_uiFB;

    /*GLuint m_bufferBackgroundDiffuse;
    GLuint m_bufferBackgroundDepth;
    GLuint m_bufferLitDiffuse;
    GLuint m_bufferLitNormal;
    GLuint m_bufferLitPosition;
        GLuint m_bufferEffectsDiffuse;*/

    /*GLuint m_tinyFramebuffer;
    GLuint m_bufferDownscaledBackgroundDiffuse;

        GLuint genTexture(int _width, int _height, GLint _format, GLint _internalFormat);*/

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Container of letter sprites, can be used to draw text
    //----------------------------------------------------------------------------------------------------------------------
    std::unordered_map<std::string, spriteSheet> m_letters;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief The view matrix
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Mat4 m_view;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Projection matrix for objects in world-space
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Mat4 m_project;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Projection matrix for the ui (which is defined in screen-space)
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Mat4 m_uiProject;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief The view-project matrix.
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Mat4 m_VP;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief This is used to transform objects before they are drawn
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Transformation m_transform;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief A container of models and textures
    //----------------------------------------------------------------------------------------------------------------------
    assetStore m_assets;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief The openGL context
    //----------------------------------------------------------------------------------------------------------------------
    SDL_GLContext m_gl_context;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief A VAO containing a single point
    //----------------------------------------------------------------------------------------------------------------------
    GLuint m_pointVAO;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief A screen-quad VAO
    //----------------------------------------------------------------------------------------------------------------------
    GLuint m_screenQuadVAO;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief A unit square VAO
    //----------------------------------------------------------------------------------------------------------------------
    GLuint m_unit_square_vao;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief A VAO for rendering sprites, contains UVs
    //----------------------------------------------------------------------------------------------------------------------
    GLuint m_spriteVAO;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief A generic VAO, used in many places
    //----------------------------------------------------------------------------------------------------------------------
    GLuint m_vao;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief A generic VBO, to be used for positional data
    //----------------------------------------------------------------------------------------------------------------------
    GLuint m_vertBuffer;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief A generic VBO, to be used for UV data
    //----------------------------------------------------------------------------------------------------------------------
    GLuint m_UVBuffer;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief A generic VBO, to be used for colour data
    //----------------------------------------------------------------------------------------------------------------------
    GLuint m_colourBuffer;

    GLuint m_genericBuffer;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief A vector containing positional data, can be loaded up then passed off to one of the member VBOs
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<vec3> m_verts;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief A vector containing colour data, can be loaded up then passed off to one of the member VBOs
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<std::array<float, 4>> m_colours;

    std::vector<std::array<float, 4>> m_genericData;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief A vector containing UV data, can be loaded up then passed off to one of the member VBOs
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<vec2> m_UVs;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Loads the MVP to the active shader
    //----------------------------------------------------------------------------------------------------------------------
    void loadMatricesToShader();

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Loads the modelling and projection matrices to the active shader
    //----------------------------------------------------------------------------------------------------------------------
    void loadTransformToShader();

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Draws the contents of the custom buffers to the screen
    //----------------------------------------------------------------------------------------------------------------------
    void drawCustomBuffers(const float _t, const vec2 _vel);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief The amount of camera shake
    //----------------------------------------------------------------------------------------------------------------------
    float m_cameraShake;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief The target position of the camera shake
    //----------------------------------------------------------------------------------------------------------------------
    vec2 m_cameraShakeTargetOffset;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief The current camera shake
    //----------------------------------------------------------------------------------------------------------------------
    vec2 m_cameraShakeOffset;

    camera m_camera;

    slot m_focus;

    std::vector<pointLight> m_pointLights;
    GLuint m_lightbuffer;
};

#endif
