//
//  Application.h
//  Chip8 Emulator
//
//  Created by David Hodgkinson on 26/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//

#ifndef __OpenGL_RTT__Application__
#define __OpenGL_RTT__Application__

#include "cpu.h"
#include "OpenGLController.h"
#include "Node.h"

namespace Application
{
    class Application
    {
    public:
        static Application* Create();
        
        void MainLoop();
    private:
        Application() {}
        bool Init();
    private:
        OpenGL::OpenGLController *_glController;
        OpenGL::Node *_node;
        Chip8::CPU *_cpu;
        
        std::vector<bool> _keys;
    };
}
#endif /* defined(__OpenGL_RTT__Application__) */
