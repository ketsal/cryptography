#include "RC4.h"
 std::vector<unsigned char>& RC4::GetEncData()
{
return _EncData;
}
 std::vector<unsigned char>& RC4::GetDecData()
{
return _DecData;
}

void RC4::swap()
{
    unsigned char temp = _M[_i];
    _M[_i] = _M[_j];
    _M[_j] = temp;
}
void RC4::MetadataSchedule(unsigned char *key)
{
    _size = sizeof(key);
    for (unsigned int k = 0; k<256; k++)
    {
        _M[k] = k;
    }
    _j = 0;
    for (_i = 0; _i<256; _i++)
    {
        _j = (_j + _M[_i] + key[_i % _size]) % 256;
        swap();
    }
    _i = _j = 0;
}
unsigned char RC4::getrandbyte()
{
    _i = (_i + 1) % 256;
    _j = (_j + _M[_i]) % 256;
    swap();
    int index = _M[(_M[_i] + _M[_j]) % 256];
    return _M[index];
}
void RC4::Decrypt(unsigned char *key, std::vector<unsigned char> Data)
{
    MetadataSchedule(key);
    for (int i = 0; i < Data.size(); i++)
    {
        _DecData.push_back(Data.at(i) ^ getrandbyte());
    }
}
void RC4::Encrypt(unsigned char *key, std::vector<unsigned char> Data)
{
    MetadataSchedule(key);
    for (int i = 0; i < Data.size(); i++)
    {
        _EncData.push_back(Data.at(i) ^ getrandbyte());
    }
}