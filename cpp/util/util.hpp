#pragma once

#include "consts.hpp"

#include <string>

extern const int BLOCK_SIZE;
extern const char BASE64_ENC[];
extern const char BASE64_DEC[];
extern const float ENG_FREQS[];

std::string byte_2_hex(std::string);

std::string hex_2_byte(std::string);

std::string byte_2_b64(std::string);

std::string b64_2_byte(std::string);

std::pair<char, float> freq_analysis(std::string);

int hamming_dist(std::string s1, std::string s2);

std::string xor_same_length(std::string, std::string);

std::string pad(std::string, int);

// these encryption decryption functions are VERY UNSAFE
// they don't provide any error checking, and expect block_size inputs

std::string encrypt_aes_128_ecb(std::string, std::string, std::string);
std::string decrypt_aes_128_ecb(std::string, std::string, std::string);

std::string encrypt_aes_128_cbc(std::string, std::string, std::string);
std::string decrypt_aes_128_cbc(std::string, std::string, std::string);