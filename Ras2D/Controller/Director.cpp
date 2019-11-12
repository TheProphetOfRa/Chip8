//
//  Director.cpp
//  Ras2D
//
//  Created by David Hodgkinson on 24/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//

#include "Director.h"

#include "Application/RasApplication.h"
#include "Controller/RenderManager.h"
#include "Util/Log.h"
#include "View/Node.h"

#include <assert.h>
#include <stdio.h>
#include <vector>

namespace Ras2D
{
	bool Director::Init()
	{
		assert(Log::Restart());

		return true;
	}

    bool Director::RegisterForUpdate(Ras2D::Node *node)
    {
        auto itr = std::find(_nodes.begin(), _nodes.end(), node);
        
        if (itr == _nodes.end())
        {
            _nodes.push_back(node);
            return true;
        }
        
        return false;
    }

    void Director::UnRegisterForUpdate(Ras2D::Node *node)
    {
        auto itr = std::find(_nodes.begin(), _nodes.end(), node);
        
        if (itr != _nodes.end())
        {
            _nodes.erase(itr);
        }
    }

	bool Director::Update(std::vector<bool> &keys, GLFWwindow* window)
	{
        //TODO: Move input to input manager
		// update other events like input handling
		glfwPollEvents();

		for (const auto n : _nodes)
		{
			n->Update();
		}

		//TODO offload this to a keymanager
		//close window when escape is pressed
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(window, 1);
		}

		keys[0] = glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS;
		keys[1] = glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS;
		keys[2] = glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS;
		keys[3] = glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS;
		keys[4] = glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS;
		keys[5] = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
		keys[6] = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;
		keys[7] = glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS;
		keys[8] = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
		keys[9] = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
		keys[10] = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
		keys[11] = glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS;
		keys[12] = glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS;
		keys[13] = glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS;
		keys[14] = glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS;
		keys[15] = glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS;


		return !glfwWindowShouldClose(window);
	}

	void Director::Destroy()
	{
	}
}
