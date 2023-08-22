#include "util/util.hpp"
#include <fstream>

int main() {
    std::ofstream of("temp.txt");
    of << pad("YELLOW SUBMARINE", 16);
}