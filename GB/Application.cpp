//
//  Application.cpp
//  Chip8 Emulator
//
//  Created by David Hodgkinson on 24/11/2015.
//  Copyright © 2015 David Hodgkinson. All rights reserved.
//

#include "Application.h"

namespace GB
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
        Ras2D::Application::Init();
        
        _cpu = new GB::CPU();
        _cpu->Init();
        _cpu->LoadGame("");
        
        _node = Ras2D::Node::Create();
        _director->AddNode(_node);
        
        return true;
    }
    
    void Application::MainLoop()
    {
        _cpu->EmulateCycle();
    }
}