//
//  Scene.h
//  Ras2D
//
//  Created by David Hodgkinson on 25/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//

#ifndef __Ras2D__Scene__
#define __Ras2D__Scene__

#include "Node.h"

namespace Ras2D
{
	class Scene : public Node
	{
		static Scene* Create();

		virtual bool Init();
	};
}

#endif