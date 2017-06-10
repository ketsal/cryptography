#include <string>
#include <iostream>
#include "../include/file.h"
#include <math.h>
#include <map>
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
        entropy += probability*log(probability) / log(8);
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
    double probability = 0;
    double entropy = 0;
    for (unsigned int i = 0; i < size; i++)
    {
        if (counter.count(Data[i]) > 0)
        {
            counter.at(Data[i])++;
        }
        else
        {
            counter.insert(std::pair<_Uint32t, int>(Data[i], 1));
        }
    }
    for (auto itMap = counter.begin(); itMap != counter.end(); itMap++)
    {
        probability = itMap->second * 1.0 / size;
        entropy += probability*log(probability) / log(32);
    }
    return entropy;
}
void main()
{
    std::string path = "../docs/plain.docx";
    std::string path2 = "../docs/sha.txt";
    std::string path3 = "../docs/myhash.txt";
    File plain(path);
    File shahashed(path2);
    File myhash(path3);
    std::cout << "Reading SHA hashed file";
    shahashed.Read();
    system("cls");
    std::cout << "Reading Plain file";
    plain.Read();
    system("cls");
    std::cout << "Reading MyHash file";
    myhash.Read();
    system("cls");
    std::cout << "Caculating 32-base entropy";
    double entropyplain32 = -CalculateEntropy32Base(plain);
    double entropymyhash32 = -CalculateEntropy32Base(myhash);
    double entropysha32 = -CalculateEntropy32Base(shahashed);
    system("cls");
    std::cout << "Caculating 16-base entropy";
    double entropyplain16 = -CalculateEntropy16Base(plain);
    double entropymyhash16 = -CalculateEntropy16Base(myhash);
    double entropysha16 = -CalculateEntropy16Base(shahashed);
    system("cls");
    std::cout << "Caculating 8-base entropy";
    double entropyplain8 = -CalculateEntropy8Base(plain);
    double entropymyhash8 = -CalculateEntropy8Base(myhash);
    double entropysha8 =- CalculateEntropy8Base(shahashed);
    system("cls");
    printf("Plain\tSHA\tMyHash\n");
    printf("%.4f\t%.4f\t%.4f\t32bits\n", entropyplain32, entropysha32 , entropymyhash32);
    printf("%.4f\t%.4f\t%.4f\t16bits\n", entropyplain16, entropysha16, entropymyhash16);
    printf("%.4f\t%.4f\t%.4f\t8bits\n", entropyplain8, entropysha8, entropymyhash8);
    system("Pause");

}