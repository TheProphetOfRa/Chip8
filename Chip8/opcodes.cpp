//
//  opcodes.cpp
//  Chip8 Emulator
//
//  Created by David Hodgkinson on 24/11/2015.
//  Copyright © 2015 David Hodgkinson. All rights reserved.
//

#include "opcodes.h"

#include "cpu.h"

namespace Chip8
{
    void registerOpCodes(CPU* cpu, std::map<int, std::function<void(unsigned short)>>& ops)
    {
    }
}