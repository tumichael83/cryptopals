#include "util/util.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

/*
    alright the string literal is:
        s = "I'm killing your brain like a poisonous mushroom"

    hex rep is:
        49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d

    b64 rep is
        SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t

*/

int main() {
    std::ifstream ifs("/Users/michaeltu/Desktop/Independent-CS-Stuff/cryptopals/cpp/c6.txt");
    std::stringstream buf;
    buf << ifs.rdbuf();
    // std::cout << buf.str();
    std::string s = b64_2_byte(buf.str().substr(0,78));
    std::ofstream ofs("/Users/michaeltu/Desktop/Independent-CS-Stuff/cryptopals/test3.txt");
    for (unsigned char c : s) {
        ofs << (int) c << "\n";
    }
}