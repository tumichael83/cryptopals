#include "util/util.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

int main() {
    std::ifstream f("/Users/michaeltu/Desktop/Independent-CS-Stuff/cryptopals/cpp/c4.txt");
    std::string line;

    std::string best_str;
    std::pair<char, float> best_key {0, 1000};
    while(f >> line) {
        std::string ctext = hex_2_byte(line);
        std::pair<char, float> key = freq_analysis(ctext);
        if (key.second < best_key.second) {
            best_str = ctext;
            best_key = key;
        }
    }

    std::string out;
    for (int i = 0; i < best_str.size(); i++) out += best_str[i] ^ best_key.first;

    std::cout << "key: " << best_key.first
              << " (0x"
              << std::setw(2) << std::setfill('0') << std::hex << (int) best_key.first
              << ")\n"
              << "score: " << best_key.second
              << "\ntext: " << out;
}