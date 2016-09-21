#include "input/inputMap.hpp"

inputMap::inputMap()
{
    m_keys.fill(false);
    m_keyEvents.fill(INPUT_EVENT_NONE);

    m_lmb = false;
    m_lmbEvent = INPUT_EVENT_NONE;
    m_rmb = false;
    m_rmbEvent = INPUT_EVENT_NONE;
}

void inputMap::update(const std::vector<SDL_Event> &_events)
{
    m_keyEvents.fill(INPUT_EVENT_NONE);
    m_lmbEvent = INPUT_EVENT_NONE;
    m_rmbEvent = INPUT_EVENT_NONE;

    for( auto &event : _events )
    {
        switch( event.type )
        {
        case SDL_KEYDOWN:
            std::cout << "key down! " << '\n';
            m_keys[SDL_GetScancodeFromKey( event.key.keysym.sym )] = true;
            m_keyEvents[SDL_GetScancodeFromKey( event.key.keysym.sym )] = INPUT_EVENT_PRESS;
            break;
        case SDL_KEYUP:
            m_keys[SDL_GetScancodeFromKey( event.key.keysym.sym )] = false;
            m_keyEvents[SDL_GetScancodeFromKey( event.key.keysym.sym )] = INPUT_EVENT_RELEASE;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if(event.button.button == SDL_BUTTON_LEFT)
            {
                m_lmb = true;
                m_lmbEvent = INPUT_EVENT_PRESS;
            }
            else if(event.button.button == SDL_BUTTON_RIGHT)
            {
                m_rmb = true;
                m_rmbEvent = INPUT_EVENT_PRESS;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if(event.button.button == SDL_BUTTON_LEFT)
            {
                m_lmb = false;
                m_lmbEvent = INPUT_EVENT_RELEASE;
            }
            else if(event.button.button == SDL_BUTTON_RIGHT)
            {
                m_rmb = false;
                m_rmbEvent = INPUT_EVENT_RELEASE;
            }
            break;
        case SDL_MOUSEWHEEL:
            m_scroll = event.wheel.y;
            break;
        case SDL_QUIT:
            g_GAME_STATE = MODE_QUIT;
            break;
        default:
            break;
        }
    }
}

inputEvent inputMap::keyEvent(SDL_Keycode _key)
{
    SDL_Scancode entry = SDL_GetScancodeFromKey(_key);
    inputEvent ret = m_keyEvents[entry];
    m_keyEvents[entry] = INPUT_EVENT_NONE;
    return ret;
}
