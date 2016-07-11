//
//  opcodes.h
//  Chip8 Emulator
//
//  Created by David Hodgkinson on 24/11/2015.
//  Copyright © 2015 David Hodgkinson. All rights reserved.
//

#ifndef opcodes_h
#define opcodes_h

#include <functional>
#include <unordered_map>

namespace Chip8
{
    class CPU;
    
    class Opcodes
    {
    public:
        
        Opcodes();
        
        void Execute(CPU* cpu, unsigned short opcode);
        
    private:
        
        void ClearScreen(CPU* cpu, unsigned short opcode);
        void ReturnFromSub(CPU* cpu, unsigned short opcode);
        
        void JumpToAddr(CPU* cpu, unsigned short opcode);
        void CallSubAtAddr(CPU* cpu, unsigned short opcode);
        
        void SkipInstrIf(CPU* cpu, unsigned short opcode);
        void SkipInstrIfNot(CPU* cpu, unsigned short opcode);
        void SkipInstrIfXY(CPU* cpu, unsigned short opcode);
        
        void SetX(CPU* cpu, unsigned short opcode);
        
        void AddNToX(CPU* cpu, unsigned short opcode);
        
        void EightCodes(CPU* cpu, unsigned short opcode);
        
        void NineCodes(CPU* cpu, unsigned short opcode);
        
        void ACodes(CPU* cpu, unsigned short opcode);
        
        void BCodes(CPU* cpu, unsigned short opcode);
        
        void CCodes(CPU* cpu, unsigned short opcode);
        
        void DCodes(CPU* cpu, unsigned short opcode);
        
        void ECodes(CPU* cpu, unsigned short opcode);
        
        void FCodes(CPU* cpu, unsigned short opcodes);
        
    private:
        std::unordered_map<unsigned short, std::function<void(CPU*, unsigned short)>> _instructionTable;
    };
}

#endif /* opcodes_h */
