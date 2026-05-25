#include "board.h"
#include <memory>
#include <iostream>
#include <fstream>


//Per a accedir comodament a l'array de celles (el tauler), aquesta funcio converteix un conjunt de coordenades en un sol nombre enter.
//Si assumim les dimensions predeterminades, el punt (3, 5) passa a ser 35 o (7,4) passa a ser (4,7)
//Per tant, on abans accediem a Tauler[x][y], ara hi accedim amb m_celles[idx(x, y)]
int Board::idx(int x, int y) const
{
    return y * m_width + x;
}
//CONSTRUCTOR: Iniciem el tauler (de la classe Board) amb punters nullptr (no apunten a res), i amb les dimensions pasades com a valor de width i height.
Board::Board(int width, int height) : m_width(width), m_height(height), m_celles(nullptr)
{
    creaTauler();
}

Board::Board(const Board& other)
{
    m_width = other.m_width;
    m_height = other.m_height;
    m_celles = other.m_celles;
    creaTauler();
    for (int i = 0; i < m_width * m_height; i++)
    {
        if (other.m_celles == nullptr)
        {
            m_celles[i] == nullptr;
        }
        else
        {
            m_celles[i] == new Candy(other.m_celles[i]->getType());
        }
    }
}

//DESTRUCTOR: Alliberem els punters de la referència, tornant-los a apuntar a nullptr (a res), i aixi alliberem la memoria i resetejem el tauler (Board).
Board::~Board()
{
    lliberaMemoria();
}

//getCell: Amb el getCell podem saber quin candy és el punter a les coordenades x y que hem passat a la funcio.
//Iniciem el resultat de la funcio a nullptr, i comprovem si les coordenades pertanyen al tauler. De ser aixi, la
//funcio retorna el punter amb el tipus de candy que té la cel·la (si esta buida, seguira en nullptr).
//La funcio és una constant ja que no ha de modificar dades.
Candy* Board::getCell(int x, int y) const
{
    Candy* result = nullptr;
    if (x >= 0 && y >= 0 && x < m_width && y < m_height) {
        result = m_celles[idx(x, y)];
    }
    return result;
}

//getWidth: Aquesta funcio ens permet recuperar quina és l'amplitud del tauler.
//La funcio és una constant ja que no ha de modificar dades.
int Board::getWidth() const
{
    return m_width;
}

//getHeight: Aquesta funcio ens permet recuperar quina és l'altura del tauler.
//La funcio és una constant ja que no ha de modificar dades.
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
        delete m_celles[idx(x, y)];
        m_celles[idx(x, y)] = candy;

    }
}

bool Board::estaDintre(int x, int y) const
{
    if (x >= 0 && x < m_width && y >= 0 && y < m_height)
    {
        return true;
    }
    return false;
}

