//
//  FileUtils.cpp
//  Chip8 Emulator
//
//  Created by David Hodgkinson on 29/10/2015.
//  Copyright © 2015 David Hodgkinson. All rights reserved.
//

#include "FileUtils.h"

namespace Ras2D
{
    bool FileUtils::ReadFromFile(std::string& data, const std::string& filename)
    {
        FILE* file = fopen(filename.c_str(), "r");
        
        if (!file)
            return false;
        
        char ch;
        while ((ch = fgetc(file)) != EOF)
            data += ch;
        
        fclose(file);
        return true;
    }
    
    FILE* FileUtils::GetFile(const std::string& filename, const std::string& mode)
    {
        return fopen(filename.c_str(), mode.c_str());
    }
}