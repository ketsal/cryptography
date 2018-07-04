#include  "../shared/openssl/pem.h"
#include  "../shared/openssl/ssl.h"
#include  "../shared/openssl/rsa.h"
#include "../shared/openssl/evp.h"
#include  "../shared/openssl/bio.h"
#include  "../shared/openssl/err.h"
#include <stdio.h>
#include <string>
#include "../shared/file.h"
#include "../shared/KeyGen.h"
class RSAproxy
{
private:
    RSA *PublicRSA = NULL;
    RSA *PrivateRSA = NULL;
    std::vector<unsigned char> _EncData;
    std::vector<unsigned char> _DecData;
    std::vector<unsigned char*> _EncBlocks;
    void ReadPublicKey(std::string PathTopublickey);
    void ReadPrivateKey(std::string PathToPrivatekey);
public:
    RSAproxy()
    {

    }
    void Encrypt(std::vector<unsigned char> &data, std::string path);
    void Decrypt(std::vector<unsigned char> &data, std::string path);
    std::vector<unsigned char> &GetEndData();
    std::vector<unsigned char> &GetDecData();
};
