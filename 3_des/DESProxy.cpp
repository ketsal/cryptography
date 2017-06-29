#include "DESProxy.h"
bool File::FileExist()
{
    bool check = false;
    _reader.open(Path, ReadAsBinary);
    check = _reader.is_open();
    return !check;
}
bool File::Read()
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
            }
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
bool File::WriteData()
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

void DesProxy::EncryptECB(std::vector<unsigned char>& Data, const char *key)
{
    DES_string_to_key(key, &_key);
    DES_set_key_checked(&_key, &_schedule);
    encrypt_ecb(Data, DES_ENCRYPT,Des);
}
void DesProxy::DecryptECB(std::vector<unsigned char> &Data, const char *key)
{
    DES_string_to_key(key, &_key);
    DES_set_key_checked(&_key, &_schedule);
    encrypt_ecb(Data, DES_DECRYPT,Des);
}
void DesProxy::EncryptCBC(std::vector<unsigned char> &Data, const char *key)
{
    DES_string_to_key(key, &_key);
    DES_set_key_checked(&_key, &_schedule);
    encrypt_cfb_ofb_cbc(Data, DES_ENCRYPT,CBC,Des);
}
void DesProxy::DecryptCBC(std::vector<unsigned char> &Data, const char *key)
{
    DES_string_to_key(key, &_key);
    DES_set_key_checked(&_key, &_schedule);
    encrypt_cfb_ofb_cbc(Data, DES_DECRYPT, CBC, Des);
}
void DesProxy::EncryptCFB(std::vector<unsigned char> &Data, const char *key)
{
    DES_string_to_key(key, &_key);
    DES_set_key_checked(&_key, &_schedule);
    encrypt_cfb_ofb_cbc(Data, DES_ENCRYPT, CFB, Des);
}
void DesProxy::DecryptCFB(std::vector<unsigned char> &Data, const char *key)
{
    DES_string_to_key(key, &_key);
    DES_set_key_checked(&_key, &_schedule);
    encrypt_cfb_ofb_cbc(Data, DES_DECRYPT, CFB, Des);
}
void DesProxy::EncryptOFB(std::vector<unsigned char> &Data, const char *key)
{
    DES_string_to_key(key, &_key);
    DES_set_key_checked(&_key, &_schedule);
    encrypt_cfb_ofb_cbc(Data, DES_ENCRYPT, OFB, Des);
}
void DesProxy::DecryptOFB(std::vector<unsigned char> &Data, const char *key)
{
    DES_string_to_key(key, &_key);
    DES_set_key_checked(&_key, &_schedule);
    encrypt_cfb_ofb_cbc(Data, DES_DECRYPT, OFB, Des);
}
void DesProxy::encrypt_cfb_ofb_cbc(std::vector<unsigned char> &Data, int Mode, int CFBorOFBorCBC,bool TripleCheck)
{
    DES_cblock IV = { 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33 };
    unsigned int Size = Data.size();
    unsigned int PseudoSize = (Size + 7) / 8 * 8;
    unsigned char *Input = new unsigned char[Size];
    unsigned char *Output = new unsigned char[Size];
    unsigned char *InputforCBC = new unsigned char[PseudoSize];
    unsigned char *OutputforCBC = new unsigned char[PseudoSize];
    int num=0;
    if (CFBorOFBorCBC != CBC)
    {
        for (unsigned int i = 0; i < Size; i++)
        {
            Input[i] = Data.at(i);
        }
    }
    else
    {
        for (unsigned int i = 0; i < Size; i++)
        {
            if (i < Size)
            {
                InputforCBC[i] = Data.at(i);
            }
            else
            {
                InputforCBC[i] = NULL;
            }
        }
    }
    if (CFBorOFBorCBC == 0)
    {
        if (TripleCheck)
        {
            DES_ede3_cfb64_encrypt(Input, Output, Size, &_ks1, &_ks2, &_ks3, &IV, &num, Mode);
        }
        else
        {
            DES_cfb_encrypt(Input, Output, 8, Size, &_schedule, &IV, Mode);
        }
    }
    else if (CFBorOFBorCBC == 1)
    {
        if (TripleCheck)
        {
            DES_ede3_ofb64_encrypt(Input,Output,Size,&_ks1,&_ks2,&_ks3,&IV,&num);
        }
        else
        {
            DES_ofb_encrypt(Input, Output, 8, Size, &_schedule, &IV);
        }
    }
    else
    {
        if (TripleCheck)
        {
            DES_ede3_cbc_encrypt(InputforCBC,OutputforCBC,Size,&_ks1,&_ks2,&_ks3,&IV,Mode);
        }
        else
        {
            DES_ncbc_encrypt(InputforCBC, OutputforCBC, Size, &_schedule, &IV, Mode);
        }
    }
    if (CFBorOFBorCBC != CBC)
    {
        for (unsigned int i = 0; i < Size; i++)
        {
            if (Mode)
            {
                _EncData.push_back(Output[i]);
            }
            else
            {
                _DecData.push_back(Output[i]);
            }
        }
    }
    else
    {
        for (unsigned int i = 0; i < PseudoSize; i++)
        {
            if (Mode)
            {
                _EncData.push_back(OutputforCBC[i]);
            }
            else
            {
                _DecData.push_back(OutputforCBC[i]);
            }
        }
    }
    delete Input;
    delete Output;
}
void DesProxy::encrypt_ecb(std::vector<unsigned char> &Data, int Mode,bool TripleCheck)
{
    DES_cblock input;
    DES_cblock output;
    unsigned int Size = Data.size();
    unsigned int PseudoSize = (Data.size() + 7) / 8 * 8;
    unsigned int k = 0;
    for (unsigned int i = 0; i <PseudoSize; i++)
    {
        if (i<Size)
        {
            input[k] = Data.at(i);
        }
        else
        {
            input[k] = NULL;
        }
        k++;
        if (k == 8)
        {
            if (TripleCheck)
            {
                DES_ecb3_encrypt(&input, &output, &_ks1, &_ks2, &_ks3, Mode);
            }
            else
            {
                DES_ecb_encrypt(&input, &output, &_schedule, Mode);
            }
            for (unsigned int j = 0; j < 8; j++)
            {
                if (Mode)
                {
                    _EncData.push_back(output[j]);
                }
                else
                {
                    _DecData.push_back(output[j]);
                }
                input[j] = NULL;
            }
            k = 0;
        }
    }
}
void  DesProxy::EncryptTripleDesECB(std::vector<unsigned char>& Data, const char *key1, const char *key2, const char *key3)
{
    DES_string_to_key(key1, &_key1);
    DES_string_to_key(key2, &_key2);
    DES_string_to_key(key3, &_key3);
    DES_set_key_checked(&_key1, &_ks1);
    DES_set_key_checked(&_key2, &_ks2);
    DES_set_key_checked(&_key3, &_ks3);
    encrypt_ecb(Data, DES_ENCRYPT, TripleDes);
};
void  DesProxy::DecryptTripleDesECB(std::vector<unsigned char>& Data, const char *key1, const char *key2, const char *key3)
{
    DES_string_to_key(key1, &_key1);
    DES_string_to_key(key2, &_key2);
    DES_string_to_key(key3, &_key3);
    DES_set_key_checked(&_key1, &_ks1);
    DES_set_key_checked(&_key2, &_ks2);
    DES_set_key_checked(&_key3, &_ks3);
    encrypt_ecb(Data, DES_DECRYPT, TripleDes);
};
void  DesProxy::EncryptTripleDesCBC(std::vector<unsigned char>& Data, const char *key1, const char *key2, const char *key3)
{
    DES_string_to_key(key1, &_key1);
    DES_string_to_key(key2, &_key2);
    DES_string_to_key(key3, &_key3);
    DES_set_key_checked(&_key1, &_ks1);
    DES_set_key_checked(&_key2, &_ks2);
    DES_set_key_checked(&_key3, &_ks3);
    encrypt_cfb_ofb_cbc(Data, DES_ENCRYPT, CBC, TripleDes);
}; 
void  DesProxy::DecryptTripleDesCBC(std::vector<unsigned char>& Data, const char *key1, const char *key2, const char *key3)
{
    DES_string_to_key(key1, &_key1);
    DES_string_to_key(key2, &_key2);
    DES_string_to_key(key3, &_key3);
    DES_set_key_checked(&_key1, &_ks1);
    DES_set_key_checked(&_key2, &_ks2);
    DES_set_key_checked(&_key3, &_ks3);
    encrypt_cfb_ofb_cbc(Data, DES_DECRYPT, CBC, TripleDes);
};
void  DesProxy::EncryptTripleDesCFB(std::vector<unsigned char>& Data, const char *key1, const char *key2, const char *key3)
{
    DES_string_to_key(key1, &_key1);
    DES_string_to_key(key2, &_key2);
    DES_string_to_key(key3, &_key3);
    DES_set_key_checked(&_key1, &_ks1);
    DES_set_key_checked(&_key2, &_ks2);
    DES_set_key_checked(&_key3, &_ks3);
    encrypt_cfb_ofb_cbc(Data, DES_ENCRYPT, CFB, TripleDes);
};
void  DesProxy::DecryptTripleDesCFB(std::vector<unsigned char>& Data, const char *key1, const char *key2, const char *key3)
{
    DES_string_to_key(key1, &_key1);
    DES_string_to_key(key2, &_key2);
    DES_string_to_key(key3, &_key3);
    DES_set_key_checked(&_key1, &_ks1);
    DES_set_key_checked(&_key2, &_ks2);
    DES_set_key_checked(&_key3, &_ks3);
    encrypt_cfb_ofb_cbc(Data, DES_DECRYPT, CFB, TripleDes);
};
void  DesProxy::EncryptTripleDesOFB(std::vector<unsigned char>& Data, const char *key1, const char *key2, const char *key3)
{
    DES_string_to_key(key1, &_key1);
    DES_string_to_key(key2, &_key2);
    DES_string_to_key(key3, &_key3);
    DES_set_key_checked(&_key1, &_ks1);
    DES_set_key_checked(&_key2, &_ks2);
    DES_set_key_checked(&_key3, &_ks3);
    encrypt_cfb_ofb_cbc(Data, DES_ENCRYPT, OFB, TripleDes);
};
void  DesProxy::DecryptTripleDesOFB(std::vector<unsigned char>& Data, const char *key1, const char *key2, const char *key3)
{
    DES_string_to_key(key1, &_key1);
    DES_string_to_key(key2, &_key2);
    DES_string_to_key(key3, &_key3);
    DES_set_key_checked(&_key1, &_ks1);
    DES_set_key_checked(&_key2, &_ks2);
    DES_set_key_checked(&_key3, &_ks3);
    encrypt_cfb_ofb_cbc(Data, DES_DECRYPT, OFB,TripleDes);
};