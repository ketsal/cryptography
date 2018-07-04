#include "RSAproxy.h"

void RSAproxy::ReadPublicKey(std::string PathTopublickey)
{
    BIO *publicBIO = NULL;
    publicBIO = BIO_new_file(PathTopublickey.c_str(), "r");
    PublicRSA = PEM_read_bio_RSAPublicKey(publicBIO, NULL, NULL, NULL);
    BIO_free(publicBIO);
}
void RSAproxy::ReadPrivateKey(std::string PathToPrivatekey)
{
    BIO *privateBIO = NULL;
    privateBIO = BIO_new_file(PathToPrivatekey.c_str(), "r");
    PrivateRSA = PEM_read_bio_RSAPrivateKey(privateBIO, NULL, NULL, NULL);
    BIO_free(privateBIO);
}

void RSAproxy::Encrypt(std::vector<unsigned char> &data, std::string path)
{
    ReadPublicKey(path);
    int flen = RSA_size(PublicRSA) - 11;
    unsigned char *temp = new unsigned char[flen];
    unsigned char *encrypted = new unsigned char[flen];
    unsigned int k = 0;
    int lenres = 0;
    unsigned int datasize = data.size();
    unsigned int pseudosize = ((datasize + flen - 1) / flen*flen) - flen;
    for (int i = 0; i < datasize; i += flen)
    {
        for (k = 0; (k < flen) && (k + i)<datasize; k++)
        {
            temp[k] = data.at(i + k);
        }
        if ((datasize - i) < flen)
        {
            flen = datasize - i;
        }
         lenres= RSA_public_encrypt(flen, temp, encrypted, PublicRSA, RSA_PKCS1_PADDING);
        for (int j = 0; j < lenres; j++)
        {
            _EncData.push_back(encrypted[j]);
        }
    }
}
void RSAproxy::Decrypt(std::vector<unsigned char> &data, std::string path)
{
    ReadPrivateKey(path);
    unsigned int k = 0;
    int flen = RSA_size(PrivateRSA);
    int lenres = 0;
    unsigned char *temp = new unsigned char[flen];
    unsigned int datasize = data.size();
    unsigned char *decrypted = new unsigned char[flen];
    for (int i = 0; i <data.size(); i += flen)
    {
        for (k = 0; k < flen; k++)
        {
            temp[k] = data.at(k + i);
        }
        lenres = RSA_private_decrypt(flen, temp, decrypted, PrivateRSA, RSA_PKCS1_PADDING);
        for (int j = 0; j < lenres; j++)
        {
            _DecData.push_back(decrypted[j]);
        }
    }
}
std::vector<unsigned char> &RSAproxy::GetEndData()
{
    return _EncData;
}
std::vector<unsigned char> &RSAproxy::GetDecData()
{
    return _DecData;
}