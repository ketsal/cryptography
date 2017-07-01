#include "../shared/mpirxx.h"
#include <iostream>
#include <Windows.h>
#include <vector>
#include <map>
#include "../shared/openssl/sha.h"
#include <string>
#include <time.h>
#include <Strsafe.h>
#include <sstream>
#include "../shared/file.h"
#define SHA true
#define MyHash false
class FileInformation
{
private:
    WIN32_FIND_DATAA _fileinfo;
    std::string _fullpath;
    FileInformation *_next;
public:
    FileInformation(WIN32_FIND_DATAA Fileinfo, std::string Fullpath)
    {
        this->_fileinfo = Fileinfo;
        this->_fullpath = Fullpath;
        this->_next = NULL;
    }
    FileInformation()
    {
        this->_next = NULL;
    }
    ~FileInformation()
    {
        if (this->_next != NULL)
        {
            delete this->_next;
        }
    }
    std::string  &GetKey()
    {
        return _fullpath;
    }
    WIN32_FIND_DATAA& GetFileInfo()
    {
        return _fileinfo;
    }
    FileInformation *GetNext()
    {
        return this->_next;
    }
    void SetNext(FileInformation *next)
    {
        this->_next = next;
    }
};
class HashTable
{
private:
    FileInformation *_table[65536];
    bool _hashfunc;
    std::string to_hex(unsigned char s) 
    {
        std::stringstream ss;
        ss << std::hex << (int)s;
        return ss.str();
    }
    int hashbysha(std::string Key)
    {
        unsigned int HASH=0;
        unsigned char digest[SHA256_DIGEST_LENGTH];
        SHA256_CTX ctx;
        SHA256_Init(&ctx);
        SHA256_Update(&ctx, &Key, Key.size());
        SHA256_Final(digest, &ctx);
        std::string output = "";
        char tonumber[65];
        for (int i = 0; i < 32; i++) 
        {
            output += to_hex(digest[i]);
        }
        for (int i = 0; i < output.size(); i++)
        {
            tonumber [i]=output[i];
        }
        tonumber[output.size()] = '\0';
        mpz_t a;
        mpz_init(a);
        mpz_t b;
        mpz_init(b);
        mpz_init_set_str(a, tonumber, 16);
        mpz_mod_ui(b, a, 65536);
        HASH=mpz_get_ui(b);
        return HASH;
    }
   int hashbymyhash(std::string Key)
    {
        int HASH = 0;
        for (int i = 0; i < Key.size(); i++)
        {
            HASH += Key[i];
            HASH += (Key[i] % (i + 1));
            HASH ^= (Key[i] >> 5);
        }
        return HASH;
    }
public:
    HashTable()
    {
        for (unsigned int i = 0; i < 65536; i++)
        {
            _table[i] = NULL;
        }
    }
    float GetAlphaAv()
    {
        float alphaav=0;
        int rowcounter=0;
        int elementscount = 0;
        FileInformation *temp = NULL;
        for (unsigned int i = 0; i < 65536; i++)
        {
            temp = _table[i];
            if (temp != NULL)
            {
                rowcounter++;
                while (temp!= NULL)
                {
                    elementscount++;
                    temp = temp->GetNext();
                }
            }
        }
        alphaav = elementscount*1.0 / rowcounter;
        return alphaav;
    }
    void PushElem(WIN32_FIND_DATAA &Fileinfo, std::string Fullpath, bool Hashfunc)
    {
        _hashfunc = Hashfunc;
        unsigned int  hash = 0;
        if (_hashfunc)
        {
            hash = hashbysha(Fullpath);
            hash = hash % 65536;
        }   
        else
        {
            hash = hashbymyhash(Fullpath) % 65536;
        }
        FileInformation *file = new FileInformation(Fileinfo, Fullpath);
        FileInformation *place = _table[hash];
        if (place == NULL)
        {
            _table[hash] = file;
            return;
        }
        while (place->GetNext() != NULL)
        {
            place = place->GetNext();
        }
        place->SetNext(file);
    }
    void Rebuild(bool Hashfunc)
    {
        FileInformation *temp = NULL;
        std::vector<std::string> names;
        int hash = 0;
        bool checkloop = true;
        for (unsigned int i = 0; i < 65536; i++)
        {
            temp = _table[i];
            if (temp != NULL)
            {
                names.clear();
                names.push_back(temp->GetKey());
                _table[i] = temp->GetNext();
                checkloop = true;
                while ((temp!= NULL) && checkloop)
                {
                    PushElem(temp->GetFileInfo(), temp->GetKey(), Hashfunc);
                    temp = _table[i];      
                    if (temp != NULL)
                    {                      
                        for (unsigned int j = 0; j < names.size(); j++)
                        {
                            if (temp->GetKey() == names.at(j))
                            {
                                checkloop = false;
                            }
                        }
                        if (checkloop)
                        {
                            _table[i] = temp->GetNext();
                        }
                        names.push_back(temp->GetKey());
                    }
                }
            }
        }
    }
    void PrintInFile(std::vector<unsigned char> &data)
    {
        FileInformation *temp = NULL;
        for (unsigned int i = 0; i < 65536; i++)
        {
            if (_table[i] != NULL)
            {
                std::string number = std::to_string(i);
                temp = _table[i];
                for (int j = 0; j < number.size(); j++)
                {
                    data.push_back(number[j]);
                }
                data.push_back(' ');
                while (temp != NULL)
                {
                    for (unsigned int j = 0; j < temp->GetKey().size(); j++)
                    {
                        data.push_back(temp->GetKey()[j]);
                    }
                    data.push_back('-');
                    data.push_back('>');
                    temp = temp->GetNext();
                }
                data.push_back(0x0A);
            }
        }
    }
    void printFind(std::string Fullpath)
    {
        bool cont = true;
        int hash = 255;
        if (_hashfunc)
        {
            hash = hashbysha(Fullpath) % 65536;
        }
        else
        {
            hash = hashbymyhash(Fullpath) % 65536;
        }
        FileInformation *find = _table[hash];
        if (!find)
        {
            std::cout << "No such file or directory";
            return;
        }
        while ((find->GetKey() != Fullpath) && cont)
        {
            if (!find->GetNext())
            {
                std::cout << "Element not found" << std::endl;
                cont = false;
            }
            if (cont)
            {
                find = find->GetNext();
            }
        }
    }
};
void FindFiles(std::string Path, HashTable &table,bool Hashfunc)
{
    WIN32_FIND_DATAA wfd;
    std::string path = Path + "\\*";
    HANDLE const hFind = FindFirstFileA(path.c_str(), &wfd);
    bool checkdots=true;
    if (INVALID_HANDLE_VALUE != hFind)
    {
        do
        {
            checkdots = true;
            if (strlen(&wfd.cFileName[0]) == 1 && strchr(wfd.cFileName, '.') != NULL)
                checkdots = false;
            if (strlen(&wfd.cFileName[0]) == 2 && strstr(wfd.cFileName, "..") != NULL)
                checkdots = false;
            if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if (checkdots)
                {
                    path = Path + "\\" + std::string(&wfd.cFileName[0]);
                    FindFiles(path, table, Hashfunc);
                }
            }
            else
            {
                path = Path + "\\" + std::string(&wfd.cFileName[0]);
                table.PushElem(wfd, path,Hashfunc);
            }
        } while (FindNextFileA(hFind, &wfd));

        FindClose(hFind);
    }
}
bool ChooseFunc()
{
    bool HashFunc = false;
    int Func = 0;
    while (Func > 2 || Func < 1)
    {
        std::cout << "Choose hash func. 1 for MyHash, 2 for SHA\n";
        std::cin >> Func;
        system("cls");
    }
    if (Func == 1)
    {
        HashFunc = MyHash;
    }
    else
    {
        HashFunc = SHA;
    }
    return HashFunc;
}
int main()
{
    std::string path = "C:\\windows\\system32";
    std::string path2= "C:\\stepan\\keke\\1039\\stepan.txt";
    std::string path1 = "../docs/table.txt";
    HashTable table;
    File file(path1);
    unsigned int start=0;
    unsigned int end=0;
    double buildtime=0;
    double rebuildtime = 0;
    double AlphaMax = 1.8;
    bool HashFunc = false;
    int Func=0;
    double av=0;
    HashFunc=ChooseFunc();
    system("cls");
    std::cout << "Building a table\n";
    start = clock();
    FindFiles(path, table, HashFunc);
    end = clock();
    system("cls");
    buildtime = (end - start)*1.0/1000;
    av = table.GetAlphaAv();
    bool cont=true;
    if (av > AlphaMax)
    {
        std::cout << "Alpha average too big\n";
        while (cont)
        {
            bool temp = ChooseFunc();
            if (HashFunc == temp)
            {
                std::cout << "Func was used before choose other\n";
            }
            else
            {
                HashFunc = temp;
                cont = false;
            }
        }
       
    }
    system("cls");
    if (!cont)
    {
        std::cout << "Rebuilding table\n";
        start = clock();
        table.Rebuild(HashFunc);
        end = clock();
        rebuildtime = (end - start)*1.0/1000;
    }
    system("cls");
    std::cout << "Writing table into file\n";
    table.PrintInFile(file.GetData());
    file.WriteData();
    system("cls");
    printf("%Build time %.3f\n", buildtime);
    if (!cont)
    {
        printf("%Rebuild time %.3f\n", rebuildtime);
    }
    system("pause");
    return 0;
}