#include "board.h"
#include <memory>
#include <iostream>
#include <fstream>

//CONSTRUCTOR / DESTRUCTOR
Board::Board(int width, int height)
{
    m_width = width;
    m_height = height;
    for (int i = 0; i < width; i++) 
    {
        for (int j = 0; j < height; j++) 
        {
            Tauler[i][j] = nullptr;
        }
    }
}

Board::~Board()
{
    for (int i = 0; i < m_width; i++)
    {
        for (int j = 0; j < m_height; j++)
        {
            delete Tauler[i][j];
            Tauler[i][j] = nullptr;
        }
    }
}

//GETTERS
int Board::getWidth() const 
{
    return m_width;
}

int Board::getHeight() const 
{
    return m_height;
}

Candy* Board::getCell(int x, int y) const
{
    //return Tauler[x][y];
    Candy* result = nullptr;
    if (x >= 0 && y >= 0 && x < m_width && y < m_height) {
        result = Tauler[x][y];
    }
    return result;
}

//SETTERS
void Board::setCell(Candy* candy, int x, int y)
{
    Tauler[x][y] = candy;
}

//METODES
bool Board::shouldExplode(int x, int y) const
{

    if (x < 0 || x >= m_width || y < 0 || y >= m_height || Tauler[x][y] == nullptr)
    {
        return false;
    }

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
    i = 0;
    while (x + i <= m_width && Tauler[x + i][y] == Tauler[x][y])
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
    i = 0;
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
    i = 0;
    while (y + i <= m_height && Tauler[x][y + i] == Tauler[x][y])
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
    i = 0;
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
    i = 0;
    while (y + i <= m_height && x - i >= 0 && Tauler[x - i][y + i] == Tauler[x][y])
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
    i = 0;
    while (y - i >= 0 && x + i <= m_width && Tauler[x + i][y - i] == Tauler[x][y])
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
    i = 0;
    while (y + i <= m_height && x + i <= m_width && Tauler[x + i][y + i] == Tauler[x][y])
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

    bool canvis = false;
    bool hanCaigut = false;
    std::vector<Candy*> CandiesAExplotar;

    do
    {
        canvis = false;
        bool explotats[DEFAULT_BOARD_WIDTH][DEFAULT_BOARD_HEIGHT] = {false};
        //Marca els caramels que cal explotar
        for (int i = 0; i < m_width; i++)
        {
            for (int j = 0; j < m_height; j++)
            {
                if (shouldExplode(i, j))
                {
                    canvis = true;
                    explotats[i][j] = true;
                }
            }
        }

        //Elimina els caramels que calgui
        if(canvis)
        {
            for (int i = 0; i < m_width; i++)
            {
                for (int j = 0; j < m_height; j++)
                {
                    if (explotats[i][j] && Tauler[i][j] != nullptr)
                    {
                        CandiesAExplotar.push_back(Tauler[i][j]);
                        Tauler[i][j] = nullptr;
                    }
                }
            }
        }
        //Baixa tots els caramels necessaris
        if (canvis)
        {
            for (int i = 0; i < m_width; i++)
            {
                for (int j = m_height - 1; j >= 0; j--)
                {
                    if (Tauler[i][j] == nullptr)
                    {
                        //Comprova la resta de la columna per si han de caure caramels
                        for (int k = j - 1; k >= 0; k--)
                        {
                            if (Tauler[i][j] != nullptr)
                            {
                                setCell(getCell(i, k), i, j);
                                setCell(nullptr, i, k);
                                break;
                            }
                        }
                    }
                }
            }
        }

    } while (canvis);
    return CandiesAExplotar;
}

bool Board::dump(const std::string& output_path) const
{
    // Implement your code here
    std::ofstream file(output_path);

    if (!file.is_open())
    {
        return false;
    }

    file << m_width << " " << m_height << std::endl;

    for (int j = 0; j < m_height; j++)
    {
        for (int i = 0; i < m_width; i++)
        {
            if (Tauler[i][j] == nullptr)
            {
                file << "-1";
            }
            else
            {
                file << static_cast<int>(Tauler[i][j]->getType()) << " ";
            }
        }
        file << std::endl;
    }
    file.close();
    return true;
    
}

bool Board::load(const std::string& input_path)
{
    // Implement your code here
    std::ifstream file(input_path);
    
    if (!file.is_open())
    {
        return false;
    }

    int width, height;
    file >> width >> height;

    if (width <= 0 || height <= 0 || width > DEFAULT_BOARD_WIDTH || height > DEFAULT_BOARD_HEIGHT)
    {
        return false;
    }

    for (int i = 0; i < m_width; i++)
    {
        for (int j = 0; j < m_height; j++)
        {
            delete Tauler[i][j];
            Tauler[i][j] = nullptr;
        }
    }

    m_width = width;
    m_height = height;

    for (int j = 0; j < m_height; j++)
    {
        for (int i = 0; i < m_width; i++)
        {
            int candyType;
            file >> candyType;

            if (candyType == -1)
            {
                Tauler[i][j] = nullptr;
            }
            else if (candyType >= 0 && candyType < static_cast<int>(CandyType::COUNT))
            {
                Tauler[i][j] = new Candy(static_cast<CandyType>(candyType));
            }
            else
            {
                return false;
            }
        }
    }
    file.close();
    return true;
}
