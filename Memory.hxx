#pragma once

#include "Types.hxx"

class Memory
{
private:
    Cpu* = nullptr;

    int mbcType;
    int numRomBanks;
    bool ramEnabled;
    int bankMode;

    

public:
    Memory(/* args */);
    ~Memory();

    unsigned_two_byte readMemory(unsigned_four_byte address);
    void writeMemory(unsigned_four_byte address, unsigned_two_byte value);
};