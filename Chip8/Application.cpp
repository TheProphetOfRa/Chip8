//
//  Application.cpp
//  Chip8 Emulator
//
//  Created by David Hodgkinson on 26/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//

#include "Application.h"

#include <cstring>

namespace Chip8
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

        _cpu = new Chip8::CPU();
        _cpu->Init();
        _cpu->LoadGame("Resources/pong2.c8");
        
        _node = Ras2D::Node::Create();
        _director->AddNode(_node);
        
        for (int i = 0 ; i < 16 ; ++i) _keys.push_back(false);
        
        _gfxBuffer = (const unsigned char*)calloc(64*32, sizeof(unsigned char));
        
        return true;
    }
    
    bool Application::Update()
    {
        //application loop
        if (!_director->Update(_keys))
        {
            End();
        }
        
        for (int i = 0 ; i < _keys.size() ; ++i)
        {
            if (_keys[i])
            {
                _cpu->SetKey(i);
            }
            else
            {
                _cpu->ResetKey(i);
            }
        }
        _cpu->EmulateCycle();
        if (_cpu->ShouldDraw())
        {
            _gfxBuffer = _cpu->GetGfx();
        }
        
        return true;
    }

    bool Application::Render()
    {
        float data[64*32];
        for (int i = 0 ; i < 64 * 32 ; ++i)
        {
            if (_gfxBuffer[i] == 1)
            {
                data[i] = 255.0f;
            }
            else
            {
                data[i] = 0.0f;
            }
        }
        _node->DrawTexture(data);
        _director->Draw();
        
        return true;
    }
}
