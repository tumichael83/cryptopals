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

enum class EncryptionMode{ ECB, CBC }; 

std::string encryption_oracle(std::string s, EncryptionMode &m) {
    std::string head = gen_rand_bytes(5 + (rand()%5));
    std::string tail = gen_rand_bytes(5 + (rand()%5));

    std::string ptext = pad(head+s+tail, 16);
    std::string key = gen_rand_key();
    std::string iv = gen_rand_bytes(BLOCK_SIZE);

    if (rand() % 2 == 0) {
        m = EncryptionMode::ECB;
        return encrypt_aes_128_ecb(ptext, key, iv);
    }
    else {
        m = EncryptionMode::CBC;
        return encrypt_aes_128_cbc(ptext, key, iv);
    }
}

void detect_encryption() {
    EncryptionMode m;
    std::string ctext = encryption_oracle("YELLOW SUBMARINEYELLOW SUBMARINEYELLOW SUBMARINE", m);
    std::string zeros = std::string(16,10);
    if (xor_same_length(ctext.substr(BLOCK_SIZE, BLOCK_SIZE), ctext.substr(2*BLOCK_SIZE, BLOCK_SIZE)) == std::string(BLOCK_SIZE, '\0')) {
        if (m == EncryptionMode::ECB) {
            std::cout << "TEST OK\n";
            std::cout << "Used: ECB\tDetected: ECB\n";
        }
        else {
            std::cout << "ERROR\n";
        }
    }
    else {
        if (m == EncryptionMode::CBC) {
            std::cout << "TEST OK\n";
            std::cout << "Used: CBC\tDetected: CBC\n";
        }
        else {
            std::cout << "ERROR\n";
        }
    }
}

int main() {
    for (int i = 0; i < 15; i++) {
        detect_encryption();
        std::cout << "\n";
    }
}