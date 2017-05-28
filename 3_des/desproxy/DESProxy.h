#include <stdio.h>
#include "../openssl/des.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#define ReadAsBinary  std::ios::in | std::ios::binary
#define WriteAsBinary std::ios::binary | std::ios::out | std::ios::trunc
#define CFB 0
#define OFB 1
#define CBC 2
#define TripleDes true
#define Des false
class DesProxy
{
private:
    DES_cblock _key;
    DES_key_schedule _schedule;
    DES_cblock _key1;
    DES_cblock _key2;
    DES_cblock _key3;
    DES_key_schedule _ks1;
    DES_key_schedule _ks2;
    DES_key_schedule _ks3;
    std::vector<unsigned char> _EncData;
    std::vector<unsigned char> _DecData;
    void encrypt_cfb_ofb_cbc(std::vector<unsigned char> &Data, int Mode, int CFBorOFBorCBC, bool TripleCheck);
    void encrypt_ecb(std::vector<unsigned char> &Data, int Mode, bool TripleCheck);
public:
    DesProxy(){}
    void EncryptECB(std::vector<unsigned char> &Data, const char *key);
    void DecryptECB(std::vector<unsigned char> &Data, const char *key);
    void EncryptCBC(std::vector<unsigned char> &Data, const char *key);
    void DecryptCBC(std::vector<unsigned char> &Data, const char *key);
    void EncryptCFB(std::vector<unsigned char> &Data, const char *key);
    void DecryptCFB(std::vector<unsigned char> &Data, const char *key);
    void EncryptOFB(std::vector<unsigned char> &Data, const char *key);
    void DecryptOFB(std::vector<unsigned char> &Data, const char *key);
    void EncryptTripleDesECB(std::vector<unsigned char>& Data, const char *key1, const char *key2, const char *key3);
    void DecryptTripleDesECB(std::vector<unsigned char>& Data, const char *key1, const char *key2, const char *key3);
    void EncryptTripleDesCBC(std::vector<unsigned char>& Data, const char *key1, const char *key2, const char *key3);
    void DecryptTripleDesCBC(std::vector<unsigned char>& Data, const char *key1, const char *key2, const char *key3);
    void EncryptTripleDesCFB(std::vector<unsigned char>& Data, const char *key1, const char *key2, const char *key3);
    void DecryptTripleDesCFB(std::vector<unsigned char>& Data, const char *key1, const char *key2, const char *key3);
    void EncryptTripleDesOFB(std::vector<unsigned char>& Data, const char *key1, const char *key2, const char *key3);
    void DecryptTripleDesOFB(std::vector<unsigned char>& Data, const char *key1, const char *key2, const char *key3);
    std::vector<unsigned char> &GetEncData(){ return _EncData; }
    std::vector<unsigned char> &GetDecData(){ return _DecData; };
};
class File
{
private:
    std::vector<unsigned char> _data;
    std::fstream _reader;
    std::fstream _writer;
public:
    std::string	Path;
    File(std::string &path) :Path(path){}
    ~File(){}
    bool FileExist();
    bool Read();
    bool WriteData();
    std::vector<unsigned char> &GetData() { return _data; }
};