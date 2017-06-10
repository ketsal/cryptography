#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <vector>
class RC4
{
private:
    unsigned int _i;
    unsigned int _j;
    unsigned char _M[256];
    unsigned int _size;
    std::vector<unsigned char> _EncData;
    std::vector<unsigned char> _DecData;
    void swap();
    void MetadataSchedule(unsigned char *key);
    unsigned char getrandbyte();
public:
    RC4()
    {
    }
    void Decrypt(unsigned char *key, std::vector<unsigned char> Data);
    void Encrypt(unsigned char *key, std::vector<unsigned char> Data);
    std::vector<unsigned char> &GetEncData();
    std::vector<unsigned char> &GetDecData();
};