#include <string>
#include <iostream>
#include <random>
#include <ctime>
#include <algorithm>
#include <iterator>
#include "../include/DESProxy.h"
#include "../include/file.h"
#include <map>
void main()
{
    DesProxy DoubleDES;
    std::string path = "../docs/plainforDDES.txt";
    std::string path2 = "../docs/cipherforDDES.txt";
    File plain(path);
    File crypted(path2);
    srand(time(NULL));
    int numberofkeys = 10;
    unsigned char **KEYS = new  unsigned char*[numberofkeys];
    for (int count = 0; count < numberofkeys; count++)
        KEYS[count] = new unsigned char[14];
    for (int i = 0; i < 14; i++)
    {
        KEYS[0][i] = 0 + rand() % 255;
    }
    plain.Read();
    bool cont = true;
    int j = 13;
    KEYS[0][13] = 255;
    for (int i = 1; i < numberofkeys; i++)
    {
        cont = true;
        j = 13;
        for (int k = 0; k < 14; k++)
        {
            KEYS[i][k] = KEYS[i - 1][k];
        }
        KEYS[i][j] = KEYS[i - 1][j] + 1;
        if (KEYS[i][j] == 0)
        {
            j--;
            KEYS[i][j] = KEYS[i - 1][j] + 1;
            if (KEYS[i][j] == 0)
            {
                j--;
                KEYS[i][j] = KEYS[i - 1][j] + 1;
            }
        }
    }
    int KeyNumber = rand() % 9 + 0;
    unsigned char KEY[14];
    std::string KEY1 = "1234567";
    std::string KEY2 = "1234567";;

    for (int i = 0; i < 7; i++)
    {
        KEY1[i] = KEYS[KeyNumber][i];
    }
    for (int i = 0; i < 7; i++)
    {
        KEY2[i] = KEYS[KeyNumber][i + 7];
    }
    const char* chars = KEY1.data();
    const  char* data_bytes = reinterpret_cast<const char*>(chars);
    const char* chars2 = KEY2.data();
    const  char* data_bytes2 = reinterpret_cast<const char*>(chars2);

    DoubleDES.EncryptECB(plain.GetData(), data_bytes);
    std::vector<unsigned char> temp;
    temp = DoubleDES.GetEncData();
    DoubleDES.GetEncData().clear();
    DoubleDES.EncryptECB(temp, data_bytes2);
    crypted.GetData() = DoubleDES.GetEncData();
    crypted.WriteData();
    DoubleDES.GetEncData().clear();
    DoubleDES.DecryptECB(crypted.GetData(), data_bytes2);
    DoubleDES.EncryptECB(plain.GetData(), data_bytes);
    std::map < unsigned char *, std::string > ENCMAP;
    std::map < unsigned char *, std::string  > DECMAP;
    for (int i = 0; i < numberofkeys; i++)
    {
        DoubleDES.GetEncData().clear();
        for (int j = 0; j < 7; j++)
        {
            KEY1[j] = KEYS[i][j];
        }
        const char* chars3 = KEY1.data();
        const  char* data_bytes3 = reinterpret_cast<const char*>(chars3);
        DoubleDES.EncryptECB(plain.GetData(), data_bytes3);
        int  size = DoubleDES.GetEncData().size();
        unsigned char *temp=new unsigned char[size];
        for (int k = 0; k < size; k++)
        {
            temp[k] = DoubleDES.GetEncData().at(k);
        }
        ENCMAP.insert(std::pair <unsigned char*, std::string >
            (temp, KEY1));
    }
    for (int i = 0; i < numberofkeys; i++)
    {
        DoubleDES.GetDecData().clear();
        for (int j = 0; j < 7; j++)
        {
            KEY2[j] = KEYS[i][j+7];
        }
        const char* chars3 = KEY2.data();
        const  char* data_bytes3 = reinterpret_cast<const char*>(chars3);
        DoubleDES.DecryptECB(crypted.GetData(), data_bytes3);
        int size = DoubleDES.GetDecData().size();
        unsigned char * temp=new unsigned char[size];
        for (int k = 0; k < size; k++)
        {
            temp[k] = DoubleDES.GetDecData().at(k);
        }
        DECMAP.insert(std::pair<unsigned char*, std::string >
            (temp,KEY2));
    }
    std::map <unsigned char*, std::string  > INTER;
    std::set_intersection(ENCMAP.begin(), ENCMAP.end()
        , DECMAP.begin(), DECMAP.end(), 
        inserter(INTER,INTER.end()));
    int size;
    cont = true;
    bool found = true;
    std::vector<unsigned char> tempvec;
    std::vector<unsigned char> tempvec2;
    std::vector<unsigned char> key;
    for (auto itMap = DECMAP.begin(), it = ENCMAP.begin(); ((itMap != DECMAP.end()) && cont); it++, itMap++)
    {
        size = sizeof(itMap->first);
        for (int j = 0; j < size; j++)
        {
            tempvec.push_back(itMap->first[j]);
            tempvec2.push_back(it->first[j]);
        }
        if (tempvec == tempvec2)
        {
            cont = false;
            key.insert(key.begin(),it->second[7]);
            key.insert(key.end(), itMap->second[7]);
            std::cout << "true";
        }
    }
    printf("%s\n", KEYS[KeyNumber]);
    for (int j = 0; j < key.size(); j++)
        std::cout << key.at(j);
    system("pause");
}