//
//  CPU.cpp
//  Chip8 Emulator
//
//  Created by David Hodgkinson on 30/10/2015.
//  Copyright © 2015 David Hodgkinson. All rights reserved.
//

#include "GBCPU.h"

#include "Util/FileUtils.h"

#include <cstring>

namespace GB
{
    GBCPU::GBCPU()
    {
    }
    
    bool GBCPU::Init()
    {
        return true;
    }

    bool GBCPU::LoadGame(const char *filename)
    {
        bool result = false;
        
        memset(&_cartridgeMemory, 0, sizeof(_cartridgeMemory));
        
        FILE* rom = nullptr;
        rom = Ras2D::FileUtils::GetFile(filename, "rb");
        
        if (rom)
        {
            result = true;
            fread(&_cartridgeMemory, 1, 0x200000, rom);
        }
        
        fclose(rom);
        
        return result;
    }
    
    void GBCPU::EmulateCycle()
    {
//        _opcode = Fetch();
//        Execute();
    }
}
