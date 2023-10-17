#include <Types.hxx>

class Memory
{
private:
    /* data */
public:
    Memory(/* args */);
    ~Memory();

    two_byte readMemory(four_byte address);
    void writeMemory(four_byte address, two_byte value);
};