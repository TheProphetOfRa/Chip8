//
//  Application.cpp
//  OpenGL RTT
//
//  Created by David Hodgkinson on 26/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//

#include "Application.h"

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
        
        _cpu = new Chip8::CPU();
        _cpu->init();
        _cpu->loadGame("Resources/invaders.c8");
        
        _node = OpenGL::Node::Create();
        _glController->AddNode(_node);
        
        return true;
    }
    
    void Application::MainLoop()
    {
        //application loop
        while (_glController->Update())
        {
            _cpu->emulateCycle();
            if (_cpu->shouldDraw())
            {
                float data[64*32];
                const unsigned char * gfx = _cpu->getGfx();
                for (int i = 0 ; i < 64 * 32 ; ++i)
                {
                    if (gfx[i] == 1)
                    {
                        data[i] = 255.0f;
                    }
                    else
                    {
                        data[i] = 0.0f;
                    }
                }
                _node->DrawTexture(data);
            }
            _glController->Draw();
        }
        
        _glController->Destroy();
    }
}