#include <immintrin.h>
#include <iostream>

int main ()
{
    unsigned short r;
    while (!(_rdseed16_step(&r)))
        ;
    std::cout << r << std::endl;
}
