/*
 -----------------------------------------------------------------------------
 This source file is part of OGRE
 (Object-oriented Graphics Rendering Engine)
 For the latest info, see http://www.ogre3d.org/

 Copyright (c) 2000-2014 Torus Knot Software Ltd

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 -----------------------------------------------------------------------------
 */
#ifndef __ApplicationContextDummy_H__
#define __ApplicationContextDummy_H__

#include "OgreApplicationContextBase.h"

namespace OgreBites
{
    class _OgreBitesExport ApplicationContextDummy : public ApplicationContextBase
    {
    public:
        using ApplicationContextBase::ApplicationContextBase;

        uint32_t getWindowID(NativeWindowType* window) override {return 0;}

        /**
        process all window events since last call
        */
        void pollEvents() override;

        /**
         * Create a new render window
         *
         * By default the values from ogre.cfg are used for w, h and miscParams.
         */
        NativeWindowPair
        createWindow(const Ogre::String& name, uint32_t w = 0, uint32_t h = 0,
                     Ogre::NameValuePairList miscParams = Ogre::NameValuePairList()) override;

        void _destroyWindow(const NativeWindowPair& win) override;
    };
}
#endif
