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
        bool eof = true;
        _reader.open(Path, ReadAsBinary);
        char temp = ' ';
        if (_reader.is_open())
        {
            while (eof)
            {
                _reader.get(temp);
                if (_reader.eof())
                {
                    eof = false;
                };
                if (eof)
                {
                    _data.push_back(temp);
                }
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
bool PathIsValid(std::string mode, std::string path)
{
    bool check = true;
    while (check)
    {
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
    return check;
}
void main(int argc, char* argv[])
{
    bool check = true;
    std::string path = "../../docs/plain.docx";
    std::string path2 = "../../docs/keyforotp.docx";
    std::string path3 = "../../docs/cipher.docx";
    std::string path4 = "../../docs/decrypted.docx";
    PathIsValid("plain",path);
    PathIsValid("key",path2);
    PathIsValid("cipher",path3);
    PathIsValid("decrypted",path4);
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
    std::cout << "Encrypting...";
    pfile.encode(keyfile, cfile);
    system("cls");
    std::cout << "Decrypting...";
    cfile.decode(keyfile, cfile2);
    system("cls");
    std::cout << "Done, press any key to exit";
    _getch();
}