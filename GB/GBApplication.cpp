//
//  Application.cpp
//  Chip8 Emulator
//
//  Created by David Hodgkinson on 24/11/2015.
//  Copyright © 2015 David Hodgkinson. All rights reserved.
//

#include "GBApplication.h"

namespace GB
{
    Ras2D::Application* GBApplication::Create()
    {
        auto* result = new GBApplication();
        if (result && result->Init())
        {
            return result;
        }
        
        return nullptr;
    }
    
    bool GBApplication::Init()
    {
        Ras2D::Application::Init();
        
        _cpu = new GBCPU();
        _cpu->Init();
        _cpu->LoadGame("");
        
        _display = Ras2D::Node::Create();
        _renderManager->SetRootNode(_display);
        
        return true;
    }
}
