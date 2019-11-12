//
//  RasApplication.h
//  Ras2D
//
//  Created by David Hodgkinson on 26/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//

#pragma once

#include "Controller/Director.h"
#include "Controller/RenderManager.h"

namespace Ras2D
{
	class Application
	{
	public:
        static Application* GetInstance();
        
        ~Application();
        
        RenderManager* GetRenderManager() { return _renderManager; }
        Director* GetDirector() { return _director; }
        
        virtual bool OnInitComplete();
        
		virtual void Run();
		virtual void End();
	protected:
        Application();
                
		virtual bool Init();
        
        virtual bool ProcessInput();
        virtual bool Update();
        virtual bool Render();
    private:
		virtual void MainLoop();
	protected:
		Director *_director;
        RenderManager *_renderManager;
        
        int _frameRate;
        
        static Application* sInstance;
	};
}
