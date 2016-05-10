#ifndef SELECTION_HPP
#define SELECTION_HPP

#include <vector>
#include "button.hpp"
#include "selection.hpp"

//----------------------------------------------------------------------------------------------------------------------
/// \file selection.hpp
/// \brief Contains the selection class, used to hold a single menu.
/// \author Ben Hawkyard
/// \version 1.0
/// \date 11/04/16
/// Revision History :
/// This is an initial version used for the game.
/// \class selection
/// \brief Contains a set of buttons, represents a single menu in-game.
//----------------------------------------------------------------------------------------------------------------------

enum selectionType {SELECTION_STATIC, SELECTION_ROLLOUT};
enum mouseState {MOUSE_OFF, MOUSE_OVER, MOUSE_DOWN};

class selection
{
public:
    //----------------------------------------------------------------------------------------------------------------------
    /// \brief ctor for the selection class.
    //----------------------------------------------------------------------------------------------------------------------
    selection();

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief ctor for the selection class.
    //----------------------------------------------------------------------------------------------------------------------
    selection(selectionType _type);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Returns a pointer to the buttons. Used to draw them and alter them from within the universe class.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<button> * getButtons() {return &m_buttons;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Returns the index of the currently selected button.
    //----------------------------------------------------------------------------------------------------------------------
    int getSelected() {return m_selected;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Adds a given button.
    //----------------------------------------------------------------------------------------------------------------------
    void add(const button b) {m_buttons.push_back(b);}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Returns whether a click at a specific point has selected a button.
    /// \param _p position of mouse.
    //----------------------------------------------------------------------------------------------------------------------
    bool click(vec2 _p);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Returns a pointer to the button at a specific index.
    /// \param _i index of the button.
    //----------------------------------------------------------------------------------------------------------------------
    button * getAt(size_t _i) {return &m_buttons[_i];}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Resets the menu to its initial state.
    //----------------------------------------------------------------------------------------------------------------------
    void reset();

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Updates the menu, and all of its buttons.
    /// \param _s current score.
    /// \param _mouse current mouse position.
    //----------------------------------------------------------------------------------------------------------------------
    void update(int _s, const vec2 _mouse);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Removes all buttons.
    //----------------------------------------------------------------------------------------------------------------------
    void clear() {m_buttons.clear();}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Getter and setter for visibility.
    //----------------------------------------------------------------------------------------------------------------------
    bool isVisible() const {return m_visible;}
    void setVisible(const bool _visibility) {m_visible = _visibility;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Toggles menu visibility.
    //----------------------------------------------------------------------------------------------------------------------
    void toggleVisible() {m_visible = !m_visible;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Interpolant getter and setter.
    //----------------------------------------------------------------------------------------------------------------------
    void setInterpolant(const float _i) {m_interpolant = _i;}
    float getInterpolant() {return m_interpolant;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Setter for m_saveSelected.
    //----------------------------------------------------------------------------------------------------------------------
    void setSaveSelected(const bool _saveSelected) {m_saveSelected = _saveSelected;}
private:
    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Mouse state for this menu.
    //----------------------------------------------------------------------------------------------------------------------
    mouseState m_mouseState;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Container of buttons.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<button> m_buttons;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Which button is selected.
    //----------------------------------------------------------------------------------------------------------------------
    int m_selected;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Whether this menu is visible.
    //----------------------------------------------------------------------------------------------------------------------
    bool m_visible;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief The selection type for this menu.
    //----------------------------------------------------------------------------------------------------------------------
    selectionType m_type;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief The selection position.
    //----------------------------------------------------------------------------------------------------------------------
    vec2 m_pos;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief The selection dimensions.
    //----------------------------------------------------------------------------------------------------------------------
    vec2 m_dim;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Float representing state of the menu.
    //----------------------------------------------------------------------------------------------------------------------
    float m_interpolant;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Whether it is a continuous multi-choice or single click menu.
    //----------------------------------------------------------------------------------------------------------------------
    bool m_saveSelected;
};

#endif
