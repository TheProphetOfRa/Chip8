//
//  Director.h
//  Ras2D
//
//  Created by David Hodgkinson on 24/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//

#pragma once

#include "Controller/RenderManager.h"

#include <string>
#include <list>

namespace Ras2D
{
	class Node;

	class Director
	{
	public:
		//This will set up the renderer and shaders and a scheduler
		bool Init();

        bool RegisterForUpdate(Ras2D::Node* node);
        void UnRegisterForUpdate(Ras2D::Node* node);
        
        //TODO: input to input manager
		bool Update(std::vector<bool> &keys, GLFWwindow* window);

		void Destroy();

	private:
		std::list<Node*> _nodes;
	};
}
