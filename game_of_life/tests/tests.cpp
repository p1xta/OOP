#include <gtest/gtest.h>
#include "../include/GameOfLife.h"

void testIterate(Grid& grid, UniverseRules& universe, int n) {
    for (int i = 0; i < n; i++) {
        updateGrid(grid, universe);
    }
}

bool compareGrids(Grid& grid1, Grid& grid2) {
    for (int i = 0; i < grid1.height; i++) {
        for (int j = 0; j < grid1.width; j++) {
            if (grid1.cells[i][j].status != grid2.cells[i][j].status) {
                return false;
            }
        }
    }
    return true;
}

TEST (LifeTest, initGrid) {
    Grid grid(10, 20);
    EXPECT_TRUE(grid.height == 10);
    EXPECT_TRUE(grid.width == 20);
    
    UniverseRules universe;
    Grid grid_life = readUniverseFromFile("/home/p1xta/OOP/game_of_life/presets/longlife.txt", universe);
    EXPECT_EQ(grid_life.height, 30);
    EXPECT_EQ(grid_life.width, 40);
    EXPECT_TRUE(grid_life.cells[11][12].status);
    EXPECT_TRUE(grid_life.cells[11][13].status);
    EXPECT_TRUE(grid_life.cells[12][11].status);
    EXPECT_TRUE(grid_life.cells[12][12].status);
    EXPECT_TRUE(grid_life.cells[13][12].status);
    EXPECT_EQ(universe.name, "Long life");
    EXPECT_EQ(universe.birth_nums[0], 3);
    EXPECT_EQ(universe.survival_nums[0], 2);
    EXPECT_EQ(universe.survival_nums[1], 3);
}

TEST (LifeTest, CellsMovement) {
    UniverseRules universe1;
    UniverseRules universe2;
    Grid grid1 = readUniverseFromFile("/home/p1xta/OOP/game_of_life/presets/glider.txt", universe1);
    Grid grid2 = readUniverseFromFile("/home/p1xta/OOP/game_of_life/presets/moved_glider.txt", universe2);
    testIterate(grid1, universe1, 4);
    EXPECT_TRUE(compareGrids(grid1, grid2));
}

TEST (LifeTest, ParseFile) {
    std::string input_file;
    std::string output_file;
    int iterations = 0;
    char* commands[] = {"prog", "-f", "input.txt", "-i", "27", "-o", "output.txt"};
    parseCommands(7, commands, iterations, input_file, output_file);
    EXPECT_EQ(input_file, "input.txt");
    EXPECT_EQ(iterations, 27);
    EXPECT_EQ(output_file, "output.txt");
}
