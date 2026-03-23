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
            Tauler[i][j] = nullptr;
        }
    }
}

//GETTERS

Candy* Board::getCell(int x, int y) const
{
    //return Tauler[x][y];
    Candy* result = nullptr;
    if (x >= 0 && y >= 0 && x < m_width && y < m_height) {
        result = Tauler[x][y];
    }
    return result;
}

int Board::getWidth() const 
{
    return m_width;
}

int Board::getHeight() const 
{
    return m_height;
}


//SETTERS
void Board::setCell(Candy* candy, int x, int y)
{
    if (x >= 0 && x < m_width && y >= 0 && y < m_height)
    {
        Tauler[x][y] = candy;
        
    }
}

//METODES
bool Board::shouldExplode(int x, int y) const
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height || Tauler[x][y] == nullptr)
    {
        return false;
    }

    CandyType type = Tauler[x][y]->getType();
    int totalCount = 0;

    // Check horizontal line (left + right)
    totalCount = 1; // Count the current candy
    // Count to the left
    for (int i = 1; x - i >= 0 && Tauler[x - i][y] != nullptr && Tauler[x - i][y]->getType() == type; i++)
    {
        totalCount++;
    }
    // Count to the right
    for (int i = 1; x + i < m_width && Tauler[x + i][y] != nullptr && Tauler[x + i][y]->getType() == type; i++)
    {
        totalCount++;
    }
    if (totalCount >= SHORTEST_EXPLOSION_LINE)
    {
        return true;
    }

    // Check vertical line (up + down)
    totalCount = 1;
    // Count upward
    for (int i = 1; y - i >= 0 && Tauler[x][y - i] != nullptr && Tauler[x][y - i]->getType() == type; i++)
    {
        totalCount++;
    }
    // Count downward
    for (int i = 1; y + i < m_height && Tauler[x][y + i] != nullptr && Tauler[x][y + i]->getType() == type; i++)
    {
        totalCount++;
    }
    if (totalCount >= SHORTEST_EXPLOSION_LINE)
    {
        return true;
    }

    // Check diagonal (top-left to bottom-right) (\) 
    totalCount = 1;
    // Count top-left direction
    for (int i = 1; x - i >= 0 && y - i >= 0 && Tauler[x - i][y - i] != nullptr && Tauler[x - i][y - i]->getType() == type; i++)
    {
        totalCount++;
    }
    // Count bottom-right direction
    for (int i = 1; x + i < m_width && y + i < m_height && Tauler[x + i][y + i] != nullptr && Tauler[x + i][y + i]->getType() == type; i++)
    {
        totalCount++;
    }
    if (totalCount >= SHORTEST_EXPLOSION_LINE)
    {
        return true;
    }

    // Check diagonal (top-right to bottom-left) (/)
    totalCount = 1;
    // Count top-right direction
    for (int i = 1; x + i < m_width && y - i >= 0 && Tauler[x + i][y - i] != nullptr && Tauler[x + i][y - i]->getType() == type; i++)
    {
        totalCount++;
    }
    // Count bottom-left direction
    for (int i = 1; x - i >= 0 && y + i < m_height && Tauler[x - i][y + i] != nullptr && Tauler[x - i][y + i]->getType() == type; i++)
    {
        totalCount++;
    }
    if (totalCount >= SHORTEST_EXPLOSION_LINE)
    {
        return true;
    }

    return false;
}

std::vector<Candy*> Board::explodeAndDrop()
{
    std::vector<Candy*> CandiesAExplotar;
    bool canvis = false;

    do
    {
        canvis = false;
        bool explotats[DEFAULT_BOARD_WIDTH][DEFAULT_BOARD_HEIGHT] = { false };

       
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

        
        if (canvis)
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

            
            for (int i = 0; i < m_width; i++)
            {
                for (int j = m_height - 1; j >= 0; j--)
                {
                    if (Tauler[i][j] == nullptr)
                    {
                        
                        for (int k = j - 1; k >= 0; k--)
                        {
                            if (Tauler[i][k] != nullptr)
                            {
                                setCell(Tauler[i][k], i, j);
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
    return {};
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

    for (int i = 0; i < DEFAULT_BOARD_WIDTH; i++) 
    {
        for (int j = 0; j < DEFAULT_BOARD_HEIGHT; j++) 
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
