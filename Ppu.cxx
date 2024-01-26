#include "Ppu.hxx"
#include "Memory.hxx"
#include <vector>
#include <iostream>
#include <algorithm>


Ppu::Ppu()
{

}

Ppu::~Ppu()
{
}

bool Ppu::getIsFrameReady()
{
    return isFrameReady;
}

void Ppu::setIsFrameReady(bool isReady)
{
    isFrameReady = isReady;
}

void Ppu::cycle()
{
    mode = 1;
    if (mode == 2)
    {
        modeTwo();
    }
    else if (mode == 3)
    {
        modeThree();
    }
    else if (mode == 0)
    {
        modeZero();
    }
    else if (mode == 1)
    {
        modeOne();
    }
}

void Ppu::setMemory(Memory *value)
{
    memory = value;
}

void Ppu::modeZero()
{
}

// Vblank Period. Scanlines 144-153
void Ppu::modeOne()
{
    const unsigned_four_byte ioStart = 0xFF00;

    currenScanlineTicks += 2;
    if (currenScanlineTicks == 456 && memory->readMemory(ioStart + 0x44) < 153)
    {
        currenScanlineTicks = 0;
        memory->writeMemory(ioStart + 0x44, memory->readMemory(ioStart + 0x44) + 1);
    }
    else if (currenScanlineTicks == 456 && memory->readMemory(ioStart + 0x44) == 153)
    {
        currenScanlineTicks = 0;
        mode = 2;
        memory->writeMemory(ioStart + 0x44, 0);
        setIsFrameReady(true);
        // hasWyEqualedLy = false;
        // renderWindow = false;
    }
}

void Ppu::modeTwo()
{
}

void Ppu::modeThree()
{
}

int Ppu::getTileMapIndex(int *tilemap, int x, int y)
{
    return tilemap[x + (y * 256)];
}

void Ppu::setTileMapIndex(int *tilemap, int x, int y, int value)
{
    tilemap[x + (y * 256)] = value;
}

std::vector<int> Ppu::decodeTile(std::vector<unsigned_two_byte> input)
{
    std::vector<int> tileA;
    std::vector<int> tileB;
    for (int i = 0; i < 16; i++)
    {
        if (i % 2 == 0)
        {
            tileA.push_back(input[i]);
        }
        else
        {
            tileB.push_back(input[i]);
        }
    }
    std::vector<int> generatedTile;

    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            int low = (tileA[y] & (1 << (7 - x))) >> (7 - x);
            int high = (tileB[y] & (1 << (7 - x))) >> (7 - x);
            generatedTile.push_back((high << 1) | low);
        }
    }
    return generatedTile;
}

void Ppu::setDebugAddresses(unsigned_four_byte background, unsigned_four_byte tilemap)
{
    backgroundDebugAddress = background;
    tilemapDebugAddress = tilemap;
}

std::vector<int> Ppu::populateBackgroundWindowMaps()
{
    std::vector<int> output;
    for (int y = 0; y < 32; y++)
    {
        for (int x = 0; x < 32; x++)
        {
            int tileNumber;
            if (backgroundDebugAddress == 0x8000)
            {
                tileNumber = memory->readMemory(tilemapDebugAddress + (x) + (y * 32));
                if(tileNumber != 0 && tileNumber != 32){
                    std::cout << tileNumber << std::endl;
                }
            }
            else
            {
                tileNumber = static_cast<signed_two_byte>(memory->readMemory(tilemapDebugAddress + (x) + (y * 32)));
            }
            std::vector<unsigned_two_byte> tileSet;
            for (int i = 0; i < 16; i++)
            {
                tileSet.push_back(memory->readMemory(backgroundDebugAddress + (tileNumber * 16) + i));
            }
            std::vector<int> decodedTile = decodeTile(tileSet);
            for (int i = 0; i < 64; i++)
            {
                if(decodedTile[i] != 0){
                    std::cout << decodedTile[i] << std::endl;
                }
                output.push_back(decodedTile[i]);
            }
        }
    }
    std::reverse(output.begin(), output.end());
    return output;
}

std::vector<int> Ppu::populateTileMap()
{
    std::vector<int> output;
    for (int y = 0; y < 24; y++)
    {
        for (int x = 0; x < 16; x++)
        {
            unsigned_four_byte base = 0x8000 + (x * 16) + (y * 192);
            std::vector<unsigned_two_byte> tileSet;
            for (int i = 0; i < 16; i++)
            {
                tileSet.push_back(memory->readMemory(base + i));
            }
            std::vector<int> decodedTile = decodeTile(tileSet);
            for (int i = 0; i < 64; i++)
            {
                output.push_back(decodedTile[i]);
            }
            
        }
    }
    std::reverse(output.begin(), output.end());
    return output;
}
