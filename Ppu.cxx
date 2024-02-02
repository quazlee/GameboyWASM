#include "Ppu.hxx"
#include "Memory.hxx"
#include <vector>
#include <queue>
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
    currentScanlineTicks++;
    if (currentScanlineTicks == 456 && memory->readMemory(0xFF44) < 143)
    {
        mode = 2;
        currentScanlineTicks = 0;
        memory->writeMemory(0xFF44, memory->readMemory(0xFF44) + 1);
        currentOamAddress = 0xFE00;
        oamFifo = {};
        backgroundFifo = {};
        backgroundFetchStep = 1;
        pixelsPushed = 0;
        viewportXTile = 0;
    }
    else if (currentScanlineTicks == 456 && memory->readMemory(0xFF44) == 143)
    {
        mode = 1;
        currentScanlineTicks = 0;
        memory->writeMemory(0xFF44, memory->readMemory(0xFF44) + 1);
        memory->writeMemory(0xFF0F, memory->readMemory(0xFF0F) | 1);
        currentOamAddress = 0xFE00;
        oamFifo = {};
        backgroundFifo = {};
        backgroundFetchStep = 1;
        pixelsPushed = 0;
        viewportXTile = 0;
    }
}

// Vblank Period. Scanlines 144-153
void Ppu::modeOne()
{
    const unsigned_four_byte ioStart = 0xFF00;

    currentScanlineTicks += 2;
    if (currentScanlineTicks == 456 && memory->readMemory(ioStart + 0x44) < 153)
    {
        currentScanlineTicks = 0;
        memory->writeMemory(ioStart + 0x44, memory->readMemory(ioStart + 0x44) + 1);
    }
    else if (currentScanlineTicks == 456 && memory->readMemory(ioStart + 0x44) == 153)
    {
        currentScanlineTicks = 0;
        mode = 2;
        memory->writeMemory(ioStart + 0x44, 0);
        setIsFrameReady(true);
        // hasWyEqualedLy = false;
        // renderWindow = false;
    }
}

void Ppu::modeTwo()
{
    oamScan();
    currentScanlineTicks += 2;
    if (currentScanlineTicks == 80)
    {
        int fetcherXPos = 0;
        mode = 3;
        int backgroundPixelsRendered = 0;
        backgroundFifo = {};
        oamFifo = {};
    }
}

