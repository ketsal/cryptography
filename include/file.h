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
    std::vector<unsigned char> _data;
    std::fstream _reader;
    std::fstream _writer;
    std::string	_Path;
public:
    File(std::string &path)
        :_Path(path)
    {

    }
    ~File()
    {

    }
    bool FileExist();
    bool Read();
    bool WriteData();
    std::vector<unsigned char> &GetData()
    {
      return _data;
    }
};