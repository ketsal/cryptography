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
    std::vector<unsigned char *> encryptedBlocks;
    void ReadPublicKey(std::string PathTopublickey)
    {
        BIO *publicBIO = NULL;
        publicBIO = BIO_new_file(PathTopublickey.c_str(), "r");
        PublicRSA = PEM_read_bio_RSAPublicKey(publicBIO,NULL, NULL, NULL);
        BIO_free(publicBIO);
    }
    void ReadPrivateKey(std::string PathToPrivatekey)
    {
        BIO *privateBIO = NULL;
        privateBIO = BIO_new_file(PathToPrivatekey.c_str(), "r");
        PrivateRSA = PEM_read_bio_RSAPrivateKey(privateBIO, NULL, NULL, NULL);
        BIO_free(privateBIO);
    }
    void printError()
    {
        char * err = (char*)malloc(130);;
        ERR_load_crypto_strings();
        ERR_error_string(ERR_get_error(), err);
        printf("ERROR: %s\n", err);
        free(err);
    }
public:
    RSAproxy()
    {

    }
    void Encrypt(std::vector<unsigned char> &data, std::string path)
    {
        ReadPublicKey(path);
        int flen = RSA_size(PublicRSA);
        unsigned char *temp=new unsigned char[flen];
        unsigned char *encrypted = new unsigned char[flen];
        unsigned int k = 0;
        unsigned int datasize=data.size();
        unsigned int pseudosize = ((datasize + flen - 1) / flen*flen)-flen;
        for (int i = 0; i < datasize;i++, k++)
        {
            if (k < flen)
            {
                if (i >= pseudosize)
                {
                    for (int j = i,k=0; j < datasize; j++,k++)
                    {
                        temp[k] = data.at(j);
                    }
                    k = flen;
                    i = datasize-2;
                }
                else
                {
                    temp[k] = data.at(i);
                }
            }
            else
            {
                int f = RSA_public_encrypt(flen,temp, encrypted, PublicRSA, RSA_NO_PADDING);
                if (f == -1)
                {
                    printError();
                    std::cout << flen << " " << i;
                }
                k = 0;
                for (int j = 0; j < f; j++)
                {
                    _EncData.push_back(encrypted[j]);
                }
                encryptedBlocks.push_back(encrypted);
            }
        }
    }
    void Decrypt(std::vector<unsigned char> &data, std::string path)
    {
        ReadPrivateKey(path);
        unsigned int k = 0;
        int flen = RSA_size(PrivateRSA);
        int f = 0;
        unsigned char *temp = new unsigned char[flen];
        unsigned int datasize = data.size();
        unsigned char *decrypted = new unsigned char[flen];
        for (int i = 0; i <data.size(); i++, k++)
        {
            if (k < flen)
            {
                if (i < datasize)
                    temp[k] = data.at(i);
            }
            else
            {
                std::cout << flen << std::endl;
                f = RSA_private_decrypt(flen, temp, decrypted, PrivateRSA, RSA_NO_PADDING);
                if(f== -1)
                printError();
                k = 0;
                for (int j = 0; j < f; j++)
                {
                    _DecData.push_back(decrypted[j]);
                }
            }
        }
    }
    std::vector<unsigned char> &getenddata()
    {
        return _EncData;
    }
        std::vector<unsigned char> &getdecdata()
    {
        return _DecData;
    }
};
void main()
{
    KeyGen gen;
    std::string path3 = "../docs/plain.docx";
    std::string path4 = "../docs/decrypted.docx";
    std::string path = "../docs/public.pem";
    std::string path2 = "../docs/private.pem";
    gen.GenerateRSAkeys(path, path2,2048);
    File plain(path3);
    File dec(path4);
    RSAproxy crypter;
    printf("Readin\n");
    plain.Read();
    printf("Enc\n");
    crypter.Encrypt(plain.GetData(), path);
    printf("Dec\n");
    crypter.Decrypt(crypter.getenddata(), path2);
    printf("Writing\n");
    dec.GetData() = crypter.getdecdata();
    dec.WriteData();
    system("pause");
}