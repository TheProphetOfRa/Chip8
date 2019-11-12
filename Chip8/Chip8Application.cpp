//
//  Application.cpp
//  Chip8 Emulator
//
//  Created by David Hodgkinson on 26/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//

#include "Chip8Application.h"

#include <cstring>

namespace Chip8
{
    Ras2D::Application* Chip8Application::Create()
    {
        auto result = new Chip8Application();
        
        if (result && result->Init())
        {
            return result;
        }
        
        delete result;
        return nullptr;
    }
    
    bool Chip8Application::Init()
    {       
		if (Ras2D::Application::Init())
        {
            return true;
        }
        
        return false;
    }

    bool Chip8Application::OnInitComplete()
    {
        Ras2D::Application::OnInitComplete();
        
        _cpu = new Chip8CPU();
        _cpu->Init();
        _cpu->LoadGame("Resources/pong2.c8");
        
        _display = Ras2D::Node::Create();
        _renderManager->SetRootNode(_display);
        
        for (int i = 0 ; i < 16 ; ++i) _keys.push_back(false);
        
        _gfxBuffer = (const unsigned char*)calloc(64*32, sizeof(unsigned char));
        
        return true;
    }
    
    bool Chip8Application::Update()
    {
        //application loop
        if (!_director->Update(_keys, _renderManager->GetWindow()))
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
            
            _display->SetTexture(data);
        }
        
        return true;
    }
}
