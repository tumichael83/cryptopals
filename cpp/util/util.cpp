#include "util.hpp"

#include <sstream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <memory>

#include <openssl/evp.h>
#include <openssl/aes.h>

std::string byte_2_hex(std::string s) {
    std::stringstream ss;
    for (int c : s) {
        ss << std::setw(2) << std::setfill('0') << std::hex << c;
    }
    return ss.str();
}

std::string hex_2_byte(std::string s) {
    std::string out;
    for (int i = 0; i < s.size(); i+=2) {
        out += std::stol(s.substr(i,2), 0, 16); // maybe cast as char?
    }
    return out;
}

std::string byte_2_b64(std::string s) {
    std::string out;
    for (int i = 0; i < s.size(); i += 3) {
        if (i+2 < s.size()){
            unsigned int t = (s[i] << 16) + (s[i+1] << 8) + s[i+2];
            out += BASE64_ENC[(t & 0xFC0000) >> 18];
            out += BASE64_ENC[(t & 0x03F000) >> 12];
            out += BASE64_ENC[(t & 0x000FC0) >> 6];
            out += BASE64_ENC[(t & 0x00003F)];
        }
        else if (i+1 < s.size()) {
            unsigned int t = (s[i] << 16) + (s[i+1] << 8);
            out += BASE64_ENC[(t & 0xFC0000) >> 18];
            out += BASE64_ENC[(t & 0x03F000) >> 12];
            out += BASE64_ENC[(t & 0x000FC0) >> 6];
            out += '=';
        }
        else {
            unsigned int t = (s[i] << 16);
            out += BASE64_ENC[(t & 0xFC0000) >> 18];
            out += BASE64_ENC[(t & 0x03F000) >> 12];
            out += '=';
            out += '=';
        }
    }
    return out;
}

std::string b64_2_byte(std::string s) {
    std::string out;
    for (int i = 0; i < s.size(); i+=4) {
        if (s[i+3] != '=') {
            unsigned int t = (BASE64_DEC[s[i]] << 18) + (BASE64_DEC[s[i+1]] << 12)
                             + (BASE64_DEC[s[i+2]] << 6) + BASE64_DEC[s[i+3]];
            out += (t & 0xFF0000) >> 16;
            out += (t & 0x00FF00) >> 8;
            out += (t & 0x0000FF);
        }
        else if (s[i+2] != '=') {
            unsigned int t = (BASE64_DEC[s[i]] << 18) + (BASE64_DEC[s[i+1]] << 12)
                             + (BASE64_DEC[s[i+2]] << 6);
            out += (t & 0xFF0000) >> 16;
            out += (t & 0x00FF00) >> 8;
        }
        else {
            unsigned int t = (BASE64_DEC[s[i]] << 18) + (BASE64_DEC[s[i+1]] << 12);
            out += (t & 0xFF0000) >> 16;
        }
    }
    return out;
}

float eng_score(float freqs[256]) {
    float score = 0;
    for (int i = 0; i < 256; i++) {
        float d = ENG_FREQS[i] - freqs[i];
        score += d*d;
    }
    return sqrt(score);
}

std::pair<char, float> freq_analysis(std::string s) {
    float best_score = 100;
    char best_key = 0;
    for (int k = 0; k < 256; k++) {
        float freqs[256] = {};
        float total = 0;
        
        for (unsigned char c : s){
            freqs[c ^ k]++;
            total++;
        }
        for (auto &c : freqs) {
            c /= total;
        }

        float score = eng_score(freqs);
        if (score < best_score) {
            best_score = score;
            best_key = k;
        }
    }
    return {best_key, best_score};
}

// only for same size strings
int hamming_dist(std::string s1, std::string s2) {
    int count = 0;
    if (s1.size() != s2.size()) return -1;
    for (int i = 0; i < s1.size(); i++) {
        unsigned char c = s1[i] ^ s2[i];
        if (c & 0x80) count++;
        if (c & 0x40) count++;
        if (c & 0x20) count++;
        if (c & 0x10) count++;
        if (c & 0x08) count++;
        if (c & 0x04) count++;
        if (c & 0x02) count++;
        if (c & 0x01) count++;
    }
    return count;
}

