//
//  main.cpp
//  Chip8 Emulator
//
//  Created by David Hodgkinson on 24/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//

#include "Application.h"

int main(int argc, const char * argv[])
{
    Application::Application::Create()->MainLoop();
    return 0;
}
