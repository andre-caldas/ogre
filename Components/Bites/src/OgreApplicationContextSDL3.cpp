// This file is part of the OGRE project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at https://www.ogre3d.org/licensing.

#include "OgreApplicationContextSDL3.h"

#include "OgreRoot.h"
#include "OgreRenderWindow.h"

#include <SDL3/SDL.h>

#include "SDL3InputMapping.h"

namespace OgreBites {

SDL_Window* ApplicationContextSDL3::getWindowPtr(NativeWindowType* window)
{
    return static_cast<SDL_Window*>(window);
}

uint32_t ApplicationContextSDL3::getWindowID(NativeWindowType* window)
{
    return SDL_GetWindowID(getWindowPtr(window));
}


NativeWindowPair ApplicationContextSDL3::createWindow(const Ogre::String& name, Ogre::uint32 w, Ogre::uint32 h, Ogre::NameValuePairList miscParams)
{
    if(!SDL_WasInit(SDL_INIT_VIDEO)) {
        if(SDL_AddGamepadMappingsFromFile("gamecontrollerdb.txt"))
            Ogre::LogManager::getSingleton().logMessage("[SDL] gamecontrollerdb.txt loaded");

        if(!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
            OgreAssert(false, "[SDL] Video driver could not be initialized."
                       " Maybe the SDL_VIDEO_DRIVER environment is not set properly.");
        }
        if(!SDL_InitSubSystem(SDL_INIT_GAMEPAD)) {
            Ogre::LogManager::getSingleton().logMessage("[SDL] Gamepad driver failed to load");
        }
    }

    // Unfortunately, there is no nice way to determine if the created window
    // is supposed to be "full screen" or "reisizable".
    // TODO: implement some sort of ApplicationContextBase::isFullScreen().
    auto p = mRoot->getRenderSystem()->getRenderWindowDescription();
    int flags = p.useFullScreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_RESIZABLE;

    auto window = SDL_CreateWindow(name.c_str(), w, h, flags | SDL_WINDOW_FULLSCREEN);

    // for tiny rendersystem
    miscParams["sdlwin"] = Ogre::StringConverter::toString(size_t(window));

// TODO: test for NULL.
#if defined(SDL_PLATFORM_WIN32)
    auto prop = SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);
    miscParams["externalWindowHandle"] = Ogre::StringConverter::toString(size_t(prop));
#elif defined(SDL_PLATFORM_MACOS)
    auto prop = SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, NULL);
    miscParams["externalWindowHandle"] = Ogre::StringConverter::toString(size_t(prop));
#elif defined(SDL_PLATFORM_LINUX)
    if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "x11") == 0) {
        auto prop = SDL_GetNumberProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0);
        Ogre::LogManager::getSingleton().logMessage("[SDL] Creating X11 window");
        miscParams["externalWindowHandle"] = Ogre::StringConverter::toString(size_t(prop));
    } else if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "wayland") == 0) {
        auto display = SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, NULL);
        auto surface = SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, NULL);
        Ogre::LogManager::getSingleton().logMessage("[SDL] Creating Wayland window");
        miscParams["externalWlDisplay"] = Ogre::StringConverter::toString(size_t(display));
        miscParams["externalWlSurface"] = Ogre::StringConverter::toString(size_t(surface));
    } else {
        OgreAssert(false && "In a unix system, we only support X11 and Wayland", "high");
    }
#elif defined(SDL_PLATFORM_IOS)
    auto prop = SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_UIKIT_WINDOW_POINTER, NULL);
    miscParams["externalWindowHandle"] = Ogre::StringConverter::toString(size_t(prop));
#endif

    return _createWindow(window, name, w, h, std::move(miscParams));
}

void ApplicationContextSDL3::_destroyWindow(const NativeWindowPair& win)
{
    ApplicationContextBase::_destroyWindow(win);
    if(win.native)
        SDL_DestroyWindow(getWindowPtr(win.native));
}

void ApplicationContextSDL3::setWindowGrab(NativeWindowType* win, bool grab)
{
    SDL_SetWindowMouseGrab(getWindowPtr(win), grab);
    SDL_SetWindowRelativeMouseMode(getWindowPtr(win), grab);
}

float ApplicationContextSDL3::getDisplayDPI() const
{
    OgreAssert(!mWindows.empty(), "create a window first");
    // TODO: Use a display index.
    int num_displays = 0;
    SDL_DisplayID* displays = SDL_GetDisplays(&num_displays);
    if(num_displays <= 0) {
        return ApplicationContextBase::getDisplayDPI();
    }

    float vdpi = SDL_GetDisplayContentScale(displays[0]);
    if(!vdpi) {
        return ApplicationContextBase::getDisplayDPI();
    }

    // Ogre will divide this by 96... so...
    // SDL3 already gives the number we want.
    return vdpi * 96.;
}

void ApplicationContextSDL3::shutdown()
{
    ApplicationContextBase::shutdown();

    if(SDL_WasInit(SDL_INIT_VIDEO)) {
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    }
}

void ApplicationContextSDL3::pollEvents()
{
    if(mWindows.empty())
    {
        // SDL events not initialized
        return;
    }

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            mRoot->queueEndRendering();
            break;
        case SDL_EVENT_WINDOW_RESIZED:
            for(auto & window : mWindows)
            {
                if(event.window.windowID != SDL_GetWindowID(getWindowPtr(window.native)))
                    continue;

                Ogre::RenderWindow* win = window.render;
                win->resize(event.window.data1, event.window.data2);
                windowResized(win);
            }
            break;
        case SDL_EVENT_JOYSTICK_ADDED:
            if(!SDL_IsGamepad(event.cdevice.which))
            {
                SDL_OpenJoystick(event.cdevice.which);
                Ogre::LogManager::getSingleton().logMessage("Opened Joystick");
            }
            break;
        case SDL_EVENT_GAMEPAD_ADDED:
            if(auto c = SDL_OpenGamepad(event.cdevice.which))
            {
                const char* name = SDL_GetGamepadName(c);
                Ogre::LogManager::getSingleton().stream() << "Opened Gamepad: " << (name ? name : "unnamed");
            }
            break;
        default:
            _fireInputEvent(convert(event), event.window.windowID);
            break;
        }
    }
}

}
