#include "util/util.hpp"

#include <iostream>
#include <vector>
#include <unordered_map>

void die() {
    std::cout << "\nYou did something with bad input >:-|";
    exit(1);
}

std::unordered_map<std::string, std::string> parse_stdin() {

    std::unordered_map<std::string, std::string> json;
    std::string key;
    std::string val;

    bool iskey = true;
    char c;
    while (std::cin >> c) {
        if (iskey) {
            if (c == '=') {
                if (json.find(key) != json.end()) {
                    die();
                }
                iskey = false;
            }
            else if (c != '&') {
                key += c;
            }
            else {
                // die :)
            }
        }
        else {
            if (c == '&') {
                json[key] = val;
                iskey = true;
                key = "";
                val = "";
            }
            else if (c != '=') {
                val += c;
            }
            else {
                die();
            }
        }
    }

    // clean up last char rip
    if (iskey) {
        die();
    }
    else {
        json[key] = val;
    }

    return json;
}

int main() {
}