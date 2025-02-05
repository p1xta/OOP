#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

void AddWordToMap(string word, map<string, int>* map) {
	if (map->find(word) != map->end()) {
		(*map)[word]++;
	}
	else {
		map->insert({word, 1});
	}
}

bool cmp(pair<string, int>& a, pair<string, int>& b) { 
    return a.second > b.second; 
} 

void SortMapByValueAndPrint(ofstream& out, map<string, int>* map, int total_words) {
	vector<pair<string, int> > pairs; 
  
    for (auto& it : (*map)) 
        pairs.push_back(it); 

	sort(pairs.begin(), pairs.end(), cmp); 

	for (auto& pair : pairs) {
		double frequency = static_cast<double>(pair.second)/total_words * 100;
        out << pair.first << ", " << pair.second << ", " << frequency << "%" << endl; 
    } 
}

int main(int argc, char** argv) {
	string input_file_name = argv[1];
	string output_file_name = argv[2];
	std::ifstream in;
	std::ofstream out;

	in.open(input_file_name);
	out.open(output_file_name);

	string words;
	std::string word = "";
	map<string, int> map;
	int total_words = 0;

	if (!in.is_open() or !out.is_open()) 
		cout<<"Error with opening files."<<endl;

	while (!in.eof()) {
		getline(in, words);
		words += '\n';
		for (const char c : words) {
			if (std::isalnum(c)) { 
				word += c;
			}
			else if (!word.empty()) {
				total_words+=1;
				AddWordToMap(word, &map);
				word.clear();
			}
		}
	}

	if (!word.empty()) {
		total_words+=1;
		AddWordToMap(word, &map);
	}

	SortMapByValueAndPrint(out, &map, total_words);

	in.close();
	out.close();
	return 0;
}