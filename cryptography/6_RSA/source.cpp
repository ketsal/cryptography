#include "RSAproxy.h"
void main()
{
    KeyGen gen;
    std::string path3 = "../docs/plain.docx";
    std::string path4 = "../docs/decrypted.docx";
    std::string path = "../docs/public.pem";
    std::string path2 = "../docs/private.pem";
    gen.GenerateRSAkeys(path,path2,2048);
    File plain(path3);
    File dec(path4);
    RSAproxy crypter;
    printf("Reading plain text...\n");
    plain.Read();
    system("cls");
    printf("Encrypting...\n");
    crypter.Encrypt(plain.GetData(), path);
    system("cls");
    printf("Decrypting...\n");
    crypter.Decrypt(crypter.GetEndData(), path2);
    system("cls");
    printf("Writing decrypted data into file...\n");
    dec.GetData() = crypter.GetDecData();
    dec.WriteData();
    system("cls");
    system("pause");
}