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

void Ppu::modeOne()
{
}

void Ppu::modeTwo()
{
}

void Ppu::modeThree()
{
}

void Ppu::modeFour()
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