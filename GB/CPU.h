//
//  CPU.hpp
//  GB Emulator
//
//  Created by David Hodgkinson on 30/10/2015.
//  Copyright © 2015 David Hodgkinson. All rights reserved.
//

#ifndef CPU_hpp
#define CPU_hpp

namespace GB
{
    typedef unsigned char BYTE;
    typedef char SIGNED_BYTE;
    typedef unsigned short WORD;
    typedef signed short SIGNED_WORD;
    
    class CPU
    {
    public:
        
        CPU();
        
        void LoadGame(const char* filename);
        
        void Update();
    private:
        void WriteMemory(WORD address, BYTE data);
        
        int ExecuteNextOpcode();
        void UpdateTimers(int cycles);
        void UpdateGraphics(int cycles);
        void DoInterupts();
        
        void Render();
    private:
        
        BYTE _cartridgeMemory[0x200000];
        BYTE _screenData[160][144][3];
        BYTE _rom[0x10000];
        
        union Register
        {
            WORD _reg;
            struct
            {
                BYTE _lo;
                BYTE _hi;
            };
            
            Register(WORD value)
            {
                _hi = value & 0xFF00;
                _lo = value & 0x00FF;
            }
            
            Register(BYTE hi, BYTE lo)
            {
                _hi = hi;
                _lo = lo;
            }
            
            Register& operator=(const WORD& value)
            {
                _hi = value & 0xFF00;
                _lo = value & 0x00FF;
                
                return *this;
            }
        };
        
        Register _regAF;
        Register _regBC;
        Register _regDE;
        Register _regHL;
        
        WORD _pc;
        Register _pStack;
    };
    
}

#endif /* CPU_hpp */
