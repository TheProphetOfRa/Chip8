#include "cpu.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

namespace Chip8
{
    const unsigned char chip8_fontset[80] =
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

    CPU::CPU()
    {
    }

    CPU::~CPU()
    {
    }

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
            _memory[i] = chip8_fontset[i]; //Load the fontset
        
        _drawFlag = false; //Set the draw call to false
        _delayTimer = 0; //Reset the delay timer
        _soundTimer = 0; //Reset the sound timer
    }

    bool CPU::LoadGame(const char *filename)
    {
        printf("Loading: %s\n", filename);
        
        // Open file
		FILE * pFile;
		//pFile = fopen(filename, "rb");
		fopen_s(&pFile, filename, "rb");
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
        switch(_opcode & 0xF000)
        {
        case 0x0000: //Top level catch for all 0x0XXX codes
            switch(_opcode & 0x000F) //Checking for last 4-bits
            {
            case 0x0000: //0x0XX0(0x00E0) Clears the screen
                for (int i = 0 ; i < (64 * 32) ; ++i) _gfx[i] = 0x0;
                _drawFlag = true;
                _pc+=2;
                break;
            case 0x000E: //0x0XXE(0x00EE) Returns from subroutine
                --_pStack;
                _pc = _stack[_pStack];
                _pc+=2;
                break;
            default:
                break;
            }
            break;
        case 0x1000: //1NNN Jump to address NNN
            _pc = _opcode & 0x0FFF;
            break;
        case 0x2000: //2NNN call subroutine at NNN
            //Push the memory location onto the stack for the subroutine return
            _stack[_pStack] = _pc;
            ++_pStack;
            _pc = _opcode & 0x0FFF;
            break;
        case 0x3000: //3XNN Skip next instruction if v[X] == NN
            if (_v[(_opcode & 0x0F00) >> 8] == (_opcode & 0x00FF))
                _pc += 4;
            else
                _pc += 2;
            break;
        case 0x4000: //4XNN Skip next instruction if v[X] != NN
            if (_v[(_opcode & 0x0F00) >> 8] != (_opcode & 0x00FF))
                _pc += 4;
            else
                _pc += 2;
            break;
        case 0x5000: //5XY0 Skip next instruction if v[X] == v[Y]
            if (_v[(_opcode & 0X0F00) >> 8] == _v[(_opcode & 0x00F0) >> 4])
                _pc += 4;
            else
                _pc += 2;
            break;
        case 0x6000: //6XNN Sets v[X] to NN
            _v[(_opcode & 0x0F00) >> 8] = _opcode & 0x00FF;
            _pc+=2;
            break;
        case 0x7000: //7XNN Adds NN to v[X]
            _v[(_opcode & 0x0F00) >> 8] += _opcode & 0x00FF;
            _pc+=2;
            break;
        case 0x8000: //Top level catch for 0x8XXX opcodes
            switch(_opcode & 0x000F)
            {
            case 0x0000: //Set v[X] to the value of v[Y]
                _v[(_opcode & 0x0F00) >> 8] = _v[(_opcode & 0x00F0) >> 4];
                _pc+=2;
                break;
            case 0x0001: //Set v[X] to v[X] OR v[Y]
                _v[(_opcode & 0x0F00) >> 8] |= _v[(_opcode & 0x00F0) >> 4];
                _pc+=2;
                break;
            case 0x0002: //Set v[X] to v[X] AND v[Y]
                _v[(_opcode & 0x0F00) >> 8] &= _v[(_opcode & 0x00F0) >> 4];
                _pc+=2;
                break;
            case 0x0003: //Set v[X] to v[X] XOR v[Y]
                _v[(_opcode & 0x0F00) >> 8] ^= _v[(_opcode & 0x00F0) >> 4];
                _pc+=2;
                break;
            case 0x0004: //Adds v[Y] to v[X] ands sets carry bit if required
                if (_v[(_opcode & 0x00F0) >> 4] > (0xFF - _v[(_opcode & 0x0F00) >> 8]))
                    _v[0xF] = 1;
                else
                    _v[0xF] = 0;
                _v[(_opcode & 0x0F00) >> 8] += _v[(_opcode & 0x00F0) >> 4];
                _pc+=2;
                break;
            case 0x0005: //Subtracts v[Y] from v[X] and sets carry bit if required
                if (_v[(_opcode & 0x00F0) >> 4] > _v[(_opcode & 0x0F00) >> 8])
                    _v[0xF] = 0;
                else
                    _v[0xF] = 1;
                _v[(_opcode & 0x0F00) >> 8] -= _v[(_opcode & 0x00F0) >> 4];
                _pc+=2;
                break;
            case 0x0006: //Shifts v[X] right by one stores the LSB in v[F](before shift)
                _v[0xF] = _v[(_opcode & 0x0F00) >> 8] & 0x1;
                _v[(_opcode & 0x0F00) >> 8] >>= 1;
                _pc+=2;
                break;
            case 0x0007: //v[X] = v[Y] - v[X], v[F] = 0 when borrow required
                if (_v[(_opcode & 0x0F00) >> 8] > _v[(_opcode & 0x00F0) >> 4])
                    _v[0xF] = 0;
                else
                    _v[0X0] = 1;
                _v[(_opcode & 0x0F00) >> 8] = _v[(_opcode & 0x00F0) >> 4] - _v[(_opcode & 0x0F00) >> 8];
                _pc+=2;
                break;
            case 0x000E: //Shift v[X] left by one and set v[F] to MSB before shift
                _v[0xF] = _v[(_opcode & 0x0F00) >> 8] >> 7;
                _v[(_opcode & 0x0F00) >> 8] <<= 1;
                _pc+=2;
                break;
            default:
                printf("\t%s\n", "Unknown 0x8000 domain opcode");
                break;
            }
            break;
        case 0x9000: //Skip next instruction if v[X] != v[Y]
            if (_v[(_opcode & 0x0F00) >> 8] != _v[(_opcode & 0x00F0) >> 4])
                _pc += 4;
            else
                _pc += 2;
            break;
        case 0xA000: //ANNN: Sets I to the address NNN
            _I = _opcode & 0x0FFF;
            _pc += 2;
           break;
        case 0xB000: //Jumps to address NNN + v[0]
           _pc = (_opcode & 0x0FFF) + _v[0];
           break;
        case 0xC000: //Set v[X] to a random number + NN
           _v[(_opcode & 0x0F00) >> 8] = (rand() % 0xFF) & (_opcode & 0x00FF);
           _pc+=2;
           break;
        case 0xD000: //Draw a sprite
           {
                const unsigned short x = _v[(_opcode & 0x0F00) >> 8];
                const unsigned short y = _v[(_opcode & 0x00F0) >> 4];
                const short height = _opcode & 0x000F;
                short pixel;
                _v[0xF] = 0;
                for (int yline = 0 ; yline < height ; ++yline)
                {
                    pixel = _memory[_I + yline];
                    for (int xline = 0 ; xline < 8 ; ++xline)
                    {
                        if ((pixel & (0x80 >> xline)) != 0)
                        {
                            if(_gfx[(x + xline + ((y + yline) * 64))] == 1)
                                _v[0xF] = 1;
                            _gfx[x + xline + ((y + yline) * 64)] ^= 1;
                        }
                    }
                }
                _drawFlag = true;
           }
           _pc+=2;
           break;
        case 0xE000:
           switch(_opcode & 0x00FF)
           {
               //0xEX9E skips the next instruction
               //if the key stored in VX is pressed
            case 0x009E:
                if (_keys[_v[(_opcode & 0x0F00) >> 8]] != 0)
                    _pc += 4;
                else
                    _pc += 2;
                break;
            case 0x00A1: //Skips the next instruction if the key in v[X] isn't pressed
                if (_keys[_v[(_opcode & 0x0F00) >> 8]] == 0)
                    _pc += 4;
                else
                    _pc += 2;
                break;
           default:
               printf("\t%s\n", "Unknown 0xE000 domain opcode");
               break;
           }
           break;
        case 0xF000: //I really don't know
           switch(_opcode & 0x00FF)
           {
            case 0x0007: //Set v[X] to the value of delayTimer
                _v[(_opcode & 0x0F00) >> 8] = _delayTimer;
                _pc+=2;
                break;
            case 0x00A: //Wait for a key press then store it in v[X]
                {
                    bool keyPress = false;
                    for (int i = 0 ; i < 16 ; ++i)
                    {
                        if (_keys[i] != 0)
                        {
                            _v[(_opcode & 0x0F00) >> 8] = i;
                            keyPress = true;
                        }
                    }
                    if (!keyPress) return;
                    _pc += 2;
                }
                break;
            case 0x0015: //Sets the delayTimer to v[X]
                _delayTimer = _v[(_opcode & 0x0F00) >> 8];
                _pc+=2;
                break;
            case 0x0018: //Set the sound timer to v[]
                _soundTimer = _v[(_opcode & 0x0F00) >> 8];
                _pc+=2;
                break;
            case 0x001E: //Adds v[X] to I
                if (_I + _v[(_opcode & 0x0F00) >> 8] > 0xFFF) //Check if we need to set the carry flag
                    _v[0xF] = 1;
                else
                    _v[0xF] = 0;
                _I += _v[(_opcode & 0x0F00) >> 8];
                _pc+=2;
                break;
            case 0x0029: //Sets I to the location of the sprite for the character in v[X].
                _I = _v[(_opcode & 0x0F00) >> 8] * 0.5;
                _pc+=2;
                break;
            case 0x0033: //Stores the binary representation of v[X] at I, I+1 and I+2
                _memory[_I]   = _v[(_opcode & 0x0F00) >> 8] / 100;
                _memory[_I+1] = (_v[(_opcode & 0x0F00) >> 8] / 10) % 10;
                _memory[_I+2] = (_v[(_opcode & 0x0F00) >> 8] % 100) % 10;
                _pc+=2;
                break;
            case 0x0055: //Stores v[0] to v[X] in memory starting at address I
                for (int i = 0 ; i < ((_opcode & 0x0F00) >> 8) ; ++i)
                    _memory[_I+i] = _v[i];
                //The original interpreter set I to I + X + 1
                _I += ((_opcode & 0x0F00) >> 8) + 1;
                _pc+=2;
                break;
            case 0x0065: //Fills v[0] to v[X] with values from memory starting at I
                for (int i = 0 ; i <= ((_opcode & 0x0F00) >> 8) ; ++i)
                    _v[i] = _memory[_I+i];
                //The original interpreter set I to I + X + 1
                _I += ((_opcode & 0x0F00) >> 8) + 1;
                _pc+=2;
                break;
           default:
               printf("\t%s\n", "Unknown 0xF000 domain opcode");
               break;
           }
           break;
        default:
           //Catch any unknown codes
           printf("%s %X\n", "Unknown opcode:", _opcode);
           break;
        }
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

    void CPU::SetKey(int c)
    {
        switch(c)
        {
            case 0:
                _keys[0x1] = 1;
                break;
            case 1:
                _keys[0x2] = 1;
                break;
            case 2:
                _keys[0x3] = 1;
                break;
            case 3:
                _keys[0xc] = 1;
                break;
            case 4:
                _keys[0x4] = 1;
                break;
            case 5:
                _keys[0x5] = 1;
                break;
            case 6:
                _keys[0x6] = 1;
                break;
            case 7:
                _keys[0xd] = 1;
                break;
            case 8:
                _keys[0x7] = 1;
                break;
            case 9:
                _keys[0x8] = 1;
                break;
            case 10:
                _keys[0x9] = 1;
                break;
            case 11:
                _keys[0xe] = 1;
                break;
            case 12:
                _keys[0xa] = 1;
                break;
            case 13:
                _keys[0x0] = 1;
                break;
            case 14:
                _keys[0xb] = 1;
                break;
            case 15:
                _keys[0xf] = 1;
                break;
            default:
                break;
        }
    }
    
    void CPU::ResetKey(int code)
    {
        switch(code)
        {
            case 0:
                _keys[0x1] = 0;
                break;
            case 1:
                _keys[0x2] = 0;
                break;
            case 2:
                _keys[0x3] = 0;
                break;
            case 3:
                _keys[0xc] = 0;
                break;
            case 4:
                _keys[0x4] = 0;
                break;
            case 5:
                _keys[0x5] = 0;
                break;
            case 6:
                _keys[0x6] = 0;
                break;
            case 7:
                _keys[0xd] = 0;
                break;
            case 8:
                _keys[0x7] = 0;
                break;
            case 9:
                _keys[0x8] = 0;
                break;
            case 10:
                _keys[0x9] = 0;
                break;
            case 11:
                _keys[0xe] = 0;
                break;
            case 12:
                _keys[0xa] = 0;
                break;
            case 13:
                _keys[0x0] = 0;
                break;
            case 14:
                _keys[0xb] = 0;
                break;
            case 15:
                _keys[0xf] = 0;
                break;
            default:
                break;
        }
    }
}