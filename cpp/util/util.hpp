#pragma once

#include "consts.hpp"
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cmath>

std::string byte_2_hex(std::string);

std::string hex_2_byte(std::string);

std::string byte_2_b64(std::string);

std::string b64_2_byte(std::string);

std::pair<char, float> freq_analysis(std::string);

int hamming_dist(std::string s1, std::string s2);

std::string pad(std::string, int);