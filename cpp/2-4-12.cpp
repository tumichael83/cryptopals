#include "util/util.hpp"

#include <string>
#include <iostream>

std::string key = "";

std::string gen_rand_bytes(int n)
{
    std::string k;
    for (int i = 0; i < n; i++)
        k += (unsigned char)rand();
    return k;
}

std::string gen_rand_key()
{
    return gen_rand_bytes(BLOCK_SIZE);
}

enum class EncryptionMode
{
    ECB,
    CBC
};

std::string encryption_oracle(std::string s)
{
    // this one doesn't add random padding
    std::string s2 = "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkg"
                     "aGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBq"
                     "dXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUg"
                     "YnkK";
                    
    s2 = b64_2_byte(s2);

    if (key.size() == 0)
        key = gen_rand_key();
    return encrypt_aes_128_ecb(pad(s + s2, BLOCK_SIZE), key, std::string(BLOCK_SIZE, '\0'));
}

int find_block_size(std::string (*oracle)(std::string))
{
    int i = 1;
    while (true)
    {
        std::string c1 = oracle(std::string(i, 'A'));
        std::string c2 = oracle(std::string(i + 1, 'A'));

        int j = 0;
        while (c1[j] == c2[j])
            j++;
        if (j > 0)
            return j;

        i++;
    }
    return -1;
}

bool detect_ecb(std::string (*oracle)(std::string), int block_size) {
    std::string ctext = oracle("YELLOW SUBMARINEYELLOW SUBMARINE");
    return ctext.substr(0, block_size) == ctext.substr(block_size, block_size);
}

std::string ecb_padding_attack(std::string (*oracle)(std::string), int block_size)
{

    std::string out = "";

    std::string ctext = oracle("");
    while (out.size() + block_size -1< ctext.size()) {
        std::string blk_ptext = "";
        for (int pad_len = block_size-1; pad_len >= 0; pad_len--) {
            std::string padding = std::string(pad_len, 'A');
            std::string target_blk = oracle(padding).substr(out.size(), block_size);

            for (int c = 0; c < 256; c++) {
                std::string pad_ptext = "";
                if (out.size() > 0) {
                    pad_ptext = out.substr(out.size() - pad_len, pad_len) + blk_ptext + std::string(1, (unsigned int)c);
                }
                else {
                    pad_ptext = padding + blk_ptext + std::string(1, (unsigned int)c);
                }

                std::string pad_blk = oracle(pad_ptext).substr(0, block_size);

                if (pad_blk == target_blk) {
                    blk_ptext += (unsigned char) c;
                    break;
                }
                
            }
        }
        out += blk_ptext;
    }

    return out;
}

// AAAAAAAA BBBBBBBB CCCCCCCC
// XXXXXXXA AAAAAAAB BBBBBBBC CCCCCCC_
// XXXXXXXA AAAAAAAA AAAAAAAB
// XXXXXXXA AAAAAAAB BBBBBBBA AAAAAAAB 

int main()
{
    const int block_size = find_block_size(encryption_oracle);
    std::cout << "Block size: " << block_size;

    if (!detect_ecb(encryption_oracle, block_size)) {
        std::cout << "Not Using ECB";
        return 0;
    }
    std::cout << "\nUsing ECB";

    std::string secret = ecb_padding_attack(encryption_oracle, block_size);
    std::cout << "\nMessage: \n" << secret;
}