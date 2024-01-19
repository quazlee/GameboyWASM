#include "MemoryBank.hxx"

MemoryBank::MemoryBank(int numberOfBanks, int size)
{
    numBanks = numberOfBanks;
    bankSize = size;

    data = new std::vector<unsigned_two_byte>(numBanks * size);

    // data = new std::vector<unsigned_two_byte>(quantity);
    // for (int i = 0; i < quantity; i++)
    // {
    //     data->at(i) = new std::vector<unsigned_two_byte>(size);
    // }
}

MemoryBank::~MemoryBank()
{
}

unsigned_two_byte MemoryBank::getData(int bank, int location)
{
    return data->at(location + (bank * bankSize));
}

void MemoryBank::setData(int bank, int location, unsigned_two_byte value)
{
    data->at(location + (bank * bankSize)) = value;
}
