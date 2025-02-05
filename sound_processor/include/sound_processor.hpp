#include <iostream>
#include <vector>
#include <queue>
#include <list>
#include <functional>
#include <memory>

class WAVFile{
public:
	WAVFile(const char* filename);
	WAVFile() {};
	~WAVFile() {};

	std::unique_ptr<FILE, decltype(&fclose)> read_wav_header(const char* str);
	void write_wav_header(const char* str);

	unsigned get_size() { return data.size(); }
	unsigned get_sample_rate() { return SampleRate; }
	unsigned get_n_channels() { return NumChannels; }
	int getDataOffset() const { return dataOffset; }

	short &operator[](unsigned i) { return data[i]; }

	std::vector<short> data;

//private:
	char type[5];
	char format[5];
	char Subchunk1ID[5];
	char Subchunk2ID[5];
	char Subchunk3ID[5];
	char* InfoChunk2;

	int ChunkSize;
	int Subchunk1Size;
	int SampleRate;
	int ByteRate;
	int Subchunk2Size;
	int Subchunk3Size;

	short AudioFormat;
	short NumChannels;
	short BlockAlign;
	short BitsPerSample;

	int NumSamples;
	int dataOffset = 0;
};
template <typename T, typename Container=std::deque<T>> 
class LimitedQueue : public std::queue<T,Container> {
public:
	LimitedQueue() = default;
	LimitedQueue(int cap) : capacity(cap) {
		std::queue<T> queue(cap);
	}
	~LimitedQueue() = default;

	std::queue<T> queue;
	void push(T elem) {
		queue.push(elem);
		if (queue.size >= capacity) {
			queue.pop();
		}
	}

	int getSize() { 
		return queue.size;
	}

	bool isFull() {
		return queue.size == capacity - 1;
	}

private:
	int capacity;
};

class Converter {
public:
	Converter() = default;
	virtual ~Converter() = default;

	virtual void process(short& sample) = 0;
	int sample_rate = 44100;
};

class MuteConverter : public Converter { 
public: 
	MuteConverter() = default;

	MuteConverter(int start, int end) : start(start), end(end) {}
	~MuteConverter() = default;
	
	void process(short& sample) override;
private:
	int start = 0;
	int end;
    int current_sample = 0;
};

class MixConverter : public Converter { 
public:
	MixConverter(int start, std::string mix_file) : start(start), mix_file(mix_file), current_sample(0) {
		mix_descriptor = (mix_wav.read_wav_header(mix_file.c_str())).release();
        if (!mix_descriptor) {
            throw std::invalid_argument("Failed to open mix file: " + mix_file);
        }
        mix_samples_remaining = mix_wav.Subchunk3Size / (mix_wav.NumChannels * (mix_wav.BitsPerSample / 8));
	}

	~MixConverter() {
		if (mix_descriptor) {
			fclose(mix_descriptor);
		}
	}

	void process(short& sample1) override;
private:
	int start;
	std::string mix_file;
	WAVFile mix_wav;
    FILE* mix_descriptor = nullptr;
	int mix_samples_remaining = 0; 
    int current_sample = 0;
};

class VolumeChanger : public Converter { 
public: 
	VolumeChanger(int start, int end, float factor) : factor(factor), start(start), end(end) {}
	~VolumeChanger() = default;
	void process(short& sample) override;

private:
	float factor;
	int start = 0;
	int end;
    int current_sample = 0;
};

class PitchShifter : public Converter {
public:
	PitchShifter(int start, int end, float factor, std::string input_file_name) : factor(factor), start(start), end(end) {}
	~PitchShifter() = default;
	void process(short& sample) override;

private:
	WAVFile output_wav;
	FILE* output_descriptor = nullptr;
	// std::list<short> sample_list; 
	float factor;
	int start = 0;
	int end;
    int current_sample = 0;
};

class WAVProcessor {
public:
	std::vector<Converter*> converters;

	WAVProcessor() = default;
	~WAVProcessor() = default;

	void processWav(std::vector<std::string>& input_files, std::string output_file);
};

class ConfigFile {
public:
	ConfigFile() {};
	~ConfigFile() {};

	void readConfigFile(std::string config_file_name, std::vector<std::string>& input_files, WAVProcessor& processor);
};

void parseCommands(int argc, char** argv, std::string& output_file, std::string& config_file, std::vector<std::string>& input_files);

void printHelp();

void tokenize(std::string& line, std::vector<std::string>& tokens);


/*
parseCommnands(args, string output, config, inputs) -> vector<WAVFile> inputs, output, ConfigFile config
||
readConfigFile(string config) -> WAVProcessor<> with converters and their args
||
convertWav(string inputs, output) and converters inside class -> converted audio
	loadWAV(input)
	mute, mix, pitch process inside for each sample 
	saveWAV()   
		writeWAV

*/