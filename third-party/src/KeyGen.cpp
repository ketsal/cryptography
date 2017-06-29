#include <random>
#include <ctime>
#include "../../shared/KeyGen.h"
#include"../../shared/openssl/pem.h"
#include "../../shared/openssl/ssl.h"
#include "../../shared/openssl/rsa.h"
#include"../../shared/openssl/evp.h"
#include "../../shared/openssl/bio.h"
#include"../../shared/openssl/err.h"
#include "../../shared/openssl/bio.h"
void KeyGen::GenerateRSAkeys(std::string PathToPublic, std::string PathToPrivate,int bits)
{
    BIGNUM *bn = NULL;
    BIO *publicBIO = NULL;
    BIO *privateBIO = NULL;
    RSA *rsa;
    unsigned long e = RSA_F4;
    bn = BN_new();
    BN_set_word(bn, e);
    rsa = RSA_new();
    RSA_generate_key_ex(rsa, bits, bn, NULL);
    publicBIO = BIO_new_file(PathToPublic.c_str(), "w+");
    PEM_write_bio_RSAPublicKey(publicBIO, rsa);
    privateBIO = BIO_new_file(PathToPrivate.c_str(), "w+");
    PEM_write_bio_RSAPrivateKey(privateBIO, rsa, NULL, NULL, 0, NULL, NULL);
    BIO_free(publicBIO);
    BIO_free(privateBIO);
}
unsigned char *KeyGen::RC4(int size)
{
    srand(time(NULL));
    unsigned char *Key = new unsigned char[size];
    for (unsigned int i = 0; i<size; i++)
        Key[i] = (0 + rand() % 255);
    return Key;
}
