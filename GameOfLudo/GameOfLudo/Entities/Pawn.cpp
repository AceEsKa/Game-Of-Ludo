#pragma once
#include "Libraries.h"
#include "Enums.cpp"
#include "Tile.cpp"

class Pawn
{
private:
    std::string piece;
    eEntityType epiece;
    int positionX;
    int positionY;
    int itile;
    bool onBoard;
    bool bHome;
    bool bcanmove;
    tile* SpawnTile;
    tile* current;
    tile* firsttile;
public:
    Pawn()
    {
        piece = " U";
        epiece = none;
        positionX = -1;
        positionY = -1;
        itile = -1;
        onBoard = false;
        bHome = false;
        bcanmove = false;
        SpawnTile = nullptr;
        current = nullptr;
        firsttile = nullptr;
    }

    Pawn(std::string player, eEntityType p, tile* ftile)
    {

        piece = player;
        positionX = -1;
        positionY = -1;
        onBoard = false;
        bHome = false;
        bcanmove = false;
        itile = 1;
        epiece = p;
        SpawnTile = nullptr;
        current = nullptr;
        firsttile = ftile;
    }

    void setSpawnTile(tile& homet) 
    {
        SpawnTile = &homet;
    }

    bool &GetLocation()//potentially enum ?
    {
        return onBoard;
    }

    bool &AtHome()
    {
        return bHome;
    }

    void placeonboard(bool dummy = false)
    {
        positionX = SpawnTile->X;
        positionY = SpawnTile->Y;
        current = SpawnTile;
        onBoard = true;
        bHome = false;
        bcanmove = true;
        if (!dummy)
        {
            current->PlacePlayerOnTile(epiece);
        }
    }

    void Move(int dice, eEntityType &p)
    {
        int tmp = itile + dice;
        if (tmp < 41 && onBoard)
        {
            current->RemovePawn(p);
            current->tilerender();
            for (int i = 0; i < dice; ++i)
            {
                if (current->next != nullptr)
                {
                    current = current->next;
                }
                else
                {
                    current = firsttile;
                }
            }
            current->stepcheck(p, 1);
            positionX = current->X;
            positionY = current->Y;
            current->tilerender();
            itile = tmp;
        }
    }

    void GoHome(eEntityType &p, int &x, int &y, int dice)
    {
        current->RemovePawn(p);
        current->tilerender();
        current = nullptr;
        itile += dice;
        onBoard = false;
        bHome = true;
        bcanmove = true;//pointles ?
        positionX = x;
        positionY = y;
    }

    //For test
    /*void GoHome(eEntityType p, int x, int y)
    {
        onBoard = false;
        bHome = true;
        bcanmove = true;
        positionX = x;
        positionY = y;
    }*/

    bool inGame()//rename
    {
        return (onBoard || bHome) ? true : false;
    }

    bool CheckOutOfRange(int roll)
    {

        return ((itile + roll) > 44) ? true : false;
    }

    bool CheckCaptured()
    {
        return current->GetCapturedPlayer() == epiece ? true : false;
    }

    void RemovePawn() {
        current->tilerender();
        current = nullptr;
        positionX = -1;
        positionY = -1;
        itile = 1;
        onBoard = false;
        bHome = false;
        bcanmove = false;
    }

    void setCanMove(bool b)
    {
        bcanmove = b;
    }

    void setPosition(int x, int y)
    {
        positionX = x;
        positionY = y;
    }

    void setTileNumber(int increment)
    {
        itile += increment;
    }

    tile* Get_current_tile()
    {
        return current;
    }

    tile* GetSpawnTile()
    {

        return SpawnTile;
    }

    int GetTileNumber()
    {
        return itile;
    }

    ~Pawn()
    {
        SpawnTile = nullptr;
        current = nullptr;
    }

}; //class Pawn