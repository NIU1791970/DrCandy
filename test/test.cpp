#include <filesystem>
#include <iostream>
#include "board.h"
#include "candy.h"
#include "game.h"
#include "bloc.cpp"
#include "util.h"

using namespace std;
// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

static bool check(bool condition, const char* message)
{
    if (!condition)
        std::cout << "TEST FAILED: " << message << std::endl;
    return condition;
}

// ---------------------------------------------------------------------------
// Board tests
// ---------------------------------------------------------------------------

static bool testBoardDimensions()
{
    cout << "TestDimensions\n";
    Board b(5, 7);
    return check(b.getWidth() == 5, "getWidth should return 5")
        && check(b.getHeight() == 7, "getHeight should return 7");
}

static bool testSetGetCell()
{
    cout << "TestSetGetCell\n";
    Board b(10, 10);
    Candy* c= new Candy(CandyType::TYPE_ORANGE);
    b.setCell(c, 0, 0);
    if (b.getCell(0, 0) == c)
    {
        return true;
    }
    //return check(b.getCell(0, 0) == &c, "getCell should return the set candy");
    return false;
}
/*
static bool testGetCellOutOfBounds()
{
    cout << "TestGetCellOutOfBounds\n";
    Board b(10, 10);
    return check(b.getCell(-1, 0) == nullptr, "getCell(-1,0) should be nullptr")
        && check(b.getCell(0, -1) == nullptr, "getCell(0,-1) should be nullptr")
        && check(b.getCell(10, 0) == nullptr, "getCell(10,0) should be nullptr")
        && check(b.getCell(0, 10) == nullptr, "getCell(0,10) should be nullptr");
}

static bool testSetCellOutOfBounds()
{
    cout << "TestSetCellOutOfBounds\n";
    // Should not crash or corrupt memory.
    Board b(10, 10);
    Candy c(CandyType::TYPE_RED);
    b.setCell(&c, -1, 0);
    b.setCell(&c, 0, -1);
    b.setCell(&c, 10, 0);
    b.setCell(&c, 0, 10);
    return check(true, "setCell out-of-bounds should not crash");
}*/

static bool testShouldExplodeHorizontal()
{
    cout << "TestShouldExplodeHoritzontal\n";
    Board b(10, 10);
    Candy* c1 = new Candy(CandyType::TYPE_RED);
    Candy* c2 = new Candy(CandyType::TYPE_RED);
    Candy* c3 = new Candy(CandyType::TYPE_RED);
    b.setCell(c1, 0, 5);
    b.setCell(c2, 1, 5);
    b.setCell(c3, 2, 5);
    return check(b.shouldExplode(0, 5), "shouldExplode: left end of horizontal line")
        && check(b.shouldExplode(1, 5), "shouldExplode: middle of horizontal line")
        && check(b.shouldExplode(2, 5), "shouldExplode: right end of horizontal line");
}

static bool testShouldExplodeVertical()
{
    cout << "TestShouldExplodeVertical\n";
    Board b(10, 10);
    Candy* c1 = new Candy(CandyType::TYPE_BLUE);
    Candy* c2 = new Candy(CandyType::TYPE_BLUE); 
    Candy* c3 = new Candy(CandyType::TYPE_BLUE);
    b.setCell(c1, 3, 0);
    b.setCell(c2, 3, 1);
    b.setCell(c3, 3, 2);
    return check(b.shouldExplode(3, 1), "shouldExplode: middle of vertical line");
}

static bool testShouldExplodeDiagonal()
{
    cout << "TestShouldExplodeDiagonal\n";
    Board b(10, 10);
    Candy* c1 = new Candy(CandyType::TYPE_GREEN);
    Candy* c2 = new Candy(CandyType::TYPE_GREEN);
    Candy* c3 = new Candy(CandyType::TYPE_GREEN);
    b.setCell(c1, 0, 0);
    b.setCell(c2, 1, 1);
    b.setCell(c3, 2, 2);
    return check(b.shouldExplode(1, 1), "shouldExplode: middle of diagonal line");
}

