#include "../include/sound_processor.hpp"
#include "../include/exceptions.hpp"
#include "../include/fft.hpp"
#include "../../circular_buffer/CircularBuffer.h"
#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <cstring>
#include <sstream>
#include <valarray>
#include <complex>

WAVFile::WAVFile(const char* filename) {
	std::string str = filename;
	if (str.substr(str.size() - 4) != ".wav")
		throw FileOpenException("Can only read WAV files.");
	read_wav_header(filename);
}

std::unique_ptr<FILE, decltype(&fclose)> WAVFile::read_wav_header(const char* filename) {
	FILE* f;
	if (!(f = fopen(filename, "rb")))
		throw FileOpenException("Failed to open file " + static_cast<std::string>(filename));

	std::unique_ptr<FILE, decltype(&fclose)> file_ptr(f, fclose);
	
	// Chunk
	fread(type, sizeof(char), 4, f);
	type[4] = '\0';
	if (strcmp(type, "RIFF"))
		throw FileReadException(static_cast<std::string>(filename) + " is not a RIFF file.");

	fread(&ChunkSize, sizeof(int), 1, f);
	fread(format, sizeof(char), 4, f);
	format[4] = '\0';
	if (strcmp(format, "WAVE"))
		throw FileReadException(static_cast<std::string>(filename) + " is not a WAVE format.");

	// 1st Subchunk
	fread(Subchunk1ID, sizeof(char), 4, f);
	if (strcmp(Subchunk1ID, "fmt "))
		throw FileReadException(static_cast<std::string>(filename) + "is missing fmt header.");

	fread(&Subchunk1Size, sizeof(int), 1, f);
	fread(&AudioFormat, sizeof(short), 1, f);
	fread(&NumChannels, sizeof(short), 1, f);
	if (NumChannels != 1)
		throw FileReadException("Only mono sound is supported. (" + std::to_string(NumChannels) + " channels were given).");

	fread(&SampleRate, sizeof(int), 1, f);
	if (SampleRate != 44100)
		throw FileReadException("Sample rate should be 44100 (" + std::to_string(SampleRate) + "was given).");

	fread(&ByteRate, sizeof(int), 1, f);
	fread(&BlockAlign, sizeof(short), 1, f);
	fread(&BitsPerSample, sizeof(short), 1, f);

	// 2nd Subchunk
	fread(Subchunk2ID, sizeof(char), 4, f);
	Subchunk2ID[4] = '\0';
	if (strcmp(Subchunk2ID, "LIST"))
		throw FileReadException("Missing LIST header.");
	fread(&Subchunk2Size, sizeof(int), 1, f);
	InfoChunk2 = new char[Subchunk2Size];
	//fseek(f, Subchunk2Size, SEEK_CUR);
	fread(InfoChunk2, sizeof(char), Subchunk2Size, f);

	fread(&Subchunk3ID, sizeof(char), 4, f);
	Subchunk3ID[4] = '\0';
	if (strcmp(Subchunk3ID, "data")) {
		throw FileException("Missing data header.");
	}
	fread(&Subchunk3Size, sizeof(int), 1, f);

	//Subchunk2Size = NumSamples * NumChannels * BitsPerSample/8
	NumSamples = Subchunk3Size / (NumChannels*(BitsPerSample / 8));

	dataOffset = ftell(f);
	return file_ptr;
}

void WAVFile::write_wav_header(const char* filename) {
	FILE* f;
	if (!(f = fopen(filename, "wb"))) 
		throw FileOpenException("Error opening file " + static_cast<std::string>(filename));

	fwrite(type, sizeof(char), 4, f);
	fwrite(&ChunkSize, sizeof(int), 1, f);
	fwrite(format, sizeof(char), 4, f);

	// 1st Subchunk
	fwrite(Subchunk1ID, sizeof(char), 4, f);
	fwrite(&Subchunk1Size, sizeof(int), 1, f);
	fwrite(&AudioFormat, sizeof(short), 1, f);
	fwrite(&NumChannels, sizeof(short), 1, f);
	fwrite(&SampleRate, sizeof(int), 1, f);
	fwrite(&ByteRate, sizeof(int), 1, f);
	fwrite(&BlockAlign, sizeof(short), 1, f);
	fwrite(&BitsPerSample, sizeof(short), 1, f);

	fwrite(Subchunk2ID, sizeof(char), 4, f);
	fwrite(&Subchunk2Size, sizeof(int), 1, f);
	fwrite(InfoChunk2, sizeof(char), Subchunk2Size, f);
	//fseek(f, Subchunk2Size, SEEK_CUR);
	fwrite(Subchunk3ID, sizeof(char), 4, f);
	fwrite(&Subchunk3Size, sizeof(int), 1, f);
}

