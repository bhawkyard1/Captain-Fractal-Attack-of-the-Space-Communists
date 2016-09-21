#ifndef RENDERER_OPENGL_HPP
#define RENDERER_OPENGL_HPP

#define MAX_LIGHTS 256

#include <array>
#include <string>
#include <unordered_map>
#include <vector>

#include "camera.hpp"
#include "common.hpp"
#include "enemy.hpp"
#include "faction.hpp"
//#include "framebuffer.hpp"
#include "laser.hpp"
#include "missile.hpp"
#include "player.hpp"
#include "pointLight.hpp"
#include "sprite_sheet_opengl.hpp"
#include "ship.hpp"
#include "vectors.hpp"

#if RENDER_MODE == 1

#include <ngl/NGLInit.h>
#include <ngl/Transformation.h>
#include <ngl/ShaderLib.h>
#include <ngl/Obj.h>

#define AMBIENT_RESOLUTION_DIVIDER 256

//----------------------------------------------------------------------------------------------------------------------
/// \file renderer_opengl.hpp
/// \brief This class abstracts rendering functionality, providing the ability to draw simple shapes, and models loaded at instantiation.
/// \author Ben Hawkyard
/// \version 1.0
/// \date 11/04/16
/// Revision History :
/// This is an initial version used for the game, an alternative SDL based renderer is available in renderer.hpp
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
    void useShader(const std::string _shader) {m_shader->use(_shader);}

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

    void addLight(pointLight _light, size_t _i) {m_pointLights[_i] = _light; m_activeLights = _i;}
    void resetLights() {m_pointLights.fill({vec3(0.0f), vec3(0.0f)}); m_activeLights = 0;}

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
    void packExtraData(const std::array<float, 4> _data) {for(int i = 0; i < 6; ++i) m_genericData.push_back(_data);}

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

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Loads an obj file and texture, returns a pointer to the ngl::Obj
    /// \param _path path of the model
    /// \param _append for other parts of the mesh, ie loadObj("ship_1", "_base") would load ship_1_base.obj (useful for objects with multiple meshes)
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Obj * loadObj(const std::string _path, const std::string _append);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Loads an obj and texture file, stores it in m_models
    /// \param _key stores it in the unordered map with this key
    /// \param _path path to the asset files
    //----------------------------------------------------------------------------------------------------------------------
    void loadAsset(const std::string _key, const std::string _path);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Draws an textured model
    /// \param _p position
    /// \param _ang asset angle
    /// \param _asset asset key in m_objects
    //----------------------------------------------------------------------------------------------------------------------
    void drawAsset(const vec3 _p, const float _ang, const std::string _asset);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Draws an textured model, but with a supplied alpha
    /// \param _p position
    /// \param _ang asset angle
    /// \param _asset asset key in m_objects
    /// \param _alpha transparency
    //----------------------------------------------------------------------------------------------------------------------
    void drawAsset(const vec2 _p, const float _ang, const std::string _asset, const float _alpha);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Draws an shield of a given radius, with a fancy shader
    /// \param _p position
    /// \param _r radius
    /// \param _dt global time for shader
    /// \param _alpha transparency
    /// \param _col colour tint
    //----------------------------------------------------------------------------------------------------------------------
    void drawShield(const vec3 _p, const float _r, const float _dt, const float _alpha, const std::array<float, 4> _col);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Draws a ship
    /// \param _p position
    /// \param _ang asset angle
    /// \param _asset asset key in m_objects
    /// \param _col colour of muzzle flash
    //----------------------------------------------------------------------------------------------------------------------
    void drawShip(const vec3 _p, const float _ang, const std::string _asset, const std::array<float, 4> _lCol);

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

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Draws white text
    /// \param _text text to draw
    /// \param _font font to use
    /// \param _pos start position
    /// \param _ws world or screen space
    /// \param _scale text scale
    //----------------------------------------------------------------------------------------------------------------------
    void drawText(std::string _text, std::string _font, vec2 _pos, const bool _ws, const float _scale);

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

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Gets the radius of a ship
    /// \param _type the type of ship (types defined in ship header)
    //----------------------------------------------------------------------------------------------------------------------
    float getTextureRadius(ship_spec _type) {return g_texture_keys[(_type)].m_radius;}

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
    GLuint SDLSurfaceToGLTexture(SDL_Surface * _s);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Enables depth-based fragment culling
    //----------------------------------------------------------------------------------------------------------------------
    void enableDepthSorting();

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Disables depth-based fragment culling
    //----------------------------------------------------------------------------------------------------------------------
    void disableDepthSorting();

    uniqueID getFocus() {return m_focus;}

    camera * getCamera() {return &m_camera;}

    void useAttachments(const std::vector<GLenum> _bufs) {glDrawBuffers(_bufs.size(), &_bufs[0]);}
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
    GLuint m_framebuffer;

    GLuint m_bufferBackgroundDiffuse;
    GLuint m_bufferBackgroundDepth;
    GLuint m_bufferLitDiffuse;
    GLuint m_bufferLitNormal;
    GLuint m_bufferLitPosition;
    GLuint m_bufferEffectsDiffuse;

    GLuint m_tinyFramebuffer;
    GLuint m_bufferDownscaledBackgroundDiffuse;

    GLuint genTexture(int _width, int _height, GLint _format, GLint _internalFormat);

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
    /// \brief An instance of ShaderLib
    //----------------------------------------------------------------------------------------------------------------------
    ngl::ShaderLib * m_shader;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief A container of models and textures
    //----------------------------------------------------------------------------------------------------------------------
    std::unordered_map<std::string, std::vector<ngl::Obj*>> m_models;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief A generic half-sphere, used to draw shields for all of the ships in-game
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Obj * m_shield;

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

    uniqueID m_focus;

    std::array<pointLight, MAX_LIGHTS> m_pointLights;
    int m_activeLights;
};

#endif

#endif

