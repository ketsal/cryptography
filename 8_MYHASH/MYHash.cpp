#include "MYHash.h"
std::vector<unsigned char> MyHash::ElemFunc(std::vector<unsigned char> &Block, std::vector<unsigned char> &PrevBlock)
{
    std::vector<unsigned char> HASH;
    for (int i = 0; i < 32; i++)
    {
        HASH.push_back(Block.at(i));
        HASH.at(i) += PrevBlock.at(i);
        HASH.at(i) += (HASH.at(i) % (i + 1));
        HASH.at(i) ^= (HASH.at(i) >> 5);
    }
    return HASH;
}
void MyHash::Hash(std::vector<unsigned char> &DataToHash, std::vector<unsigned char>& IV)
{
    _hashed.clear();
    int size = DataToHash.size();
    int pseudosize = (size + 31) / 32 * 32;
    if (pseudosize < 0)
    {
        pseudosize = -pseudosize;
    }
    for (int i = 0; i < pseudosize - size; i++)
    {
        DataToHash.push_back('s');
    }
    std::vector<unsigned char> SUMM;
    std::vector<unsigned char> Size;
    for (int i = 0; i<pseudosize; i = i + 32)
    {
        if (i == 0)
        {
            _hashed= ElemFunc(DataToHash, IV);
            for (int j = 0; j < 32; j++)
            {
                SUMM.push_back(DataToHash.at(j));
            }
        }
        else
        {
            std::vector<unsigned char> TEMP;
            for (int j = i; j < i + 32; j++)
            {
                TEMP.push_back(DataToHash.at(j));
            }
           _hashed = ElemFunc(TEMP, _hashed);
            for (int j = i, k = 0; j < i + 32; k++, j++)
            {
                SUMM.at(k) ^= DataToHash.at(j);
            }
        }
    }
    for (int j = 0; j < 32; j++)
    {
        Size.push_back((unsigned char)size);
    }
    _hashed = ElemFunc(Size, _hashed);
    _hashed = ElemFunc(SUMM, _hashed);
}
std::vector<unsigned char> &MyHash::GetHashedData()
{
    return _hashed;
}