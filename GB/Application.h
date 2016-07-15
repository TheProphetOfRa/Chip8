//
//  Application.h
//  Chip8 Emulator
//
//  Created by David Hodgkinson on 24/11/2015.
//  Copyright © 2015 David Hodgkinson. All rights reserved.
//

#ifndef Application_h
#define Application_h

#include "GB/CPU.h"

#include "Application/RasApplication.h"
#include "View/Node.h"

namespace GB
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
        CPU *_cpu;
        
        std::vector<bool> _keys;
    };
}

#endif /* Application_h */
