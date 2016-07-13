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
        switch (_opcode & 0xF000)
        {
            case 0x0000: ExecuteZeroCode(); break;
            case 0x1000: JumpToAddr(); break;
            case 0x2000: CallSubAtAddr(); break;
            case 0x3000: SkipInstrIf(); break;
            case 0x4000: SkipInstrIfNot(); break;
            case 0x5000: SkipInstrIfXY(); break;
            case 0x6000: SetX(); break;
            case 0x7000: AddNToX(); break;
            case 0x8000: ExecuteEightCode(); break;
            case 0x9000: NineCodes(); break;
            case 0xA000: SetI(); break;
            case 0xB000: JumpToAddrPlusVZero(); break;
            case 0xC000: SetVXToRandomNumber(); break;
            case 0xD000: RenderSprite(); break;
            case 0xE000: ExecuteECode(); break;
            case 0xF000: ExecuteFCode(); break;
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

    void CPU::SetKey(int code)
    {
        _keys[kKeycodes[code]] = 1;
    }
    
    void CPU::ResetKey(int code)
    {
        _keys[kKeycodes[code]] = 0;
    }
    
#pragma mark - Opcodes
    
    void CPU::ExecuteZeroCode()
    {
        switch (_opcode & 0x000F)
        {
            case 0x0000: ClearScreen(); break;
            case 0x000E: ReturnFromSub(); break;
        }
    }
    
    //1NNN Jump to address NNN
    void CPU::JumpToAddr()
    {
        _pc = _opcode & 0x0FFF;
    }
    
    //2NNN call subroutine at NNN
    void CPU::CallSubAtAddr()
    {
        //Push the memory location onto the stack for the subroutine return
        _stack[_pStack] = _pc;
        ++_pStack;
        _pc = _opcode & 0x0FFF;
    }
    
    //3XNN Skip next instruction if v[X] == NN
    void CPU::SkipInstrIf()
    {
        if (_v[(_opcode & 0x0F00) >> 8] == (_opcode & 0x00FF))
            _pc += 4;
        else
            _pc += 2;
    }
    
    //4XNN Skip next instruction if v[X] != NN
    void CPU::SkipInstrIfNot()
    {
        if (_v[(_opcode & 0x0F00) >> 8] != (_opcode & 0x00FF))
            _pc += 4;
        else
            _pc += 2;
    }
    
    //5XY0 Skip next instruction if v[X] == v[Y]
    void CPU::SkipInstrIfXY()
    {
        if (_v[(_opcode & 0X0F00) >> 8] == _v[(_opcode & 0x00F0) >> 4])
            _pc += 4;
        else
            _pc += 2;
    }
    
    //6XNN Sets v[X] to NN
    void CPU::SetX()
    {
        _v[(_opcode & 0x0F00) >> 8] = _opcode & 0x00FF;
        _pc+=2;
    }
    
    //7XNN Adds NN to v[X]
    void CPU::AddNToX()
    {
        _v[(_opcode & 0x0F00) >> 8] += _opcode & 0x00FF;
        _pc+=2;
    }
    
    void CPU::ExecuteEightCode()
    {
        switch (_opcode & 0x000F)
        {
            case 0x0000: SetVXToVY(); break;
            case 0x0001: SetVXToVXXORVY(); break;
            case 0x0002: SetVXToVXANDVY(); break;
            case 0x0003: SetVXToVXXORVY(); break;
            case 0x0004: AddVYToVXAndCarry(); break;
            case 0x0005: SubVYFromVXAndCarry(); break;
            case 0x0006: ShiftVXRight(); break;
            case 0x0007: SubVXFromVY(); break;
            case 0x000E: ShiftVXLeft(); break;
        }
    }
    
    void CPU::NineCodes()
    {
        if (_v[(_opcode & 0x0F00) >> 8] != _v[(_opcode & 0x00F0) >> 4])
            _pc += 4;
        else
            _pc += 2;
    }
    
    //Set I to the adress 0x0NNN
    void CPU::SetI()
    {
        _I = _opcode & 0x0FFF;
        _pc += 2;
    }
    
    //Jump to address at 0x0NNN + v[0]
    void CPU::JumpToAddrPlusVZero()
    {
        _pc = (_opcode & 0x0FFF) + _v[0];
    }
    
    //Set v[x] to random number & 0x00NN
    void CPU::SetVXToRandomNumber()
    {
        _v[(_opcode & 0x0F00) >> 8] = (rand() % 0xFF) & (_opcode & 0x00FF);
        _pc+=2;
    }
    
    //Draws sprite at coords (v[x], v[y]) with width 8 and height N
    void CPU::RenderSprite()
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
        _pc+=2;
    }
    
    void CPU::ExecuteECode()
    {
        switch (_opcode & 0x00FF)
        {
            case 0x009E: SkipIfKeyPressed(); break;
            case 0x00A1: SkipIfKeyNotPressed(); break;
        }
    }
    
    void CPU::ExecuteFCode()
    {
        switch (_opcode & 0x00FF)
        {
            case 0x0007: SetVXToDelay(); break;
            case 0x000A: StoreKeyInVX(); break;
            case 0x0015: SetDelayTimerToVX(); break;
            case 0x0018: SetSoundTimerToVX(); break;
            case 0x001E: AddVXToI(); break;
            case 0x0029: SetIToSpriteLocation(); break;
            case 0x0033: StoreVXAtI(); break;
            case 0x0055: StoreContentOfVAtI(); break;
            case 0x0065: FillContentOfVFromI(); break;
        }
    }
    
