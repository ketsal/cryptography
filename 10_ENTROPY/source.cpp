#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <iostream>
#include "../include/file.h"
#include <math.h>
#include <map>
#include <random>
#include <ctime>
#include "../8_MYHASH/MYHash.h"
#include "../include/SHA256.h"
#include "../include/openssl/sha.h"
#include "../include/zip.h"

#define SHA 1
#define MYHASHFUNC 2
double CalculateEntropy8Base(File &FileToCalc)
{
    unsigned int *counter = new unsigned int[256];
    unsigned int size = FileToCalc.GetData().size();
    double probability = 0;
    double entropy = 0;
    for (unsigned int i = 0; i < 256; i++)
    {
        counter[i] = 0;
    }
    for (unsigned int i = 0; i < size; i++)
    {
        counter[FileToCalc.GetData().at(i)]++;
    }
    for (unsigned int i = 0; i < 256; i++)
    {
        probability = counter[i] * 1.0 / size;
        if (probability != 0)
        {
            entropy += probability*log(probability) / log(8);
        }
    }
    return entropy;
}
double CalculateEntropy16Base(File &FileToCalc)
{
    unsigned int *counter = new unsigned int[65536];
    unsigned int size = FileToCalc.GetData().size()/2;
    unsigned short*Data = (unsigned short*)FileToCalc.GetData().data();
    double probability = 0;
    double entropy = 0;
    for (unsigned int i = 0; i < 65536; i++)
    {
        counter[i]=0;
    }
    for (unsigned int i = 0; i < size; i++)
    {
        counter[Data[i]]++;
    }
    for (unsigned int i = 0; i < 65536; i++)
    {
        probability = counter[i] * 1.0 / size;
        if(probability!=0)
            entropy += probability*log(probability) / log(16);
    }
    return entropy;
}
double CalculateEntropy32Base(File &FileToCalc)
{
    std::map<_Uint32t, int> counter;
    unsigned int size = FileToCalc.GetData().size()/4;
    _Uint32t *Data = (_Uint32t*)FileToCalc.GetData().data();
    std::pair<std::map<_Uint32t, int>::iterator, bool> ret;
    double probability = 0;
    double entropy = 0;
    for (unsigned int i = 0; i < size; i++)
    {        
        ret=counter.insert(std::pair<_Uint32t, int>(Data[i], 1));
        if (ret.second == false)
        {
            counter.at(Data[i])++;
        }
    }
    for (auto itMap = counter.begin(); itMap != counter.end(); itMap++)
    {
        probability = itMap->second * 1.0 / size;
        entropy += probability*log(probability) / log(32);
    }
    return entropy;
}
double CalculateWithIntersection(File &FileToCalc)
{
    int  *counter=new int[65536];
    unsigned int size = FileToCalc.GetData().size() - 1;
    double probability = 0;
    double entropy = 0;
    unsigned char *_8bits = (unsigned char *)FileToCalc.GetData().data();
    unsigned short *_16bits = (unsigned short *)FileToCalc.GetData().data();
    for (int i = 0; i < 65536; i++)
    {
        counter[i] = 0;
    }
    for (unsigned int i = 0; i < size; i++)
    {
        _16bits = (unsigned short*)_8bits;
        counter[(int)*_16bits]++;
        _8bits++;
    }
    for (int i = 0; i < 65536; i++)
    {
        probability = counter[i] * 1.0 / size;
        if (probability != 0)
        {
            entropy += probability*log(probability) / log(16);
        }
    }
    return entropy;
 }
