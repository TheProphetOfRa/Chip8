//
//  RasApplication.h
//  Ras2D
//
//  Created by David Hodgkinson on 26/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//

#ifndef __Ras2D__Application__
#define __Ras2D__Application__

#include "Controller/Director.h"

namespace Ras2D
{
	class Application
	{
	public:
		static Application* Create();
        
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
        
        int _frameRate;
	};
}
#endif
