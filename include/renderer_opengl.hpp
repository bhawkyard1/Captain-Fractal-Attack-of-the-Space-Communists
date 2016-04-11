#ifndef RENDERER_OPENGL_HPP
#define RENDERER_OPENGL_HPP

#include <string>
#include <unordered_map>
#include <array>
#include <vector>

#include "vectors.hpp"

#include "common.hpp"
#include "missile.hpp"
#include "enemy.hpp"
#include "ship.hpp"
#include "laser.hpp"
#include "player.hpp"
#include "faction.hpp"
#include "sprite_sheet_opengl.hpp"

#if RENDER_MODE == 1

#include <ngl/NGLInit.h>
#include <ngl/Transformation.h>
#include <ngl/ShaderLib.h>
#include <ngl/Obj.h>

//#include <ngl/Text.h>

//----------------------------------------------------------------------------------------------------------------------
/// @file renderer_opengl.hpp
/// @brief This class abstracts rendering functionality, providing the ability to draw simple shapes, and models loaded at instantiation.
/// @author Ben Hawkyard
/// @version 1.0
/// @date 11/04/16
/// Revision History :
/// This is an initial version used for the game, an alternative SDL based renderer is available in renderer.hpp
/// @class renderer_ngl
/// @brief Contains all the data for the window, elements, primitives to be drawn.
//----------------------------------------------------------------------------------------------------------------------

class renderer_ngl
{
  //The window we'll be rendering to
  SDL_Window * m_window = NULL;
  //SDL_Renderer * m_renderer = NULL;
  int m_w, m_h;

  std::unordered_map<std::string, spriteSheet> m_letters;

  ngl::Mat4 m_view;
  ngl::Mat4 m_project;
  ngl::Mat4 m_uiProject;
  ngl::Mat4 m_VP;
  ngl::Transformation m_transform;
  ngl::ShaderLib * m_shader;

  std::unordered_map<std::string, std::vector<ngl::Obj*>> m_models;
  ngl::Obj * m_shield;

  ngl::Obj * m_test_ship;
  ngl::Texture m_test_texture;
  GLuint m_test_texture_id;

  SDL_GLContext m_gl_context;

  GLuint m_pointVAO;
  GLuint m_screenQuadVAO;
  GLuint m_unit_square_vao;
  GLuint m_spriteVAO;
  GLuint m_vao;

  GLuint m_vertBuffer;
  GLuint m_UVBuffer;
  GLuint m_colourBuffer;

  std::vector<vec3> m_verts;
  std::vector<std::array<float, 4>> m_colours;
  std::vector<vec2> m_UVs;

  void loadMatricesToShader();
  void loadTransformToShader();

