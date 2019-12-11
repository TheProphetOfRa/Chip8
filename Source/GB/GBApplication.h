//
//  Application.h
//  Chip8 Emulator
//
//  Created by David Hodgkinson on 24/11/2015.
//  Copyright © 2015 David Hodgkinson. All rights reserved.
//

#ifndef Application_h
#define Application_h

#include "GBCPU.h"

#include "Application/RasApplication.h"
#include "View/Node.h"

#include <vector>

namespace GB
{
    class GBApplication : public Ras2D::Application
    {
    public:
        static Ras2D::Application* Create();
        
    private:
        GBApplication() {}
        bool Init() override;
    private:
        Ras2D::Node *_display;
        GBCPU *_cpu;
        
        std::vector<bool> _keys;
    };
}

#endif /* Application_h */
