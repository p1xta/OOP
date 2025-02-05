#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector> 

class Cell {
public:
    int x, y;
    int alive_around;
    bool status; // 1 - alive or 0 - dead

    Cell() : alive_around(0), status(0) {}
    Cell(int x, int y, bool st) : x(x), y(y), status(st), alive_around(0) {}
    ~Cell() = default;
};

class Grid {
public:
    int width;
    int height;
    Cell** cells;

    Grid() : cells(nullptr), height(0), width(0) {}

    Grid(int h, int w) : height(h), width(w) {
        cells = new Cell* [h];
        for (int i = 0; i < h; i++) {
            cells[i] = new Cell[w];
            for (int j = 0; j < w; j++) {
                cells[i][j].x = i;
                cells[i][j].y = j;
                cells[i][j].status = 0;
                cells[i][j].alive_around = 0;
            }
        }
    }
    
    ~Grid() {
        for (int i = 0; i < height; i++) 
            delete[] cells[i];
        delete[] cells;
    }
};

class UniverseRules {
public:
    std::vector<int> birth_nums;
    std::vector<int> survival_nums;
    std::string name;

    UniverseRules() {}
    UniverseRules(std::vector<int> b, std::vector<int> s) {
        this->birth_nums = b;
        this->survival_nums = s;
    }
    ~UniverseRules() {}
};

void parseCommands(int argc, char** argv, int& iterations, std::string& input_file, std::string& output_file);

void tickCommand(int n, sf::RenderWindow& win, Grid& grid, UniverseRules& universe, int& current_it);

void helpCommand();

void parseRuleString(UniverseRules& universe, std::string str);

std::string intVectorToStr(std::vector<int> v);

Grid readUniverseFromFile(std::string file_name, UniverseRules& universe);

void saveUniverseToFile(std::string output_file_name, Grid& grid, UniverseRules& universe);

Grid initializeGrid(const std::string& input_file_name, UniverseRules& universe);

void drawGrid(sf::RenderWindow& win, Grid& grid, int squareSize);

void updateGrid(Grid& grid, UniverseRules& universe);

Grid loadPreset(UniverseRules& universe);

void offlineMode(std::string input_file_name, std::string output_file_name, int n);

void onlineMode(std::string input_file_name);