void Board::creaTauler()
{
    int nCelles = m_width * m_height;
    m_celles = new Candy*[nCelles];
    for (int i = 0; i < nCelles; i++)
    {
        m_celles[i] = nullptr;
    }
}
void Board::lliberaMemoria()
{
    int nCelles = m_width * m_height;
    for (int i = 0; i < nCelles; i++)
    {
        delete m_celles[i];
    }
    delete[] m_celles;
    m_celles = nullptr;
}
//sholudExplode: comprova si, des de les coordenades x y proporcionades, seguint qualsevol de les vuit 
//direccions i comptant-se a si mateixa, hi ha tres o més cel·les del mateix tipus de punter candy. De ser aixi retorna true. Si no hi ha tres 
// tipus de candy iguals retorna false.
bool Board::shouldExplode(int x, int y) const
{
    //Comprova que les coordenades estiguin dintre el tauler.
    if (x < 0 || x >= m_width || y < 0 || y >= m_height || m_celles[idx(x, y)] == nullptr)
    {
        return false;
    }

    CandyType type = m_celles[idx(x, y)]->getType();
    int totalCount = 0;

    //Comprova linia horitzontal (esquerra i dreta).
    totalCount = 1; // Comptador de candy actual.
    //Compta cap a la esquerra.
    for (int i = 1; x - i >= 0 && m_celles[idx(x-i, y)] != nullptr && m_celles[idx(x - i, y)]->getType() == type; i++)
    {
        totalCount++;
    }
    //Compta cap a la dreta.
    for (int i = 1; x + i < m_width && m_celles[idx(x + i, y)] != nullptr && m_celles[idx(x + i, y)]->getType() == type; i++)
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
    for (int i = 1; y - i >= 0 && m_celles[idx(x, y - i)] != nullptr && m_celles[idx(x, y - i)]->getType() == type; i++)
    {
        totalCount++;
    }
    //Compta abaix.
    for (int i = 1; y + i < m_height && m_celles[idx(x, y + i)] != nullptr && m_celles[idx(x, y + i)]->getType() == type; i++)
    {
        totalCount++;
    }
    if (totalCount >= SHORTEST_EXPLOSION_LINE)
    {
        return true;
    }

    //Comprova diagonals (adalt-esquerrra i abaix-dreta) (). 
    totalCount = 1;
    //Compta adalt-esquerrra
    for (int i = 1; x - i >= 0 && y - i >= 0 && m_celles[idx(x - i, y - i)] != nullptr && m_celles[idx(x - i, y - i)]->getType() == type; i++)
    {
        totalCount++;
    }
    //Compta abaix-dreta.
    for (int i = 1; x + i < m_width && y + i < m_height && m_celles[idx(x + i, y + i)] != nullptr && m_celles[idx(x + i, y + i)]->getType() == type; i++)
    {
        totalCount++;
    }
    if (totalCount >= SHORTEST_EXPLOSION_LINE)
    {
        return true;
    }

    //Comprova diagonals (adalt-dreta i esquerra-abaix) ().
    totalCount = 1;
    //Compta adalt-dreta
    for (int i = 1; x + i < m_width && y - i >= 0 && m_celles[idx(x + i, y - i)] != nullptr && m_celles[idx(x + i, y - i)]->getType() == type; i++)
    {
        totalCount++;
    }
    //Compta esquerra-abaix.
    for (int i = 1; x - i >= 0 && y + i < m_height && m_celles[idx(x - i, y + i)] != nullptr && m_celles[idx(x - i, y + i)]->getType() == type; i++)
    {
        totalCount++;
    }
    if (totalCount >= SHORTEST_EXPLOSION_LINE)
    {
        return true;
    }
    //Si cap de les direccions té més de 3 candy igual seguits al mateix de les coordenades, no hi ha d'haver cap explosio.
    return false;
}

