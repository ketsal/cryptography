#include <vector>
#include "openssl/sha.h"
class SHA256Proxy
{
private:
    unsigned char _digest[SHA256_DIGEST_LENGTH];
    std::vector<unsigned char> _hashed;
public:
    SHA256Proxy()
    {
    }
    void Hash(std::vector<unsigned char> &Input,int blocksize);
    std::vector<unsigned char> &GetHashedData();
};