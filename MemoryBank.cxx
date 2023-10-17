#include "MemoryBank.hxx"

unsigned_two_byte MemoryBank::getData(int location)
{
    return data[location];
}

void MemoryBank::setData(int location, unsigned_two_byte value)
{
    data[location] = value;
}
