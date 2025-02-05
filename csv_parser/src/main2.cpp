#include <iostream>
#include "../include/basic_parser.hpp"

int main() {
    // std::ifstream file("people-1000.csv");
    // CSVParser<int, std::string, std::string, std::string, std::string, std::string, \
    //     std::string, std::string, std::string> parser(file, 1, ',', '"');
    std::ifstream file("test.csv");
    CSVParser<int, std::string, double> parser(file, 1, '%', '/');

    for (auto & rs : parser) {
       std::cout << rs << std::endl;
    }

    return 0;
}