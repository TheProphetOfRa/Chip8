//
//  RasApplication.cpp
//  Ras2D
//
//  Created by David Hodgkinson on 26/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//

#include "RasApplication.h"

namespace Ras2D
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

    Application::Application()
    : _frameRate(30)
    {
    }

	bool Application::Init()
	{
		_director = Director::GetInstance();
		_director->Init();

		return true;
	}

    bool Application::ProcessInput()
    {
        return true;
    }
    
    bool Application::Update()
    {
        return true;
    }

    bool Application::Render()
    {
        return true;
    }

	void Application::Run()
	{
		MainLoop();
	}

	void Application::MainLoop()
	{
		while (true)
        {
            ProcessInput();
            
            //TODO: some kind of fixed update logic
            // render fast, tick constant
            bool shouldProcessFrame = true;
            
            if (shouldProcessFrame)
                Update();
            
            Render();
        }
	}

	void Application::End()
	{
		_director->Destroy();
        _director = nullptr;
	}
}