void HashFile(File &filetohash, int FUNC, File &output)
{
    srand(time(NULL));
    std::cout << "Reading plain text to hash";
    filetohash.Read();
    system("cls");
    int size = filetohash.GetData().size();
    int blocksize = 32;
    std::vector<unsigned char> temp;
    std::vector<unsigned char> IV(32,'a');
    std::cout << "Hashing";
    for (int i = 0; i < size; i += blocksize)
    {
        if ((size - i) < blocksize)
            blocksize = size - i;
        if (FUNC == SHA)
        {
            SHA256Proxy Hasher;
            for (int j = i; j < blocksize+i; j++)
            {
                temp.push_back(filetohash.GetData().at(j));
            }
            temp.push_back(rand()%255+0);
            Hasher.Hash(temp, blocksize+1);
            for (int j = 0; j < 32; j++)
            {
                output.GetData().push_back(Hasher.GetHashedData().at(j));
            }
            temp.clear();
        }
        if (FUNC == MYHASHFUNC)
        {
            MyHash Hasher;
            for (int j = i; j < blocksize-1+i; j++)
            {
                temp.push_back(filetohash.GetData().at(j));
            }
            temp.push_back(rand() % 255 + 0);
            Hasher.Hash(temp, IV);
            for (int j = 0; j < 32; j++)
            {
                output.GetData().push_back(Hasher.GetHashedData().at(j));
            }
            temp.clear();
        }
    }
    system("cls");
}
void ZipFile(std::string name)
{
    TCHAR* path = 0;
    path = new TCHAR[name.size() + 1];
    strcpy(path, name.c_str());
    HZIP hz = CreateZip("../docs/zippedfile.zip", 0);
    ZipAdd(hz, "zip.docx",path);
    CloseZip(hz);
}
void main()
{
    std::string path = "../docs/plain.docx";
    std::string path2 = "../docs/sha.docx";
    std::string path3 = "../docs/myhash.docx";
    std::string path4 = "../docs/zippedfile.zip";
    File plain(path);
    double zipratio = 0;
    File shahashed(path2);
    File myhash(path3);
    File zip(path4);
    ZipFile(path);
    bool cont = true;
    int file=0;
    int alphabet = 0;
    double entropy = 0;
    while (cont)
    {
        std::cout << "Choose file to calculate entropy";
        std::cout << "\n1 for plain,2 for MyHash, 3 for sha hashed file, 4 for zip-file\n";
        std::cin >> file;
        if (file> 4 || file < 1)
        {
            std::cout << "Choose wisely";
        }
        else
        {
            cont = false;
        }
        system("cls");
    }
    cont = true;
    if (file == 4)
    {
        cont = false;
    }
    while (cont)
    {
        std::cout << "Choose alphabet";
        std::cout << "\n1 for 8-bits,2 for 16-bits, 3 for 32-bits, 4 for 16-bits(with intersec)\n";
        std::cin >> alphabet;
        if (alphabet> 4 || alphabet < 1)
        {
            std::cout << "Choose wisely";
        }
        else
        {
            cont = false;
        }
        system("cls");
    }
    if (file == 1)
    {
        std::cout << "Reading plain text";
        plain.Read();
        system("cls");
        std::cout << "Calculating entropy";
        switch (alphabet)
        {
        case 1:entropy = CalculateEntropy8Base(plain); break;
        case 2:entropy = CalculateEntropy16Base(plain); break;
        case 3:entropy = CalculateEntropy32Base(plain); break;
        case 4:entropy = CalculateWithIntersection(plain); break;
        }
    }
    if (file == 2)
    {
        HashFile(plain, MYHASHFUNC, myhash);
        system("cls");
        std::cout << "Calculating entropy";
        switch (alphabet)
        {
        case 1:entropy = CalculateEntropy8Base(myhash); break;
        case 2:entropy = CalculateEntropy16Base(myhash); break;
        case 3:entropy = CalculateEntropy32Base(myhash); break;
        case 4:entropy = CalculateWithIntersection(myhash); break;
        }
    }
    cont = true;
    if (file == 3)
    {
        HashFile(plain, SHA, shahashed);
        system("cls");
        std::cout << "Calculating entropy";
        switch (alphabet)
        {
        case 1:entropy = CalculateEntropy8Base(shahashed); break;
        case 2:entropy = CalculateEntropy16Base(shahashed); break;
        case 3:entropy = CalculateEntropy32Base(shahashed); break;
        case 4:entropy = CalculateWithIntersection(shahashed); break;
        }
    }
    if (file == 4)
    {
       
        while (cont)
        {
            std::cout << "What file to zip? 1 for plain,2 for my hash,3 for SHA\n";
            std::cin >> file;
            if (file < 1 || file>3)
            {
                std::cout << "Choose wisely";
            }
            else
            {
                cont = false;
            }
            system("cls");
        }
        if (file == 1)
        {        
            std::cout << "Reading plain text ";
            plain.Read();
            system("cls");
            std::cout << "Zipping";
            ZipFile(path);
            system("cls");
            std::cout << "Reading zip file";
            zip.Read();
            zipratio = plain.GetData().size()*1.0 / zip.GetData().size();
        }
        if (file == 2)
        {
            system("cls");
            HashFile(plain,MYHASHFUNC,myhash);
            std::cout << "Writing hashed data";
            myhash.WriteData();
            system("cls");
            std::cout << "Zipping";
            ZipFile(path3);
            system("cls");
            std::cout << "Reading zip file";
            zip.Read();
            zipratio = myhash.GetData().size()*1.0 / zip.GetData().size();
        }
        if (file == 3)
        {
            system("cls");
            HashFile(plain, SHA, shahashed);
            std::cout << "Writing hashed data";
            shahashed.WriteData();
            system("cls");
            std::cout << "Zipping";
            ZipFile(path2);
            system("cls");
            std::cout << "Reading zip file";
            zip.Read();
            zipratio = shahashed.GetData().size()*1.0 / zip.GetData().size();
        }
    }
    system("cls");
    if (zipratio != 0)
    {
        std::cout << "Zip ratio=" << zipratio << "\n";
    }
    else
    {
        std::cout << "Entropy=" << -entropy << "\n";
    }
    system("Pause");
}