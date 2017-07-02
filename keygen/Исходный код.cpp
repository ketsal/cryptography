#include <string>
#include <windows.h>
#include <vector>
#include <iostream>
char String[256];
std::vector<unsigned char> __stdcall sub_4012F9()
{
    int v1; // esi@1// edi@1
    unsigned char v3;
    int v8=5; // ecx@5
    std::vector<unsigned char> vec;
    v1 = (int)String;
    v3 = 0;
    int i = 0;
    while (*(BYTE *)v1)
    {
        v3 = String[i];
        v3 = v3 + i;
        v3 = v3 ^ i;
        v3 = v3 % 0x1Au;
        v3 = v3 + 65;
        vec.push_back(v3);
        ++v1;
        i++;
    }
    for (int i = 0; i < vec.size(); i++)
    {
        std::cout << vec.at(i);
    }
    return  vec;
}
void main()
{
    std::cout << "Enter name\n";
    std::cin >> String;
    std::cout << "Serial\n";
    sub_4012F9();
    std::cin >> String;
}