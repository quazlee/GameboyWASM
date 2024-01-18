#include "Gameboy.hxx"
#include <fstream>
#include <iostream>
#include <vector>
#include <emscripten/val.h>

Gameboy::Gameboy()
{
}

Gameboy::~Gameboy()
{
}

void Gameboy::readRom(uintptr_t arrayBuffer, int size)
{
    auto romContents = reinterpret_cast<unsigned_two_byte *>(arrayBuffer);
    memory->initialize(romContents, size);
}

void Gameboy::initialize()
{
    cpu->setMemory(memory);
    cpu->setPpu(ppu);
}

void Gameboy::mainLoop()
{
    while (!ppu->getIsFrameReady())
    {
        // this.cpu.interrupt();

        // this.debug.logger();

        cpu->execute();
        // this.controls.updateInputState();
    }
}
