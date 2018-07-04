#include <string>
#include <iostream>
#include <random>
#include <ctime>
#include <algorithm>
#include <iterator>
#include "../shared/DESProxy.h"
#include "../shared/file.h"
#include <map>
#include <thread>
#include <mutex>
std::mutex locker;
void ResultOfEncryption(
     std::map <std::vector<unsigned char>,std::string > &MAP
    ,unsigned char **KEYS
    ,std::vector<unsigned char> data
    ,int start
    ,int end
    ,int DecOrEnc)
{
    DesProxy DoubleDES;
    std::string KEY = "1234567";
    for (int i = start; i < end; i++)
    {
        DoubleDES.GetEncData().clear();
        DoubleDES.GetDecData().clear();
        locker.lock();
        if (DecOrEnc == 1)
        {
            for (int j = 0; j < 7; j++)
            {
                KEY[j] = KEYS[i][j];
            }
        }
        else
        {
            for (int j = 0; j < 7; j++)
            {
                KEY[j] = KEYS[i][j + 7];
            }
        }
        locker.unlock();
        const char* charsKEY = KEY.data();
        const  char* KEYbytes = reinterpret_cast<const char*>(charsKEY);
        if (DecOrEnc==1)
        {
            DoubleDES.EncryptCFB(data, KEYbytes);
        }
        else
        {
            DoubleDES.DecryptCFB(data, KEYbytes);
        }
        std::vector<unsigned char> temp;
        if (DecOrEnc == 1)
        {
            temp = DoubleDES.GetEncData();
        }
        else
        {
            temp = DoubleDES.GetDecData();
        }
        MAP.insert(std::pair<std::vector<unsigned char>,std::string>
            (temp, KEY));
        temp.clear();
    }
}
void CreateKeys(unsigned char **Keys, int numberofkeys)
{
    int j = 13;
    bool cont = true;
    for (int i = 0; i < 14; i++)
    {
        Keys[0][i] = 0 + rand() % 255;
    }
    for (int i = 1; i < numberofkeys; i++)
    {
        cont = true;
        j = 13;
        for (int k = 0; k < 14; k++)
        {
            Keys[i][k] = Keys[i - 1][k];
        }
        Keys[i][j] = Keys[i - 1][j] + 1;
        if (Keys[i][j] == 0)
        {
            while (Keys[i][j] == 0)
            {
                j--;
                Keys[i][j] = Keys[i - 1][j] + 1;
            }
        }
    }
}
bool FindPairs(
      std::map <std::vector<unsigned char>, std::string> &ENCMAP
    , std::map <std::vector<unsigned char>, std::string> &DECMAP1
    , std::map <std::vector<unsigned char>, std::string> &DECMAP2
    , std::map <std::vector<unsigned char>, std::string> &DECMAP3
    , std::map <std::vector<unsigned char>, std::string> &DECMAP4
    , std::vector<unsigned char> &Data
    , std::vector<unsigned char> &Key)
{
    bool found = false;
    for (auto it = ENCMAP.begin(); it != ENCMAP.end(); it++)
    {
        if ((DECMAP1.count(it->first) > 0) || (DECMAP2.count(it->first) > 0) || (DECMAP3.count(it->first) > 0) || (DECMAP4.count(it->first) > 0))
        {
            Data = it->first;
            for (int j = 0; j < 7; j++)
            {
                if (j < 7)
                {
                    Key.push_back(it->second[j]);
                }
            }
            found = true;
        }
    }
    return found;
}
void main()
{
    DesProxy DoubleDES;
    std::string KEY1 = "1234567";
    std::string KEY2 = "1234567";
    std::string path = "../docs/plainforDDES.txt";
    std::string path2 = "../docs/cipherforDDES.txt";
    File plain(path);
    File crypted(path2);
    srand(time(NULL));
    int numberofkeys = 65536;
    bool cont = true;
    bool found = false;
    int KeyNumber = rand() % numberofkeys + 0;
    unsigned char **KEYS = new  unsigned char*[numberofkeys];
    for (int count = 0; count < numberofkeys; count++)
    {
        KEYS[count] = new unsigned char[14];
    }
    plain.Read();
    CreateKeys(KEYS, numberofkeys);

    for (int i = 0; i < 7; i++)
    {
        KEY1[i] = KEYS[KeyNumber][i];
    }
    for (int i = 0; i < 7; i++)
    {
        KEY2[i] = KEYS[KeyNumber][i + 7];
    }
    const char* KEY1chars = KEY1.data();
    const  char* KEY1bytes = reinterpret_cast<const char*>(KEY1chars);
    const char* KEY2chars = KEY2.data();
    const  char* KEY2bytes = reinterpret_cast<const char*>(KEY2chars);
    DoubleDES.EncryptCFB(plain.GetData(), KEY1bytes);
    std::vector<unsigned char> temp;
    temp = DoubleDES.GetEncData();
    DoubleDES.GetEncData().clear();
    DoubleDES.EncryptCFB(temp, KEY2bytes);
    crypted.GetData() = DoubleDES.GetEncData();
    crypted.WriteData();
    DoubleDES.GetEncData().clear();
    temp.clear();
    std::map <  std::vector<unsigned char>, std::string > ENCMAP1;
    std::map <  std::vector<unsigned char>, std::string > DECMAP1;
    std::map <  std::vector<unsigned char>, std::string > ENCMAP2;
    std::map <  std::vector<unsigned char>, std::string > DECMAP2;
    std::map <  std::vector<unsigned char>, std::string > ENCMAP3;
    std::map <  std::vector<unsigned char>, std::string > DECMAP3;
    std::map <  std::vector<unsigned char>, std::string > ENCMAP4;
    std::map <  std::vector<unsigned char>, std::string > DECMAP4;
    std::thread Thread1(ResultOfEncryption, std::ref(ENCMAP1), KEYS, plain.GetData(), 0, 16384, 1);
    std::thread Thread2(ResultOfEncryption, std::ref(ENCMAP2), KEYS, plain.GetData(), 16384, 32768, 1);
    std::thread Thread3(ResultOfEncryption, std::ref(ENCMAP3), KEYS, plain.GetData(), 32768, 49152, 1);
    std::thread Thread4(ResultOfEncryption, std::ref(ENCMAP4), KEYS, plain.GetData(), 49152, numberofkeys, 1);
    std::thread Thread5(ResultOfEncryption, std::ref(DECMAP1), KEYS, crypted.GetData(), 0, 16384, 0);
    std::thread Thread6(ResultOfEncryption, std::ref(DECMAP2), KEYS, crypted.GetData(), 16384, 32768, 0);
    std::thread Thread7(ResultOfEncryption, std::ref(DECMAP3), KEYS, crypted.GetData(), 32768, 49152, 0);
    std::thread Thread8(ResultOfEncryption, std::ref(DECMAP4), KEYS, crypted.GetData(), 49152, numberofkeys, 0);
    std::cout << "Forming tables...\n";
    Thread1.join();
    Thread2.join();
    Thread3.join();
    Thread4.join();
    Thread5.join();
    Thread6.join();
    Thread7.join();
    Thread8.join();
    system("cls");
    std::cout << "Searching for pairs...\n";
    std::vector<unsigned char> Data;
    std::vector<unsigned char> Key;
    found=FindPairs(ENCMAP1, DECMAP1, DECMAP2, DECMAP3, DECMAP4, Data, Key);
    if (!found)
    {
        found = FindPairs(ENCMAP2, DECMAP1, DECMAP2, DECMAP3, DECMAP4, Data, Key);
    }
    if (!found)
    {
        found = FindPairs(ENCMAP3, DECMAP1, DECMAP2, DECMAP3, DECMAP4, Data, Key);
    }
    if (!found)
    {
        found = FindPairs(ENCMAP4, DECMAP1, DECMAP2, DECMAP3, DECMAP4, Data, Key);
    }
    int Decmapnumber = 0;
    cont = true;
    if ((DECMAP1.count(Data) > 0)&&cont)
    {
        Decmapnumber = 1;
        cont = false;
    }
    if ((DECMAP2.count(Data) > 0) && cont)
    {
        Decmapnumber = 2;
        cont = false;
    }
    if ((DECMAP3.count(Data) >0) && cont)
    {
        Decmapnumber = 3;
        cont = false;
    }
    if ((DECMAP4.count(Data) > 0) && cont)
    {
        Decmapnumber = 4;
        cont = false;
    }
    for (int j = 0; j < 7; j++)
    {
        if (Decmapnumber == 1)
            Key.push_back(DECMAP1.at(Data)[j]);
        if (Decmapnumber == 2)
            Key.push_back(DECMAP2.at(Data)[j]);
        if (Decmapnumber == 3)
            Key.push_back(DECMAP3.at(Data)[j]);
        if (Decmapnumber == 4)
            Key.push_back(DECMAP4.at(Data)[j]);
    }
    system("cls");
    printf("Found the key\n");
    for (int j = 0; j < Key.size(); j++)
    {
        printf("%X", Key.at(j));
    }
    printf("\nThe key used for encryption\n");
    for (int j = 0; j < 14; j++)
    {
        printf("%X", KEYS[KeyNumber][j]);
    }
    printf("\n");
    std::cin >> numberofkeys;
}