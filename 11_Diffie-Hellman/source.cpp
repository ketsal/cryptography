#include "../include/mpirxx.h"
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <time.h>
int main()
{
    mpz_t a;
    mpz_t P;
    mpz_t b;
    mpz_t N;
    mpz_t A;
    mpz_t B;
    mpz_t res;
    mpz_t res2;
    mpz_init(res);
    mpz_init(res2);
    mpz_init(A);
    mpz_init(B);
    mpz_init(a);
    mpz_init(P);
    mpz_init(b);
    mpz_init(N);
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state,time(NULL));
    mpz_urandomb(a, state, 1000);
    mpz_urandomb(P, state, 3000);
    mpz_urandomb(b, state, 1000);
    mpz_urandomb(N, state, 100);
    mpz_powm(A, P, a, N);
    mpz_powm(B, P, b, N);
    mpz_powm(res, A, b, N);
    mpz_powm(res2, B, a, N);
    std::cout <<res<< std::endl;
    std::cout << res2<< std::endl;
    std::cin >> a;
}