#include "Gameboy.hxx"
#include <fstream>
#include <iostream>
#include <vector>
#include <emscripten/val.h>
#include <string>

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

    ppu->setMemory(memory);
}

void Gameboy::mainLoop()
{
    while (!ppu->getIsFrameReady())
    {
        cpu->interrupt();

        // this.debug.logger();

        cpu->execute();
        // this.controls.updateInputState();
    }
}

void Gameboy::drawBackground()
{
}

void Gameboy::setBackgroundSettings(std::string backgroundAddress, std::string tilemapAddress)
{
    unsigned_four_byte backgroundAddressConverted = std::stoi(backgroundAddress, 0, 16);
    unsigned_four_byte tilemapAddressConverted = std::stoi(tilemapAddress, 0, 16);

    ppu->setDebugAddresses(backgroundAddressConverted, tilemapAddressConverted);
}
