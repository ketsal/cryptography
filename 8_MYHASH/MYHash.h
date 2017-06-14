#include "../include/openssl/sha.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
class MyHash
{
private:
    std::vector<unsigned char> _hashed;
    std::vector<unsigned char> ElemFunc(std::vector<unsigned char> &Block, std::vector<unsigned char> &PrevBlock);
public:
    MyHash()
    {
    }
    void Hash(std::vector<unsigned char> &DataToHash, std::vector<unsigned char>& IV);
    std::vector<unsigned char> &GetHashedData();
};

