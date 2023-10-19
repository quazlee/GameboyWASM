#include "RegisterCollection.hxx"

RegisterCollection::RegisterCollection()
{
    data[registerID::B] = 0x00;
    data[registerID::C] = 0x13;
    data[registerID::D] = 0x00;
    data[registerID::E] = 0xD8;
    data[registerID::H] = 0x01;
    data[registerID::L] = 0x4D;
    data[registerID::A] = 0x01;
    data[registerID::F] = 0xB0;
}

RegisterCollection::~RegisterCollection()
{
}

void RegisterCollection::setRegister(int registerIn, unsigned_two_byte value)
{
    data[registerIn] = value;
}

void RegisterCollection::setRegisterDouble(int registerHigh, int registerLow, unsigned_two_byte valueHigh, unsigned_two_byte valueLow)
{
    data[registerHigh] = valueHigh;
    data[registerLow] = valueLow;
}

unsigned_two_byte RegisterCollection::getRegister(int registerIn)
{
    return data[registerIn];
}

unsigned_two_byte RegisterCollection::getRegisterDouble(int registerHigh, int registerLow)
{
    return (data[registerHigh] << 8) | data[registerLow];
}

void RegisterCollection::incRegisterDouble(int registerHigh, int registerLow)
{
    unsigned_two_byte high = data[registerHigh];
    unsigned_two_byte low = data[registerLow];
    unsigned_four_byte combined = (high << 8) | low;
    combined++;
    data[registerHigh] = (combined & 0xFF00) >> 8;
    data[registerLow] = combined & 0x00FF;
}
