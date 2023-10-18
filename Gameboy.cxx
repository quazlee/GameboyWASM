#include "Gameboy.hxx"

void Gameboy::initialize()
{
    cpu->setMemory(memory);
    cpu->setPpu(ppu);
}