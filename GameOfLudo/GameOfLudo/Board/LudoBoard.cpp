#pragma once
#include "Libraries.h"
#include "Player.cpp"
#include "AI.cpp"
#include "Tile.cpp"
#include "Enums.cpp"
#include "Pawn.cpp"
#include "Printer.cpp"

class Board
{
private:
    tile firsttile;
    std::vector<std::vector<std::string>> gameboard;
    Printer printer = Printer();
public:
    Board()
    {
        MakeBoard();
    };


    Board(int ParticipatingPlayers)
    {
        if (gameboard.empty())
        {
            MakeBoard();
        }
        for (int i = 0; i < 40; ++i)
        {
            if (i != 0)
            {
                AllocTile(ParticipatingPlayers, i);
            }
            else
            {
                AllocTile(ParticipatingPlayers, i, true);
            }
        }
    }

    void MakeBoard()
    {
        bool a1 = false;
        std::vector<std::string> array;
        array.reserve(3);
        std::vector<std::string> array2;  //pomocna array
        array2.reserve(11);
        gameboard.reserve(11);

        for (int i = 0; i < 11; ++i)
        {
            if (i == 0 || i == 10) // 0 a 8
            {
                a1 = true;
                for (int o = 0; o < 3; ++o)
                {
                    array.emplace_back(" *");
                }
            }
            else if (i == 5)  // 5
            {
                array2.emplace_back(" *");
                for (int k = 0; k < 2; ++k)
                {
                    for (int l = 0; l < 4; ++l)
                    {
                        array2.emplace_back(" D");
                    }
                }
                array2.emplace_back(" *");
                array2.insert(array2.begin() + 5, " X");
            }
            else if (i == 4 || (i == 6)) // 4 and 6 
            {
                for (int m = 0; m < 2; ++m)
                {
                    for (int j = 0; j < 5; ++j)
                    {
                        array2.emplace_back(" *");
                    }
                }
                array2.insert(array2.begin() + 5, " D");
            }
            else
            {
                a1 = true;
                for (int e = 0; e < 2; ++e)
                {
                    array.emplace_back(" *");
                }
                array.insert(array.begin() + 1, " D");
            }
            if (a1)
            {
                a1 = false;
                gameboard.emplace_back(array);
                array.clear();
                array.reserve(3);
            }
            else
            {
                gameboard.emplace_back(array2);
                array2.clear();
                array2.reserve(11);
            }
        }
    }

    void tilelistaddto(tile* toadd, bool tFirst = false)
    {
        tile tmp;

        if (tFirst)
        {
            toadd->next = nullptr;

            firsttile = *toadd;
            firsttile.next = nullptr;
        }
        else
        {
            toadd->next = firsttile.next;
            firsttile.next = toadd;
        }
    }


    void AllocTile(int NoParticipatingPlayers, int i, bool tFirst = false)
    {
        tile* newtile;
        int index = i - 1;

        if (i != 0)
        {
            //couldnt be bothered to make algorythm, maybe in a future update
            int x[] = { 0, 0, 0, 4, 3, 2, 1, 0, 0, 0, 1, 2, 3, 4, 0, 0, 0,  0,  1,  2, 2, 2, 2, 6,  7, 8,  9, 10 ,10, 10, 9, 8, 7, 6, 2, 2, 2, 2, 1 };
            int y[] = { 1, 2, 3, 4, 4, 4, 4, 4, 5, 6, 6, 6, 6, 6, 7, 8, 9, 10, 10, 10, 9, 8, 7, 6,  6, 6,  6,  6,  5,  4, 4, 4, 4, 4, 3, 2, 1, 0, 0 };

            newtile = new tile(NoParticipatingPlayers, 40 - i, x[index], y[index]);
        }
        else
        {
            newtile = new tile(NoParticipatingPlayers, i, 0, 0);
        }

        tilelistaddto(newtile, tFirst);
    }
    //
    void EditBoard(AI &P)
    {
        int CoordX;
        int CoordY;
        tile tmp = firsttile;
        int numberoftiles = 40;
        int *x;
        int *y;

        if (P.GetNoPawnsHome() > 0)
        {
            for (int i = 0; i < 4; ++i)
            {
                if (P.GetHomePositionStatus(i) == taken)
                {
                    CoordX = P.GetXhomeCoord(i);
                    CoordY = P.GetYhomeCoord(i);
                    std::string name = P.GetName();
                    gameboard[CoordY][CoordX] = name;
                }
            }
        }

        for (int i = 0; i < numberoftiles; ++i)
        {
            x = &tmp.X;
            y = &tmp.Y;
            gameboard[*y][*x] = tmp.render;
            if (tmp.next != nullptr)
            {
                tmp = *tmp.next;
            }
            else
            {
                std::cout << std::endl;
            }
        }
    }


    void Tutorial() {
        gameboard[0][1] = " E";
        gameboard[0][2] = " S";
        gameboard[10][1] = " E";
        gameboard[10][0] = " S";
    }

    void print(AI player[], int iplayers = 0)
    {
        if (iplayers == 1 || iplayers == 0)
        {
            iplayers = 2;
        }

        for (int i = 0; i < iplayers; i++)
        {
            EditBoard(player[i]);
        }
        printer.print(gameboard);
    }

    std::vector<std::vector<std::string>> &getBoard()
    {
        return gameboard;
    }

    //to test
    tile* GetFirstTile()
    {
        return &firsttile;
    }

    ~Board()
    {
        tile* next = firsttile.next;
        tile* crnnt = &firsttile;
        for (int i = 0; i < 41; i++)
        {
            if (next != nullptr) 
            {
                next = crnnt->next;
            }
            crnnt = nullptr;
            crnnt = next;
        }
    }
};