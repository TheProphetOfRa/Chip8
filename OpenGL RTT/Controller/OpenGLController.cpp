//
//  OpenGLController.cpp
//  OpenGL RTT
//
//  Created by David Hodgkinson on 24/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//

#include "OpenGLController.h"

#include "Log.h"
#include "Triangle.h"

#include <assert.h>
#include <fstream>
#include <stdio.h>
#include <streambuf>

namespace OpenGL
{
    static OpenGLController* sharedInstance = nullptr;
    
    void GLFWError(int error, const char *desc)
    {
        Log::Log_Err("GLFW Error: code %i, msg %s\n", error, desc);
    }
    
    OpenGLController* OpenGLController::sharedController()
    {
        if (sharedInstance == nullptr)
        {
            sharedInstance = new OpenGLController();
            if (sharedInstance && sharedInstance->Init())
            {
                return sharedInstance;
            }
            delete sharedInstance;
            sharedInstance = nullptr;
            exit(-1);
        }
        else
        {
            return sharedInstance;
        }
    }
    
    bool OpenGLController::Init()
    {
        assert(Log::Restart());
        
        Log::Log("Starting GLFW\n%s\n", glfwGetVersionString ());
        
        glfwSetErrorCallback(GLFWError);
        //start GL Context and OS Window using glfw
        if (!glfwInit())
        {
            fprintf(stderr, "Error: Could not start GLFW3\n");
            return false;
        }
        
        //hints for turning on opengl features
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 4); //4 anti-aliasing passes
        
        //Setup window
        _monitor = glfwGetPrimaryMonitor();
        _vmode = glfwGetVideoMode(_monitor);
        _window = glfwCreateWindow(_vmode->width, _vmode->height, "OpenGL Application", _monitor, nullptr);
        if (!_window)
        {
            fprintf(stderr, "Error: Could not open window with GLFW3\n");
            glfwTerminate();
            return false;
        }
        glfwMakeContextCurrent(_window);
        
        Log::LogGLParams();
        
        //start GLEW extension handler
        glewExperimental = GL_TRUE;
        glewInit();
        
        _renderer = glGetString(GL_RENDERER);
        _version = glGetString(GL_VERSION);
        
        printf("Renderer: %s\n", _renderer);
        printf("Version: %s\n", _version);
        
        //Tell GL to only draw to a pixel if the shape is closer to the viewer
        glEnable(GL_DEPTH_TEST); //Enable depth testing
        glDepthFunc(GL_LESS); //Interp. smaller values as closer
        
        return true;
    }
    
    const GLchar* OpenGLController::LoadShaderFromFile(const std::string &fileName) const
    {
        //load file to string
        std::ifstream shaderInputStream(fileName.c_str());
        std::string shaderString((std::istreambuf_iterator<char>(shaderInputStream)), std::istreambuf_iterator<char>());
        return shaderString.c_str();
    }
    
    GLuint OpenGLController::LoadShader(const std::string &fileName) const
    {
        GLuint shader = glCreateProgram();
     
        //load vertex shader
        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        const GLchar* vert_shader = LoadShaderFromFile(fileName + "_vert.glsl");
        glShaderSource(vs, 1, &vert_shader, nullptr);
        glCompileShader(vs);
        
        //load fragment shader
        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        const GLchar* frag_shader = LoadShaderFromFile(fileName + "_frag.glsl");
        glShaderSource(fs, 1, &frag_shader, nullptr);
        glCompileShader(fs);
        
        //oad them into shader program
        glAttachShader(shader, fs);
        glAttachShader(shader, vs);
        glLinkProgram(shader);
        
        return shader;
    }
    
    bool OpenGLController::update()
    {
        // update other events like input handling
        glfwPollEvents();
        
        //close window when escape is pressed
        if (GLFW_PRESS == glfwGetKey (_window, GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose (_window, 1);
        }
        
        return !glfwWindowShouldClose(_window);
    }
    
    void OpenGLController::draw()
    {
        glClearColor(0.6f, 0.6f, 0.6f, 0.8f);
        
        // wipe the drawing surface clear
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        for (const auto t : _triangles)
        {
            glUseProgram (t->GetShaderProgram());
            glBindVertexArray (t->GetVAO());
            glDrawArrays (GL_TRIANGLES, 0, 3);
        }

        // put the stuff we've been drawing onto the display
        glfwSwapBuffers (_window);
    }
    
    void OpenGLController::Destroy()
    {
        glfwTerminate();
    }
}