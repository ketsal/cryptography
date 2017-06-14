#include "SHA256.h"
#include "openssl/sha.h"
std::vector<unsigned char> &SHA256Proxy::GetHashedData()
{
    return _hashed;
}
void SHA256Proxy::Hash(std::vector<unsigned char> &Input, int blocksize)
{
    _hashed.clear();
    unsigned char * temp = new unsigned char[blocksize];
    for (int i = 0; i < blocksize; i++)
    {
        temp[i] = Input.at(i);
    }
    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, temp, blocksize);
    SHA256_Final(_digest, &ctx);
    for (int j = 0; j < 32; j++)
    {
        _hashed.push_back(_digest[j]);
    }
}
