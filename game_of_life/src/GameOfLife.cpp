#include <SFML/Graphics.hpp>
#include "../include/GameOfLife.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
#include <sstream>
#include <random>

void parseCommands(int argc, char** argv, int& iterations, std::string& input_file, std::string& output_file) {
    if (argc == 1) { return; }
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--iterations") == 0) {
            iterations = std::atoi(argv[++i]);
        } 
        else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--input") == 0) {
            input_file = argv[++i];
        }
        else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
            output_file = argv[++i];
        } 
        else {
            std::cerr << "Invalid argument: " << argv[i] << "\n";
        }
    }
}

void tickCommand(int n, sf::RenderWindow& win, Grid& grid, UniverseRules& universe, int& current_it) {
    sf::Clock clock;
    float interval = 0.1;
    for (int i = 0; i < n;) {
        if (clock.getElapsedTime().asSeconds() >= interval) {
            win.clear();
            drawGrid(win, grid, 20);
            updateGrid(grid, universe);
            win.display();
            clock.restart();
            current_it++;
            i++;
        }
    }
}

void helpCommand() {
    std::cout << "Commands:\n";
    std::cout << "dump <filename> - save the universe to file\n";
    std::cout << "tick <n=1> или t <n=1> - calculate n iterations and display result\n";
    std::cout << "exit - exit game\n";
    std::cout << "help - available commands\n";
}

void parseRuleString(UniverseRules& universe, std::string str) {
    std::string rule_str = str.substr(4);
    int s_start = 0;
    for (auto c : rule_str) {
        s_start++;
        if (c == '/') { break; }
        universe.birth_nums.push_back(c - '0');
    }
    s_start++;
    std::string surv_str = rule_str.substr(s_start);
    for (auto c : surv_str) {
        if (c == '\n') { break; }
        universe.survival_nums.push_back(c - '0');
    }
}

std::string intVectorToStr(std::vector<int> v) {
    std::stringstream ss;
    for (auto it = v.begin(); it != v.end(); it++) 
        ss << *it;
    
    return ss.str();
}

Grid readUniverseFromFile(std::string file_name, UniverseRules& universe) {
    std::ifstream f;
    f.open(file_name);
    std::string str;

    if (!f.is_open()) 
        throw(std::invalid_argument("Failed to open universe file."));
    
	std::getline(f, str);
	std::getline(f, str);
    universe.name = str.substr(3);

	std::getline(f, str);
    parseRuleString(universe, str);

    std::getline(f, str, ' ');
    int grid_height = stoi(str);
    std::getline(f, str);
    int grid_width = stoi(str);
    Grid grid(grid_height, grid_width);

    int x, y;
	while (getline(f, str, ' ')) {
		x = stoi(str);
		std::getline(f, str);
        y = stoi(str);

        x = (x + grid.height) % grid.height;
        y = (y + grid.width) % grid.width;
        grid.cells[x][y].x = x;
        grid.cells[x][y].y = y;
        grid.cells[x][y].status = 1;
	}
    f.close();
    return grid;
}

void saveUniverseToFile(std::string output_file_name, Grid& grid, UniverseRules& universe) {
    std::ofstream f;
    f.open(output_file_name);
    if (!f.is_open()) 
        throw(std::invalid_argument("Failed to open save file."));
    
    f << "Life 1.06\n";
    f << "#N Saved universe\n";
    f << "#R B" + intVectorToStr(universe.birth_nums) + "/S" + intVectorToStr(universe.survival_nums) + "\n";
    f << std::to_string(grid.height) + " " + std::to_string(grid.width) + "\n";
    for (int i = 0; i < grid.height; i++) 
        for (int j = 0; j < grid.width; j++) 
            if (grid.cells[i][j].status) 
                f << std::to_string(i) + " " + std::to_string(j) + "\n";
    std::cout << "Universe saved to file " << output_file_name << std::endl;
}

Grid initializeGrid(const std::string& input_file_name, UniverseRules& universe) {
    if (!input_file_name.empty()) 
        return readUniverseFromFile(input_file_name, universe);
    else 
        return loadPreset(universe);
}