//explodeAndDrop: bucle de joc on es fa al moviment de baixar les candy una posicio quan hi ha hagut una explosio.
std::vector<Candy*> Board::explodeAndDrop()
{
    //CandiesAExplotar és el vector on marcarem tots els candies explotats, aquest és el que retornem
    std::vector<Candy*> CandiesAExplotar;
    //Canvis s'encarrega de determinar a cada bucle si hi ha hagut algun moviment /explosio, quan detecti que no hi ha canvis, deixa de comprovar condicions i surt del bucle
    bool canvis = false;

    do
    {
        //Establim el bool canvis a false, ja que ha de comprovar a l'inici de cada bucle si hi ha hagut canvis.
        canvis = false;
        //La matriu de bools explotats[][] s'encarrega de guardar totes les posicions que explotaran.
        bool explotats[DEFAULT_BOARD_WIDTH][DEFAULT_BOARD_HEIGHT] = { false };


        //Marquem quines posicions han d'explotar i les posem a la matriu explotats[][] per a poder-les marcar i DESPRÉS explotar-les. 
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
                    if (explotats[i][j] && m_celles[idx(i, j)] != nullptr)
                    {
                        CandiesAExplotar.push_back(m_celles[idx(i, j)]);
                        m_celles[idx(i, j)] = nullptr;
                    }
                }
            }

            //Part encarregada de la gravetat dels caramels
            //Comprova a cada posicio si hi ha un espai blanc, si es aixi, busca a sobre d'aquesta si hi ha caramels, si n'hi ha, els baixa.
            for (int i = 0; i < m_width; i++)
            {
                for (int j = m_height - 1; j >= 0; j--)
                {
                    if (m_celles[idx(i, j)] == nullptr)
                    {

                        for (int k = j - 1; k >= 0; k--)
                        {
                            if (m_celles[idx(i, k)] != nullptr)
                            {
                                m_celles[idx(i, j)] = m_celles[idx(i, k)];
                                m_celles[idx(i, k)] = nullptr;
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

bool Board::operator==(const Board& altre) const
{
    bool diferents = false;
    if (m_width != altre.m_width || m_height != altre.m_height)
    {
        return false;
    }
    int nCelles = m_width * m_height;
    int i = 0;
    while (i < nCelles && !diferents)
    {
        bool aquestBuit = m_celles[i] == nullptr;
        bool altreBuit = altre.m_celles[i] == nullptr;
        if (aquestBuit != altreBuit)
        {
            diferents = true;
        }
        else if (!aquestBuit && m_celles[i]->getType() != altre.m_celles[i]->getType())
        {
            diferents = true;
        }
        i++;
    }
    return !diferents;
}

Board& Board::operator=(const Board& other)
{
    if (this == &other)
    {
        return *this;
    }
    lliberaMemoria();
    m_width = other.m_width;
    m_height = other.m_height;
    creaTauler();
    int mida = m_width * m_height;
    for (int i = 0; i < mida; i++)
    {
        if (other.m_celles[i] == nullptr)
        {
            m_celles[i] = nullptr;
        }
        else
        {
            m_celles[i] = new Candy(other.m_celles[i]->getType());
        }
    }
    return *this;
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
            if (m_celles[idx(i, j)] == nullptr)
            {
                file << "-1 ";
            }
            else
            {
                file << static_cast<int>(m_celles[idx(i, j)]->getType()) << " ";
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
    std::ifstream file(input_path);
    if (!file.is_open())
    {
        return false;
    }
    //Establim les dimensions del tauler amb les donades al fitxer
    int width, height;
    file >> width >> height;

    lliberaMemoria();
    m_width = width;
    m_height = height;
    creaTauler();

    //Per a evitar conflictes, borrem el tauler que hi pugues haver abans d'aquest
    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            delete m_celles[idx(x, y)];
        }
    }

    //Recorrem la matriu sencera i assignem a cada posicio el valor que se li assigna al fitxer d'input.
    //A cada linea hi ha un nombre del 0 al 5 on cada nombre representa un tipus de caramel
    //Tambe hi ha el -1 que indica cel·la buida
    for (int y = 0; y < m_height; y++)
    {
        for (int x = 0; x < m_width; x++)
        {
            int type;
            file >> type;
            switch (type)
            {
            case 0:
                m_celles[idx(x, y)] = new Candy(CandyType::TYPE_RED);
                break;
            case 1:
                m_celles[idx(x, y)] = new Candy(CandyType::TYPE_BLUE);
                break;
            case 2:
                m_celles[idx(x, y)] = new Candy(CandyType::TYPE_GREEN);
                break;
            case 3:
                m_celles[idx(x, y)] = new Candy(CandyType::TYPE_YELLOW);
                break;
            case 4:
                m_celles[idx(x, y)] = new Candy(CandyType::TYPE_PURPLE);
                break;
            case 5:
                m_celles[idx(x, y)] = new Candy(CandyType::TYPE_ORANGE);
                break;
            default:
                m_celles[idx(x, y)] = nullptr;
                break;
            }
        }
    }

    file.close();
    return true;

}