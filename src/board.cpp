#include "board.h"
#include <memory>
#include <iostream>
#include <fstream>

//CONSTRUCTOR: Iniciem el tauler (de classe Board) amb punters nullptr (no apunten a res), i amb les dimensions passades com a valor de width i height.
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

//DESTRUCTOR: Alliberem els punters de la referencia, tornant-los a apuntar a nullptr (a res), i aixi alliberem la memoria i resetejem el tauler (Board).
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

//getCell: Amb el getCell podem saber quin candy es el punter a les coordenades x y que hem passat a la funcio.
//Iniciem el resultat de la funcio a nullptr, i comprovem si les coordenades pertanyen al tauler. De ser aixi, la
//funcio retorna el punter amb el tipus de candy que te la cella (si esta buida, seguira en nullptr).
//La funcio es una constant ja que no ha de modificar dades.
Candy* Board::getCell(int x, int y) const
{
    Candy* result = nullptr;
    if (x >= 0 && y >= 0 && x < m_width && y < m_height) {
        result = Tauler[x][y];
    }
    return result;
}

//getWidth: Aquesta funcio ens permet recuperar quina es l'amplitud del tauler.
//La funcio es una constant ja que no ha de modificar dades.
int Board::getWidth() const 
{
    return m_width;
}

//getHeight: Aquesta funcio ens permet recuperar quina es l'altura del tauler.
//La funcio es una constant ja que no ha de modificar dades.
int Board::getHeight() const 
{
    return m_height;
}


//setCell: la funcio rep un punter del tipus de candy i unes coordenades, i assigna el punter del tauler en les coordenades 
//x y proporcionades al mateix tipus de candy que se li ha passat. 
void Board::setCell(Candy* candy, int x, int y)
{
    if (x >= 0 && x < m_width && y >= 0 && y < m_height)
    {
        Tauler[x][y] = candy;
        
    }
}

//sholudExplode: comprova si, des de les coordenades x y proporcionades, seguint qualsevol de les vuit 
//direccions i comptant-se a si mateixa, hi ha tres o mes celles del mateix tipus de punter candy. De ser aixi retorna true. Si no hi ha tres 
// tipus de candy iguals retorna false.
bool Board::shouldExplode(int x, int y) const
{
    //Comprova que les coordenades estiguin dintre el tauler.
    if (x < 0 || x >= m_width || y < 0 || y >= m_height || Tauler[x][y] == nullptr)
    {
        return false;
    }

    CandyType type = Tauler[x][y]->getType();
    int totalCount = 0;

    //Comprova linia horitzontal (esquerra i dreta).
    totalCount = 1; // Comptador de candy actual.
    //Compta cap a la esquerra.
    for (int i = 1; x - i >= 0 && Tauler[x - i][y] != nullptr && Tauler[x - i][y]->getType() == type; i++)
    {
        totalCount++;
    }
    //Compta cap a la dreta.
    for (int i = 1; x + i < m_width && Tauler[x + i][y] != nullptr && Tauler[x + i][y]->getType() == type; i++)
    {
        totalCount++;
    }
    if (totalCount >= SHORTEST_EXPLOSION_LINE)
    {
        return true;
    }

    //Comprova linia vertical (adalt i abaix).
    totalCount = 1;
    //Compta adalt.
    for (int i = 1; y - i >= 0 && Tauler[x][y - i] != nullptr && Tauler[x][y - i]->getType() == type; i++)
    {
        totalCount++;
    }
    //Compta abaix.
    for (int i = 1; y + i < m_height && Tauler[x][y + i] != nullptr && Tauler[x][y + i]->getType() == type; i++)
    {
        totalCount++;
    }
    if (totalCount >= SHORTEST_EXPLOSION_LINE)
    {
        return true;
    }

    //Comprova diagonals (adalt-esquerrra i abaix-dreta) (\). 
    totalCount = 1;
    //Compta adalt-esquerrra
    for (int i = 1; x - i >= 0 && y - i >= 0 && Tauler[x - i][y - i] != nullptr && Tauler[x - i][y - i]->getType() == type; i++)
    {
        totalCount++;
    }
    //Compta abaix-dreta.
    for (int i = 1; x + i < m_width && y + i < m_height && Tauler[x + i][y + i] != nullptr && Tauler[x + i][y + i]->getType() == type; i++)
    {
        totalCount++;
    }
    if (totalCount >= SHORTEST_EXPLOSION_LINE)
    {
        return true;
    }

    //Comprova diagonals (adalt-dreta i esquerra-abaix) (/).
    totalCount = 1;
    //Compta adalt-dreta
    for (int i = 1; x + i < m_width && y - i >= 0 && Tauler[x + i][y - i] != nullptr && Tauler[x + i][y - i]->getType() == type; i++)
    {
        totalCount++;
    }
    //Compta esquerra-abaix.
    for (int i = 1; x - i >= 0 && y + i < m_height && Tauler[x - i][y + i] != nullptr && Tauler[x - i][y + i]->getType() == type; i++)
    {
        totalCount++;
    }
    if (totalCount >= SHORTEST_EXPLOSION_LINE)
    {
        return true;
    }
    //Si cap de les direccions te mes de 3 candy igual seguits al mateix de les coordenades, no hi ha d'haver cap explosio.
    return false;
}

