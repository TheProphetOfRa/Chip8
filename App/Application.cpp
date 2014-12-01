//
//  Application.cpp
//  OpenGL RTT
//
//  Created by David Hodgkinson on 26/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//

#include "Application.h"

#include "Node.h"

namespace Application
{
    Application* Application::Create()
    {
        auto result = new Application();
        if (result && result->Init())
        {
            return result;
        }
        return nullptr;
    }
    
    bool Application::Init()
    {
        _glController = OpenGL::OpenGLController::sharedController();
        
        auto n = OpenGL::Node::Create();
        _glController->AddNode(n);
        
        return true;
    }
    
    void Application::MainLoop()
    {
        //application loop
        while (_glController->Update())
        {
            _glController->Draw();
        }
        
        _glController->Destroy();
    }
}