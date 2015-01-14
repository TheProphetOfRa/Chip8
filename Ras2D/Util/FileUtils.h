//
//  FileUtils.h
//  Ras2D
//
//  Created by David Hodgkinson on 26/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//

#ifndef __Ras2D__FileUtils__
#define __Ras2D__FileUtils__

#include <string>

namespace Ras2D
{
	namespace FileUtils
	{
		bool ReadFromFile(std::string& data, std::string& filename);
	}
}

#endif