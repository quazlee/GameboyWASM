#pragma once

#include "Types.hxx"
#include "MemoryBank.hxx"

class Memory
{
private:
    Cpu * = nullptr;

    int mbcType;
    int numRomBanks;
    bool ramEnabled;
    int bankMode;

    MemoryBank *rom = nullptr;

    /** 0x8000-0x9FFF */
    MemoryBank vram(1, 8192);

    /** 0xA000-0xBFFF */
    MemoryBank *ram = nullptr;

    /** 0xC000-0xDFFF */
    MemoryBank *vram = nullptr;

    /** 0xE000-0xFDFF
     * ECHO RAM SO IGNORE IT
     */
    MemoryBank echoram(1, 7680);

    /** 0xFE00-0xFE9F */
    MemoryBank vram(1, 160);

    /** 0xFEA0-0xFEFF
     * NOT USABLE SO IGNORE IT
     */
    MemoryBank prohibited(1, 160);

    /** 0xFF00-0xFF7F */
    MemoryBank io(1, 128);

    /** 0xFF80-0xFFFE */
    MemoryBank hram(1, 127);

    /** 0xFFFF */
    bool ie = false;

public:
    Memory(/* args */);
    ~Memory();

    void initialize();

    unsigned_two_byte readMemory(unsigned_four_byte address);
    void writeMemory(unsigned_four_byte address, unsigned_two_byte value);

    void writeMemoryMbcZero(unsigned_four_byte address, unsigned_two_byte value);
};