// This file is part of the OGRE project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at https://www.ogre3d.org/licensing.
// SPDX-License-Identifier: MIT

#ifndef BITES_SDL3_INPUTMAPPING
#define BITES_SDL3_INPUTMAPPING

#include <SDL3/SDL.h>

#include "OgreInput.h"

namespace {
    OgreBites::Event convert(const SDL_Event& in)
    {
        OgreBites::Event out;

        out.type = 0;

        switch(in.type)
        {
        case SDL_EVENT_KEY_DOWN:
            out.type = OgreBites::KEYDOWN;
            OGRE_FALLTHROUGH;
        case SDL_EVENT_KEY_UP:
            if(!out.type)
                out.type = OgreBites::KEYUP;
            out.key.repeat = in.key.repeat;
            out.key.keysym.sym = in.key.key;
            out.key.keysym.mod = in.key.mod;
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            out.type = OgreBites::MOUSEBUTTONUP;
            OGRE_FALLTHROUGH;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            if(!out.type)
                out.type = OgreBites::MOUSEBUTTONDOWN;
            out.button.x = in.button.x;
            out.button.y = in.button.y;
            out.button.button = in.button.button;
            out.button.clicks = in.button.clicks;
            break;
        case SDL_EVENT_MOUSE_WHEEL:
            out.type = OgreBites::MOUSEWHEEL;
            out.wheel.y = in.wheel.y;
            break;
        case SDL_EVENT_MOUSE_MOTION:
            out.type = OgreBites::MOUSEMOTION;
            out.motion.x = in.motion.x;
            out.motion.y = in.motion.y;
            out.motion.xrel = in.motion.xrel;
            out.motion.yrel = in.motion.yrel;
            out.motion.windowID = in.motion.windowID;
            break;
        case SDL_EVENT_FINGER_DOWN:
            out.type = OgreBites::FINGERDOWN;
            OGRE_FALLTHROUGH;
        case SDL_EVENT_FINGER_UP:
            if(!out.type)
                out.type = OgreBites::FINGERUP;
            OGRE_FALLTHROUGH;
        case SDL_EVENT_FINGER_MOTION:
            if(!out.type)
                out.type = OgreBites::FINGERMOTION;
            out.tfinger.x = in.tfinger.x;
            out.tfinger.y = in.tfinger.y;
            out.tfinger.dx = in.tfinger.dx;
            out.tfinger.dy = in.tfinger.dy;
            out.tfinger.fingerId = in.tfinger.fingerID;
            break;
        case SDL_EVENT_TEXT_INPUT:
            out.type = OgreBites::TEXTINPUT;
            out.text.chars = in.text.text;
            break;
        case SDL_EVENT_JOYSTICK_AXIS_MOTION:
            out.type = OgreBites::JOYAXISMOTION;
            out.axis.which = in.jaxis.which;
            out.axis.axis = in.jaxis.axis;
            out.axis.value = in.jaxis.value;
            break;
        case SDL_EVENT_GAMEPAD_AXIS_MOTION:
            out.type = OgreBites::CONTROLLERAXISMOTION;
            out.axis.which = in.gaxis.which;
            out.axis.axis = in.gaxis.axis;
            out.axis.value = in.gaxis.value;
            break;
        case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
            out.type = OgreBites::CONTROLLERBUTTONDOWN;
            OGRE_FALLTHROUGH;
        case SDL_EVENT_GAMEPAD_BUTTON_UP:
            if(!out.type)
                out.type = OgreBites::CONTROLLERBUTTONUP;
            out.cbutton.which = in.gbutton.which;
            out.cbutton.button = in.gbutton.button;
            break;
        }

        return out;
    }
}

#endif
