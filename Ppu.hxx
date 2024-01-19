#pragma once
#include "Memory.hxx"

class Ppu
{
private:
    Memory *memory = nullptr;
    // Determines if the next frame is ready to be drawn. Will be true after scanline 153 is complete.
    bool isFrameReady = false;
    int mode;

    int tileMapOne[65536];
    int tileMapTwo[65536];

    int currentScanline;
    int currenScanlineTicks;

public:
    Ppu(/* args */);
    ~Ppu();

    void setMemory(Memory *value);

    bool getIsFrameReady();
    void setIsFrameReady(bool isReady);

    // General function that drives the ppu. One pass of each mode lasts two clock cycles.
    void cycle();

    void modeZero();
    void modeOne();
    void modeTwo();
    void modeThree();

    int getTileMapIndex(int *tilemap, int x, int y);
    void setTileMapIndex(int *tilemap, int x, int y, int value);
};
