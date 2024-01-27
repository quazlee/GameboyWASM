#include "Gameboy.hxx"
#include <fstream>
#include <iostream>
#include <vector>
#include <emscripten/val.h>
#include <string>
#include <emscripten.h>
#include "Types.hxx"
#include <sstream>
using emscripten::val;
using namespace emscripten;

#define formatter2 std::uppercase << std::setfill('0') << std::setw(2) << std::hex
#define formatter4 std::uppercase << std::setfill('0') << std::setw(4) << std::hex

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
    // emptyDebugString();
    // debugStringLength = 0;
    while (!ppu->getIsFrameReady())
    {
        cpu->interrupt();

        // getNextInstructionDebugLine();
        cpu->execute();
        // this.controls.updateInputState();
    }
    ppu->setIsFrameReady(false);
}

uintptr_t Gameboy::getBackground()
{
    delete[] debugBackgroundMap;
    std::vector<int> data = ppu->populateBackgroundWindowMaps();
    debugBackgroundMap = new int[65536];
    std::copy(data.begin(), data.end(), debugBackgroundMap);
    return uintptr_t(debugBackgroundMap);
}

uintptr_t Gameboy::getTileMap()
{
    delete[] debugTileMap;
    std::vector<int> data = ppu->populateTileMap();
    debugTileMap = new int[24576];
    std::copy(data.begin(), data.end(), debugTileMap);
    return uintptr_t(debugTileMap);
}

void Gameboy::setBackgroundSettings(std::string backgroundAddress, std::string tilemapAddress)
{
    unsigned_four_byte backgroundAddressConverted = std::stoi(backgroundAddress, 0, 16);
    unsigned_four_byte tilemapAddressConverted = std::stoi(tilemapAddress, 0, 16);

    ppu->setDebugAddresses(backgroundAddressConverted, tilemapAddressConverted);
}

void Gameboy::getNextInstructionDebugLine()
{
    std::stringstream debugString;
    debugString << "A: " << formatter2 << static_cast<int>(cpu->getRegisters()->getRegister(registerID::A)) << " "
                << "F: " << formatter2 << static_cast<int>(cpu->getRegisters()->getRegister(registerID::F)) << " "
                << "B: " << formatter2 << static_cast<int>(cpu->getRegisters()->getRegister(registerID::B)) << " "
                << "C: " << formatter2 << static_cast<int>(cpu->getRegisters()->getRegister(registerID::C)) << " "
                << "D: " << formatter2 << static_cast<int>(cpu->getRegisters()->getRegister(registerID::D)) << " "
                << "E: " << formatter2 << static_cast<int>(cpu->getRegisters()->getRegister(registerID::E)) << " "
                << "H: " << formatter2 << static_cast<int>(cpu->getRegisters()->getRegister(registerID::H)) << " "
                << "L: " << formatter2 << static_cast<int>(cpu->getRegisters()->getRegister(registerID::L)) << " "
                << "SP: " << formatter4 << static_cast<int>(cpu->getStackPointer()) << " "
                << "PC: 00:" << formatter4 << static_cast<int>(cpu->getProgramCounter()) << " "
                << "(" << formatter2 << static_cast<int>(memory->readMemory(cpu->getProgramCounter())) << " "
                << formatter2 << static_cast<int>(memory->readMemory(cpu->getProgramCounter() + 1)) << " "
                << formatter2 << static_cast<int>(memory->readMemory(cpu->getProgramCounter() + 2)) << " "
                << formatter2 << static_cast<int>(memory->readMemory(cpu->getProgramCounter() + 3)) << ")"
                << std::endl;

    std::string oldString = std::string(debugStringFull);
    delete[] debugStringFull;
    std::string currentString = debugString.str();
    debugStringLength += currentString.length();
    std::string newString = oldString + currentString;
    debugStringFull = new char[newString.length() + 1];
    strcpy(debugStringFull, newString.c_str());
}

uintptr_t Gameboy::getDebugStringFull()
{
    return uintptr_t(debugStringFull);
}

void Gameboy::emptyDebugString()
{
    delete[] debugStringFull;
    debugStringFull = new char[0];
}

int Gameboy::getDebugStringLength()
{
    return debugStringLength;
}
