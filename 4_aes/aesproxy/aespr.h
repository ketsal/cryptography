#include "../../include/openssl/aes.h"
#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#define ReadAsBinary  std::ios::in | std::ios::binary
#define WriteAsBinary std::ios::binary | std::ios::out | std::ios::trunc
#define CFB 0
#define OFB 1
#define CBC 2
#define CTR 3
class AesProxy
{
private:
    AES_KEY _EncKey;
    AES_KEY _DecKey;
    std::vector<unsigned char> _EncData;
    std::vector<unsigned char> _DecData;
    void encrypt_cfb_ofb_cbc_ctr(std::vector<unsigned char> &Data, int Mode, int CFBorOFBorCBCorCTR);
    void encrypt_ecb(std::vector<unsigned char> &Data, int Mode);
public:
    AesProxy(){}
    void EncryptCTR(std::vector<unsigned char> &Data, const unsigned char *key);
    void DecryptCTR(std::vector<unsigned char> &Data, const unsigned char *key);
    void EncryptECB(std::vector<unsigned char> &Data, const unsigned char *key);
    void DecryptECB(std::vector<unsigned char> &Data, const unsigned char *key);
    void EncryptCBC(std::vector<unsigned char> &Data, const unsigned char *key);
    void DecryptCBC(std::vector<unsigned char> &Data, const unsigned char *key);
    void EncryptCFB(std::vector<unsigned char> &Data, const unsigned char *key);
    void DecryptCFB(std::vector<unsigned char> &Data, const unsigned char *key);
    void EncryptOFB(std::vector<unsigned char> &Data, const unsigned char *key);
    void DecryptOFB(std::vector<unsigned char> &Data, const unsigned char *key);
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