void parseCommands(int argc, char** argv, std::string& output_file, std::string& config_file, std::vector<std::string>& input_files) {
    if (argc == 1) { return; }
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-c") == 0) {
            config_file = argv[++i];
			output_file = argv[++i];
			while (++i < argc) {
				input_files.push_back(argv[i]);
			}
        }
        else if (strcmp(argv[i], "-h") == 0) {
            printHelp();
        }
        else {
            std::cerr << "Invalid argument: " << argv[i] << "\n";
        }
    }
	if (output_file.empty()) {
		throw FileOpenException("No output file was given.");
        printHelp();
		return;
	}
	if (config_file.empty()) {
		throw FileOpenException("No config file was given.");
        printHelp();
		return;
	}
	if (input_files.empty()) {
		throw FileOpenException("No input files were given.");
        printHelp();
		return;
	}
}

void printHelp() {
	std::cout << "Usage:\n> sound_processor -c <config.txt> <output.wav> <input1.wav> [<input2.wav> …]" << std::endl;
}

void ConfigFile::readConfigFile(std::string config_file_name, std::vector<std::string>& input_files, WAVProcessor& processor) {
	std::ifstream file(config_file_name);
	std::string line;

	while (std::getline(file, line)) {
		std::vector<std::string> tokens;
		if (line[0] == '#') { continue; }
		tokenize(line, tokens);
		if (tokens[0] == "mute") {
			int start = std::stoi(tokens[1]);
            int end = std::stoi(tokens[2]);
			if (end < start) {
				throw ConfigException("End cannot be less than start.");
			}
			processor.converters.push_back(new MuteConverter(start, end));
		}
		else if (tokens[0] == "mix") {
			int file_index = std::stoi(tokens[1].substr(1)) - 1;
            int start = std::stoi(tokens[2]);
			if (file_index < 0) {
                throw MixConverterException("Invalid file index for mix: " + tokens[1]);
            }
			processor.converters.push_back(new MixConverter(start, input_files[file_index]));
		}
		// else if (tokens[0] == "pitch") {
		// 	int start = std::stoi(tokens[1]);
        //     int end = std::stoi(tokens[2]);
		// 	float factor = std::stof(tokens[3]);
		// 	if (end < start) {
		// 		throw ConfigException("End cannot be less than start.");
		// 	}
		// 	processor.converters.push_back(new PitchShifter(start, end, factor));
		// }
		else if (tokens[0] == "volume") {
			int start = std::stoi(tokens[1]);
            int end = std::stoi(tokens[2]);
			float factor = std::stof(tokens[3]);
			if (end < start) {
				throw ConfigException("End cannot be less than start.");
			}
			processor.converters.push_back(new VolumeChanger(start, end, factor));
		}
		else {
			throw ConverterException("Invalid argument in config file: " + tokens[0]);
		}
	}
}

void tokenize(std::string& line, std::vector<std::string>& tokens) {
	std::stringstream check1(line);
    std::string token;
    
    while(getline(check1, token, ' ')) {
        tokens.push_back(token);
    }
}

void MuteConverter::process(short& sample) {
    if (current_sample >= start * sample_rate && current_sample <= end * sample_rate) {
        sample = 0;
    }
    ++current_sample;
}

void MixConverter::process(short& sample1) {
    if (current_sample >= start * sample_rate && mix_samples_remaining > 0) {
		short sample2 = 0;
		if (fread(&sample2, sizeof(short), 1, mix_descriptor) == 1) {
			sample1 = static_cast<short>((sample1 + sample2) / 2);
			--mix_samples_remaining;
		}
		else {
			mix_samples_remaining = 0;
		}
    }
    ++current_sample;
}

void VolumeChanger::process(short& sample) {
	if (current_sample >= start * sample_rate && current_sample <= end * sample_rate) {
		sample *= factor;
	}
	if (sample > 32767) sample = 32767;
    if (sample < -32768) sample = -32768;
	++current_sample;
}

void PitchShifter::process(short& sample) {
	// std::valarray<Complex> sample_arr;
	// sample_list.push_back(sample);
	// int i = 0;
	// for (auto& sample : sample_list) {
	// 	sample_arr[i++] = static_cast<double>(sample);
	// }

	// if (sample_arr.size() == 100) {
	// 	fft(sample_arr);
	// }
	if (current_sample >= start * sample_rate && current_sample <= sample_rate * end) {
		for (int i = 0; i < 100 * factor; i++) {
			fwrite(&sample, sizeof(short), 1,output_descriptor);
		}
		++current_sample;
	}

}


void WAVProcessor::processWav(std::vector<std::string>& input_files, std::string output_file) {
	FILE* input = fopen(input_files[0].c_str(), "rb");
    if (!input) {
        throw FileOpenException("Failed to open input file: " + input_files[0]);
    }

    FILE* output = fopen(output_file.c_str(), "wb");
    if (!output) {
        fclose(input);
        throw FileOpenException("Failed to create output file: " + output_file);
    }
	WAVFile wav;
    wav.read_wav_header(input_files[0].c_str());
    wav.write_wav_header(output_file.c_str());
	for (auto& converter : converters) {
		converter->sample_rate = wav.get_sample_rate();
	}
	
	short sample;
	while (fread(&sample, sizeof(short), 1, input) == 1) {
        for (auto& converter : converters) {
            converter->process(sample);
        }
        fwrite(&sample, sizeof(short), 1, output);
    }
    fclose(input);
    fclose(output);
}
