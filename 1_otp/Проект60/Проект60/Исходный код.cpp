#include <fstream>
#include <vector>
#include "iostream"
#include <ctime>
#include<string>
#include <conio.h>
#define ReadAsBinary  std::ios::in | std::ios::binary
#define WriteAsBinary std::ios::binary | std::ios::out | std::ios::trunc

class File
{
private:
    std::vector<char> _data;
    std::fstream _reader;
    std::fstream _writer;
public:
    std::string	Path;
    File(std::string &path)
        :Path(path)
    {

    }
    ~File()
    {

    }
    bool FileExist()
    {
        bool check = false;
        _reader.open(Path, ReadAsBinary);
        check = _reader.is_open();
        return !check;
    }
    bool Read()
    {
        _reader.open(Path, ReadAsBinary);
        char temp = ' ';
        if (_reader.is_open())
        {
            while (!_reader.eof())
            {
                _reader.get(temp);
                if (_reader.eof()) break;
                _data.push_back(temp);
            }
            _reader.close();
            return false;
        }
        else
            return true;
    }
    bool WriteData()
    {
        _writer.open(Path, WriteAsBinary);
        char temp = ' ';
        if (_writer.is_open())
        {
            for (unsigned int i = 0; i < _data.size(); i++)
            {
                temp = _data.at(i);
                _writer << temp;
            }
            _writer.close();
            return false;
        }
        else
            return true;
    }
    void KeyGen(int psize)
    {
        srand(time(NULL));
        for (int i = 0; i < psize; i++)
        {
            _data.push_back(0 + rand() % 255);
        }
    }
    std::vector<char> &GetData() { return _data; }
    void encode(File &key, File &cfile)
    {
        if (cfile.Path == Path)
        {
            _data.clear();
        }
        for (unsigned int i = 0; i < _data.size(); i++)
            cfile.GetData().push_back(_data.at(i) ^ key.GetData().at(i));
        cfile.WriteData();
    }
    void decode(File &key, File &pfile)
    {
        encode(key, pfile);
    }
};
std::string PathIsValid(std::string mode)
{
    std::string path;
    bool check = true;
    std::string Please = "Enter path to " + mode + " file \n";
    while (check)
    {
        std::cout << Please;
        std::getline(std::cin, path);
        File checkfile(path);
        check = checkfile.FileExist();
        if (check)
        {
            std::cout << "No such file or directory:" << path;
            std::cout << "\nPress any key to continue";
            _getch();
        }
        system("cls");
    }
    return path;
}
void main(int argc, char* argv[])
{
    bool check = true;
    std::string path = "";
    std::string path2 = "";
    std::string path3 = "";
    std::string path4 = "";
    path = PathIsValid("plain");
    path2 = PathIsValid("key");
    path3 = PathIsValid("cipher");
    path4 = PathIsValid("decrypted");
    File pfile(path);
    File keyfile(path2);
    File cfile(path3);
    File cfile2(path4);
    std::cout << "Reading plain text...";
    pfile.Read();
    system("cls");
    std::cout << "Generating key...";
    keyfile.KeyGen(pfile.GetData().size());
    keyfile.WriteData();
    keyfile.GetData().clear();
    keyfile.Read();
    system("cls");
    std::cout << "Encoding...";
    pfile.encode(keyfile, cfile);
    system("cls");
    std::cout << "Decoding...";
    cfile.decode(keyfile, cfile2);
    system("cls");
    std::cout << "Done, press any key to exit";
    _getch();
}