  float m_cameraShake;
  vec2 m_cameraShakeTargetOffset;
  vec2 m_cameraShakeOffset;
public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief ctor for the ngl renderer
  /// @param _w window width, _h window height
  //----------------------------------------------------------------------------------------------------------------------
  renderer_ngl(int _w, int _h);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief dtor for our NGL drawing class
  //----------------------------------------------------------------------------------------------------------------------
  ~renderer_ngl();

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Initialises SDL for use in creating the window
  //----------------------------------------------------------------------------------------------------------------------
  int init();

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Creates a shader program using a vertex and fragment file, using ngl::ShaderLib
  /// @param _name name of the program, _vert name of the vertex file (minus .glsl), _frag name of the frag file (minus .glsl)
  //----------------------------------------------------------------------------------------------------------------------
  void createShaderProgram(const std::string _name, const std::string _vert, const std::string _frag);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Updates the renderer, mostly for screen shaking
  /// @param _dt time difference since last call
  //----------------------------------------------------------------------------------------------------------------------
  void update(const float _dt);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Sets the shader for the renderer to use
  /// @param _shader name of the shader program
  //----------------------------------------------------------------------------------------------------------------------
  void useShader(const std::string _shader) {m_shader->use(_shader);}

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Creates a VAO, returns the id
  /// @param _verts vector of vertices
  //----------------------------------------------------------------------------------------------------------------------
  GLuint createVAO(std::vector<ngl::Vec3> _verts);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Creates a VAO, returns the id
  /// @param _verts vector of vertices, _cols vector of colours
  //----------------------------------------------------------------------------------------------------------------------
  GLuint createVAO(std::vector<ngl::Vec3> _verts, std::vector<ngl::Vec4> _cols);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Creates a VAO, returns the id
  /// @param _verts vector of vertices, _cols vector of colours, _UVs vector of uv coordinates
  //----------------------------------------------------------------------------------------------------------------------
  GLuint createVAO(std::vector<ngl::Vec3> _verts, std::vector<ngl::Vec4> _cols, std::vector<ngl::Vec2> _UVs);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Clears the vectors m_verts, m_colours and m_UVs
  /// @param _verts vector of vertices, _cols vector of colours, _UVs vector of uv coordinates
  //----------------------------------------------------------------------------------------------------------------------
  void clearVectors();

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Draws the background
  /// @param _dt time difference, _v velocity of the universe, _p position of the universe, _cCol colour of the universe
  //----------------------------------------------------------------------------------------------------------------------
  void drawBackground(float _dt, vec2 _v, vec2 _p, std::array<float, 3> _cCol);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Draws a button (ie glorified coloured rectangle)
  /// @param _p position of the button, _d button dimensions, _ang angle of the button, _col colour of the button
  //----------------------------------------------------------------------------------------------------------------------
  void drawButton(const vec2 _p, const vec2 _d, const float _ang, std::array<float, 4> _col);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Adds six verts to the member vertex attribute vectors. This allows me to batch draw simple objects.
  /// @param _dt time difference, _v velocity of the universe, _p position of the universe, _cCol colour of the universe
  //----------------------------------------------------------------------------------------------------------------------
  void addRect(const vec2 _p, const vec2 _d, const float _ang, const std::array<float, 4> _col);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Takes the vert attribute vectors, treats them as rectangles, and draw them.
  /// @param _ws place the rectangles in the world (true), or on the screen (false)
  //----------------------------------------------------------------------------------------------------------------------
  void drawRects(const bool _ws);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Draw a single rectangle
  /// @param _p position of the rectangle, _d dimensions, _ang angle, _ws place in-world (true) or on the screen (false)
  //----------------------------------------------------------------------------------------------------------------------
  void drawRect(const vec2 _p, const vec2 _d, const float _ang, const bool _ws);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Draws a rectangle with the smoke shader
  /// @param _p position, _d dimension, _dt global time for shader, _col colour tint
  //----------------------------------------------------------------------------------------------------------------------
  void drawSmoke(const vec2 _pos, const vec2 _d, const float _dt, const std::array<float, 4> _col);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Draws a circle
  /// @param _p position, _d radius, _ws world space (true) or screen space (false)
  //----------------------------------------------------------------------------------------------------------------------
  void drawCircle(const vec2 _p, const float _d, const bool _ws);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Makes the member gl context and window active
  //----------------------------------------------------------------------------------------------------------------------
  void makeCurrent() const { SDL_GL_MakeCurrent(m_window, m_gl_context); }

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Swaps the window
  //----------------------------------------------------------------------------------------------------------------------

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Clears the window
  //----------------------------------------------------------------------------------------------------------------------
  void clear() const {glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);}

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Exits the program, prints and error message
  /// @param _msg message to print
  //----------------------------------------------------------------------------------------------------------------------
  void errorExit(const std::string &_msg);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Loads an obj file and texture, returns a pointer to the ngl::Obj
  /// @param _path path of the model, _append for other parts of the mesh, ie loadObj("ship_1", "_base") would load ship_1_base.obj (useful for objects with multiple meshes).
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Obj * loadObj(const std::string _path, const std::string _append);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Loads an obj and texture file, stores it in m_models
  /// @param _key stores it in the unordered map with this key, _path path to the asset files
  //----------------------------------------------------------------------------------------------------------------------
  void loadAsset(const std::string _key, const std::string _path);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Draws an textured model
  /// @param _p position, _ang asset angle, _asset asset key in m_objects
  //----------------------------------------------------------------------------------------------------------------------
  void drawAsset(const vec2 _p, const float _ang, const std::string _asset);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Draws an textured model, but with a supplied alpha
  /// @param _p position, _ang asset angle, _asset asset key in m_objects, _alpha transparency
  //----------------------------------------------------------------------------------------------------------------------
  void drawAsset(const vec2 _p, const float _ang, const std::string _asset, const float _alpha);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Draws an shield of a given radius, with a fancy shader
  /// @param _p position, _r radius, _dt global time for shader, _alpha transparency, _col colour tint
  //----------------------------------------------------------------------------------------------------------------------
  void drawShield(const vec2 _p, const float _r, const float _dt, const float _alpha, const std::array<float, 4> _col);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Draws a ship
  /// @param _p position, _ang asset angle, _asset asset key in m_objects, _col colour of muzzle flash
  //----------------------------------------------------------------------------------------------------------------------
  void drawShip(const vec2 _p, const float _ang, const std::string _asset, const std::array<float, 4> _lCol);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Treats the entries in the vertex attribute vectors as lines, and draws them with a laser shader
  //----------------------------------------------------------------------------------------------------------------------
  void drawLasers();

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Treats the entries in the vertex attribute vectors as lines, and draws them
  /// @param _width width of the lines
  //----------------------------------------------------------------------------------------------------------------------
  void drawLines(float _width);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Draws an explosion
  /// @param _pos position, _d dimensions, _col colour tint
  //----------------------------------------------------------------------------------------------------------------------
  void drawExplosion(const vec2 _pos, const float _d, const std::array<float, 4> _col);
  void drawExplosion(const vec2 _pos, const vec2 _d, const std::array<float, 4> _col);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Draws jet flames
  /// @param _width width of the lines
  //----------------------------------------------------------------------------------------------------------------------
  void drawFlames(const vec2 _pos, const vec2 _d, float _ang, std::array<float, 4> _col, const float _t, const float _speed);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Loads ascii characters using SDL_TTF, and saves them as openGL textures.
  /// @param _name the name to give the vector of
  //----------------------------------------------------------------------------------------------------------------------
  void loadFontSpriteSheet(std::string _name, std::string _path, int _size);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Draws white text
  /// @param _text text to draw, _font font to use, _pos start position, _ws world or screen space, _scale text scale
  //----------------------------------------------------------------------------------------------------------------------
  void drawText(std::string _text, std::string _font, vec2 _pos, const bool _ws, const float _scale);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Draws coloured text
  /// @param _text text to draw, _font font to use, _pos start position, _ws world or screen space, _scale text scale, _col text colour
  //----------------------------------------------------------------------------------------------------------------------
  void drawText(std::string _text, std::string _font, vec2 _pos, const bool _ws, const float _scale, const std::array<float, 4> _col);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Adds a line to the member vertex attribute vectors, with position, uvs and colour
  /// @param _start start of the line, _end end of the line, _lCol colour of the line
  //----------------------------------------------------------------------------------------------------------------------
  void addLine(const vec2 _start, const vec2 _end, const std::array<float, 4> _lCol);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Gets the radius of a ship
  /// @param _type the type of ship (types defined in ship header)
  //----------------------------------------------------------------------------------------------------------------------
  float getTextureRadius(ship_spec _type) {return g_texture_keys[(_type)].m_radius;}

  //UI drawing
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Draws the minimap
  /// @param _mp ref to missile vector, _ep ref to agent vector, _ap ref to asteroid vector, _lp ref to laser vector, _fp ref to faction pointer
  //----------------------------------------------------------------------------------------------------------------------
  void drawMap(std::vector<missile> * _mp, std::vector<enemy> *_ep, std::vector<ship> * _ap, std::vector<laser> * _lp, std::vector<faction> * _fp);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Draws health, shield and energy bars
  /// @param _ply ref to player
  //----------------------------------------------------------------------------------------------------------------------
  void statusBars(player * _ply);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Draw the stats of the current weapon
  /// @param _ply ref to the player
  //----------------------------------------------------------------------------------------------------------------------
  void drawWeaponStats(player * _ply);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Swaps the window
  //----------------------------------------------------------------------------------------------------------------------
  void finalise() { SDL_GL_SwapWindow(m_window); }

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Loads an image and returns a SDL_Surface
  /// @param _path path to image file
  //----------------------------------------------------------------------------------------------------------------------
  SDL_Surface * getSurface(std::string _path);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Adds camera shake
  /// @param _s shake magnitude
  //----------------------------------------------------------------------------------------------------------------------
  void addShake(float _s);

  GLuint SDLSurfaceToGLTexture(SDL_Surface * _s);

  void enableDepthSorting();
  void disableDepthSorting();
};

#endif

#endif

