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

	bool Application::Init()
	{
		_director = Director::GetInstance();
		_director->Init();

		return true;
	}

	void Application::Run()
	{
		MainLoop();
	}

	void Application::MainLoop()
	{
		End();
	}

	void Application::End()
	{
		_director->Destroy();
	}
}