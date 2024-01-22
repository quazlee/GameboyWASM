#pragma once
#include "Types.hxx"
#include <vector>

class Memory;
class Ppu
{
private:
    Memory *memory = nullptr;
    // Determines if the next frame is ready to be drawn. Will be true after scanline 153 is complete.
    bool isFrameReady = false;
    int mode;

    int backgroundMap[65536];
    int windowMap[65536];

    int currentScanline;
    int currenScanlineTicks;

    unsigned_four_byte backgroundDebugAddress = 0x8000;
    unsigned_four_byte tilemapDebugAddress = 0x9800;

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

    void populateBackgroundWindowMaps(unsigned_four_byte backgroundAddress, unsigned_four_byte tilemapAddress);

    std::vector<int> decodeTile(std::vector<unsigned_two_byte> input);

    void setDebugAddresses(unsigned_four_byte background, unsigned_four_byte tilemap);
};
