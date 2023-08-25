#include "util/util.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

#include <openssl/aes.h>
#include <openssl/evp.h>

// i need to do some error checking, but it scares me
// i also need to modularize these encrypting functions

// freaking cbc
// xor ctext with ptext of next block before encrypting
// interfacing with openssl is gonna be a pain maybe

int main() {
    std::ifstream ifs("/Users/michaeltu/Desktop/Independent-CS-Stuff/cryptopals/temp.txt");
    std::stringstream buf;
    buf << ifs.rdbuf();
    std::string text = buf.str();
    // std::cout << text;


    std::string ptext = text.substr(0,BLOCK_SIZE);
    std::string key = "YELLOW SUBMARINE";
    std::string iv = "YELLOW SUBMARINE";

    std::string c1 = encrypt_aes_128_ecb(ptext, key, iv);
    std::cout << "Encrypt first 16 bytes (ecb): " << c1 << "\n";
    
    std::string p1 = decrypt_aes_128_ecb(c1, key, iv);
    std::cout << "Decrypt first 16 bytes (ecg): " << p1 << "\n";

    ptext = pad(text, BLOCK_SIZE);
    int n = ptext.size()/BLOCK_SIZE;
    std::string c2;
    for (int i = 0; i < ptext.size(); i+= BLOCK_SIZE) {
        c2 += encrypt_aes_128_ecb(ptext.substr(i, BLOCK_SIZE), key, iv);
    }
    std::cout << "Encrypt first " << n*BLOCK_SIZE << " bytes (ecb): " << c2 << "\n";

    std::string p2;
    for (int i = 0; i < ptext.size(); i+= BLOCK_SIZE) {
        p2 += decrypt_aes_128_ecb(c2.substr(i, BLOCK_SIZE), key, iv);
    }
    std::cout << "Decrypt first 32 bytes (ecb): " << p2 << "\n";


    std::string c3 = encrypt_aes_128_cbc(ptext, key, iv);
    std::cout << "Encrypt first " << n*BLOCK_SIZE << " bytes (cbc): " << c3 << "\n";

    std::string p3 = decrypt_aes_128_cbc(c3, key, iv);
    std::cout << "Decrypt first " << n*BLOCK_SIZE << " bytes (cbc): " << p3 << "\n";


}