#pragma mark - 0 Codes
    
    void CPU::ClearScreen()
    {
        for (int i = 0 ; i < (64 * 32) ; ++i) _gfx[i] = 0x0;
        _drawFlag = true;
        _pc+=2;
    }
    
    void CPU::ReturnFromSub()
    {
        --_pStack;
        _pc = _stack[_pStack];
        _pc += 2;
    }
    
#pragma mark - 8 Codes
    
    //Set v[X] to the value of v[Y]
    void CPU::SetVXToVY()
    {
        _v[(_opcode & 0x0F00) >> 8] = _v[(_opcode & 0x00F0) >> 4];
        _pc+=2;
    }
    
    //Set v[X] to v[X] OR v[Y]
    void CPU::SetVXToVXORVY()
    {
        _v[(_opcode & 0x0F00) >> 8] |= _v[(_opcode & 0x00F0) >> 4];
        _pc+=2;
    }
    
    //Set v[X] to v[X] AND v[Y]
    void CPU::SetVXToVXANDVY()
    {
        _v[(_opcode & 0x0F00) >> 8] &= _v[(_opcode & 0x00F0) >> 4];
        _pc+=2;
    }
    
    //Set v[X] to v[X] XOR v[Y]
    void CPU::SetVXToVXXORVY()
    {
        _v[(_opcode & 0x0F00) >> 8] ^= _v[(_opcode & 0x00F0) >> 4];
        _pc+=2;
    }
    
    //Adds v[Y] to v[X] ands sets carry bit if required
    void CPU::AddVYToVXAndCarry()
    {
        if (_v[(_opcode & 0x00F0) >> 4] > (0xFF - _v[(_opcode & 0x0F00) >> 8]))
            _v[0xF] = 1;
        else
            _v[0xF] = 0;
        _v[(_opcode & 0x0F00) >> 8] += _v[(_opcode & 0x00F0) >> 4];
        _pc+=2;
    }
    
    //Subtracts v[Y] from v[X] and sets carry bit if required
    void CPU::SubVYFromVXAndCarry()
    {
        if (_v[(_opcode & 0x00F0) >> 4] > _v[(_opcode & 0x0F00) >> 8])
            _v[0xF] = 0;
        else
            _v[0xF] = 1;
        _v[(_opcode & 0x0F00) >> 8] -= _v[(_opcode & 0x00F0) >> 4];
        _pc+=2;
    }
    
    //Shifts v[X] right by one stores the LSB in v[F](before shift)
    void CPU::ShiftVXRight()
    {
        _v[0xF] = _v[(_opcode & 0x0F00) >> 8] & 0x1;
        _v[(_opcode & 0x0F00) >> 8] >>= 1;
        _pc+=2;
    }
    
    //v[X] = v[Y] - v[X], v[F] = 0 when borrow required
    void CPU::SubVXFromVY()
    {
        if (_v[(_opcode & 0x0F00) >> 8] > _v[(_opcode & 0x00F0) >> 4])
            _v[0xF] = 0;
        else
            _v[0X0] = 1;
        _v[(_opcode & 0x0F00) >> 8] = _v[(_opcode & 0x00F0) >> 4] - _v[(_opcode & 0x0F00) >> 8];
        _pc+=2;
    }
    
    //Shift v[X] left by one and set v[F] to MSB before shift
    void CPU::ShiftVXLeft()
    {
        _v[0xF] = _v[(_opcode & 0x0F00) >> 8] >> 7;
        _v[(_opcode & 0x0F00) >> 8] <<= 1;
        _pc+=2;
    }
    
