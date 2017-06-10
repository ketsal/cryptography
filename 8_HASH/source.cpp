#include "../include/openssl/sha.h"
#include "../include/file.h"
#include <iostream>
#include <sstream>
#include <string>
std::vector<unsigned char> ElemFunc(std::vector<unsigned char> &Block, std::vector<unsigned char> &PrevBlock)
{
    std::vector<unsigned char> HASH;    
    for (int i = 0; i < 32; i++)
    {
        HASH.push_back(Block.at(i));
        HASH.at(i) += PrevBlock.at(i);
        HASH.at(i) += (HASH.at(i)% (i+1));
        HASH.at(i) ^= (HASH.at(i) >> 5);
    }
    return HASH;
}
std::vector<unsigned char> MyHash(std::vector<unsigned char> &DataToHash, std::vector<unsigned char>& IV)
{
    int size = DataToHash.size();
    int pseudosize = (size + 31) / 32 * 32;
    if (pseudosize < 0)
    {
        pseudosize = -pseudosize;
    }
    for (int i = 0; i < pseudosize - size; i++)
    {
        DataToHash.push_back('s');
    }
    std::vector<unsigned char> SUMM;
    std::vector<unsigned char> HASH;
    std::vector<unsigned char> PrevHash;
    std::vector<unsigned char> Size;
    for (int i = 0; i<pseudosize; i = i + 32)
    {
        if (i == 0)
        {
            PrevHash = ElemFunc(DataToHash, IV);
            for (int j = 0; j < 32; j++)
            {
                SUMM.push_back( DataToHash.at(j));
            }
        }
        else
        {
            std::vector<unsigned char> TEMP;
            for (int j = i; j < i+32; j++)
            {
                TEMP.push_back(DataToHash.at(j));
            }
            PrevHash = ElemFunc(TEMP, PrevHash);
            for (int j = i,k=0; j < i+32;k++, j++)
            {
                SUMM.at(k) ^= DataToHash.at(j);
            }
        }
    }
    for (int j = 0; j < 32;  j++)
    {
        Size.push_back((unsigned char)size);
    }
    PrevHash = ElemFunc(Size, PrevHash);
    PrevHash = ElemFunc(SUMM, PrevHash);
    return PrevHash;
}
void HashFiles(File &Input, File &MyOutput, File &ShaOutput, std::vector<unsigned char>  IV)
{
    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    unsigned char digest[SHA256_DIGEST_LENGTH];
    unsigned int size = Input.GetData().size();
    std::vector<unsigned char> TEMP;
    std::vector<unsigned char> TEMP2;
    unsigned int k = 0;
    std::cout << "hashing";
    for (unsigned int i = 0; i < size; i+=128)
    {
        for (int j = i; (j < i+128)&&(j<size); j++)
        {
            TEMP.push_back(Input.GetData().at(j));
        }
        SHA256_Update(&ctx,&TEMP, 128);
        SHA256_Final(digest, &ctx);
        TEMP2=MyHash(TEMP, IV);
        for (int j = 0; j < 32; j++)
        {
            MyOutput.GetData().push_back(TEMP2.at(j));
            ShaOutput.GetData().push_back(digest[j]);
        }
        TEMP.clear();
    }
    system("cls");
    std::cout << "Writing result of my hash function";
    MyOutput.WriteData();
    system("cls");
    std::cout << "Writing result of SHA256 function";
    ShaOutput.WriteData();
    system("cls");
}
void main()
{
    unsigned char digest[SHA256_DIGEST_LENGTH];
    std::vector<unsigned char>  IV;
    std::string path = "../docs/sha.txt";
    std::string path2 = "../docs/myhash.txt";
    std::string path3 = "../docs/plain.docx";
    File sha(path);
    File myhash(path2);
    File plain(path3);
    std::cout << "Reading plain";
    plain.Read();
    system("cls");
    for (unsigned int i = 0; i < 32; i++)
    {
        IV.push_back((unsigned char)i);
    }
    HashFiles(plain, myhash,sha, IV);
    system("Pause");
}