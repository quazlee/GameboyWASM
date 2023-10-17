#include <vector>;

class MemoryBank
{
private:
    /* data */
    int size;
    std::vector<two_byte> data(size);
public:
    MemoryBank(/* args */);
    ~MemoryBank();
    two_byte getData(int location);
    void setData(int location, two_byte value);
};
