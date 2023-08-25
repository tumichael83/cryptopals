#include "util/util.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

#include <openssl/aes.h>
#include <openssl/evp.h>

const int BLOCK_SIZE = 16;
// i need to do some error checking, but it scares me
// i also need to modularize these encrypting functions

// freaking cbc
// xor ctext with ptext of next block before encrypting
// interfacing with ssl is gonna be a pain maybe

// encrypts a single block using aes_128_ecb
// assumes ptext and key and iv are properly block sized

// openssl example
// void aes_encrypt(const byte key[KEY_SIZE], const byte iv[BLOCK_SIZE], const secure_string& ptext, secure_string& ctext)
// {
//     EVP_CIPHER_CTX_ptr ctx(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free);
//     int rc = EVP_EncryptInit_ex(ctx.get(), EVP_aes_256_cbc(), NULL, key, iv);
//     if (rc != 1)
//       throw std::runtime_error("EVP_EncryptInit_ex failed");

//     // Cipher text expands upto BLOCK_SIZE
//     ctext.resize(ptext.size()+BLOCK_SIZE);
//     int out_len1 = (int)ctext.size();

//     rc = EVP_EncryptUpdate(ctx.get(), (byte*)&ctext[0], &out_len1, (const byte*)&ptext[0], (int)ptext.size());
//     if (rc != 1)
//       throw std::runtime_error("EVP_EncryptUpdate failed");
  
//     int out_len2 = (int)ctext.size() - out_len1;
//     rc = EVP_EncryptFinal_ex(ctx.get(), (byte*)&ctext[0]+out_len1, &out_len2);
//     if (rc != 1)
//       throw std::runtime_error("EVP_EncryptFinal_ex failed");

//     // Set cipher text size now that we know it
//     ctext.resize(out_len1 + out_len2);
// }

std::string aes_encrypt_block(std::string ptext, std::string key, std::string iv) {\

    std::string ctext;

    std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)> ctx(EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free);

    EVP_EncryptInit_ex(ctx.get(), EVP_aes_128_ecb(), nullptr, reinterpret_cast<const unsigned char *>(key.data()), reinterpret_cast<const unsigned char *>(iv.data()));

    EVP_CIPHER_CTX_set_padding(ctx.get(), 0);

    // Cipher text expands up to BLOCK_SIZE
    ctext.resize(ptext.size()+BLOCK_SIZE);
    int out_len1 = (int)ctext.size();

    EVP_EncryptUpdate(ctx.get(), (unsigned char *)&ctext[0], &out_len1, (unsigned char *)&ptext[0], (int)ptext.size());
  
    int out_len2 = (int)ctext.size() - out_len1;
    EVP_EncryptFinal_ex(ctx.get(), (unsigned char *)&ctext[0]+out_len1, &out_len2);

    // Set cipher text size now that we know it
    ctext.resize(out_len1 + out_len2);
    assert(ctext.size() == BLOCK_SIZE);

    return ctext;
}

std::string aes_decrypt_block(std::string ctext, std::string key, std::string iv) {

    std::string ptext;

    std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)> ctx(EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free);

    EVP_DecryptInit_ex(ctx.get(), EVP_aes_128_ecb(), nullptr, reinterpret_cast<const unsigned char *>(key.data()), reinterpret_cast<const unsigned char *>(iv.data()));

    EVP_CIPHER_CTX_set_padding(ctx.get(), 0);

    ptext.resize(ctext.size()+BLOCK_SIZE);
    int out_len1 = (int)ptext.size();

    EVP_DecryptUpdate(ctx.get(), (unsigned char *)&ptext[0], &out_len1, (const unsigned char *)&ctext[0], (int)ctext.size());
    // if (out_len1 != 16) std::cout << out_len1;

    int out_len2 = (int)ptext.size() - out_len1;
    EVP_DecryptFinal_ex(ctx.get(), (unsigned char *)&ptext[out_len1], &out_len2); //

    ptext.resize(out_len1 + out_len2);

    return ptext;
}

std::string xor_same_length(std::string s1, std::string s2) {
    std::string out;
    for (int i = 0; i < s1.size(); i++) out += (unsigned char)s1[i] ^ (unsigned char)s2[i];
    return out;
}

int main() {
    std::ifstream ifs("/Users/michaeltu/Desktop/Independent-CS-Stuff/cryptopals/temp.txt");
    std::stringstream buf;
    buf << ifs.rdbuf();
    std::string text = buf.str();
    // std::cout << text;


    std::string ptext = text.substr(0,BLOCK_SIZE);
    std::string key = "YELLOW SUBMARINE";
    std::string iv = std::string(BLOCK_SIZE, '\0');

    std::string c1 = aes_encrypt_block(ptext, key, iv);
    std::cout << "Encrypt first 16 bytes (ecb): " << c1 << "\n";
    
    std::string p1 = aes_decrypt_block(c1, key, iv);
    std::cout << "Decrypt first 16 bytes (ecg): " << p1 << "\n";

    ptext = pad(text, BLOCK_SIZE);
    int n = ptext.size()/BLOCK_SIZE;
    std::string c2;
    for (int i = 0; i < ptext.size(); i+= BLOCK_SIZE) {
        c2 += aes_encrypt_block(ptext.substr(i, BLOCK_SIZE), key, iv);
    }
    std::cout << "Encrypt first " << n*BLOCK_SIZE << " bytes (ecb): " << c2 << "\n";

    std::string p2;
    for (int i = 0; i < ptext.size(); i+= BLOCK_SIZE) {
        p2 += aes_decrypt_block(c2.substr(i, BLOCK_SIZE), key, iv);
    }
    std::cout << "Decrypt first 32 bytes (ecb): " << p2 << "\n";


    std::string prev = std::string(16,'\0');
    std::string c3;
    for (int i = 0; i < ptext.size(); i+= BLOCK_SIZE) {
        std::string m = xor_same_length(ptext.substr(i, BLOCK_SIZE), prev); // this was the problem along with padding in encrypt
        std::string c = aes_encrypt_block(m, key, iv);
        c3 += c;
        prev = c;
    }
    std::cout << "Encrypt first " << n*BLOCK_SIZE << " bytes (cbc): " << c3 << "\n";

    std::string p3;
    for (int i = 0; i < ptext.size(); i+= BLOCK_SIZE) {
        std::string p = aes_decrypt_block(c3.substr(i, BLOCK_SIZE), key, iv);
        if (i >= BLOCK_SIZE) {
            p = xor_same_length(p, c3.substr(i-BLOCK_SIZE, BLOCK_SIZE));
        }
        p3 += p;
    }
    std::cout << "Decrypt first " << n*BLOCK_SIZE << " bytes (cbc): " << p3 << "\n";


}