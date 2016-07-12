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
        
#pragma mark - 0 Codes
        
        void ClearScreen(CPU* cpu, unsigned short opcode);
        void ReturnFromSub(CPU* cpu, unsigned short opcode);
        
#pragma mark -
        
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
        
        void ExecuteFCode(CPU* cpu, unsigned short opcode);
        
#pragma mark - F Codes
        
        void SetVXToDelay(CPU* cpu, unsigned short opcode);
        
        void StoreKeyInVX(CPU* cpu, unsigned short opcode);
        
        void SetDelayTimerToVX(CPU* cpu, unsigned short opcode);
        
        void SetSoundTimerToVX(CPU* cpu, unsigned short opcode);
        
        void AddVXToI(CPU* cpu, unsigned short opcode);
        
        void SetIToSpriteLocation(CPU* cpu, unsigned short opcode);
        
        void StoreVXAtI(CPU* cpu, unsigned short opcode);
        
        void StoreContentOfVAtI(CPU* cpu, unsigned short opcode);
        
        void FillContentOfVFromI(CPU* cpu, unsigned short opcode);
        
#pragma mark -
        
    private:
        std::unordered_map<unsigned short, std::function<void(CPU*, unsigned short)>> _instructionTable;
        
        std::unordered_map<unsigned short, std::function<void(CPU*, unsigned short)>> _fCodeTable;
    };
}

#endif /* opcodes_h */
