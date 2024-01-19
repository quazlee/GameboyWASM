#include "Ppu.hxx"

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
    currenScanlineTicks += 2;
    if (currenScanlineTicks == 456 && memory->io.getData(0x44) < 153)
    {
        currenScanlineTicks = 0;
        memory->io.setData(0x44, memory->io.getData(0x44) + 1);
    }
    else if (currenScanlineTicks == 456 && memory->io.getData(0x44) == 153)
    {
        currenScanlineTicks = 0;
        mode = 2;
        memory->io.setData(0x44, 0);
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