static bool testShouldNotExplode()
{
    cout << "TestShouldNotExplode\n";
    Board b(10, 10);
    Candy* c1 = new Candy(CandyType::TYPE_RED);
    Candy* c2 = new Candy(CandyType::TYPE_BLUE);
    b.setCell(c1, 0, 0);
    b.setCell(c2, 1, 0);
    return check(!b.shouldExplode(0, 0), "shouldExplode should be false with no line")
        && check(!b.shouldExplode(5, 5), "shouldExplode on empty cell should be false");
}

static bool testExplodeAndDrop()
{
    cout << "TestExplodeAndDrop\n";
    Board b(10, 10);
    Candy* c1 = new Candy(CandyType::TYPE_RED);
    Candy* c2 = new Candy(CandyType::TYPE_RED);
    Candy* c3 = new Candy(CandyType::TYPE_RED);
    Candy* diff = new Candy(CandyType::TYPE_BLUE);
    b.setCell(c1, 0, 5);
    b.setCell(c2, 1, 5);
    b.setCell(c3, 2, 5);
    b.setCell(diff, 0, 6);

    std::vector<Candy*> exploded = b.explodeAndDrop();
   
    return check(exploded.size() >= 3, "explodeAndDrop should return at least 3 exploded candies")
        && check(b.getCell(1, 5) == nullptr, "exploded cell should be empty")
        && check(b.getCell(0, 9) == diff || b.getCell(0, 6) == diff,
            "non-exploded candy should remain somewhere in its column");
}

static bool testExplodeChain()
{
    cout << "TestExplodeChain\n";
    // Place two lines that chain: first line explodes, drop creates second line.
    Board b(10, 10);
    // Three greens in a row at y=9 (bottom).
    Candy g1(CandyType::TYPE_GREEN), g2(CandyType::TYPE_GREEN), g3(CandyType::TYPE_GREEN);
    b.setCell(&g1, 0, 9);
    b.setCell(&g2, 1, 9);
    b.setCell(&g3, 2, 9);
    // Three reds sitting on top of them.
    Candy r1(CandyType::TYPE_RED), r2(CandyType::TYPE_RED), r3(CandyType::TYPE_RED);
    b.setCell(&r1, 0, 8);
    b.setCell(&r2, 1, 8);
    b.setCell(&r3, 2, 8);

    std::vector<Candy*> exploded = b.explodeAndDrop();
    // Both lines should explode (chain).
    return check(exploded.size() == 6, "chain explosion should remove 6 candies");
}

static bool testDumpLoad()
{
    cout << "TestDumpLoad\n";
    std::string path = getDataDirPath() + "test_board_dump.txt";

    Board b1(10, 10);
    Candy* c = new Candy(CandyType::TYPE_PURPLE);
    b1.setCell(c, 3, 4);
    
    if (!check(b1.dump(path), "dump should succeed"))
        return false;
    Board b2;
    if (!check(b2.load(path), "load should succeed"))
    {
        std::filesystem::remove(path);
        return false;
    }
    
    bool ok = check(b2.getCell(3, 4) != nullptr, "loaded cell should not be null")
        && check(b2.getCell(3, 4)->getType() == CandyType::TYPE_PURPLE,
            "loaded candy type should match")
        && check(b2.getCell(0, 0) == nullptr, "empty cell should remain empty after load");

    std::filesystem::remove(path);
    return ok;
}

static bool testBoardEquality()
{
    cout << "TestBoardEquality\n";
    Board b1(5, 5), b2(5, 5);
    Candy* c1 = new Candy(CandyType::TYPE_YELLOW);
    Candy* c2 = new Candy(CandyType::TYPE_YELLOW);
    b1.setCell(c1, 2, 2);
    b2.setCell(c2, 2, 2);
    return check(b1 == b2, "identical boards should be equal");
}

static bool testBoardInequalitySize()
{
    cout << "TestBoardInequalitySize\n";
    Board b1(5, 5), b2(6, 5);
    return check(!(b1 == b2), "boards of different sizes should not be equal");
}

