//
//  opcodes.cpp
//  Chip8 Emulator
//
//  Created by David Hodgkinson on 24/11/2015.
//  Copyright © 2015 David Hodgkinson. All rights reserved.
//

#include "opcodes.h"

#include "cpu.h"

#include <stdlib.h>

namespace Chip8
{
    Opcodes::Opcodes()
    {
        //Top level catch for all 0x0XXX codes
        _instructionTable[0x0000] = std::bind(&Opcodes::ExecuteZeroCode, this, std::placeholders::_1, std::placeholders::_2);
        
        _instructionTable[0x1000] = std::bind(&Opcodes::JumpToAddr, this, std::placeholders::_1, std::placeholders::_2);
        
        _instructionTable[0x2000] = std::bind(&Opcodes::CallSubAtAddr, this, std::placeholders::_1, std::placeholders::_2);
        
        _instructionTable[0x3000] = std::bind(&Opcodes::SkipInstrIf, this, std::placeholders::_1, std::placeholders::_2);
        
        _instructionTable[0x4000] = std::bind(&Opcodes::SkipInstrIfNot, this, std::placeholders::_1, std::placeholders::_2);
        
        _instructionTable[0x5000] = std::bind(&Opcodes::SkipInstrIfXY, this, std::placeholders::_1, std::placeholders::_2);
        
        _instructionTable[0x6000] = std::bind(&Opcodes::SetX, this, std::placeholders::_1, std::placeholders::_2);
        
        _instructionTable[0x7000] = std::bind(&Opcodes::AddNToX, this, std::placeholders::_1, std::placeholders::_2);
        
        _instructionTable[0x8000] = std::bind(&Opcodes::ExecuteEightCode, this, std::placeholders::_1, std::placeholders::_2);
        
        _instructionTable[0x9000] = std::bind(&Opcodes::NineCodes, this, std::placeholders::_1, std::placeholders::_2);
        
        _instructionTable[0xA000] = std::bind(&Opcodes::ACodes, this, std::placeholders::_1, std::placeholders::_2);
        
        _instructionTable[0xB000] = std::bind(&Opcodes::BCodes, this, std::placeholders::_1, std::placeholders::_2);
        
        _instructionTable[0xC000] = std::bind(&Opcodes::CCodes, this, std::placeholders::_1, std::placeholders::_2);
        
        _instructionTable[0xD000] = std::bind(&Opcodes::DCodes, this, std::placeholders::_1, std::placeholders::_2);
        
        _instructionTable[0xE000] = std::bind(&Opcodes::ExecuteECode, this, std::placeholders::_1, std::placeholders::_2);
        
        _instructionTable[0xF000] = std::bind(&Opcodes::ExecuteFCode, this, std::placeholders::_1, std::placeholders::_2);
        
        
        
        _zeroCodeTable[0x0000] = std::bind(&Opcodes::ClearScreen, this, std::placeholders::_1, std::placeholders::_2);
        
        _zeroCodeTable[0x000E] = std::bind(&Opcodes::ReturnFromSub, this, std::placeholders::_1, std::placeholders::_2);
        
        
        
        _eightCodeTable[0x0000] = std::bind(&Opcodes::SetVXToVY, this, std::placeholders::_1, std::placeholders::_2);
        
        _eightCodeTable[0x0001] = std::bind(&Opcodes::SetVXToVXORVY, this, std::placeholders::_1, std::placeholders::_2);
        
        _eightCodeTable[0x0002] = std::bind(&Opcodes::SetVXToVXANDVY, this, std::placeholders::_1, std::placeholders::_2);
        
        _eightCodeTable[0x0003] = std::bind(&Opcodes::SetVXToVXXORVY, this, std::placeholders::_1, std::placeholders::_2);
        
        _eightCodeTable[0x0004] = std::bind(&Opcodes::AddVYToVXAndCarry, this, std::placeholders::_1, std::placeholders::_2);
        
        _eightCodeTable[0x0005] = std::bind(&Opcodes::SubVYFromVXAndCarry, this, std::placeholders::_1, std::placeholders::_2);
        
        _eightCodeTable[0x0006] = std::bind(&Opcodes::ShiftVXRight, this, std::placeholders::_1, std::placeholders::_2);
        
        _eightCodeTable[0x0007] = std::bind(&Opcodes::SubVXFromVY, this, std::placeholders::_1, std::placeholders::_2);
        
        _eightCodeTable[0x000E] = std::bind(&Opcodes::ShiftVXLeft, this, std::placeholders::_1, std::placeholders::_2);

        
        
        
        _eCodeTable[0x009E] = std::bind(&Opcodes::SkipIfKeyPressed, this, std::placeholders::_1, std::placeholders::_2);
        
        _eCodeTable[0x00A1] = std::bind(&Opcodes::SkipIfKeyNotPressed, this, std::placeholders::_1, std::placeholders::_2);
        
        
        
        
        _fCodeTable[0x0007] = std::bind(&Opcodes::SetVXToDelay, this, std::placeholders::_1, std::placeholders::_2);
        
        _fCodeTable[0x000A] = std::bind(&Opcodes::StoreKeyInVX, this, std::placeholders::_1, std::placeholders::_2);
        
        _fCodeTable[0x0015] = std::bind(&Opcodes::SetDelayTimerToVX, this, std::placeholders::_1, std::placeholders::_2);
        
        _fCodeTable[0x0018] = std::bind(&Opcodes::SetSoundTimerToVX, this, std::placeholders::_1, std::placeholders::_2);
        
        _fCodeTable[0x001E] = std::bind(&Opcodes::AddVXToI, this, std::placeholders::_1, std::placeholders::_2);
        
        _fCodeTable[0x0029] = std::bind(&Opcodes::SetIToSpriteLocation, this, std::placeholders::_1, std::placeholders::_2);
        
        _fCodeTable[0x0033] = std::bind(&Opcodes::StoreVXAtI, this, std::placeholders::_1, std::placeholders::_2);
                                        
        _fCodeTable[0x0055] = std::bind(&Opcodes::StoreContentOfVAtI, this, std::placeholders::_1, std::placeholders::_2);
        
        _fCodeTable[0x0065] = std::bind(&Opcodes::FillContentOfVFromI, this, std::placeholders::_1, std::placeholders::_2);
    }
    
