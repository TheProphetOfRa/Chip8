//
//  Log.h
//  Ras2D
//
//  Created by David Hodgkinson on 25/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//

#ifndef __Ras2D__Log__
#define __Ras2D__Log__

#include <time.h>
#include <stdarg.h>
#include <stdio.h>

#include "Controller/RenderManager.h"

namespace Ras2D
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

#endif 
