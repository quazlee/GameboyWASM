#pragma once
#include "Types.hxx"
#include <vector>
#include <queue>
// #include "canvas.h"

class Memory;

struct Sprite
{
    Sprite(int yPos, int xPos, int tileIndex, int attributes, int oamAddress)
    {
        this->yPos = yPos;
        this->xPos = xPos;
        this->tileIndex = tileIndex;
        this->attributes = attributes;
        this->oamAddress = oamAddress;
    };
    int yPos;
    int xPos;
    int tileIndex;
    int attributes;
    int oamAddress;
};
class Ppu
{
private:
    Memory *memory = nullptr;
    // Determines if the next frame is ready to be drawn. Will be true after scanline 153 is complete.
    bool isFrameReady = false;
    int mode;

    int currentScanline;
    int currenScanlineTicks;

    int currentOamAddress = 0;
    std::vector<Sprite> oamBuffer;

    unsigned_two_byte* viewport = new unsigned_two_byte[160*144];
    int viewportX = 0;
    int viewportY = 0;

    bool renderWindow = false;
    
    int backgroundFetchStep = 1;

    std::queue<unsigned_two_byte> backgroundFifo;
    std::queue<unsigned_two_byte> oamFifo;

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

    void oamScan();
    int getSpriteHeight();

    int getTileMapIndex(int *tilemap, int x, int y);
    void setTileMapIndex(int *tilemap, int x, int y, int value);

    std::vector<int> populateBackgroundWindowMaps();
    std::vector<int> populateTileMap();

    std::vector<int> decodeTile(std::vector<unsigned_two_byte> input);

    void setDebugAddresses(unsigned_four_byte background, unsigned_four_byte tilemap);
};
