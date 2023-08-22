#include "util/util.hpp"
#include <string>
#include <iostream>

//  okay now we want to actually improve this thing to find it automatically

// X#$@#$ frequency analysis didn't correct for case! which is why i lost points on that
// one assignment

int main()
{
    std::string c = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
    c = hex_2_byte(c);
    int n = c.size();

    char k = freq_analysis(c).first;
    std::string out;
    for (int i = 0; i < n; i++) out += c[i] ^ k;
    std::cout <<out << "\n";
}