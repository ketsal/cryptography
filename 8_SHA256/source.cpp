#include "../shared/openssl/sha.h"
#include "../shared/file.h"
#include <iostream>
#include <sstream>
#include <string>
#include "../shared/SHA256.h"
void HashFiles(File &Input, File &ShaOutput)
{
    SHA256Proxy Hasher;
    unsigned int size = Input.GetData().size();
    unsigned int min = 0;
    unsigned int blocksize = 128;
    std::cout << "hashing";
    std::vector<unsigned char> temp;
    for (unsigned int i = 0; i < size; i += blocksize)
    {
        if ((size - i) < blocksize)
            blocksize = size - i;
        for (int j = i; j < blocksize + i; j++)
        {
            temp.push_back(Input.GetData().at(j));
        }
        Hasher.Hash(temp, blocksize);
        for (int j = 0; j < 32; j++)
        {
            ShaOutput.GetData().push_back(Hasher.GetHashedData().at(j));
        }
        temp.clear();
    }
    system("cls");
    std::cout << "Writing result of SHA256 function";
    ShaOutput.WriteData();
    system("cls");
}
void main()
{
    std::string path = "../docs/sha.txt";
    std::string path3 = "../docs/plain.docx";
    File sha(path);
    File plain(path3);
    std::cout << "Reading plain";
    plain.Read();
    system("cls");
    HashFiles(plain,sha);
    system("Pause");
}