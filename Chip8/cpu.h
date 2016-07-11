#pragma once

#include "opcodes.h"

#include <functional>
#include <unordered_map>

namespace Chip8
{
    class CPU
    {
    public:
        
        friend class Opcodes;
        
        const static int kScreenWidth = 64;
        const static int kScreenHeight = 32;

        void Init(void);
        bool LoadGame(const char *filename);

        void EmulateCycle(void);

        bool ShouldDraw() const {return _drawFlag;}

        const unsigned char * GetGfx();

        void SetKey(int code);
        void ResetKey(int code);
    private:
        unsigned short Fetch(void) const;
        void Execute(void);

        void UpdateTimers(void);

    private:
        unsigned short _opcode;
        unsigned char _memory[4096];
        unsigned char _v[16];
        unsigned short _I;
        unsigned short _pc;
        unsigned char _gfx[64 * 32];
        unsigned char _delayTimer;
        unsigned char _soundTimer;
        unsigned short _stack[16];
        unsigned short _pStack;
        unsigned char _keys[16];

        bool _drawFlag;
        
        Opcodes opcodes;
    };
}
