//
//  Scene.cpp
//  Ras2D
//
//  Created by David Hodgkinson on 25/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//

#include "Scene.h"

#include "Application/RasApplication.h"

namespace Ras2D
{
	Scene* Scene::Create()
	{
		auto rtn = new Scene();

		if (rtn && rtn->Init())
		{
			return rtn;
		}
		return nullptr;
	}

	bool Scene::Init()
	{
		Node::Init();

		const Size& winSize = Application::GetInstance()->GetRenderManager()->GetWindowSize();

		SetSize(winSize);
		SetPosition({winSize.width / 2, winSize.height / 2});

		return true;
	}
}
