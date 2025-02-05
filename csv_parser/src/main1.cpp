#include "../include/parser1.hpp"
#include <iostream>
#include <tuple>

int main() {
    const std::tuple<int, std::string, double> t = {42, "hello", 3.14};
    
    std::cout << t << std::endl;

    return 0;
}