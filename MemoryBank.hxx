#ifndef MEMORYBANK_H
#define MEMORYBANK_H

#include <vector>
#include "Types.hxx"

class MemoryBank
{
private:
    /* data */
    int size;
    std::vector<unsigned_two_byte>* data;
public:
    MemoryBank(/* args */);
    ~MemoryBank();
    unsigned_two_byte getData(int location);
    void setData(int location, unsigned_two_byte value);
};
#endif