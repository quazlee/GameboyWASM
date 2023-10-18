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

unsigned_two_byte MemoryBank::getData(int location)
{
    return data->at(location);
}

void MemoryBank::setData(int location, unsigned_two_byte value)
{
    data->at(location) = value;
}
