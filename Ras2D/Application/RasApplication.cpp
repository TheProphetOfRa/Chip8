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
    Application* Application::sInstance = nullptr;
    
    Application* Application::GetInstance()
    {
        return sInstance;
    }

    Application::Application()
    : _frameRate(30)
    {
        sInstance = this;
    }

    Application::~Application()
    {
        End();
    }

	bool Application::Init()
	{
        bool success = true;
        
		_director = new Director();
		success = _director->Init();

        _renderManager = new RenderManager();
        success &= _renderManager->Init();
        
		return success;
	}

    bool Application::OnInitComplete()
    {
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
        _renderManager->Render();
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
        
        _renderManager->Destroy();
        _renderManager = nullptr;
	}
}