    void Opcodes::Execute(CPU *cpu, unsigned short opcode)
    {
        _instructionTable[opcode & 0xF000](cpu, opcode);
    }
    
    void Opcodes::ExecuteZeroCode(Chip8::CPU *cpu, unsigned short opcode)
    {
        _zeroCodeTable[opcode & 0x000F](cpu, opcode);
    }
    
    //1NNN Jump to address NNN
    void Opcodes::JumpToAddr(Chip8::CPU *cpu, unsigned short opcode)
    {
        cpu->_pc = opcode & 0x0FFF;
    }
    
    //2NNN call subroutine at NNN
    void Opcodes::CallSubAtAddr(Chip8::CPU *cpu, unsigned short opcode)
    {
        //Push the memory location onto the stack for the subroutine return
        cpu->_stack[cpu->_pStack] = cpu->_pc;
        ++cpu->_pStack;
        cpu->_pc = opcode & 0x0FFF;
    }
    
    //3XNN Skip next instruction if v[X] == NN
    void Opcodes::SkipInstrIf(Chip8::CPU *cpu, unsigned short opcode)
    {
        if (cpu->_v[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
            cpu->_pc += 4;
        else
            cpu->_pc += 2;
    }
    
    //4XNN Skip next instruction if v[X] != NN
    void Opcodes::SkipInstrIfNot(Chip8::CPU *cpu, unsigned short opcode)
    {
        if (cpu->_v[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
            cpu->_pc += 4;
        else
            cpu->_pc += 2;
    }
    
    //5XY0 Skip next instruction if v[X] == v[Y]
    void Opcodes::SkipInstrIfXY(Chip8::CPU *cpu, unsigned short opcode)
    {
        if (cpu->_v[(opcode & 0X0F00) >> 8] == cpu->_v[(opcode & 0x00F0) >> 4])
            cpu->_pc += 4;
        else
            cpu->_pc += 2;
    }
    
    //6XNN Sets v[X] to NN
    void Opcodes::SetX(Chip8::CPU *cpu, unsigned short opcode)
    {
        cpu->_v[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
        cpu->_pc+=2;
    }
    
    //7XNN Adds NN to v[X]
    void Opcodes::AddNToX(Chip8::CPU *cpu, unsigned short opcode)
    {
        cpu->_v[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
        cpu->_pc+=2;
    }
    
    void Opcodes::ExecuteEightCode(Chip8::CPU *cpu, unsigned short opcode)
    {
        _eightCodeTable[opcode & 0x000F](cpu, opcode);
    }
    
    void Opcodes::NineCodes(Chip8::CPU *cpu, unsigned short opcode)
    {
        if (cpu->_v[(opcode & 0x0F00) >> 8] != cpu->_v[(opcode & 0x00F0) >> 4])
            cpu->_pc += 4;
        else
            cpu->_pc += 2;
    }
    
    void Opcodes::ACodes(Chip8::CPU *cpu, unsigned short opcode)
    {
        cpu->_I = opcode & 0x0FFF;
        cpu->_pc += 2;
    }
    
    void Opcodes::BCodes(Chip8::CPU *cpu, unsigned short opcode)
    {
        cpu->_pc = (opcode & 0x0FFF) + cpu->_v[0];
    }
    
    void Opcodes::CCodes(Chip8::CPU *cpu, unsigned short opcode)
    {
        cpu->_v[(opcode & 0x0F00) >> 8] = (rand() % 0xFF) & (opcode & 0x00FF);
        cpu->_pc+=2;
    }
    
    void Opcodes::DCodes(Chip8::CPU *cpu, unsigned short opcode)
    {
        const unsigned short x = cpu->_v[(opcode & 0x0F00) >> 8];
        const unsigned short y = cpu->_v[(opcode & 0x00F0) >> 4];
        const short height = opcode & 0x000F;
        short pixel;
        cpu->_v[0xF] = 0;
        for (int yline = 0 ; yline < height ; ++yline)
        {
            pixel = cpu->_memory[cpu->_I + yline];
            for (int xline = 0 ; xline < 8 ; ++xline)
            {
                if ((pixel & (0x80 >> xline)) != 0)
                {
                    if(cpu->_gfx[(x + xline + ((y + yline) * 64))] == 1)
                        cpu->_v[0xF] = 1;
                    cpu->_gfx[x + xline + ((y + yline) * 64)] ^= 1;
                }
            }
        }
        cpu->_drawFlag = true;
        cpu->_pc+=2;
    }
    
    void Opcodes::ExecuteECode(Chip8::CPU *cpu, unsigned short opcode)
    {
        _eCodeTable[opcode & 0x00FF](cpu, opcode);
    }
    
    void Opcodes::ExecuteFCode(Chip8::CPU *cpu, unsigned short opcode)
    {
        _fCodeTable[opcode & 0x00FF](cpu, opcode);
    }
    
#pragma mark - 0 Codes
    
    void Opcodes::ClearScreen(Chip8::CPU *cpu, unsigned short opcode)
    {
        for (int i = 0 ; i < (64 * 32) ; ++i) cpu->_gfx[i] = 0x0;
        cpu->_drawFlag = true;
        cpu->_pc+=2;
    }
    
    void Opcodes::ReturnFromSub(Chip8::CPU *cpu, unsigned short opcode)
    {
        --cpu->_pStack;
        cpu->_pc = cpu->_stack[cpu->_pStack];
        cpu->_pc += 2;
    }
    
#pragma mark - 8 Codes
    
    //Set v[X] to the value of v[Y]
    void Opcodes::SetVXToVY(CPU* cpu, unsigned short opcode)
    {
        cpu->_v[(opcode & 0x0F00) >> 8] = cpu->_v[(opcode & 0x00F0) >> 4];
        cpu->_pc+=2;
    }
    
    //Set v[X] to v[X] OR v[Y]
    void Opcodes::SetVXToVXORVY(CPU* cpu, unsigned short opcode)
    {
        cpu->_v[(opcode & 0x0F00) >> 8] |= cpu->_v[(opcode & 0x00F0) >> 4];
        cpu->_pc+=2;
    }
    
    //Set v[X] to v[X] AND v[Y]
    void Opcodes::SetVXToVXANDVY(CPU* cpu, unsigned short opcode)
    {
        cpu->_v[(opcode & 0x0F00) >> 8] &= cpu->_v[(opcode & 0x00F0) >> 4];
        cpu->_pc+=2;
    }
    
    //Set v[X] to v[X] XOR v[Y]
    void Opcodes::SetVXToVXXORVY(CPU* cpu, unsigned short opcode)
    {
        cpu->_v[(opcode & 0x0F00) >> 8] ^= cpu->_v[(opcode & 0x00F0) >> 4];
        cpu->_pc+=2;
    }
    
    //Adds v[Y] to v[X] ands sets carry bit if required
    void Opcodes::AddVYToVXAndCarry(CPU* cpu, unsigned short opcode)
    {
        if (cpu->_v[(opcode & 0x00F0) >> 4] > (0xFF - cpu->_v[(opcode & 0x0F00) >> 8]))
            cpu->_v[0xF] = 1;
        else
            cpu->_v[0xF] = 0;
        cpu->_v[(opcode & 0x0F00) >> 8] += cpu->_v[(opcode & 0x00F0) >> 4];
        cpu->_pc+=2;
    }
    
    //Subtracts v[Y] from v[X] and sets carry bit if required
    void Opcodes::SubVYFromVXAndCarry(CPU* cpu, unsigned short opcode)
    {
        if (cpu->_v[(opcode & 0x00F0) >> 4] > cpu->_v[(opcode & 0x0F00) >> 8])
            cpu->_v[0xF] = 0;
        else
            cpu->_v[0xF] = 1;
        cpu->_v[(opcode & 0x0F00) >> 8] -= cpu->_v[(opcode & 0x00F0) >> 4];
        cpu->_pc+=2;
    }
    
    //Shifts v[X] right by one stores the LSB in v[F](before shift)
    void Opcodes::ShiftVXRight(CPU* cpu, unsigned short opcode)
    {
        cpu->_v[0xF] = cpu->_v[(opcode & 0x0F00) >> 8] & 0x1;
        cpu->_v[(opcode & 0x0F00) >> 8] >>= 1;
        cpu->_pc+=2;
    }
    
    //v[X] = v[Y] - v[X], v[F] = 0 when borrow required
    void Opcodes::SubVXFromVY(CPU* cpu, unsigned short opcode)
    {
        if (cpu->_v[(opcode & 0x0F00) >> 8] > cpu->_v[(opcode & 0x00F0) >> 4])
            cpu->_v[0xF] = 0;
        else
            cpu->_v[0X0] = 1;
        cpu->_v[(opcode & 0x0F00) >> 8] = cpu->_v[(opcode & 0x00F0) >> 4] - cpu->_v[(opcode & 0x0F00) >> 8];
        cpu->_pc+=2;
    }
    
    //Shift v[X] left by one and set v[F] to MSB before shift
    void Opcodes::ShiftVXLeft(CPU* cpu, unsigned short opcode)
    {
        cpu->_v[0xF] = cpu->_v[(opcode & 0x0F00) >> 8] >> 7;
        cpu->_v[(opcode & 0x0F00) >> 8] <<= 1;
        cpu->_pc+=2;
    }
    
#pragma mark - E Codes
    
    //0xEX9E skips the next instruction
    //if the key stored in VX is pressed
    void Opcodes::SkipIfKeyPressed(Chip8::CPU *cpu, unsigned short opcode)
    {
        if (cpu->_keys[cpu->_v[(opcode & 0x0F00) >> 8]] != 0)
            cpu->_pc += 4;
        else
            cpu->_pc += 2;
    }
    
    //Skips the next instruction if the key in v[X] isn't pressed
    void Opcodes::SkipIfKeyNotPressed(Chip8::CPU *cpu, unsigned short opcode)
    {
        if (cpu->_keys[cpu->_v[(opcode & 0x0F00) >> 8]] == 0)
            cpu->_pc += 4;
        else
            cpu->_pc += 2;
    }
    
#pragma mark - F Codes
    
    //Set v[X] to the value of delayTimer
    void Opcodes::SetVXToDelay(Chip8::CPU *cpu, unsigned short opcode)
    {
        cpu->_v[(opcode & 0x0F00) >> 8] = cpu->_delayTimer;
        cpu->_pc+=2;
    }
    
    //Wait for a key press then store it in v[X]
    void Opcodes::StoreKeyInVX(Chip8::CPU *cpu, unsigned short opcode)
    {
        bool keyPress = false;
        for (int i = 0 ; i < 16 ; ++i)
        {
            if (cpu->_keys[i] != 0)
            {
                cpu->_v[(opcode & 0x0F00) >> 8] = i;
                keyPress = true;
            }
        }
        if (!keyPress) return;
        cpu->_pc += 2;
    }
    
    //Sets the delayTimer to v[X]
    void Opcodes::SetDelayTimerToVX(Chip8::CPU *cpu, unsigned short opcode)
    {
        cpu->_delayTimer = cpu->_v[(opcode & 0x0F00) >> 8];
        cpu->_pc+=2;
    }
    
    //Set the sound timer to v[]
    void Opcodes::SetSoundTimerToVX(Chip8::CPU *cpu, unsigned short opcode)
    {
        cpu->_soundTimer = cpu->_v[(opcode & 0x0F00) >> 8];
        cpu->_pc+=2;
    }
    
    //Adds v[X] to I
    void Opcodes::AddVXToI(Chip8::CPU *cpu, unsigned short opcode)
    {
        if (cpu->_I + cpu->_v[(opcode & 0x0F00) >> 8] > 0xFFF) //Check if we need to set the carry flag
            cpu->_v[0xF] = 1;
        else
            cpu->_v[0xF] = 0;
        cpu->_I += cpu->_v[(opcode & 0x0F00) >> 8];
        cpu->_pc+=2;
    }
    
    //Sets I to the location of the sprite for the character in v[X].
    void Opcodes::SetIToSpriteLocation(Chip8::CPU *cpu, unsigned short opcode)
    {
        cpu->_I = cpu->_v[(opcode & 0x0F00) >> 8] * 0.5;
        cpu->_pc+=2;
    }
    
    //Stores the binary representation of v[X] at I, I+1 and I+2
    void Opcodes::StoreVXAtI(Chip8::CPU *cpu, unsigned short opcode)
    {
        cpu->_memory[cpu->_I]   = cpu->_v[(opcode & 0x0F00) >> 8] / 100;
        cpu->_memory[cpu->_I+1] = (cpu->_v[(opcode & 0x0F00) >> 8] / 10) % 10;
        cpu->_memory[cpu->_I+2] = (cpu->_v[(opcode & 0x0F00) >> 8] % 100) % 10;
        cpu->_pc+=2;
    }
    
    //Stores v[0] to v[X] in memory starting at address I
    void Opcodes::StoreContentOfVAtI(Chip8::CPU *cpu, unsigned short opcode)
    {
        for (int i = 0 ; i < ((opcode & 0x0F00) >> 8) ; ++i)
            cpu->_memory[cpu->_I+i] = cpu->_v[i];
        //The original interpreter set I to I + X + 1
        cpu->_I += ((opcode & 0x0F00) >> 8) + 1;
        cpu->_pc+=2;
    }
    
    //Fills v[0] to v[X] with values from memory starting at I
    void Opcodes::FillContentOfVFromI(Chip8::CPU *cpu, unsigned short opcode)
    {
        for (int i = 0 ; i <= ((opcode & 0x0F00) >> 8) ; ++i)
            cpu->_v[i] = cpu->_memory[cpu->_I+i];
        //The original interpreter set I to I + X + 1
        cpu->_I += ((opcode & 0x0F00) >> 8) + 1;
        cpu->_pc+=2;
    }
    
#pragma mark -
    
}