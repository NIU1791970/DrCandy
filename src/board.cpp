#include "board.h"
#include <memory>
#include <iostream>

Board::Board(int width, int height)
{
    for (int i = 0; i < width; i++) 
    {
        for (int j = 0; j < height; j++0) 
        {
            Tauler[j][i] = 0;
        }
    }
}

Board::~Board()
{
    delete Tauler;
}


Candy* Board::getCell(int x, int y) const
{
    // Implement your code here
    return nullptr;
}

void Board::setCell(Candy* candy, int x, int y)
{
    // Implement your code here
}


int Board::getWidth() const
{
    // Implement your code here
    return -1;
}


int Board::getHeight() const
{
    // Implement your code here
    return -1;
}

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
