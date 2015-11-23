//
//  CPU.cpp
//  Chip8 Emulator
//
//  Created by David Hodgkinson on 30/10/2015.
//  Copyright © 2015 David Hodgkinson. All rights reserved.
//

#include "CPU.h"

#include "Util/FileUtils.h"

#include <cstring>

namespace GB
{
    static const int kMaxCycles = 69905;
    
    static const int kFlagZ = 7;
    static const int kFlagN = 6;
    static const int kFlagH = 5;
    static const int kFlagC = 4;

    CPU::CPU()
    : _pc(0x100)
    , _regAF(0x01B0)
    , _regBC(0x0013)
    , _regDE(0x00D8)
    , _regHL(0x014D)
    , _pStack(0xFFFE)
    {
        for (int i = 0 ; i < 0x10000 ; ++i)
            _rom[i] = 0x00;
        
        _rom[0xFF05] = 0x00;
        _rom[0xFF06] = 0x00;
        _rom[0xFF07] = 0x00;
        _rom[0xFF10] = 0x80;
        _rom[0xFF11] = 0xBF;
        _rom[0xFF12] = 0xF3;
        _rom[0xFF14] = 0xBF;
        _rom[0xFF16] = 0x3F;
        _rom[0xFF17] = 0x00;
        _rom[0xFF19] = 0xBF;
        _rom[0xFF1A] = 0x7F;
        _rom[0xFF1B] = 0xFF;
        _rom[0xFF1C] = 0x9F;
        _rom[0xFF1E] = 0xBF;
        _rom[0xFF20] = 0xFF;
        _rom[0xFF21] = 0x00;
        _rom[0xFF22] = 0x00;
        _rom[0xFF23] = 0xBF;
        _rom[0xFF24] = 0x77;
        _rom[0xFF25] = 0xF3;
        _rom[0xFF26] = 0xF1;
        _rom[0xFF40] = 0x91;
        _rom[0xFF42] = 0x00;
        _rom[0xFF43] = 0x00;
        _rom[0xFF45] = 0x00;
        _rom[0xFF47] = 0xFC;
        _rom[0xFF48] = 0xFF;
        _rom[0xFF49] = 0xFF;
        _rom[0xFF4A] = 0x00;
        _rom[0xFF4B] = 0x00;
        _rom[0xFFFF] = 0x00;
    }
    
    void CPU::LoadGame(const char *filename)
    {
        memset(_cartridgeMemory, 0, sizeof(_cartridgeMemory));
        
        FILE* rom = Ras2D::FileUtils::GetFile("file", "rb");
        fread(_cartridgeMemory, 1, 0x200000, rom);
        fclose(rom);
    }
    
    void CPU::Update()
    {
        int cyclesThisUpdate = 0;
        
        while (cyclesThisUpdate < kMaxCycles)
        {
            //int cycles = ExecuteNextOpcode();
            //cyclesThisUpdate += cycles;
            //UpdateTimers(cycles);
            //UpdateGraphics(cycles);
            //DoInterupts();
        }
        //Render();
    }
    
    void CPU::WriteMemory(WORD address, BYTE data)
    {
        if (address < 0x8000)
        {
            //Don't write to read only memory
        }
        //Writing to echo ram also writes to ram
        else if ((address >= 0xE000) && (address < 0xFE00))
        {
            _rom[address] = data;
            WriteMemory(address - 0x2000, data);
        }
        //this area is restricted
        else if ((address >= 0xFEA0) && (address <= 0xFEFF))
        {
            
        }
        //no control needed
        else
        {
            _rom[address] = data;
        }
    }
}