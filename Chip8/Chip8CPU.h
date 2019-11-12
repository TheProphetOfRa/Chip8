#pragma once

namespace Chip8
{
    class Chip8CPU
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
        void Jump();
        void Call();
        void IfEqual();
        void IfNotEqual();
        void IfEqualRegister();
        void Assign();
        void Add();
        void ExecuteEightCode();
        void IfNotEqualRegister();
        void SetAddress();
        void JumpAddress();
        void Rand();
        void RenderSprite();
        void ExecuteECode();
        void ExecuteFCode();
        
#pragma mark - 0 Codes
        
        void ClearScreen();
        void ReturnFromSub();
        
#pragma mark - 8 Codes
        
        void AssignRegister();
        void Or();
        void And();
        void Xor();
        void AddRegister();
        void SubtractRegister();
        void ShiftRight();
        void Subtract();
        void ShiftLeft();
        
#pragma mark - E Codes
        
        void KeyPressed();
        void KeyNotPressed();
        
#pragma mark - F Codes
        
        void GetDelayTimer();
        void GetKeyPressed();
        void SetDelayTimer();
        void SetSoundTimer();
        void AddAddress();
        void HexSprite();
        void StoreBCD();
        void SaveRegisters();
        void LoadRegisters();
        
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
