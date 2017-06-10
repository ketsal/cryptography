#include "RC4.h"
#include "..\..\include\file.h"
#include "..\..\include\KeyGen.h"
int main(int argc, char **argv)
{
    std::string path = "../../docs/plain.docx";
    std::string path2 = "../../docs/cipher.docx";
    std::string path3 = "../../docs/decrypted.docx";
    File plain(path);
    File cipher(path2);
    File decrypted(path3);
    KeyGen keygen;
    RC4 crypter;
    int size=32;
    unsigned char *key = new unsigned char[size];
    key=keygen.RC4(size);
    std::cout << "Reading plain.docx...";
    plain.Read();
    system("cls");
    std::cout << "Encrypting plain.docx...";
    crypter.Encrypt(key, plain.GetData());
    system("cls");
    cipher.GetData() = crypter.GetEncData();
    std::cout << "Writing encrypted data into cipher.docx...";
    cipher.WriteData();
    system("cls");
    std::cout << "Decrypting...";
    crypter.Decrypt(key, cipher.GetData());
    system("cls");
    std::cout << "Writing decrypted data into decrypted.docx...";
    decrypted.GetData() = crypter.GetDecData();
    decrypted.WriteData();
    system("cls");
    system("pause");
    return 0;
}