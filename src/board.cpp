#include "board.h"
#include <memory>
#include <iostream>

//CONSTRUCTOR / DESTRUCTOR
Board::Board(int width, int height)
{
    for (int i = 0; i < width; i++) 
    {
        for (int j = 0; j < height; j++) 
        {
            Tauler[j][i] = nullptr;
        }
    }
}

Board::~Board()
{
    delete Tauler;
}

//GETTERS
int Board::getWidth() const 
{
    return DEFAULT_BOARD_WIDTH;
}

int Board::getHeight() const 
{
    return DEFAULT_BOARD_HEIGHT;
}

Candy* Board::getCell(int x, int y) const
{
    return Tauler[x][y];
}

//SETTERS
void Board::setCell(Candy* candy, int x, int y)
{
    // Implement your code here
}

//METODES
bool Board::shouldExplode(int x, int y) const
{
    // Implement your code here
    return false;
}

std::vector<Candy*> Board::explodeAndDrop()
{
    // Implement your code here
    return {};
}

bool Board::dump(const std::string& output_path) const
{
    // Implement your code here
    return false;
}

bool Board::load(const std::string& input_path)
{
    // Implement your code here
    return false;
}