#pragma mark - E Codes
    
    //0xEX9E skips the next instruction
    //if the key stored in VX is pressed
    void CPU::SkipIfKeyPressed()
    {
        if (_keys[_v[(_opcode & 0x0F00) >> 8]] != 0)
            _pc += 4;
        else
            _pc += 2;
    }
    
    //Skips the next instruction if the key in v[X] isn't pressed
    void CPU::SkipIfKeyNotPressed()
    {
        if (_keys[_v[(_opcode & 0x0F00) >> 8]] == 0)
            _pc += 4;
        else
            _pc += 2;
    }
    
#pragma mark - F Codes
    
    //Set v[X] to the value of delayTimer
    void CPU::SetVXToDelay()
    {
        _v[(_opcode & 0x0F00) >> 8] = _delayTimer;
        _pc+=2;
    }
    
    //Wait for a key press then store it in v[X]
    void CPU::StoreKeyInVX()
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
    
    //Sets the delayTimer to v[X]
    void CPU::SetDelayTimerToVX()
    {
        _delayTimer = _v[(_opcode & 0x0F00) >> 8];
        _pc+=2;
    }
    
    //Set the sound timer to v[]
    void CPU::SetSoundTimerToVX()
    {
        _soundTimer = _v[(_opcode & 0x0F00) >> 8];
        _pc+=2;
    }
    
    //Adds v[X] to I
    void CPU::AddVXToI()
    {
        if (_I + _v[(_opcode & 0x0F00) >> 8] > 0xFFF) //Check if we need to set the carry flag
            _v[0xF] = 1;
        else
            _v[0xF] = 0;
        _I += _v[(_opcode & 0x0F00) >> 8];
        _pc+=2;
    }
    
    //Sets I to the location of the sprite for the character in v[X].
    void CPU::SetIToSpriteLocation()
    {
        _I = _v[(_opcode & 0x0F00) >> 8] * 0.5;
        _pc+=2;
    }
    
    //Stores the binary representation of v[X] at I, I+1 and I+2
    void CPU::StoreVXAtI()
    {
        _memory[_I]   = _v[(_opcode & 0x0F00) >> 8] / 100;
        _memory[_I+1] = (_v[(_opcode & 0x0F00) >> 8] / 10) % 10;
        _memory[_I+2] = (_v[(_opcode & 0x0F00) >> 8] % 100) % 10;
        _pc+=2;
    }
    
    //Stores v[0] to v[X] in memory starting at address I
    void CPU::StoreContentOfVAtI()
    {
        for (int i = 0 ; i < ((_opcode & 0x0F00) >> 8) ; ++i)
            _memory[_I+i] = _v[i];
        //The original interpreter set I to I + X + 1
        _I += ((_opcode & 0x0F00) >> 8) + 1;
        _pc+=2;
    }
    
    //Fills v[0] to v[X] with values from memory starting at I
    void CPU::FillContentOfVFromI()
    {
        for (int i = 0 ; i <= ((_opcode & 0x0F00) >> 8) ; ++i)
            _v[i] = _memory[_I+i];
        //The original interpreter set I to I + X + 1
        _I += ((_opcode & 0x0F00) >> 8) + 1;
        _pc+=2;
    }
    
#pragma mark -
    
}