#define __STDC_WANT_LIB_EXT1__ 1
#define  _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <vector>
#include <map>
#include "../include/openssl/sha.h"
#include <string>
#include <time.h>
#include <Strsafe.h>
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
    int alpha=1;
    bool _hashfunc;
    int _size;
    int hashbysha(char *Key)
    {
        int size = sizeof(Key);
        unsigned char digest[SHA256_DIGEST_LENGTH];
        SHA256_CTX ctx;
        SHA256_Init(&ctx);
        SHA256_Update(&ctx, Key, size);
        SHA256_Final(digest, &ctx);
        return digest[15];
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
    HashTable(bool Hashfunc, int size)
    {
        this->_hashfunc = Hashfunc;
        this->_size = size;
        for (int i = 0; i < _size; i++)
        {
            _table[i] = NULL;
        }
    }
    int GetAlpha()
    {
        return alpha;
    }
    void PushElem(WIN32_FIND_DATAA &Fileinfo, std::string Fullpath)
    {
        char * path = new char[Fullpath.length() + 1];
        std::strcpy(path, Fullpath.c_str());
        int hash = 0;
        if (_hashfunc)
        {
            hash=hashbysha(path);
        }   
        else
        {
            hash = hashbymyhash(path)%65536;
        }
        FileInformation *file = new FileInformation(Fileinfo, Fullpath);
        FileInformation *place = _table[hash];
        int counter = 1;
        if (place == NULL)
        {
            _table[hash] = file;
            return;
        }
        while (place->GetNext() != NULL)
        {
            place = place->GetNext();
            counter++;
        }
        if (counter > alpha)
        {
            alpha = counter;
        }
        place->SetNext(file);
    }
    void printFind(std::string Fullpath)
    {
        bool cont = true;
        char * path = new char[Fullpath.length() + 1];
        std::strcpy(path, Fullpath.c_str());
        int hash = hashbysha(path);
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
        printf("File name:%s\nFile alternative name:%s\nFile atribute:%u\nFile size(high):%u\nFile size low:%u\n",
            &find->GetFileInfo().cFileName[0]
            , &find->GetFileInfo().cAlternateFileName[0]
            , find->GetFileInfo().dwFileAttributes
            , find->GetFileInfo().nFileSizeHigh
            , find->GetFileInfo().nFileSizeLow
            );
    }
};
void FindFiles(std::string Path, HashTable &table)
{
    WIN32_FIND_DATAA wfd;
    char * path = new char[Path.length() + 1];
    std::strcpy(path, Path.c_str());
    path = strcat(path, "\\*");
    HANDLE const hFind = FindFirstFileA(path, &wfd);
    path[strlen(path) - strlen(strstr(path, "*"))] = '\0';
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
                    strcat(path, &wfd.cFileName[0]);
                    FindFiles(path,table);
                    path[strlen(path) - strlen(&wfd.cFileName[0])] = '\0';
                }
            }
            else
            {
                strcat(path, &wfd.cFileName[0]);
                std::cout << &wfd.cFileName[0] <<" "<<table.GetAlpha()<<std::endl;
                table.PushElem(wfd, path);
                path[strlen(path) - strlen(&wfd.cFileName[0])] = '\0';
            }
        } while (FindNextFileA(hFind, &wfd));

        FindClose(hFind);
    }
}
int main()
{
    char path[MAX_PATH] = "C:\\program files";
    std::string path2= "C:\\stepan\\keke\\1039\\stepan.txt";
    HashTable table(MyHash,65536);
    FindFiles(path,table);
    table.printFind(path2);
    std::cout << table.GetAlpha();
    system("pause");
    return 0;
}
