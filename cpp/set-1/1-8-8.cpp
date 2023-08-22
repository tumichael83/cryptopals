#include <string>
#include <iostream>
#include "util/util.hpp"

#include <fstream>
#include <memory>

// this interface is such a damn mess
// this probably demands going into utils
// and rewriting some stuff
#include <openssl/aes.h>
#include <openssl/evp.h>

int main() {
    std::string buf;
    std::fstream fs("/Users/michaeltu/Desktop/Independent-CS-Stuff/cryptopals/cpp/c7.txt");
    std::string text;
    while (fs >> buf) {
        text += buf;
    }
    // std::cout << text << "\n";
    std::string ctext = b64_2_byte(text);
    std::string key = "YELLOW SUBMARINE";
    std::string iv = "YELLOW SUBMARINE";

    std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)> ctx(EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free);

    // EVP_EncryptInit_ex(ctx.get(), EVP_aes_128_ecb(), nullptr, reinterpret_cast<const unsigned char *>(key.data()), reinterpret_cast<const unsigned char *>(iv.data()));

    // ctext.resize(ptext.size() + 128);
    // int out_len1 = (int)ctext.size();

    // EVP_EncryptUpdate(ctx.get(), (unsigned char *)&ctext[0], &out_len1, (const unsigned char *)&ptext[0], (int)ptext.size());

    // int out_len2 = (int)ctext.size() - out_len1;
    // EVP_EncryptFinal_ex(ctx.get(), (unsigned char *)&ctext[0]+out_len1, &out_len2);

    // ctext.resize(out_len1 + out_len2);

    // std::cout << ctext << "\n";

    std::string ptext2;
    ctx = std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)>(EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free);

    EVP_DecryptInit_ex(ctx.get(), EVP_aes_128_ecb(), nullptr, reinterpret_cast<const unsigned char *>(key.data()), reinterpret_cast<const unsigned char *>(iv.data()));

    ptext2.resize(ctext.size()+128);
    int out_len1 = (int)ptext2.size();
    
    EVP_DecryptUpdate(ctx.get(), (unsigned char *)&ptext2[0], &out_len1, (const unsigned char *)&ctext[0], (int)ctext.size());

    int out_len2 = (int)ptext2.size() - out_len1;
    EVP_DecryptFinal_ex(ctx.get(), (unsigned char *)&ptext2[0]+out_len1, &out_len2);

    ptext2.resize(out_len1 + out_len2);

    std::cout << ptext2 << "\n";
}

// #include "util/util.hpp"

// #include <string>
// #include <iostream>
// #include <fstream>
// #include <memory>

// // key is YELLOW SUBMARINE, encryption is aes-128-ecb

// // you know what? it's probably easier not to use this damn thing for now
// // ah shit. the algo is aes-128. since idk how to implement that, it's time to learn
// // how to use openssl
// #include <openssl/aes.h>
// #include <openssl/evp.h>    // apparently you can use this or BIO

// int main() {
//     // just no error checking

//     std::ifstream f("/Users/michaeltu/Desktop/Independent-CS-Stuff/cryptopals/cpp/c6.txt");
//     std::string buf;
//     std::string text;
//     while (f >> buf) {
//         text += buf;
//     }
//     std::string ctext = b64_2_byte(text);
//     std::string ptext;
//     std::string key = "YELLOW SUBMARINE";
//     std::string iv =  "YELLOW SUBMARINE"; // irrelevant for ecb


//     // going to use a smart pointer, exciting
//     // using the custom delete because cryptography is tricky business -- i assume this
//     // does some important cache related stuff
//     std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)> ctx(EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free);

//     EVP_DecryptInit_ex(ctx.get(), EVP_aes_128_ecb(), nullptr, reinterpret_cast<const unsigned char *>(key.data()), reinterpret_cast<const unsigned char *>(iv.data()));

//     ptext.resize(ctext.size()+128);
//     int out_len1 = (int)ptext.size();
    
//     EVP_DecryptUpdate(ctx.get(), (unsigned char *)&ptext[0], &out_len1, (const unsigned char *)&ctext[0], (int)ctext.size());

//     int out_len2 = (int)ptext.size() - out_len1;
//     EVP_DecryptFinal_ex(ctx.get(), (unsigned char *)&ptext[0]+out_len1, &out_len2);

//     ptext.resize(out_len1 + out_len2);

//     std::cout << ptext.size()/3 << "\n";
//     std::cout << ctext.size()/4 << "\n";
//     std::cout << out_len1/4 << "\n";
//     std::cout << out_len2 << "\n";
// }