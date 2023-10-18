#include "Cpu.hxx"
#include "Ppu.hxx"
#include "Memory.hxx"

class Gameboy
{
private:
    Cpu* cpu = new Cpu();
    Ppu* ppu = new Ppu();
    Memory* memory = new Memory();
public:
    Gameboy(/* args */);
    ~Gameboy();

    void initialize();
    void mainLoop();
};
