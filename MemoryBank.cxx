#include "MemoryBank.hxx"

MemoryBank::MemoryBank()
{
    data = new std::vector<unsigned_two_byte>(size);
}

MemoryBank::~MemoryBank()
{
}

unsigned_two_byte MemoryBank::getData(int location)
{
    return data->at(location);
}

void MemoryBank::setData(int location, unsigned_two_byte value)
{
    data->at(location) = value;
}