void drawGrid(sf::RenderWindow& win, Grid& grid, int squareSize) {
    for (int i = 0; i < grid.height; i++) {
        for (int j = 0; j < grid.width; j++) {
            sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));
            square.setPosition(j * squareSize, i * squareSize);
            square.setFillColor(grid.cells[i][j].status ? sf::Color::Black : sf::Color::White);
            square.setOutlineColor(sf::Color::Black);
            square.setOutlineThickness(1);
            win.draw(square);
        }
    }
}

void updateGrid(Grid& grid, UniverseRules& universe) {
    Cell** next_gen = new Cell* [grid.height];
    for (int i = 0; i < grid.height; ++i) {
        next_gen[i] = new Cell[grid.width];
    }
    
    for (int i = 0; i < grid.height; ++i) {
        for (int j = 0; j < grid.width; ++j) {
            int alive_around = 0;
            for (int r = i-1; r <= i+1; r++) {
                for (int l = j-1; l <= j+1; l++) {
                    if (r == i && l == j) continue;
                    int r_ = (r + grid.height) % grid.height;
                    int l_ = (l + grid.width) % grid.width;
                    if (grid.cells[r_][l_].status)
                        alive_around++;
                }
            }
            grid.cells[i][j].alive_around = alive_around;
            for (int num : universe.survival_nums){
                if (grid.cells[i][j].status && grid.cells[i][j].alive_around == num) {
                    next_gen[i][j].status = 1;
                }   
            }
            for (int num : universe.birth_nums) {
                if (!grid.cells[i][j].status && grid.cells[i][j].alive_around == num) {
                    next_gen[i][j].status = 1;
                }
            }
        }
    }
    delete[] grid.cells;
    grid.cells = next_gen;
}

Grid loadPreset(UniverseRules& universe) {
    std::srand(time(0));
    int preset_number = std::rand() % 4;
    switch (preset_number)
    {
    case 0:
        //LONG LIVING THING
        return readUniverseFromFile("/home/p1xta/OOP/game_of_life/presets/longlife.txt", universe);
        break;
    case 1:
        //GLIDER
        return readUniverseFromFile("/home/p1xta/OOP/game_of_life/presets/glider.txt", universe);
        break;
    case 2:
        //PULSAR
        return readUniverseFromFile("/home/p1xta/OOP/game_of_life/presets/pulsar.txt", universe);
        break;
    case 3:
        //GLIDER GUN
        return readUniverseFromFile("/home/p1xta/OOP/game_of_life/presets/glider_gun.txt", universe);
    default:
        break;
    }
    Grid grid;
    return grid;
}

void offlineMode(std::string input_file_name, std::string output_file_name, int n) {
    std::ofstream out;
	out.open(output_file_name);

    UniverseRules universe;
    Grid grid = readUniverseFromFile(input_file_name, universe);
    int current_it = 0;
    sf::RenderWindow window(sf::VideoMode(grid.width*20, grid.height*20), "Game of life");
    window.setPosition(sf::Vector2i(10, 50));

    tickCommand(n, window, grid, universe, current_it);
    saveUniverseToFile(output_file_name, grid, universe);
    out.close();
}

void onlineMode(std::string input_file_name) {
    UniverseRules universe;
    Grid grid = initializeGrid(input_file_name, universe);
    std::cout << "Universe name : " << universe.name << std::endl;
    sf::RenderWindow window(sf::VideoMode(grid.width*20, grid.height*20), "Game of life");
    window.setPosition(sf::Vector2i(10, 50));

    int current_it = 0;
    std::string command;
    while(true) {
        std::getline(std::cin, command);
        if (command.substr(0, 4) == "dump") {
            std::string file_name = command.substr(5);
            saveUniverseToFile(file_name, grid, universe);
        }
        else if (command.substr(0, 4) == "tick") {
            int iterations = 1;
            if (command.length() > 4) {
                try {
                    iterations = std::stoi(command.substr(5));
                }
                catch (std::invalid_argument &e){
                    std::cout << "Invalid argument to tick command" << std::endl;
                    std::cout << e.what() << std::endl;
                }
            }
            tickCommand(iterations, window, grid, universe, current_it);
            std::cout << "Current iteration: " << current_it << std::endl;
        }
        else if (command.substr(0, 4) == "help") {
            helpCommand();
        }
        else if (command.substr(0, 4) == "exit") {
            break;
        }
        else {
            std::cout << "Invalid command. Type \"help\" to see all commands" << std::endl;
        }
    }
}

