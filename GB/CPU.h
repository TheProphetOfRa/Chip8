//
//  CPU.h
//  GB Emulator
//
//  Created by David Hodgkinson on 30/10/2015.
//  Copyright © 2015 David Hodgkinson. All rights reserved.
//

#pragma once

#include <stdint.h>

namespace GB
{
    class CPU
    {
    public:
        CPU();
        ~CPU();
        
        bool Init();
        
        bool LoadGame(const char* filename);
        
        void EmulateCycle();
        
    private:
        
        
        
    private:

        struct Registers
        {
            struct
            {
                union
                {
                    struct
                    {
                        uint8_t _f;
                        uint8_t _a;
                    };
                    
                    uint16_t _af;
                };
            };
            
            struct
            {
                union
                {
                    uint8_t _c;
                    uint8_t _b;
                };
                
                uint16_t _bc;
            };
            
            struct
            {
                union
                {
                    struct
                    {
                        uint8_t _e;
                        uint8_t _d;
                    };
                    uint16_t _de;
                };
            };
            
            struct
            {
                union
                {
                    struct
                    {
                        uint8_t _l;
                        uint8_t _h;
                    };
                    uint16_t _hl;
                };
            };
            
            uint16_t _pStack;   //Stack Pointer
            uint16_t _pc;       //Program Counter
            
        } _registers;
        
      
        unsigned char _cartridgeMemory;
        
    };
}

