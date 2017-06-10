#include "file.h"
bool File::FileExist()
{
    bool check = false;
    _reader.open(_Path, ReadAsBinary);
    check = _reader.is_open();
    _reader.close();
    return !check;
}
bool  File::Read()
{ 
    bool eof = true;
    _reader.open(_Path, ReadAsBinary);
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
bool  File::WriteData()
{
    _writer.open(_Path, WriteAsBinary);
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