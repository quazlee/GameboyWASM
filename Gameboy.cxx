#include "Gameboy.hxx"
#include <fstream>
#include <vector>

Gameboy::Gameboy()
{
}

Gameboy::~Gameboy()
{
}

void Gameboy::readRom()
{
    std::ifstream stream("Tetris.gb", std::ios::in | std::ios::binary);
    std::vector<unsigned_two_byte> contents((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    memory->initialize(contents);
}

void Gameboy::initialize()
{
    cpu->setMemory(memory);
    cpu->setPpu(ppu);
}

void Gameboy::mainLoop()
{
}
