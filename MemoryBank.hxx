#pragma once

#include <vector>
#include "Types.hxx"

class MemoryBank
{
private:
    /* data */
    int bankSize;
    int numBanks;
    std::vector<std::vector<unsigned_two_byte>> data;
public:
    MemoryBank(int quantity, int size);
    ~MemoryBank();
    unsigned_two_byte getData(int location);
    void setData(int location, unsigned_two_byte value);
};