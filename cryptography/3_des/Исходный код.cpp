#include "DESProxy.h"
#include  <conio.h>
int main(int argc, char **argv)
{
    std::string path = "../../docs/plain.docx";
    std::string path2 = "../../docs/cipher.docx";
    std::string path3 = "../../docs/decrypted.docx";
    File plain(path);
    File cypher(path2);
    File decrypted(path3);
    int destype=0;
    int blockmode=0;
    bool cont = true;
    DesProxy crypter;
    while (cont)
    {
        std::cout << "Choose Des type: 1 for TripleDes,2 for common Des\n";
        std::cin >> destype;
        if (destype > 2 || destype < 1)
        {
            system("cls");
            std::cout << "Choose wisely";
            _getch();
        }
        else cont = false;
        system("cls");
    }
    cont = true;
    while (cont)
    {
        std::cout << "Choose encryption mode: 1 for OFB, 2 for CFB, 3 for CBC, 4 for ECB(CTR not supported)\n";
        std::cin >> blockmode;
        if (blockmode > 4 || blockmode < 1)
        {
            system("cls");
            std::cout << "Choose wisely";
            _getch();
        }
        else cont = false;
        system("cls");
    }
    std::cout << "Reading file plain.docx...";
    plain.Read();
    system("cls");
    std::cout << "Encrypting plain.docx...";
    switch (blockmode)
    {
    case 1:
    {
        if (destype == 1)
        {
            crypter.EncryptTripleDesOFB(plain.GetData(), "}42d^dag", "*01pmz!-", "`-+dwa5e");
        }
        else
        {
            crypter.EncryptOFB(plain.GetData(), "}42d^dag");
        }

    }break;
    case 2:
    {
        if (destype == 1)
        {
            crypter.EncryptTripleDesCFB(plain.GetData(), "}42d^dag", "*01pmz!-", "`-+dwa5e");
        }
        else
        {
            crypter.EncryptCFB(plain.GetData(), "}42d^dag");
        }
    }break;
    case 3:
    {
        if (destype == 1)
        {
            crypter.EncryptTripleDesCBC(plain.GetData(), "}42d^dag", "*01pmz!-", "`-+dwa5e");
        }
        else
        {
            crypter.EncryptCBC(plain.GetData(), "}42d^dag");
        }
    }break;
    case 4:
    {
        if (destype == 1)
        {
            crypter.EncryptTripleDesECB(plain.GetData(), "}42d^dag", "*01pmz!-", "`-+dwa5e");
        }
        else
        {
            crypter.EncryptECB(plain.GetData(), "}42d^dag");
        }
    }break;
    }
    system("cls");
    std::cout << "Writing encrypted data into cipher.docx...";
    cypher.GetData() = crypter.GetEncData();
    cypher.WriteData();
    cypher.GetData().clear();
    cypher.Read();
    system("cls");
    std::cout << "Decrypting cipher.docx...";
    switch (blockmode)
    {
    case 1:
    {
        if (destype == 1)
        {
            crypter.DecryptTripleDesOFB(cypher.GetData(), "}42d^dag", "*01pmz!-", "`-+dwa5e");
        }
        else
        {
            crypter.DecryptOFB(cypher.GetData(), "}42d^dag");
        }

    }break;
    case 2:
    {
        if (destype == 1)
        {
            crypter.DecryptTripleDesCFB(cypher.GetData(), "}42d^dag", "*01pmz!-", "`-+dwa5e");
        }
        else
        {
            crypter.DecryptCFB(cypher.GetData(), "}42d^dag");
        }
    }break;
    case 3:
    {
        if (destype == 1)
        {
            crypter.DecryptTripleDesCBC(cypher.GetData(), "}42d^dag", "*01pmz!-", "`-+dwa5e");
        }
        else
        {
            crypter.DecryptCBC(cypher.GetData(), "}42d^dag");
        }
    }break;
    case 4:
    {
        if (destype == 1)
        {
            crypter.DecryptTripleDesECB(cypher.GetData(), "}42d^dag", "*01pmz!-", "`-+dwa5e");
        }
        else
        {
            crypter.DecryptECB(cypher.GetData(), "}42d^dag");
        }
    }break;
    }
    system("cls");
    std::cout << "Writing decrypted data into decoded.docx...";
    decrypted.GetData() = crypter.GetDecData();
    decrypted.WriteData();
    system("cls");
    system("pause");
    return 0;
}