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
    const unsigned_four_byte ioStart = 0xFF00;
    // Check if the current scanline is where the top of the window layer is.
    if (currenScanlineTicks == 0)
    {
        unsigned_two_byte ly = memory->readMemory(ioStart + 0x44);
        unsigned_two_byte wy = memory->readMemory(ioStart + 0x4A);
        if (wy == ly)
        {
            bool hasWyEqualedLy = true;
        }
    }
    oamScan();
    currenScanlineTicks += 2;
    if (currenScanlineTicks == 80)
    {
        int fetcherXPos = 0;
        mode = 3;
        int backgroundPixelsRendered = 0;
    }
}

void Ppu::modeThree()
{
    const unsigned_four_byte ioStart = 0xFF00;

    // Check if the window should be displayed instead of the background for the rest of the scanline
    if ((!renderWindow &&
         (lcdc & 0x20) >> 5) == 1 && // LCDC window Enabled Bit
        wy == memory->readMemory(ioStart + 0x44) &&
        backgroundPixelsRendered >= (wx - 7))
    {
        renderWindow = true;
        backgroundFetchStep = 1;
        backgroundFifo.empty();
    }

    switch (backgroundFetchStep)
    {
    // Get Tile
    case 1:
    {
        unsigned_two_byte scy = memory->readMemory(ioStart + 0x42); // display screen top left Y coordinate
        unsigned_two_byte scx = memory->readMemory(ioStart + 0x43); // display screen top left X coordinate

        unsigned_two_byte wy = memory->readMemory(ioStart + 0x4A); // Window top left Y coordinate
        unsigned_two_byte wx = memory->readMemory(ioStart + 0x4B); // Window top left X coordinate

        unsigned_two_byte lcdc = memory->readMemory(ioStart + 0x40); // LCD Control

        unsigned_four_byte tileMapAddress = 0x9800;

        if ((((lcdc & 0x4) >> 3) == 1 && viewportX < wx - 7) || (((lcdc & 0x40) >> 4) == 1 && viewportX >= wx - 7))
        {
            tileMapAddress = 0x9C00;
        }

        backgroundFetchStep = 2;
        break;
    }
    // Get Tile Data Low
    case 2:
        backgroundFetchStep = 3;
        break;
    // Get Tile Data High
    case 3:
        backgroundFetchStep = 4;
        break;
    case 4:
        backgroundFetchStep = 1;
        break;
    }

    // Sleep
    // Push
}

void Ppu::oamScan()
{
    const unsigned_four_byte ioStart = 0xFF00;
    unsigned_two_byte ly = memory->readMemory(0xFF44);
    unsigned_two_byte spriteX = memory->readMemory(currentOamAddress + 1);
    unsigned_two_byte spriteY = memory->readMemory(currentOamAddress);
    if ((spriteX > 0) && (ly + 16 >= spriteY) && (ly + 16 < spriteY + getSpriteHeight()) && (oamBuffer.size() < 10))
    {
        oamBuffer.push_back(Sprite(spriteY,
                                   spriteX,
                                   memory->readMemory(currentOamAddress + 2),
                                   memory->readMemory(currentOamAddress + 3),
                                   currentOamAddress));
    }
    currentOamAddress += 4;
}

int Ppu::getSpriteHeight()
{
    int spriteSize = ((memory->readMemory(0xFF00 + 0x40) & 0x4) >> 2);
    if (spriteSize == 1)
    {
        return 16;
    }
    else
    {
        return 8;
    }
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

/*-----------------------------------------------------------------------------------------------*/

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
