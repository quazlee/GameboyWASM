#pragma once

#include "Types.hxx"

class RegisterCollection
{
private:
    unsigned_two_byte data[9];

public:
    RegisterCollection(/* args */);
    ~RegisterCollection();

    void setRegister(int registerIn, unsigned_two_byte value);
    void setRegisterDouble(int registerHigh, int registerLow, unsigned_two_byte valueHigh, unsigned_two_byte valueLow);

    unsigned_two_byte getRegister(int registerIn);
    unsigned_two_byte getRegisterDouble(int registerHigh, int registerLow);

    void addA(unsigned_two_byte targetValue);
    void adcA(unsigned_two_byte targetValue);
    void subA(unsigned_two_byte targetValue);
    void sbcA(unsigned_two_byte targetValue);
    void andA(unsigned_two_byte targetValue);
    void xorA(unsigned_two_byte targetValue);
    void orA(unsigned_two_byte targetValue);
    void cpA(unsigned_two_byte targetValue);
    void addHL(unsigned_two_byte value);
    void incRegister(int registerIn);
    void incRegisterDouble(int registerHigh, int registerLow);
    void decRegister(int registerIn);
    void decRegisterDouble(int registerHigh, int registerLow);
    void rotateRightA();
    void rotateLeftA();
};
