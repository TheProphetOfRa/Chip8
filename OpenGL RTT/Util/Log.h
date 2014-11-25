//
//  Log.h
//  OpenGL RTT
//
//  Created by David Hodgkinson on 25/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//

#ifndef __OpenGL_RTT__Log__
#define __OpenGL_RTT__Log__

#include <time.h>
#include <stdarg.h>
#include <stdio.h>

#include "OpenGLController.h"

namespace OpenGL
{
    namespace Log
    {
        bool Restart();
        
        void LogGLParams();
        
        bool Log(const char *message, ...);
        bool Error(const char *messag, ...);
        void LogShader(GLuint shaderProgram);
    }
}

#endif /* defined(__OpenGL_RTT__Log__) */
