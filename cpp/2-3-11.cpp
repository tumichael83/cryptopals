#include "util/util.hpp"

#include <string>
#include <iostream>

std::string gen_rand_bytes(int n) {
    std::string k;
    for (int i = 0; i < n; i++) k += (unsigned char)rand();
    return k;
}

std::string gen_rand_key() {
    return gen_rand_bytes(BLOCK_SIZE);
}

std::string encryption_oracle(std::string s) {
    std::string head = gen_rand_bytes(5 + (rand()%5));
    std::string tail = gen_rand_bytes(5 + (rand()%5));

    std::string ptext = pad(head+s+tail, 16);
    std::string key = gen_rand_key();
    std::string iv = gen_rand_bytes(BLOCK_SIZE);

    if (rand() % 2 == 0) {
        std::cout << "Encryption Mode: ECB\n";
        return encrypt_aes_128_ecb(ptext, key, iv);
    }
    else {
        std::cout << "Encryption Mode: CBC\n";
        return encrypt_aes_128_cbc(ptext, key, iv);
    }
}

void detect_encryption() {
    std::string ctext = encryption_oracle("YELLOW SUBMARINEYELLOW SUBMARINEYELLOW SUBMARINEYELLOW SUBMARINE");
    std::string zeros = std::string(16,10);
    if (xor_same_length(ctext.substr(BLOCK_SIZE, BLOCK_SIZE), ctext.substr(2*BLOCK_SIZE, BLOCK_SIZE)) == std::string(BLOCK_SIZE, '\0')) {
        std::cout << "Detected: ECB encryption\n";
    }
    else {
        std::cout << "Detected: CBC encryption\n";
    }
    
}

int main() {
    for (int i = 0; i < 15; i++) {
        detect_encryption();
        std::cout << "\n";
    }
}