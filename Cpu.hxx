#include <RegisterCollection.hxx>
#include <Ppu.hxx>
#include <Memory.hxx>
#include <Types.hxx>

class Cpu
{
private:
    RegisterCollection *registers = new RegisterCollection();
    Memory *memory = nullptr;
    Ppu *ppu = nullptr;

    unsigned_four_byte programCounter = 0x0100;
    unsigned_four_byte stackPointer = 0xFFFE;
    unsigned_four_byte sysClock = 0xAB00;
    bool halt = false;
    bool ime = 0;

public:
    Cpu(/* args */);
    ~Cpu();

    unsigned_four_byte fetch();
    void execute();
    void interrupt();
    void tickClock();

    void setRegisters(RegisterCollection* value);
    void setMemory(Memory* value);
    void setPpu(Ppu* value);
};