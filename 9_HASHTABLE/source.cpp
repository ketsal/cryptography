#define  _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <vector>
#include <map>
#include "../include/openssl/sha.h"
#include <string>
#include <time.h>
#include <Strsafe.h>
#include <sstream>
#include "../include/file.h"
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
        std::string::size_type sz;
        unsigned int HASH=0;
        unsigned char digest[SHA256_DIGEST_LENGTH];
        SHA256_CTX ctx;
        SHA256_Init(&ctx);
        SHA256_Update(&ctx, &Key, Key.size());
        SHA256_Final(digest, &ctx);
        std::string output = "";
        for (int i = 0; i < 32; i++) 
        {
            output += to_hex(digest[i]);
        }
        for (int i = 0; i<output.size(); i++)
        {
            HASH += output[i];
        }
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
        std::cout << rowcounter << " " << elementscount << "\n";
        alphaav = elementscount*1.0 / rowcounter;
        return alphaav;
    }
    void PushElem(WIN32_FIND_DATAA &Fileinfo, std::string Fullpath, bool Hashfunc)
    {
        _hashfunc = Hashfunc;
        char * path = new char[Fullpath.length()+1];
        std::strcpy(path, Fullpath.c_str());
        unsigned int  hash = 0;
        if (_hashfunc)
        {
            hash = hashbysha(path);
            hash = hash % 65536;
        }   
        else
        {
            hash = hashbymyhash(path) % 65536;
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
        char * path = new char[Fullpath.length()+1];
        std::strcpy(path, Fullpath.c_str());
        int hash = 255;
        if (_hashfunc)
        {
            hash = hashbysha(path) % 65536;
        }
        else
        {
            hash = hashbymyhash(path)%65536;
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
int main()
{
    char path[MAX_PATH] = "C:\\windows\\system32";
    std::string path2= "C:\\stepan\\keke\\1039\\stepan.txt";
    std::string path1 = "../docs/table.txt";
    HashTable table;
    File file(path1);
    FindFiles(path, table, MyHash);
    printf("%.3f ", table.GetAlphaAv());
    table.PrintInFile(file.GetData());
    file.WriteData();
   
   /* table.Rebuild(SHA);
    std::cout << "\n";
    printf("%.3f ", table.GetAlphaAv());
    std::cout << "\n";
    HashTable table2;
    FindFiles(path, table2, SHA);
    printf("%.3f ", table2.GetAlphaAv());*/
    system("pause");
    return 0;
}
//printf("File name:%s\nFile alternative name:%s\nFile atribute:%u\nFile size(high):%u\nFile size low:%u\n",
//    &find->GetFileInfo().cFileName[0]
//    , &find->GetFileInfo().cAlternateFileName[0]
//    , find->GetFileInfo().dwFileAttributes
//    , find->GetFileInfo().nFileSizeHigh
//    , find->GetFileInfo().nFileSizeLow
//    );