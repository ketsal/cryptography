#include "MYHash.h"
#include "../include/file.h"
void HashFiles(File &Input, File &MyOutput, std::vector<unsigned char>  IV)
{
    MyHash Hasher;
    int size = Input.GetData().size();
    std::vector<unsigned char> TEMP;
    std::vector<unsigned char> TEMP2;
    unsigned int k = 0;
    std::cout << "hashing";
    for (unsigned int i = 0; i < size; i += 128)
    {
        for (int j = i; (j < i + 128) && (j<size); j++)
        {
            TEMP.push_back(Input.GetData().at(j));
        }
        Hasher.Hash(TEMP, IV);
        TEMP2 = Hasher.GetHashedData();
        for (int j = 0; j < 32; j++)
        {
            MyOutput.GetData().push_back(TEMP2.at(j));
        }
        TEMP.clear();
    }
    system("cls");
    std::cout << "Writing result of my hash function";
    MyOutput.WriteData();
    system("cls");
}

void main()
{
    unsigned char digest[SHA256_DIGEST_LENGTH];
    std::vector<unsigned char>  IV;
    std::string path2 = "../docs/myhash.txt";
    std::string path3 = "../docs/plain.docx";
    File myhash(path2);
    File plain(path3);
    std::cout << "Reading plain";
    plain.Read();
    system("cls");
    for (unsigned int i = 0; i < 32; i++)
    {
        IV.push_back((unsigned char)i);
    }
    HashFiles(plain, myhash, IV);
    system("Pause");
}