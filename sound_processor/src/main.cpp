#include "../include/sound_processor.hpp"
#include "../include/exceptions.hpp"
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char** argv) {
    std::vector<std::string> input_files;
    std::string output_file;
    std::string config_file_name;
    WAVProcessor processor;
    ConfigFile config;

    parseCommands(argc, argv, output_file, config_file_name, input_files);
    
    config.readConfigFile(config_file_name, input_files, processor);

    processor.processWav(input_files, output_file);
    std::cout << "Processing complete. Output saved to: " << output_file << std::endl;

    return 0;
}
// ./sound_processor -c ../samples/config.txt ../samples/output.wav ../samples/yung_lean.wav   ../samples/Crystals.wav
// g++ src/main.cpp src/sound_processor.cpp -Iinclude -o main