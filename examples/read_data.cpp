// -std=c++20 -fno-exceptions -fno-unwind-tables -fno-rtti
#include <iostream>
#include <string_view>
#include <string>
#include <fstream>
#include <vector>
#include <span>
#include <ranges>
#include <charconv>
#include <system_error>

#include "../result.h"

using Error = std::string_view;

Result<std::vector<unsigned>, Error> read_data(std::string_view filename) {
    std::ifstream fs(filename.data());
    if (!fs.good()) {
        return { "Error: Couldn't open file." };
    }

    std::string line;
    std::getline(fs, line);

    std::vector<unsigned> numbers;
    
    for (auto const& token : std::views::split(line, ';')) {
        unsigned n {};
        if (std::from_chars(token.data(), token.data() + token.size(), n).ec
            != std::errc{}
        ) {
            return { "Error: Couldn't convert string to unsigned." };
        }
        numbers.push_back(n);
    }
    
    
    return numbers;
}

void print(std::vector<unsigned> const& v) {
    std::cout << "[ ";
    for (auto const& n : v) {
        std::cout << n << ' ';
    }
    std::cout << "]\n";
}

int main() {
    std::vector<unsigned> v = read_data("data.csv").unpack();

    print(v);
    
    return 0;
}
