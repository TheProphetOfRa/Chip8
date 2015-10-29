//
//  main.cpp
//  Chip8 Emulator
//
//  Created by David Hodgkinson on 24/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//

#include "Chip8/Application.h"
#include "Config.h"

#include <assert.h>
#include <iostream>

int main(int argc, const char * argv[])
{
	Chip8Emulator::Application* app = Chip8Emulator::Application::Create();
	assert(app);
	app->Run();
#ifdef DEBUG
	getchar();
#endif
    return 0;
}
