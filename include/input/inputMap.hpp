#ifndef INPUTMAP_HPP
#define INPUTMAP_HPP

#include <array>

#include <SDL.h>

#include "command.hpp"

enum inputEvent {INPUT_EVENT_NONE, INPUT_EVENT_PRESS, INPUT_EVENT_RELEASE};

class inputMap
{
public:
    inputMap();
    void update(const std::vector<SDL_Event> &_events);

    bool lmb() const {return m_lmb;}
    inputEvent lmbEvent() const {return m_lmbEvent;}
    bool rmb() const {return m_rmb;}
    inputEvent rmbEvent() const {return m_rmbEvent;}
    bool key(SDL_Keycode _key) const {return m_keys[SDL_GetScancodeFromKey(_key)];}
    inputEvent keyEvent(SDL_Keycode _key);
    void deactivate(const SDL_Keycode _key) {m_keys[SDL_GetScancodeFromKey(_key)] = false;}
    void deactivatelmb() {m_lmb = false;}
    void deactivatermb() {m_rmb = false;}
    int scroll() const {return m_scroll;}
    void clearScroll() {m_scroll = 0;}
private:
    std::array<bool, SDL_SCANCODE_SLEEP - SDL_SCANCODE_UNKNOWN> m_keys;
    std::array<inputEvent, SDL_SCANCODE_SLEEP - SDL_SCANCODE_UNKNOWN> m_keyEvents;
    bool m_lmb;
    inputEvent m_lmbEvent;
    bool m_rmb;
    inputEvent m_rmbEvent;
    int m_scroll;
};

#endif
