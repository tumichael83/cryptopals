#include "util/util.hpp"
#include <string>
#include <iostream>
#include <fstream>

// Repeating Key XOR with ptext
// "Burning 'em, if you ain't quick and nimble"
// "I go crazy when I hear a cymbal"

// ciphertext:
// 0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272
// a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f

std::string fixed_key_xor(std::string m, std::string k) {
    std::string out;
    int numkeys = k.size();
    for (int i = 0; i < m.size(); i++) {
        out += m[i] ^ k[i%numkeys];
    }
    return out;
}

// let's write a bonus function to modify stuff in a file
void fixed_key_xor_file(std::string fname, std::string k) {
    // eh we'll do it later
}

int main() {
    std::string s = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
    std::cout << byte_2_hex(fixed_key_xor(s, "ICE")) << "\n";
}