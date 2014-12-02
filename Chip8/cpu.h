
namespace Chip8
{
    class CPU
    {
    public:
        
        const static int kScreenWidth = 64;
        const static int kScreenHeight = 32;
        
        CPU();
        ~CPU();

        void init(void);
        bool loadGame(const char *filename);

        void emulateCycle(void);

        bool shouldDraw() const {return drawFlag;}

        const unsigned char * getGfx();

        void setKey(int code);
        void resetKey(int code);
    private:
        unsigned short fetch(void) const;
        void execute(void);

        void updateTimers(void);
    private:
        unsigned short opcode;
        unsigned char memory[4096];
        unsigned char v[16];
        unsigned short I;
        unsigned short pc;
        unsigned char gfx[64 * 32];
        unsigned char delayTimer;
        unsigned char soundTimer;
        unsigned short stack[16];
        unsigned short pStack;
        unsigned char keys[16];

        bool drawFlag;
    };
}
