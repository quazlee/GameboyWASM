#pragma once

typedef short four_byte;
typedef unsigned short unsigned_four_byte;
typedef char two_byte;
typedef unsigned char unsigned_two_byte;

// class unsigned_four_byte
// {
// private:
//     unsigned short value = 0;

// public:
//     unsigned_four_byte(unsigned short value)
//     {
//         this->value = value;
//     }

//     unsigned_four_byte(unsigned char high, unsigned char low)
//     {
//         this->value = (high << 8) | low;
//     }

//     unsigned_four_byte operator=(unsigned_four_byte const& value)
//     {
//         this->value = value;
//     }

//     unsigned_four_byte operator<(unsigned short const& value)
//     {
//         return this->value < value;
//     }

//     unsigned_four_byte operator>(unsigned short const& value)
//     {
//         return this->value > value;
//     }

//     unsigned_four_byte operator&(unsigned short const& value)
//     {
//         return this->value & value;
//     }
// }

// class unsigned_two_byte
// {
// private:
//     unsigned char value = 0;

// public:
//     unsigned_two_byte(unsigned char value)
//     {
//         this->value = value;
//     }

    

// }

enum registerID {
    B,
    C,
    D,
    E,
    H,
    L,
    HL,
    A,
    F
};