#include "MemoryBank.hxx"

MemoryBank::MemoryBank(int quantity, int size)
{
    for (int i = 0; i < quantity; i++)
    {
        for (int j = 0; j < size; j++)
        {
            data[i][j] = 0;
        }
    }
}

MemoryBank::~MemoryBank()
{
}

unsigned_two_byte MemoryBank::getData(int bank, int location)
{
    return data[bank][location];
}

void MemoryBank::setData(int bank, int location, unsigned_two_byte value)
{
    data[bank][location] = value;
}
