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
    Tauler[y][x] = candy;
}

//METODES
bool Board::shouldExplode(int x, int y) const
{
    //Codi que segurament es pot millorar a futur:

    if (Tauler[x - 2] == Tauler[x - 1] == Tauler[x])
    {
        return true;
    }
    else if (Tauler[x - 1] == Tauler[x] == Tauler[x + 1])
    {
        return true;
    }
    else if (Tauler[x] == Tauler[x + 1] == Tauler[x + 2])
    {
        return true;
    }
    else if (Tauler[y - 2] == Tauler[y- 1] == Tauler[y])
    {
        return true;
    }
    else if (Tauler[y - 1] == Tauler[y] == Tauler[y + 1])
    {
        return true;
    }
    else if (Tauler[y] == Tauler[y + 1] == Tauler[y + 2])
    {
        return true;
    }
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