void Ppu::modeThree()
{
    const unsigned_four_byte ioStart = 0xFF00;

    // Check if the window should be displayed instead of the background for the rest of the scanline
    if (!renderWindow &&
        (memory->readMemory(ioStart + 0x40) & 0x20) >> 5 == 1 &&                    // LCDC window Enabled Bit
        memory->readMemory(ioStart + 0x4A) == memory->readMemory(ioStart + 0x44) && // curruent scanline is
        pixelsPushed >= (memory->readMemory(ioStart + 0x4B) - 7))
    {
        renderWindow = true;
        backgroundFetchStep = 1;
        backgroundFifo = {};
    }

    int ticksPassed;
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
        unsigned_two_byte ly = memory->readMemory(ioStart + 0x44);

        // Check which tile map you should use
        unsigned_four_byte tileMapAddress = 0x9800;
        if ((((lcdc & 0x4) >> 3) == 1 && viewportXTile < wx - 7) || (((lcdc & 0x40) >> 4) == 1 && viewportXTile >= wx - 7))
        {
            tileMapAddress = 0x9C00;
        }

        // Check which region of memory should be used for tile data
        unsigned_four_byte tileDataBase = 0x9000;
        if ((lcdc & 0x16) >> 4)
        {
            tileDataBase = 0x8000;
        }

        int xOffset, yOffset;
        unsigned_two_byte tileNumber;
        unsigned_four_byte tileAddressBase;
        if (!renderWindow) // background
        {
            xOffset = ((scx / 8) + (viewportXTile)) & 0x1F;
            yOffset = ((scy + ly) & 0xFF);
        }
        else // window
        {
            xOffset = (pixelsPushed - static_cast<int>(wx)) / 8;
            yOffset = (static_cast<int>(ly) - static_cast<int>(wy)) / 8;
        }
        tileNumber = memory->readMemory(tileMapAddress + xOffset + (32 * (yOffset / 8)));
        tileAddressBase = tileDataBase + 16 * (tileDataBase == 0x8000 ? tileNumber : static_cast<signed_two_byte>(tileNumber));
        tileFetchAddress = tileAddressBase + (2 * (yOffset % 8));

        if (tileNumber != 32)
        {
            std::cout << formatter2 << static_cast<int>(tileNumber) << std::endl;
        }

        backgroundFetchStep = 2;
        ticksPassed = 2;
        break;
    }
    // Get Tile Data Low
    case 2:
    {
        tileFetchLow = memory->readMemory(tileFetchAddress);
        backgroundFetchStep = 3;
        ticksPassed = 2;
        break;
    }
    // Get Tile Data High
    case 3:
    {
        tileFetchHigh = memory->readMemory(tileFetchAddress + 1);
        backgroundFetchStep = 4;
        ticksPassed = 2;
        break;
    }
    // Sleep
    case 4:
    {
        backgroundFetchStep = 5;
        ticksPassed = 2;
        break;
    }
    // Push
    case 5:
    {
        if (backgroundFifo.empty() && pixelsPushed < 160)
        {
            backgroundFifo = decodeTileFetchData(tileFetchLow, tileFetchHigh, memory->readMemory(0xFF47));

            if (pixelsPushed == 0) // Account for subtile scrolling by discarding pixels from the first tile that are not in the viewport.
            {
                unsigned_two_byte scx = memory->readMemory(0xFF43);
                for (int i = 0; i < scx % 8; i++)
                {
                    backgroundFifo.pop();
                }
            }
            backgroundFetchStep = 1;
            viewportXTile++; // increment viewport x tile offset.
        }
        if (pixelsPushed == 160)
        {
            mode = 0;
        }
        ticksPassed = 1;
        break;
    }
    }

    if (oamBuffer.empty())
    {
        unsigned_two_byte ly = memory->readMemory(0xFF44);
    }
    bool fetchSprite = false;
    if (fetchSprite)
    {
        switch (oamFetchStep)
        {
        case 1:
        {
            unsigned_two_byte ly = memory->readMemory(0xFF44);
            // unsigned_four_byte address = 0x8000 + (16 * this.currentOamTile.tileIndex);
            // spriteFetchAddress = address + (2 * (ly - (this.currentOamTile.yPos - 16)));
            oamFetchStep = 2;
            break;
        }
        case 2:
        {
            spriteFetchLow = memory->readMemory(spriteFetchAddress);
            oamFetchStep = 3;
            break;
        }
        case 3:
        {
            spriteFetchHigh = memory->readMemory(spriteFetchAddress + 1);
            oamFetchStep = 4;
            break;
        }
        case 4:
        {
            oamFetchStep = 5;
            break;
        }
        case 5:
        {
            oamFetchStep = 1;
            break;
        }
        }
    }

    // Push to viewport
    // push background no sprite

    for (int i = 0; i < ticksPassed; i++)
    {
        if (!backgroundFifo.empty())
        {
            viewport.push_back(backgroundFifo.front());
            backgroundFifo.pop();
            pixelsPushed++;
        }
        // std::cout << backgroundFifo.size() << std::endl;
    }
    currentScanlineTicks += ticksPassed;
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

std::queue<unsigned_two_byte> Ppu::decodeTileFetchData(unsigned_two_byte dataLow, unsigned_two_byte dataHigh, unsigned_two_byte colorPalette)
{
    std::queue<unsigned_two_byte> generatedTile;

    for (int i = 0; i < 8; i++)
    {
        unsigned_two_byte low = (dataHigh & (1 << (7 - i))) >> (7 - i);
        unsigned_two_byte high = (dataLow & (1 << (7 - i))) >> (7 - i);
        generatedTile.push((colorPalette >> 2 * ((high << 1) | low)) & 3);
    }
    return generatedTile;
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

std::vector<int> Ppu::getViewPort()
{
    return std::vector<int>();
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
