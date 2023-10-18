#ifndef MEMORY_H
#define MEMORY_H

#include "Types.hxx"

#include <emscripten/emscripten.h>
#include <emscripten/bind.h>

using namespace emscripten;

class Memory
{
private:
    /* data */
public:
    Memory(/* args */);
    ~Memory();

    unsigned_two_byte readMemory(unsigned_four_byte address);
    void writeMemory(unsigned_four_byte address, unsigned_two_byte value);
};

EMSCRIPTEN_BINDINGS(memory)
{
    class_<Memory>("Memory")
    .constructor()
    .function("readMemory", &Memory::readMemory)
    .function("writeMemory", &Memory::writeMemory)
    ;
}
#endif