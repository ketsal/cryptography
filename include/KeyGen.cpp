#include <random>
#include <ctime>
#include "KeyGen.h"
#include <../include/openssl/pem.h>
#include <../include/openssl/ssl.h>
#include <../include/openssl/rsa.h>
#include <../include/openssl/evp.h>
#include <../include/openssl/bio.h>
#include <../include/openssl/err.h>
#include "../include/openssl/bio.h"
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
unsigned char* KeyGen::GenerateTripleDesKey()
{
    srand(time(NULL));
    unsigned char Key[168];
    for (unsigned int i = 0; i<168; i++)
        Key[i] = (0 + rand() % 255);
    return Key;
}
unsigned char *KeyGen::GenerateDesKey()
{
    srand(time(NULL));
    unsigned char Key[56];
    for (unsigned int i = 0; i<56; i++)
        Key[i] = (0 + rand() % 255);
    return Key;
}
unsigned char *KeyGen::GenerateAesKey(int size)
{
    srand(time(NULL));
    unsigned char *Key = new unsigned char[size];
    for (unsigned int i = 0; i<size; i++)
        Key[i] = (0 + rand() % 255);
    return Key;
}
unsigned char *KeyGen::RC4(int size)
{
    srand(time(NULL));
    unsigned char *Key = new unsigned char[size];
    for (unsigned int i = 0; i<size; i++)
        Key[i] = (0 + rand() % 255);
    return Key;
}
unsigned char *KeyGen::Otp(int size)
{
    srand(time(NULL));
    unsigned char *Key = new unsigned char[size];
    for (unsigned int i = 0; i<size; i++)
        Key[i] = (0 + rand() % 255);
    return Key;
}
