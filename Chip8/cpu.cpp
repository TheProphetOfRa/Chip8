#include "cpu.h"

#include "Util/FileUtils.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

namespace Chip8
{
    static const unsigned char kChip8Fontset[80] =
    { 
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    
    static const unsigned char kKeycodes[16] =
    {
        0x1, 0x2, 0x3, 0xc, 0x4, 0x5, 0x6, 0xd,
        0x7, 0x8, 0x9, 0xe, 0xa, 0x0, 0xb, 0xf
    };
    
    void CPU::Init(void)
    {
        _pc = 0x200; //Program starts at 0x200
        _opcode = 0; //Reset current opcode
        _I = 0;      //Reset index register
        _pStack = 0; //Reset stack pointer

        for (int i = 0 ; i < (64*32) ; i++) _gfx[i] = 0; //Clear gfx buffer
        for (int i = 0 ; i < 16 ; i++) _stack[i] = 0;    //Clear the stack
        for (int i = 0 ; i < 16 ; i++) _v[i] = 0;        //Clear the registers
        for (int i = 0 ; i < 4096 ; i++) _memory[i] = 0; //Clear memory
        for (int i = 0 ; i < 16 ; i++) _keys[i] = 0;      //Clear the key buffer

        for (int i = 0 ; i < 80 ; i++)
            _memory[i] = kChip8Fontset[i]; //Load the fontset
        
        _drawFlag = false; //Set the draw call to false
        _delayTimer = 0; //Reset the delay timer
        _soundTimer = 0; //Reset the sound timer
    }

    bool CPU::LoadGame(const char *filename)
    {
        printf("Loading: %s\n", filename);
        
        // Open file
        FILE * pFile = Ras2D::FileUtils::GetFile(filename, "rb");
 
        if (pFile == NULL)
        {
            fputs ("File error", stderr);
            return false;
        }
        
        // Check file size
        fseek(pFile , 0 , SEEK_END);
        long lSize = ftell(pFile);
        rewind(pFile);
        printf("Filesize: %d\n", (int)lSize);
        
        // Allocate memory to contain the whole file
        char * buffer = (char*)malloc(sizeof(char) * lSize);
        if (buffer == NULL)
        {
            fputs ("Memory error", stderr);
            return false;
        }
        
        // Copy the file into the buffer
        size_t result = fread (buffer, 1, lSize, pFile);
        if (result != lSize)
        {
            fputs("Reading error",stderr);
            return false;
        }
        
        // Copy buffer to Chip8 memory
        if((4096-512) > lSize)
        {
            for(int i = 0; i < lSize; ++i)
                _memory[i + 512] = buffer[i];
        }
        else
            printf("Error: ROM too big for memory");
        
        // Close file, free buffer
        fclose(pFile);
        free(buffer);
        
        printf("%s", "ROM loading success");
        
        return true;
    }

    void CPU::EmulateCycle(void)
    {
        _opcode = Fetch(); //Fetch the next opcode
        Execute(); //Execute the opcode
        UpdateTimers(); //Update the delay and audio timer
    }

    unsigned short CPU::Fetch(void) const
    {
        return _memory[_pc] << 8 | _memory[_pc+1]; //Grab two locations in memory and return them to be saved
    }

    void CPU::Execute(void)
    {
        opcodes.Execute(this, _opcode);
    }

    void CPU::UpdateTimers(void)
    {
        if (_delayTimer > 0) --_delayTimer;
        if (_soundTimer > 0)
        {
            //if the sound timer hits one then print beep for now
            if (_soundTimer == 1)
                printf("%s\n", "BEEP!");
            --_soundTimer;
        }
    }
    
    const unsigned char * CPU::GetGfx()
    {
        unsigned char * rtn = new unsigned char[kScreenWidth * kScreenHeight];
        for (int i = 0 ; i < kScreenWidth * kScreenHeight ; i++)
            rtn[i] = _gfx[i];
        _drawFlag = false;
        return rtn;
    }

    void CPU::SetKey(int code)
    {
        _keys[kKeycodes[code]] = 1;
    }
    
    void CPU::ResetKey(int code)
    {
        _keys[kKeycodes[code]] = 0;
    }
}