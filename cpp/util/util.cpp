#include "util.hpp"

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

float eng_score(std::vector<float> freqs) {
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
        std::vector<float> freqs(256,0);
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