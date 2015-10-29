//
//  FileUtils.cpp
//  Ras2D
//
//  Created by David Hodgkinson on 25/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//

#include "Util/FileUtils.h"

namespace Ras2D
{
	bool FileUtils::ReadFromFile(std::string& data, std::string& filename)
	{
		FILE* file;
		fopen_s(&file, filename.c_str(), "r");

		if (!file)
			return false;

		char ch;
		while ((ch = fgetc(file)) != EOF)
			data += ch;

		fclose(file);
		return true;
	}
}