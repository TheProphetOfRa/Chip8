//
//  Application.cpp
//  Chip8 Emulator
//
//  Created by David Hodgkinson on 26/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//

#include "Application.h"

namespace Chip8Emulator
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
        _cpu->init();
        _cpu->loadGame("Resources/pong2.c8");
        
        _node = Ras2D::Node::Create();
        _director->AddNode(_node);
        
        for (int i = 0 ; i < 16 ; ++i) _keys.push_back(false);
        
        return true;
    }
    
    void Application::MainLoop()
    {
        //application loop
        while (_director->Update(_keys))
        {
            for (int i = 0 ; i < _keys.size() ; ++i)
            {
                if (_keys[i])
                {
                    _cpu->setKey(i);
                }
                else
                {
                    _cpu->resetKey(i);
                }
            }
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
				_director->Draw();
            }
        }
		End();
    }
}