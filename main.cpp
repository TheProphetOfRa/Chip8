//
//  main.cpp
//  Chip8 Emulator
//
//  Created by David Hodgkinson on 24/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//

#include "Chip8/Chip8Application.h"
#include "GB/GBApplication.h"

#include <assert.h>
#include <iostream>

int main(int argc, const char * argv[])
{
	Ras2D::Application* app = Chip8::Chip8Application::Create();
    //GB::Application* app = GB::Application::Create();
	assert(app);
    
    app->OnInitComplete();
    
	app->Run();

    return 0;
}
