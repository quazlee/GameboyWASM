#include "MemoryBank.hxx"

MemoryBank::MemoryBank(int quantity, int size)
{
    data = new std::vector<std::vector<unsigned_two_byte>*>(quantity);
    for (int i = 0; i < quantity; i++)
    {
        data->at(i) = new std::vector<unsigned_two_byte>(size);
    }
}

MemoryBank::~MemoryBank()
{
}

unsigned_two_byte MemoryBank::getData(int bank, int location)
{
    return data->at(bank)->at(location);
}

void MemoryBank::setData(int bank, int location, unsigned_two_byte value)
{
    data->at(bank)->at(location) = value;
}
