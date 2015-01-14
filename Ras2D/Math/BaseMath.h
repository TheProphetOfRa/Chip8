//
//  Scene.h
//  Ras2D
//
//  Created by David Hodgkinson on 25/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//

#ifndef __Ras2D__Base_Math__
#define __Ras2D__Base_Math__

namespace Ras2D
{
	struct Point
	{
		float x;
		float y;

		Point() : x(0.0f), y(0.0f) {}
		Point(float x, float y) : x(x), y(y) {}
	};

	struct Size
	{
		float width;
		float height;

		Size() : width(1.0f), height(1.0f) {}
		Size(float w, float h) : width(w), height(h) {}
	};
}

#endif