//explodeAndDrop: bucle de joc on es fa al moviment de baixar les candy una posicio quan hi ha hagut una explosio.
std::vector<Candy*> Board::explodeAndDrop()
{
    //CandiesAExplotar es el vector on marcarem tots els candies explotats, aquest es el que retornem
    std::vector<Candy*> CandiesAExplotar;
    //Canvis s'encarrega de determinar a cada bucle si hi ha hagut algun moviment/explosio, quan detecti que no hi ha canvis, deixa de comprovar condicions i surt del bucle
    bool canvis = false;

    do
    {
        //Establim el bool canvis a false, ja que ha de comprovar a l'inici de cada bucle si hi ha hagut canvis.
        canvis = false;
        //La matriu de bools explotats[][] s'encarrega de guardar totes les posicions que explotaran.
        bool explotats[DEFAULT_BOARD_WIDTH][DEFAULT_BOARD_HEIGHT] = { false };

       //Marquem quines posicions han d'explotar i les posem a la matriu explotats[][] per a poder-les marcar i DESPReS explotar-les. 
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

        //Eliminem tots els caramels que hem marcat abans per a explotar
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

            //Part encarregada de la gravetat dels caramels
            //Comprova a cada posicio si hi ha un espai blanc, si es aixi, busca a sobre d'aquesta si hi ha caramels, si n'hi ha, els baixa.
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

//dump: Guardem tota la informacio del tauler en un fitxer extern que ens permet guardar de forma permanent la partida en l'estat actual.
bool Board::dump(const std::string& output_path) const
{
    //Declarem el fitxer que utilitzarem per gurdar les dades.
    std::ofstream file(output_path);

    if (!file.is_open())
    {
        return false;
    }
    //Primer guardem les mesures del tauler per poder-lo recrear adequadament.
    file << m_width << " " << m_height << std::endl;

    //Guardem tota la informacio del tauler: si el punter esta buit (nullptr), el guardem amb el valor -1, qualsevol altre tipus de canvi se li atribuieix un
    //valor i es guarda la posicio (x,y) amb aquest valor per tal de poder-la recuperar.
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

//load: carregar a la memoria una partida guardada en un fitxer extern.
bool Board::load(const std::string& input_path)
{
    //Declarem el fitxer que utilitzarem per llegir les dades.
    std::ifstream file(input_path);
    
    if (!file.is_open())
    {
        return false;
    }

    int width, height;
    //Llegim del fitxer les mesures del tauler.
    file >> width >> height;

    if (width <= 0 || height <= 0 || width > DEFAULT_BOARD_WIDTH || height > DEFAULT_BOARD_HEIGHT)
    {
        return false;
    }
    //Iniciem tota la matriu de punters amb les mesures adequades a nullptr.
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
            //Llegim el tipus de candy de la iteracio actual.
            file >> candyType;

            if (candyType == -1)
            {
                //Si el CandyType es -1, se li assigna el nullptr, una casella buida.
                Tauler[i][j] = nullptr;
            }
            else if (candyType >= 0 && candyType < static_cast<int>(CandyType::COUNT))
            {
                //En qualsevol altre cas, se li reassigna el tipus de candy que li pertoca.
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
