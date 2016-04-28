#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <string>
#include <unordered_map>
#include <vector>

#include <SDL_image.h>

#include "common.hpp"
#include "enemy.hpp"
#include "faction.hpp"
#include "laser.hpp"
#include "missile.hpp"
#include "player.hpp"
#include "ship.hpp"
#include "sprite_sheet.hpp"
#include "vectors.hpp"

//----------------------------------------------------------------------------------------------------------------------
/// @file renderer.hpp
/// @brief This file contains the old SDL renderer.
/// @author Ben Hawkyard
/// @version 1.0
/// @date 11/04/16
/// Revision History :
/// This is an initial version used for the game.
/// @class renderer
/// @brief Wrapper around SDL rendering functionality. Has not been updated in a bit, use at your
/// own risk.
//----------------------------------------------------------------------------------------------------------------------

class renderer
{
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Main SDL window
    //----------------------------------------------------------------------------------------------------------------------
    SDL_Window * m_window = NULL;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Main SDL renderer
    //----------------------------------------------------------------------------------------------------------------------
    SDL_Renderer * m_renderer = NULL;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Window dimensions
    //----------------------------------------------------------------------------------------------------------------------
    int m_w;
    int m_h;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief SDL texture storage
    //----------------------------------------------------------------------------------------------------------------------
    std::unordered_map<std::string, std::vector<SDL_Texture*>> m_textures;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Rendered glyph storage
    //----------------------------------------------------------------------------------------------------------------------
    std::unordered_map<std::string, sprite_sheet> m_letters;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Camera shake magnitude
    //----------------------------------------------------------------------------------------------------------------------
    float m_cameraShake;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Target camera offset
    //----------------------------------------------------------------------------------------------------------------------
    vec2 m_cameraShakeTargetOffset;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Current camera offset
    //----------------------------------------------------------------------------------------------------------------------
    vec2 m_cameraShakeOffset;
public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief ctor for the renderer
    /// @param _w window width, _h window height
    //----------------------------------------------------------------------------------------------------------------------
    renderer(int _w, int _h);

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief dtor for the renderer, frees up memory tied to all the dynamically allocated SDL objects
    //----------------------------------------------------------------------------------------------------------------------
    ~renderer();

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Initialises essential SDL functionality
    //----------------------------------------------------------------------------------------------------------------------
    int init();

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Loads all of the required textures
    //----------------------------------------------------------------------------------------------------------------------
    void loadTextures();

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Updates the renderer, mostly used to update camera shake
    /// @param _dt time difference
    //----------------------------------------------------------------------------------------------------------------------
    void update(const float _dt);

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Loads glyphs from a font and renders them to SDL textures
    /// @param _name identifier for the set of loaded glyphs, _path path to the font, _size font size
    //----------------------------------------------------------------------------------------------------------------------
    void loadFontSpriteSheet(std::string _name, std::string _path, int _size);

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Loads an image and stores it as an SDL texture.
    /// @param _key identifier for the texture, _path path to the image, _b blendmode for the texture (transparency handling)
    //----------------------------------------------------------------------------------------------------------------------
    void loadTexture(std::string _key, std::string _path, SDL_BlendMode _b);

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Loads a set of images, converts them to textures, and stores them
    /// @param _key identifier for the set, _set path to the files
    //----------------------------------------------------------------------------------------------------------------------
    void loadTextureSet(std::string _key, std::string _set);

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Loads a set of images, converts them to textures, and stores them
    /// @param _key identifier for the set, _set path to the files
    //----------------------------------------------------------------------------------------------------------------------
    void setBlendMode (SDL_BlendMode _b) {SDL_SetRenderDrawBlendMode(m_renderer, _b);}

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Clears the screen
    //----------------------------------------------------------------------------------------------------------------------
    void clear();

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Draws a set of textures to the screen, used for objects like ships, which have multiple components.
    /// @param _key identifier for the set, _pos position, _orient angle, _col colour tint
    //----------------------------------------------------------------------------------------------------------------------
    void drawTextureSet(std::string _key, vec2 _pos, float _orient, std::array<float, 4> _col);

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Draws a texture to the screen.
    /// @param _key identifier for the texture, _index index of the texture in the vector at '_key', _pos position, _orient angle, _col colour tint
    //----------------------------------------------------------------------------------------------------------------------
    void drawTexture(std::string _key, size_t _index, vec2 _pos, float _orient, std::array<float, 4> _col);

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Draws text to the screen.
    /// @param _text text to draw, _font the font id, _pos starting position, _ss whether to draw the text in screen-space, _mul size multiplier
    //----------------------------------------------------------------------------------------------------------------------
    void drawText(std::string _text, std::string _font, vec2 _pos, bool _ss, const float _mul);

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief A wrapper around SDL's line drawing function, draws a coloured line.
    /// @param _start start position, _end end position, _col line colour
    //----------------------------------------------------------------------------------------------------------------------
    void drawLine(vec2 _start, vec2 _end, std::array<float,4> _col );
    void drawLine(vec2 _start, vec2 _end, std::array<int,4> _col);

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Draws line whose colour varies.
    /// @param _p line start, _e line end, _scol start colour, _ecol end colour
    //----------------------------------------------------------------------------------------------------------------------
    void drawLineGr(vec2 _p, vec2 _e, std::array<float, 4> _scol, std::array<float, 4> _ecol);

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Draws a rectangle.
    /// @param _p position, _d dimensions, _col colour, _wire draw a rectangle outline, or a filled rectangle
    //----------------------------------------------------------------------------------------------------------------------
    void drawRect(vec2 _p, vec2 _d, std::array<int, 4> col, bool _wire);

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Draws a circle.
    /// @param _x x-coord, _y y-coord, _radius circle radius, _col circle colour
    //----------------------------------------------------------------------------------------------------------------------
    void drawCircle(int _x, int _y, int _radius, std::array<float, 4> _col);
    void drawCircleUI(int _x, int _y, int _radius, std::array<int, 4> _col);

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Returns the dimensions of a texture.
    /// @param _identifier texture identifier, _index index of the texture at '_identifier', _w reference to be filled with texture width, _h reference to be filled with texture height
    //----------------------------------------------------------------------------------------------------------------------
    void queryTexture(std::string _identifier, int _index, int * _w, int * _h);

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Returns the radius of the texture of a ship type.
    /// @param _type ship type to test for
    //----------------------------------------------------------------------------------------------------------------------
    float getTextureRadius(ship_spec _type) {return g_texture_keys[(_type)].m_radius;}

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Draws the minimap.
    /// @param _mp ref to missile vector, _ep ref to agent vector, _ap ref to asteroid vector, _lp ref to laser vector, _fp ref to faction pointer
    //----------------------------------------------------------------------------------------------------------------------
    void drawMap(std::vector<missile> * _mp, std::vector<enemy> * _ep, std::vector<ship> * _ap, std::vector<laser> * _lp, std::vector<faction> * _fp);

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Draws health, shield and energy bars.
    /// @param _ply ref to player
    //----------------------------------------------------------------------------------------------------------------------
    void statusBars(player * _ply);

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Draw the stats of the current weapon.
    /// @param _ply ref to the player
    //----------------------------------------------------------------------------------------------------------------------
    void drawWeaponStats(player * _ply);

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Displays the contents of the renderer.
    //----------------------------------------------------------------------------------------------------------------------
    void finalise();

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Loads an image as an SDL Surface, and returns a pointer to it.
    /// @param _path path to the image file
    //----------------------------------------------------------------------------------------------------------------------
    SDL_Surface * getSurface(std::string _path);

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Adds screen shake.
    /// @param _s magnitude
    //----------------------------------------------------------------------------------------------------------------------
    void addShake(float _s);
};

#endif
