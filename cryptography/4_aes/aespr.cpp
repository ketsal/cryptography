#include "aespr.h"
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
void AesProxy::EncryptCTR(std::vector<unsigned char> &Data, const unsigned char *key)
{
    AES_set_encrypt_key(key, 128, &_EncKey);
    encrypt_cfb_ofb_cbc_ctr(Data, AES_ENCRYPT, CTR);
}
void AesProxy::DecryptCTR(std::vector<unsigned char> &Data, const unsigned char *key)
{
    AES_set_encrypt_key(key, 128, &_DecKey);
    encrypt_cfb_ofb_cbc_ctr(Data, AES_DECRYPT, CTR);
}
void AesProxy::EncryptECB(std::vector<unsigned char> &Data, const unsigned char *key)
{
    AES_set_encrypt_key(key, 128, &_EncKey);
    encrypt_ecb(Data, AES_ENCRYPT);
}
void AesProxy::DecryptECB(std::vector<unsigned char> &Data, const unsigned char *key)
{
    AES_set_decrypt_key(key, 128, &_DecKey);
    encrypt_ecb(Data, AES_DECRYPT);
}
void AesProxy::EncryptCBC(std::vector<unsigned char> &Data, const unsigned char *key)
{
    AES_set_encrypt_key(key, 128, &_EncKey);
    encrypt_cfb_ofb_cbc_ctr(Data, AES_ENCRYPT, CBC);
}
void AesProxy::DecryptCBC(std::vector<unsigned char> &Data, const unsigned char *key)
{
    AES_set_decrypt_key(key, 128, &_DecKey);
    encrypt_cfb_ofb_cbc_ctr(Data, AES_DECRYPT, CBC);
}
void AesProxy::EncryptCFB(std::vector<unsigned char> &Data, const unsigned char *key)
{
    AES_set_encrypt_key(key, 128, &_EncKey);
    encrypt_cfb_ofb_cbc_ctr(Data, AES_ENCRYPT,CFB);
}
void AesProxy::DecryptCFB(std::vector<unsigned char> &Data, const unsigned char *key)
{
    AES_set_encrypt_key(key, 128, &_DecKey);
    encrypt_cfb_ofb_cbc_ctr(Data, AES_DECRYPT,CFB);
}
void AesProxy::EncryptOFB(std::vector<unsigned char> &Data, const unsigned char  *key)
{
    AES_set_encrypt_key(key, 128, &_EncKey);
    encrypt_cfb_ofb_cbc_ctr(Data, AES_ENCRYPT, OFB);
}
void AesProxy::DecryptOFB(std::vector<unsigned char> &Data, const unsigned char *key)
{
    AES_set_encrypt_key(key, 128, &_DecKey);
    encrypt_cfb_ofb_cbc_ctr(Data, AES_DECRYPT, OFB);
}
void AesProxy::encrypt_ecb(std::vector<unsigned char> &Data, int Mode)
{
    unsigned int Size = Data.size();
    unsigned int PseudoSize = (Size + 15) / 16 * 16;
    unsigned char Input[16];
    unsigned char Output[16];
    unsigned k = 0;
    for (int i = 0; i < PseudoSize; i++)
    {
        if (i < Size)
        {
            Input[k] = Data.at(i);
        }
        else
        {
            Input[k] = NULL;
        }
        k++;
        if (k == 16)
        {
            if (Mode == 1)
            {
                AES_ecb_encrypt(Input, Output, &_EncKey, Mode);
            }
            else 
            {
                AES_ecb_encrypt(Input, Output, &_DecKey, Mode);
            }
            for (int i = 0; i < 16; i++)
            {
                if (Mode == 1)
                {
                    _EncData.push_back(Output[i]);
                }
                else
                {
                    _DecData.push_back(Output[i]);
                }
            }
            k = 0;
        }
    }
}
void AesProxy::encrypt_cfb_ofb_cbc_ctr(std::vector<unsigned char> &Data, int Mode, int CFBorOFBorCBCorCTR)
{
    unsigned int Size = Data.size();
    unsigned int PseudoSize = (Size + 15) / 16 * 16;
    unsigned char *Input= new unsigned char[Size];
    unsigned char *Output = new unsigned char[Size];
    unsigned char *OutputforCBC = new unsigned char[PseudoSize];
    unsigned char *InputforCBC = new unsigned char[PseudoSize];
    unsigned char IV[AES_BLOCK_SIZE] = "1qa$gv[ú567*12^";
    unsigned char ecount[AES_BLOCK_SIZE] = "Y}d1_-÷v3^'215=";
    unsigned int numCTR = 0;
    int num = 0;
    if (CFBorOFBorCBCorCTR != CBC)
    {
        for (int i = 0; i < Size; i++)
        {
            Input[i] = Data.at(i);
        }
    }
    else
    {
        for (int i = 0; i < PseudoSize; i++)
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
    if (Mode == 1)
    {
        switch (CFBorOFBorCBCorCTR)
        {
        case CFB:AES_cfb128_encrypt(Input, Output, Size, &_EncKey, IV, &num, Mode); break;
        case OFB:AES_ofb128_encrypt(Input, Output, Size, &_EncKey, IV, &num); break;
        case CBC:AES_cbc_encrypt(InputforCBC, OutputforCBC, PseudoSize, &_EncKey, IV, Mode); break;
        case CTR:AES_ctr128_encrypt(Input,Output,Size,&_EncKey,IV,ecount,&numCTR);break;
        }
    }
    else
    {
        switch (CFBorOFBorCBCorCTR)
        {
        case CFB:AES_cfb128_encrypt(Input, Output, Size, &_DecKey, IV, &num, Mode); break;
        case OFB:AES_ofb128_encrypt(Input, Output, Size, &_DecKey, IV, &num); break;
        case CBC:AES_cbc_encrypt(InputforCBC, OutputforCBC, PseudoSize, &_DecKey, IV, Mode); break;
        case CTR:AES_ctr128_encrypt(Input, Output, Size, &_DecKey, IV, ecount, &numCTR); break;
        }
    }
    if (CFBorOFBorCBCorCTR != CBC)
    {
        for (int i = 0; i < Size; i++)
        {
            if (Mode == 1)
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
        for (int i = 0; i < PseudoSize; i++)
        {
            if (Mode == 1)
            {
                _EncData.push_back(OutputforCBC[i]);
            }
            else
            {
                _DecData.push_back(OutputforCBC[i]);
            }
        }
    }
}