//
//  OpenGLController.cpp
//  OpenGL RTT
//
//  Created by David Hodgkinson on 24/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//

#include "OpenGLController.h"

#include "Log.h"
#include "Node.h"

#include <assert.h>
#include <fstream>
#include <stdio.h>
#include <streambuf>

namespace OpenGL
{
    static OpenGLController* sharedInstance = nullptr;
    
    void GLFWError(int error, const char *desc)
    {
        Log::Error("GLFW Error: code %i, msg %s\n", error, desc);
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
        if (_windowed)
        {
            _window = glfwCreateWindow(640, 480, "OpenGL", nullptr, nullptr);
        }
        else
        {
            _monitor = glfwGetPrimaryMonitor();
            _vmode = glfwGetVideoMode(_monitor);
            _window = glfwCreateWindow(_vmode->width, _vmode->height, "OpenGL Application", _monitor, nullptr);        }
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
    
    std::string OpenGLController::LoadShaderFromFile(const std::string &fileName) const
    {
        //load file to string
        std::ifstream shaderInputStream(fileName.c_str());
        std::string shaderString((std::istreambuf_iterator<char>(shaderInputStream)), std::istreambuf_iterator<char>());
        return shaderString;
    }
    
    bool OpenGLController::CheckForShaderError(const GLuint shader) const
    {
        int params = -1;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &params);
        if (GL_TRUE != params)
        {
            Log::Error("ERROR: GL shader index %i did not compile\n", shader);
            int max_length = 2048;
            int actual_length = 0;
            char log[2048];
            glGetShaderInfoLog (shader, max_length, &actual_length, log);
            Log::Error("Shader info log for GL index %u:\n%s\n", shader, log);
            glfwSetWindowShouldClose(_window, 1);
        }
        return GL_TRUE == params;
    }
    
    bool OpenGLController::CheckForShaderLinkErrors(const GLuint shaderProgram) const
    {
        int params = -1;
        glGetProgramiv (shaderProgram, GL_LINK_STATUS, &params);
        if (GL_TRUE != params)
        {
            Log::Error("ERROR: Could not link shader programme GL index %u\n",                     shaderProgram);
            int max_length = 2048;
            int actual_length = 0;
            char log[2048];
            glGetProgramInfoLog (shaderProgram, max_length, &actual_length, log);
            Log::Error("Program info log for GL index %u:\n%s", shaderProgram, log);
            glfwSetWindowShouldClose(_window, 1);
        }
        return GL_TRUE == params;
    }
    
    GLuint OpenGLController::LoadShader(const std::string &fileName) const
    {
        GLuint shader = glCreateProgram();
     
        //load vertex shader
        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        std::string vert_shader = LoadShaderFromFile(fileName + "_vs.glsl");
        const GLchar *vert = vert_shader.c_str();
        glShaderSource(vs, 1, &vert, nullptr);
        glCompileShader(vs);
        CheckForShaderError(vs);
        
        //load fragment shader
        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        std::string frag_shader = LoadShaderFromFile(fileName + "_fs.glsl");
        const GLchar* frag = frag_shader.c_str();
        glShaderSource(fs, 1, &frag, nullptr);
        glCompileShader(fs);
        CheckForShaderError(fs);
        
        //Load them into shader program
        glAttachShader(shader, fs);
        glAttachShader(shader, vs);
        glLinkProgram(shader);
        IsShaderProgramValid(shader);
        CheckForShaderLinkErrors(shader);
        
        return shader;
    }
    
    bool OpenGLController::Update()
    {
        // update other events like input handling
        glfwPollEvents();
        
        for (const auto n : _nodes)
        {
            n->Update();
        }
        
        //close window when escape is pressed
        if (GLFW_PRESS == glfwGetKey (_window, GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose (_window, 1);
        }
        
        return !glfwWindowShouldClose(_window);
    }
    
    void OpenGLController::Draw()
    {
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        
        glEnable (GL_CULL_FACE); // cull face
        glCullFace (GL_BACK); // cull back face
        glFrontFace (GL_CW); // GL_CCW for counter clock-wise
        
        // wipe the drawing surface clear
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        for (const auto n : _nodes)
        {
            n->Draw();
        }

        // put the stuff we've been drawing onto the display
        glfwSwapBuffers (_window);
    }
    
    bool OpenGLController::IsShaderProgramValid(const GLuint shader) const
    {
        glValidateProgram (shader);
        int params = -1;
        glGetProgramiv (shader, GL_VALIDATE_STATUS, &params);
        Log::Log("Program %i GL_VALIDATE_STATUS = %i\n", shader, params);
        if (GL_TRUE != params)
        {
            int max_length = 2048;
            int actual_length = 0;
            char log[2048];
            glGetProgramInfoLog (shader, max_length, &actual_length, log);
            Log::Log("Program info log for GL index %u:\n%s", shader, log);
        }
        return GL_TRUE == params;
    }
    
    void OpenGLController::Destroy()
    {
        glfwTerminate();
    }
}