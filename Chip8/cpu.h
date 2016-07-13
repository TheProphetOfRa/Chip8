#pragma once

namespace Chip8
{
    class CPU
    {
    public:
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
        
    private: //Opcodes
        
#pragma mark - Opcodes
        
        void ExecuteZeroCode();
        void JumpToAddr();
        void CallSubAtAddr();
        void SkipInstrIf();
        void SkipInstrIfNot();
        void SkipInstrIfXY();
        void SetX();
        void AddNToX();
        void ExecuteEightCode();
        void NineCodes();
        void SetI();
        void JumpToAddrPlusVZero();
        void SetVXToRandomNumber();
        void RenderSprite();
        void ExecuteECode();
        void ExecuteFCode();
        
#pragma mark - 0 Codes
        
        void ClearScreen();
        void ReturnFromSub();
        
#pragma mark - 8 Codes
        
        void SetVXToVY();
        void SetVXToVXORVY();
        void SetVXToVXANDVY();
        void SetVXToVXXORVY();
        void AddVYToVXAndCarry();
        void SubVYFromVXAndCarry();
        void ShiftVXRight();
        void SubVXFromVY();
        void ShiftVXLeft();
        
#pragma mark - E Codes
        
        void SkipIfKeyPressed();
        void SkipIfKeyNotPressed();
        
#pragma mark - F Codes
        
        void SetVXToDelay();
        void StoreKeyInVX();
        void SetDelayTimerToVX();
        void SetSoundTimerToVX();
        void AddVXToI();
        void SetIToSpriteLocation();
        void StoreVXAtI();
        void StoreContentOfVAtI();
        void FillContentOfVFromI();
        
#pragma mark -
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
    };
}
