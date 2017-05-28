#include "aespr.h"
#include <conio.h>
int main()
{
    unsigned char key[] = "vkl+4ml1} jkj-1f";
    std::string path1 = "../../docs/plain.docx";
    std::string path2 = "../../docs/cipher.docx";
    std::string path3 = "../../docs/decrypted.docx";
    File plain(path1);
    File cypher(path2);
    File decrypted(path3);
    AesProxy crypter;
    bool cont = true;
    int type;
    while (cont)
    {
        std::cout << "Choose AES encryption mode type: 1 for CFB, 2 for OFB, 3 for CBC, 4 for CTR and 5 for ECB\n";
        std::cin >> type;
        if (type > 5 || type < 1)
        {
            system("cls");
            std::cout << "Choose wisely";
            _getch();
        }
        else cont = false;
        system("cls");
    }
    std::cout << "Reading plain.docx...";
    plain.Read();
    system("cls");
    std::cout << "Encrypting plain.docx...";
    switch (type)
    {
    case 1:crypter.EncryptCFB(plain.GetData(), key); break;
    case 2:crypter.EncryptOFB(plain.GetData(), key); break;
    case 3:crypter.EncryptCBC(plain.GetData(), key); break;
    case 4:crypter.EncryptCTR(plain.GetData(), key); break;
    case 5:crypter.EncryptECB(plain.GetData(), key); break;
    }
    system("cls");
    std::cout << "Writing encrypted data into cypher.docx";
    cypher.GetData() = crypter.GetEncData();
    cypher.WriteData();
    system("cls");
    std::cout << "Decrypting data cypher.docx";
    switch (type)
    {
    case 1:crypter.DecryptCFB(cypher.GetData(), key); break;
    case 2:crypter.DecryptOFB(cypher.GetData(), key); break;
    case 3:crypter.DecryptCBC(cypher.GetData(), key); break;
    case 4:crypter.DecryptCTR(cypher.GetData(), key); break;
    case 5:crypter.DecryptECB(cypher.GetData(), key); break;
    }
    system("cls");
    std::cout << "Writing decrypted data into decrypted.docx";
    decrypted.GetData() = crypter.GetDecData();
    decrypted.WriteData();
    system("cls");
    system("pause");
}