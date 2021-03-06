#include "../shared/mpirxx.h"
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <time.h>
#include "../shared//DESProxy.h"
class DiffieHellman
{
private:
    mpz_t a;
    mpz_t b;
    mpz_t P;
    mpz_t N;
    mpz_t A;
    mpz_t B;
    mpz_t res;
    gmp_randstate_t state;
    static const int buffer_size = 1024;
    char buf[buffer_size];
public:
    DiffieHellman()
    {
        mpz_init(res);
        mpz_init(A);
        mpz_init(B);
        mpz_init(a);
        mpz_init(b);
        mpz_init(P);
        mpz_init(N);
        gmp_randinit_mt(state);
        gmp_randseed_ui(state, time(NULL));
    }
    void ExchangeAsServer(SOCKET ClientSock)
    {
        mpz_urandomb(P, state, 10);
        mpz_urandomb(a, state, 1000);
        int result = 0;
        result = recv(ClientSock, buf, buffer_size, 0);
        mpz_init_set_str(N, buf, 16);
        mpz_get_str(buf, 16, P);
        result = send(ClientSock, buf, buffer_size, 0);
        mpz_powm(A, P, a, N);
        result = recv(ClientSock, buf, buffer_size, 0);
        mpz_init_set_str(B, buf, 16);
        mpz_get_str(buf, 16, A);
        result = send(ClientSock, buf, buffer_size, 0);
        mpz_powm(res, B, a, N);
    }
    void ExchangeAsClient(SOCKET ServerSock)
    {
        mpz_urandomb(N, state, 3000);
        mpz_urandomb(b, state, 1000);
        int result = 0;
        mpz_get_str(buf, 16, N);
        result = send(ServerSock, buf, buffer_size, 0);
        result = recv(ServerSock, buf, buffer_size, 0);
        mpz_init_set_str(P, buf, 16);
        mpz_powm(B, P, b, N);
        mpz_get_str(buf, 16, B);
        result = send(ServerSock, buf, buffer_size, 0);
        result = recv(ServerSock, buf, buffer_size, 0);
        mpz_init_set_str(A, buf, 16);
        mpz_powm(res, A, b, N);
    }
    char * GetResult()
    {
        mpz_get_str(buf, 16, res);
        return buf;
    }

};
int main()
{
    SOCKET  ClientSocket=0;
    int result;
    DiffieHellman sock;
    DesProxy crypter;
    SOCKET ListenSocket=0;
    std::string Message = "Privet drug";
    std::string  Key = "1234567";
    std::vector<unsigned char> message;
    for (int i = 0; i < Message.length(); i++)
    {
        message.push_back(Message[i]);
    }
    char *buf;
    WSADATA wsaData;
    result= WSAStartup(MAKEWORD(2, 2), &wsaData);
    struct addrinfo* addr = NULL;
    struct addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
    result = getaddrinfo("127.0.0.1", "11000", &hints, &addr);
    bool checkmode = true;
    if (result != 0)
    {
        WSACleanup();
    }
    ListenSocket = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    if (ListenSocket == INVALID_SOCKET)
    {
        freeaddrinfo(addr);
        WSACleanup();
    }
    result = bind(ListenSocket, addr->ai_addr, (int)addr->ai_addrlen);
    if (result == SOCKET_ERROR)
    {
        result = connect(ListenSocket, addr->ai_addr, (int)addr->ai_addrlen);
        checkmode = false;
    }
    if (checkmode)
    {
        std::cout << "Waiting for connection...\n";
        if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)
        {
            closesocket(ListenSocket);
            WSACleanup();
        }
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET)
        {
            closesocket(ListenSocket);
            WSACleanup();
        }
        std::cout << "Connected\n";
        system("cls");
    }
    std::cout << "Starting diffie-hellman exchange\n";
    if (checkmode)
    {
        sock.ExchangeAsServer(ClientSocket);
    }
    else
    {
        sock.ExchangeAsClient(ListenSocket);
    }
    std::cout << "Done";
    system("cls");
    buf = sock.GetResult();
    for (int i = 0; i < 7; i++)
    {
        Key[i] = buf[i];
    }
    const char* charsKEY = Key.data();
    const  char* KEYbytes = reinterpret_cast<const char*>(charsKEY);
    if (checkmode)
    {
        message.clear();
        result = recv(ClientSocket, buf, 1024, 0);
        std::cout << "Recieved message\n";
        for (int i = 0; i < result; i++)
        {
            std::cout << buf[i];
            message.push_back(buf[i]);
        }
        crypter.DecryptCFB(message, KEYbytes);
        std::cout << "\nDecrypted message\n";
        for (int i = 0; i < crypter.GetDecData().size(); i++)
        {
            std::cout << crypter.GetDecData().at(i);
        }
    }
    else
    {
        std::cout << "Encrypting message\n";
        crypter.EncryptCFB(message, KEYbytes);
        char *buffer = new char[crypter.GetEncData().size()];
        for (int i = 0; i < crypter.GetEncData().size(); i++)
        {
            buffer[i] = crypter.GetEncData().at(i);
        }
        std::cout << "Sending off message\n";
        result = send(ListenSocket, buffer, crypter.GetEncData().size(), 0);
        system("cls");
        std::cout << "Message sent\n";
        for (int i = 0; i < result; i++)
        {
            std::cout << buffer[i];
        }
    }
    closesocket(ClientSocket);
    closesocket(ListenSocket);
    freeaddrinfo(addr);
    WSACleanup();
    int A;
    std::cin >> A;
}