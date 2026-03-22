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
    //return Tauler[x][y];
    Candy* result = nullptr;
    if (x >= 0 && y >= 0 && x < m_width && y < m_height) {
        result = m_board[x][y];
    }
    return result;
}

//SETTERS
void Board::setCell(Candy* candy, int x, int y)
{
    Tauler[y][x] = candy;
}

//METODES
bool Board::shouldExplode(int x, int y) const
{
    //Si el comptador arriba a 3 en qualsevol direcció, s'ha de retornar true
    int comptador = 0;
    //Horitzontal esquerra:
    int i = 0;
    while (x - i >= 0 && Tauler[x - i][y] == Tauler[x][y])
    {
        comptador++;
        if (comptador == 3)
        {
            return true;
        }
        i++;
    }
    comptador = 0;

    //Horizontal dreta:
    int i = 0;
    while (x + i <= DEFAULT_BOARD_WIDTH && Tauler[x + i][y] == Tauler[x][y])
    {
        comptador++;
        if (comptador == 3)
        {
            return true;
        }
        i++;
    }
    comptador = 0;
    
    //Verdical adalt:
    int i = 0;
    while (y - i >= 0 && Tauler[x][y-i] == Tauler[x][y])
    {
        comptador++;
        if (comptador == 3)
        {
            return true;
        }
        i++;
    }
    comptador = 0;

    //Vertical abaix:
    int i = 0;
    while (y + i <= DEFAULT_BOARD_HEIGHT && Tauler[x][y + i] == Tauler[x][y])
    {
        comptador++;
        if (comptador == 3)
        {
            return true;
        }
        i++;
    }
    comptador = 0;
        
    //Diagonal esquerra-adalt:
    int i = 0;
    while (y - i >= 0 && x-i >= 0 && Tauler[x - i][y - i] == Tauler[x][y])
    {
        comptador++;
        if (comptador == 3)
        {
            return true;
        }
        i++;
    }
    comptador = 0;

    //Diagonal esquerra-abaix:
    int i = 0;
    while (y + i <= DEFAULT_BOARD_HEIGHT && x - i >= 0 && Tauler[x - i][y + i] == Tauler[x][y])
    {
        comptador++;
        if (comptador == 3)
        {
            return true;
        }
        i++;
    }
    comptador = 0;

    //Diagonal dreta-adalt:
    int i = 0;
    while (y - i >= 0 && x + i <= DEFAULT_BOARD_WIDTH && Tauler[x + i][y - i] == Tauler[x][y])
    {
        comptador++;
        if (comptador == 3)
        {
            return true;
        }
        i++;
    }
    comptador = 0;

    //Diagonal dreta-abaix:
    int i = 0;
    while (y + i <= DEFAULT_BOARD_HEIGHT && x + i <= DEFAULT_BOARD_WIDTH && Tauler[x + i][y + i] == Tauler[x][y])
    {
        comptador++;
        if (comptador == 3)
        {
            return true;
        }
        i++;
    }
    comptador = 0;

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
