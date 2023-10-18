#include "Gameboy.hxx"

Gameboy::Gameboy()
{
}

Gameboy::~Gameboy()
{
}

void Gameboy::initialize()
{
    cpu->setMemory(memory);
    cpu->setPpu(ppu);
}

void Gameboy::mainLoop()
{
}
