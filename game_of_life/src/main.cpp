#include "../include/GameOfLife.h"
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    std::string input_file;
    std::string output_file;
    int iterations = 0;

    parseCommands(argc, argv, iterations, input_file, output_file);

    if (iterations > 0 && !output_file.empty()) {
        offlineMode(input_file, output_file, iterations);
    }
    else {
        onlineMode(input_file);
    }
    return 0;
}