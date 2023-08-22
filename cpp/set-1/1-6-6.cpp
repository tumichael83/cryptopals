#include "util/util.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

std::string fixed_key_xor(std::string m, std::string k) {
    std::string out;
    int numkeys = k.size();
    for (int i = 0; i < m.size(); i++) {
        out += (unsigned char)(m[i] ^ k[i%numkeys]);
    }
    return out;
}

int main() {
    std::ifstream f("/Users/michaeltu/Desktop/Independent-CS-Stuff/cryptopals/cpp/c6.txt");
    std::string buf;
    std::string text;
    while (f >> buf) {
        text += buf;
    }
    text = b64_2_byte(text);

    std::vector<float> key_lengths;
    for (int i = 2; i < 64; i++) {
        std::vector<std::string> strings;
        float score = 0.0;
        for (int j = 0; j < 65; j+=i) {
            strings.push_back(text.substr(j, i));
        }

        int n = strings.size();
        for (int j = 0; j < n; j++) {
            for (int k = j+1; k < n; k++) {
                score += hamming_dist(strings[j], strings[k]);
            }
        }

        score /= (i * n * (n-1));   // keysize and num of combos
        if (score < 1.5) {
            key_lengths.push_back(i);
        }
    }

    std::string best_key = "";
    float best_score = 10000000;
    for (auto i : key_lengths) {
        std::string key = "";
        float score = 0;
        for (int j = 0; j < i; j++) {
            std::string ss = "";
            for (int k = j; k < text.size(); k += i) {
                ss += text[k];
            }
            std::pair<char, float> ks = freq_analysis(ss);
            key += ks.first;
            score += ks.second;
        }
        // something is wrong? (29) should be notably better --> didn't strip newlines
        score /= i;
        if (score < best_score) {
            best_score = score;
            best_key = key;
        }
    }

    std::cout << "key: " + best_key + "\n\n" << fixed_key_xor(text, best_key) << "\n";
    std::ofstream ofs("solution2.txt");
    ofs << fixed_key_xor(text, best_key) << "\n";
}