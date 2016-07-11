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
        ops[0x0000] = [&cpu](unsigned short code){
            switch(code & 0x000F) //Checking for last 4-bits
            {
                case 0x0000: //0x0XX0(0x00E0) Clears the screen
                    for (int i = 0 ; i < (64 * 32) ; ++i) _gfx[i] = 0x0;
                    cpu->_drawFlag = true;
                    cpu->_pc+=2;
                    break;
                case 0x000E: //0x0XXE(0x00EE) Returns from subroutine
                    --_pStack;
                    _pc = _stack[_pStack];
                    _pc+=2;
                    break;
                default:
                    break;
            }
        };
    }
}