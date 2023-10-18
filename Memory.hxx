#ifndef MEMORY_H
#define MEMORY_H

#include "Types.hxx"
class Memory
{
private:
    /* data */
public:
    Memory(/* args */);
    ~Memory();

    unsigned_two_byte readMemory(unsigned_four_byte address);
    void writeMemory(unsigned_four_byte address, unsigned_two_byte value);
};

#endif