std::string pad(std::string s, int len) {
    // if it's a multiple, you add an empty block
    int pval = len - (s.size() % len);
    return s + std::string(pval,(unsigned char)pval);
}

std::string xor_same_length(std::string s1, std::string s2) {
    std::string out;
    for (int i = 0; i < s1.size(); i++) out += (unsigned char)s1[i] ^ (unsigned char)s2[i];
    return out;
}

// these encryption / decryption functions are VERY UNSAFE. They don't provide any 
// kind of block size checking, nor do they tell you if errors have occurred in 
// the process of encryption

std::string encrypt_aes_128_ecb(std::string ptext, std::string key, std::string iv) {
    std::string ctext;

    std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)> ctx(EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free);

    EVP_EncryptInit_ex(ctx.get(), EVP_aes_128_ecb(), nullptr, reinterpret_cast<const unsigned char *>(key.data()), reinterpret_cast<const unsigned char *>(iv.data()));

    EVP_CIPHER_CTX_set_padding(ctx.get(), 0);   // this is line is what makes it all work!

    // Cipher text expands up to BLOCK_SIZE
    ctext.resize(ptext.size()+BLOCK_SIZE);
    int out_len1 = (int)ctext.size();

    EVP_EncryptUpdate(ctx.get(), (unsigned char *)&ctext[0], &out_len1, (unsigned char *)&ptext[0], (int)ptext.size());
  
    int out_len2 = (int)ctext.size() - out_len1;
    EVP_EncryptFinal_ex(ctx.get(), (unsigned char *)&ctext[0]+out_len1, &out_len2);

    // Set cipher text size now that we know it
    ctext.resize(out_len1 + out_len2);
    // assert(ctext.size() == BLOCK_SIZE);

    return ctext;
}

std::string decrypt_aes_128_ecb(std::string ctext, std::string key, std::string iv) {

    std::string ptext;

    std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)> ctx(EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free);

    EVP_DecryptInit_ex(ctx.get(), EVP_aes_128_ecb(), nullptr, reinterpret_cast<const unsigned char *>(key.data()), reinterpret_cast<const unsigned char *>(iv.data()));

    EVP_CIPHER_CTX_set_padding(ctx.get(), 0);   // this line is critical!!

    ptext.resize(ctext.size()+BLOCK_SIZE);
    int out_len1 = (int)ptext.size();

    EVP_DecryptUpdate(ctx.get(), (unsigned char *)&ptext[0], &out_len1, (const unsigned char *)&ctext[0], (int)ctext.size());
    // if (out_len1 != 16) std::cout << out_len1;

    int out_len2 = (int)ptext.size() - out_len1;
    EVP_DecryptFinal_ex(ctx.get(), (unsigned char *)&ptext[out_len1], &out_len2); //

    ptext.resize(out_len1 + out_len2);

    return ptext;
}

std::string encrypt_aes_128_cbc(std::string ptext, std::string key, std::string iv) {
    std::string prev = iv;
    std::string ctext;
    for (int i = 0; i < ptext.size(); i+= BLOCK_SIZE) {
        std::string c = xor_same_length(prev, ptext.substr(i, BLOCK_SIZE));
        c = encrypt_aes_128_ecb(c, key, iv);
        ctext += c;
        prev = c;
    }
    return ctext;
}

std::string decrypt_aes_128_cbc(std::string ctext, std::string key, std::string iv) {
    std::string ptext;
    for (int i = 0; i < ctext.size(); i+= BLOCK_SIZE) {
        std::string p = decrypt_aes_128_ecb(ctext.substr(i, BLOCK_SIZE), key, iv);
        if (i >= BLOCK_SIZE) {
            p = xor_same_length(p, ctext.substr(i-BLOCK_SIZE, BLOCK_SIZE));
        }
        else {
            p = xor_same_length(p, iv);
        }
        ptext += p;
    }
    return ptext;
}



