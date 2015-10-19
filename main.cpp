#include "ipseeker.h"
#include <iostream>
#include <random>

int main(int argc, char *argv[])
{
    IPSeeker s;
    std::cout << cc << "   " << rr << std::endl;
    //s.test();
    std::default_random_engine generator;
    std::uniform_int_distribution<uint32_t> distribution(0x1000000, 0xFF000000);
    std::string c, r;
    for (int i = 0; i < 100000; ++i)
    {
        s.seek(distribution(generator), c, r);
	std::cout << c << "    " << r <<std::endl;
    }
    std::cout << "END" <<std::endl;
    return 0;
}
