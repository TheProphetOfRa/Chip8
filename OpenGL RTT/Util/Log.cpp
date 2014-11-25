//
//  Log.cpp
//  OpenGL RTT
//
//  Created by David Hodgkinson on 25/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//

#include "Log.h"

#include "OpenGLController.h"

namespace OpenGL
{
    static const char * kLogFile = "gl.log";
    
    bool Log::Restart()
    {
        FILE* file = fopen(kLogFile, "w");
        if (!file)
        {
            fprintf (stderr, "ERROR: could not open GL_LOG_FILE log file %s for writing\n", kLogFile);
            return false;
        }
        time_t now = time (NULL);
        char* date = ctime (&now);
        fprintf (file, "GL_LOG_FILE log. local time %s\n", date);
        fclose (file);
        return true;
    }
    
    void Log::LogGLParams()
    {
        GLenum params[] = {
            GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
            GL_MAX_CUBE_MAP_TEXTURE_SIZE,
            GL_MAX_DRAW_BUFFERS,
            GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
            GL_MAX_TEXTURE_IMAGE_UNITS,
            GL_MAX_TEXTURE_SIZE,
            GL_MAX_VARYING_FLOATS,
            GL_MAX_VERTEX_ATTRIBS,
            GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
            GL_MAX_VERTEX_UNIFORM_COMPONENTS,
            GL_MAX_VIEWPORT_DIMS,
            GL_STEREO,
        };
        const char* names[] = {
            "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
            "GL_MAX_CUBE_MAP_TEXTURE_SIZE",
            "GL_MAX_DRAW_BUFFERS",
            "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
            "GL_MAX_TEXTURE_IMAGE_UNITS",
            "GL_MAX_TEXTURE_SIZE",
            "GL_MAX_VARYING_FLOATS",
            "GL_MAX_VERTEX_ATTRIBS",
            "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
            "GL_MAX_VERTEX_UNIFORM_COMPONENTS",
            "GL_MAX_VIEWPORT_DIMS",
            "GL_STEREO",
        };
        Log("GL Context Params:\n");
        char msg[256];
        // integers - only works if the order is 0-10 integer return types
        for (int i = 0; i < 10; i++) {
            int v = 0;
            glGetIntegerv (params[i], &v);
            Log("%s %i\n", names[i], v);
        }
        // others
        int v[2];
        v[0] = v[1] = 0;
        glGetIntegerv (params[10], v);
        Log("%s %i %i\n", names[10], v[0], v[1]);
        unsigned char s = 0;
        glGetBooleanv (params[11], &s);
        Log("%s %u\n", names[11], (unsigned int)s);
        Log("-----------------------------\n");
    }
    
    bool Log::Log(const char *message, ...)
    {
        va_list argptr;
        FILE* file = fopen(kLogFile, "a");
        if (!file)
        {
            fprintf(stderr, "ERROR: could not open GL_LOG_FILE %s file for appending\n", kLogFile);
            return false;
        }
        va_start(argptr, message);
        vfprintf(file, message, argptr);
        va_end(argptr);
        fclose(file);
        return true;
    }
    
    bool Log::Log_Err(const char* message, ...)
    {
        va_list argptr;
        FILE* file = fopen (kLogFile, "a");
        if (!file)
        {
            fprintf (stderr, "ERROR: could not open GL_LOG_FILE %s file for appending\n", kLogFile);
            return false;
        }
        va_start (argptr, message);
        vfprintf (file, message, argptr);
        va_end (argptr);
        va_start (argptr, message);
        vfprintf (stderr, message, argptr);
        va_end (argptr);
        fclose (file);
        return true;
    }
}