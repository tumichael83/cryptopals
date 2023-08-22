#include "util/util.hpp"
#include <string>
#include <iostream>
#include <iomanip>

int main() {
    std::string s1 = hex_2_byte("1c0111001f010100061a024b53535009181c");
    std::string s2 = hex_2_byte("686974207468652062756c6c277320657965");
    std::string out;
    for(int i = 0; i < s1.size(); i++) out += s1[i] ^ s2[i];

    std::cout << s1 
              << "\nXOR'd with\n" 
              << s2 
              << "\nproduces\n" 
              << out
              << "\n";
}