// This file is part of the OGRE project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at https://www.ogre3d.org/licensing.

#include "OgreApplicationContextDummy.h"

#include "OgreWindowEventUtilities.h"

namespace OgreBites {

NativeWindowPair ApplicationContextDummy::createWindow(const Ogre::String& name, Ogre::uint32 w, Ogre::uint32 h, Ogre::NameValuePairList miscParams)
{
    auto ret = _createWindow(NULL, name, w, h, std::move(miscParams));
    WindowEventUtilities::_addRenderWindow(ret.render);
    return ret;
}

void ApplicationContextDummy::_destroyWindow(const NativeWindowPair& win)
{
    // remove window event listener before destroying it
    WindowEventUtilities::_removeRenderWindow(win.render);
    ApplicationContextBase::_destroyWindow(win);
}

void ApplicationContextDummy::pollEvents()
{
    WindowEventUtilities::messagePump();
}

}
