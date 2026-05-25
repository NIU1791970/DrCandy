#include "game.h"
#include <random>
#include "graphics.h"
#include "candy.h"

#include <fstream>
#include <iostream>
#include <string>
using namespace std;

Game::Game()
{
    m_board = Board(DEFAULT_BOARD_WIDTH, DEFAULT_BOARD_HEIGHT);
    m_comptadorFrames = 0;
    m_gameOver = false;
    m_score = 0;
}

Game::~Game()
{
    // Implement your code here
}


bool Game::potEsquerra() const
{
    bool pot = true;
    int col = m_bloc.getColumna();
    int fila = m_bloc.getFila();
    if (col - 1 < 0)
    {
        pot = false;
    }
    else
    {
        for (int i = 0; i < MIDA_BLOC; i++)
        {
            if (fila - i >= 0 && fila - i < m_board.getHeight())
            {
                if (m_board.getCell(col - 1, fila - i) != nullptr)
                {
                    pot = false;
                }
            }
        }
    }
    return pot;
}

bool Game::potDreta() const
{
    bool pot = true;
    int col = m_bloc.getColumna();
    int fila = m_bloc.getFila();
    if (col + 1 >= m_board.getWidth())
    {
        pot = false;
    }
    else
    {
        for (int i = 0; i < MIDA_BLOC; i++)
        {
            if (fila - i >= 0 && fila - i < m_board.getHeight())
            {
                if (m_board.getCell(col + 1, fila - i) != nullptr)
                {
                    pot = false;
                }
            }
        }
    }
    return pot;
}

bool Game::potCaure() const
{
    bool pot = true;
    int col = m_bloc.getColumna();
    int fila = m_bloc.getFila();
    if (fila + 1 >= m_board.getHeight())
    {
        pot = false;
    }
    if (fila >= 0 && m_board.getCell(col, fila + 1) != nullptr)
    {
        pot = false;
    }
    return pot;
}

void Game::blocAlTerra()
{
    int col = m_bloc.getColumna();
    int fila = m_bloc.getFila();

    for(int i = 0; i < MIDA_BLOC; i++)
    {
        int filaCandy = fila - (MIDA_BLOC - 1 - i);
        if (filaCandy >= 0 && filaCandy < m_board.getHeight())
        {
            if (m_board.getCell(col, filaCandy) == nullptr)
            {
                Candy* c = new Candy(m_bloc.getCandyType(i));
                m_board.setCell(c, col, filaCandy);
            }
            
        }
    }

    vector<Candy*> exploded = m_board.explodeAndDrop();
    m_score += exploded.size();
    for (int i = 0; i < exploded.size(); i++)
    {
        delete exploded[i];
    }

    creaBloc();
}

void Game::creaBloc()
{
    m_bloc = Bloc();
    if (m_board.getCell(m_bloc.getColumna(), 0) != nullptr)
    {
        m_gameOver = true;
    }
}

void Game::update(const Controller& controller)
{
    if (m_gameOver)
    {
        return;
    }
    if (controller.isLeftPressed() && potEsquerra())
    {
        cout << "ESQUERRA\n";
        m_bloc.esquerra();
    }
    if (controller.isRightPressed() && potDreta())
    {
        m_bloc.dreta();
        cout << "DRETA\n";
    }
    if (controller.isKey1Pressed())
    {
        cout << "CICLE\n";
        m_bloc.cicle();
    }
    if (controller.isKey2Pressed())
    {
        dump("save.txt");
    }

    m_comptadorFrames++;

    if (controller.isDownPressed() || m_comptadorFrames >= 60)
    {
        m_comptadorFrames = 0;
        if (potCaure())
        {
            m_bloc.baixar();
        }
        else
        {
            if (m_bloc.getFila() < 0)
            {
                m_gameOver = true;
                return;
            }
            blocAlTerra();
        }
    }
    
    // Implement your code here
}

