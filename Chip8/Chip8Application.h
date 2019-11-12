//
//  Application.h
//  Chip8 Emulator
//
//  Created by David Hodgkinson on 26/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//

#pragma once

#include "Chip8CPU.h"

#include "Application/RasApplication.h"
#include "View/Node.h"

#include <vector>

namespace Chip8
{
    class Chip8Application : public Ras2D::Application
    {
    public:
        static Ras2D::Application* Create();
        
    protected:
        bool Update() override;
    private:
        Chip8Application() {}
        
        bool Init() override;
        bool OnInitComplete() override;
    private:
        Ras2D::Node *_display;
        Chip8CPU *_cpu;
        
        std::vector<bool> _keys;
        
        const unsigned char* _gfxBuffer;
    };
}
