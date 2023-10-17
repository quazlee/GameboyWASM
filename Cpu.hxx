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

    four_byte programCounter = 0x0100;
    four_byte stackPointer = 0xFFFE;
    four_byte sysClock = 0xAB00;
    bool halt = false;
    bool ime = 0;

public:
    Cpu(/* args */);
    ~Cpu();

    two_byte fetch();
    void decode();
    void execute();
    void interrupt();
    void tickClock();

    void setRegisters(RegisterCollection* value);
    void setMemory(Memory* value);
    void setPpu(Ppu* value);
};