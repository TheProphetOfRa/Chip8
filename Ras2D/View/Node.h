//
//  Node.h
//  Ras2D
//
//  Created by David Hodgkinson on 25/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//

#pragma once

#include "Math/BaseMath.h"

#include <GL/glew.h>

namespace Ras2D
{
	class Node
	{
	public:
		static Node* Create();

		const Point& GetPosition() const { return _position; }
		const Size& GetSize() const { return _size; }
		void SetPosition(Point pos);
		void SetSize(Size size);

		const GLuint& GetVAO() const { return _vao; }
		const GLuint& GetShaderProgram() const { return _shader; }

		virtual void Render() const;
		virtual void Update();

		virtual void SetTexture(const float * data);
	protected:
		Node() {}
		bool Init();

		void GenBuffers();
	protected:
		Point _position;
		Size _size;

		GLuint _vao;
		GLuint _shader;
		GLuint _tex;
        //240
	};
}
