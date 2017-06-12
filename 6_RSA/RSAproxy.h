#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <stdio.h>
#include <string>
#include "../include/file.h"
#include "../include/KeyGen.h"
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