static bool testBoardCopyConstructor()
{
    Board b1(5, 5);
    Candy c(CandyType::TYPE_RED);
    b1.setCell(&c, 1, 1);

    Board b2(b1);
    return check(b2.getCell(1, 1) != nullptr, "copied board should have candy at (1,1)")
        && check(b2.getCell(1, 1)->getType() == CandyType::TYPE_RED,
            "copied board candy type should match");
}

// ---------------------------------------------------------------------------
// Bloc tests
// ---------------------------------------------------------------------------

static bool testBlocSpawn()
{
    cout << "TestBlocSpawn\n";
    Bloc block;
    return check(block.getColumna() == COLUMNA_INICI, "block spawns at correct column")
        && check(block.getFila() == -1, "block starts just above board");
}

static bool testBlocCicle()
{
    cout << "TestBlocCicle\n";
    CandyType types[MIDA_BLOC] = { CandyType::TYPE_RED, CandyType::TYPE_BLUE, CandyType::TYPE_GREEN };
    Bloc block(3, 5, types);
    block.cicle();  // RED BLUE GREEN -> BLUE GREEN RED
    return check(block.getCandyType(0) == CandyType::TYPE_BLUE, "cicle: new top is BLUE")
        && check(block.getCandyType(1) == CandyType::TYPE_GREEN, "cicle: middle is GREEN")
        && check(block.getCandyType(2) == CandyType::TYPE_RED, "cicle: bottom is RED");
}

static bool testBlocMove()
{
    cout << "TestBlocMove\n";
    Bloc block;
    int startCol = block.getColumna();
    block.esquerra();
    bool leftOk = check(block.getColumna() == startCol - 1, "esquerra decrements column");
    block.dreta();
    block.dreta();
    bool rightOk = check(block.getColumna() == startCol + 1, "dreta increments column");
    block.baixar();
    bool downOk = check(block.getFila() == 0, "baixar increments row");
    return leftOk && rightOk && downOk;
}

// ---------------------------------------------------------------------------
// Game tests
// ---------------------------------------------------------------------------

static bool testGameDumpLoad()
{
    cout << "TestGameDumpLoad\n";
    cout.flush();
    std::string path = getDataDirPath() + "test_game_dump.txt";
    cout << path << endl;
    cout.flush();
    
    Game g1;
    if (!check(g1.dump(path), "Game::dump should succeed"))
        return false;
    cout << "g1" << endl;
    Game g2;
    if (!check(g2.load(path), "Game::load should succeed"))
    {
        std::filesystem::remove(path);
        return false;
    }
    cout << "g2" << endl;
    bool ok = check(g1 == g2, "loaded game should equal original");
    std::filesystem::remove(path);
    return ok;
}

static bool testGameEquality()
{
    cout << "TestGameEquality\n";
    Game g1, g2;
    // Two freshly-constructed games are NOT necessarily equal because
    // Bloc uses random types. But they should be equal after
    // a dump/load cycle (tested above). Here we just verify operator==
    // compiles and returns a bool.
    bool result = (g1 == g2);
    (void)result;
    return check(true, "operator== compiles and runs without crash");
}

// ---------------------------------------------------------------------------
// Entry point
// ---------------------------------------------------------------------------

bool test()
{
    bool ok = true;

    // Board tests
    ok &= testBoardDimensions();
    ok &= testSetGetCell();
    //ok &= testGetCellOutOfBounds();
    //ok &= testSetCellOutOfBounds();
    ok &= testShouldExplodeHorizontal();
    ok &= testShouldExplodeVertical();
    ok &= testShouldExplodeDiagonal();
    ok &= testShouldNotExplode();
    //ok &= testExplodeAndDrop();
    ok &= testExplodeChain();
    ok &= testDumpLoad();
    ok &= testBoardEquality();
    ok &= testBoardInequalitySize();
    //ok &= testBoardCopyConstructor();

    // Bloc tests
    ok &= testBlocSpawn();
    ok &= testBlocCicle();
    ok &= testBlocMove();

    // Game tests
    ok &= testGameDumpLoad();
    ok &= testGameEquality();

    if (ok)
        std::cout << "TESTS PASSED" << std::endl;

    return ok;
}