void Game::render(GraphicManager& graphics)
{
    graphics.drawImage("img/logo_small.png", 10, 10);

    graphics.drawRectangle(
        CANDY_IMAGE_HEIGHT * 3, CANDY_IMAGE_HEIGHT * 3,
        CANDY_IMAGE_WIDTH * 10,
        CANDY_IMAGE_HEIGHT * 10,
        5, 150, 150, 150);
    for (int y = 0; y < m_board.getHeight(); y++)
    {
        for (int x = 0; x < m_board.getWidth(); x++)
        {
            Candy* c = m_board.getCell(x, y);
            if (c != nullptr)
            {
                graphics.drawImage(c->getResourceName(),
                    CANDY_IMAGE_WIDTH * (3 + x),
                    CANDY_IMAGE_HEIGHT * (3+ y));
            }
        }
    }

    int col = m_bloc.getColumna();
    int fila = m_bloc.getFila();
    for (int i = 0; i < MIDA_BLOC; i++)
    {
        int filaCandy = fila - (MIDA_BLOC - 1 - i);
        Candy temporal(m_bloc.getCandyType(i));
        graphics.drawImage(temporal.getResourceName(), 
            CANDY_IMAGE_WIDTH * (3 + col), 
            CANDY_IMAGE_HEIGHT * (3 + filaCandy));
    }

    if (m_gameOver)
    {
        graphics.drawText("GAME OVER", 200, 10, 50, 200, 80, 80);
        graphics.drawText("Exit [ESC]", 25, 700, 20, 100, 100, 100);
    }
    else
    {
        graphics.drawText("Movement: [Up] [Down] [Left] [Right]  --  "
            "Buttons: [Q] [W] [E]  --  Exit [ESC]",
            25, 700, 20, 100, 100, 100);
        graphics.drawText("Score: " + std::to_string(m_score), 450, 10, 70, 125, 200, 125);
    }

    /*
    // Implement your code here
    
    // Note: the following code exhibits the main graphic library features
    // Board: border [draw rectangles] and a single piece of candy
    const int board_size = 10;
    const int board_padding = 3;
    graphics.drawRectangle(
        CANDY_IMAGE_HEIGHT * board_padding, CANDY_IMAGE_HEIGHT * board_padding,
        CANDY_IMAGE_WIDTH * board_size,
        CANDY_IMAGE_HEIGHT * board_size,
        5, 150, 150, 150);
    // Board: place a candy piece

    

    graphics.drawImage(Candy(CandyType::TYPE_PURPLE).getResourceName(),
        CANDY_IMAGE_WIDTH * 3,
        CANDY_IMAGE_HEIGHT * 3);
    // Title [draw images]
    graphics.drawImage("img/logo_small.png", 10, 10);
    // Score and footer [draw text]
    graphics.drawText("Movement: [Up] [Down] [Left] [Right]  --  "
                      "Buttons: [Q] [W] [E]  --  Exit [ESC]",
                      25, 700, 20, 100, 100, 100);
    graphics.drawText("Score: ", 450, 10, 70, 125, 200, 125);*/
}

void Game::run()
{
    const int screen_width = 750;
    const int screen_height = 750;
    const int bg_red = 255;
    const int bg_green = 255;
    const int bg_blue = 255;
    runGraphicGame(*this, screen_width, screen_height, bg_red, bg_green, bg_blue);
}

bool Game::dump(const string& output_path) const
{
    ofstream fitxer(output_path);
    if (!fitxer.is_open())
    {
        return false;
    }
    int temp = 0;
    if (m_gameOver == true)
    {
        temp = 1;
    }
    fitxer << m_score << " " << temp << " " << m_comptadorFrames << " " << endl;
    fitxer << m_bloc.getColumna() << " " << m_bloc.getFila() << " ";
    for (int i = 0; i < MIDA_BLOC; i++)
    {
        
        fitxer << " " << static_cast<int>(m_bloc.getCandyType(i));
        /*if (i < MIDA_BLOC - 1)
        {
            fitxer << " ";
        }*/
        
    }
    fitxer << endl;
    

    fitxer << m_board.getWidth() << " " << m_board.getHeight() << endl;
    for (int y = 0; y < m_board.getHeight(); y++)
    {
        cout << "A" << y << endl;
        for (int x = 0; x < m_board.getWidth(); x++)
        {
            cout << "B" << x << endl;
            Candy* c = m_board.getCell(x, y);
            cout << "C" << endl;
            if (c == nullptr)
            {
                fitxer << -1 << " ";
            }
            else
            {
                //cout << "AHA - " << c << endl;
                CandyType type = c->getType();
                fitxer << static_cast<int>(type) << " ";
                //cout << "Hmm ";
            }
        }
        fitxer << endl;
    }
    
    return true;
}

bool Game::load(const string& input_path)
{
    ifstream fitxer(input_path);
    if (!fitxer.is_open())
    {
        return false;
    }
    int temp;
    fitxer >> m_score >> temp >> m_comptadorFrames;
    if (temp == 0)
    {
        m_gameOver = false;
    }
    else
    {
        m_gameOver = true;
    }

    int col, fila;
    fitxer >> col >> fila;
    CandyType types[MIDA_BLOC];
    for (int i = 0; i < MIDA_BLOC; i++)
    {
        int tipus;
        fitxer >> tipus;
        types[i] = static_cast<CandyType>(tipus);
    }
    m_bloc = Bloc(col, fila, types);

    int width, height;
    fitxer >> width >> height;

    m_board = Board(width, height);
    
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int tipus;
            fitxer >> tipus;
            if (tipus >= 0 && tipus < static_cast<int>(CandyType::COUNT))
            {
                Candy* c = new Candy(static_cast<CandyType>(tipus));
                m_board.setCell(c, x, y);
            }
        }
    }
    fitxer.close();
    return true;
}

bool Game::operator==(const Game& other) const
{
    if (m_gameOver != other.m_gameOver)
    {
        return false;
    }
    if (m_score != other.m_score)
    {
        return false;
    }
    if (!(m_board == other.m_board))
    {
        return false;
    }
    if (m_bloc.getColumna() != other.m_bloc.getColumna())
    {
        return false;
    }
    if (m_bloc.getFila() != other.m_bloc.getFila())
    {
        return false;
    }
    for (int i = 0; i < MIDA_BLOC; i++)
    {
        if (m_bloc.getCandyType(i) != other.m_bloc.getCandyType(i))
        {
            return false;
        }
    }
    return true;
}