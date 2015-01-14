//
//  Application.h
//  Chip8 Emulator
//
//  Created by David Hodgkinson on 26/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//

#ifndef __Chip8__Application__
#define __Chip8__Application__

#include "Chip8/cpu.h"

#include "Application/RasApplication.h"
#include "View/Node.h"

namespace Chip8Emulator
{
    class Application : public Ras2D::Application
    {
    public:
        static Application* Create();
        
        void MainLoop();
    private:
        Application() {}
        bool Init();
    private:
        Ras2D::Node *_node;
        Chip8::CPU *_cpu;
        
        std::vector<bool> _keys;
    };
}
#endif /* defined(__OpenGL_RTT__Application__) */
