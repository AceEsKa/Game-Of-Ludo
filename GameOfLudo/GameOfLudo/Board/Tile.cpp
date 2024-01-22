#pragma once
#include "Libraries.h"
#include "Enums.cpp"

struct tile
{
    std::string render;
    int PlayersPawnCount[4];// A B C F
    int number_of_pawns;
    int allplayers;
    int X;
    int Y;
    bool bEmpty;
    bool safezone;
    eEntityType out;
    eEntityType ontile[4];
    int ID;

    tile* next;

    tile() 
    {
        render= " *";
        std::fill_n(PlayersPawnCount,4,0);// A B C F
        number_of_pawns = 0;
        allplayers = 0;
        X = -1;
        Y = -1;
        bEmpty = true;
        safezone = false;
        out = none;
        std::fill_n(ontile,4,none);
        ID = 0;

        next = nullptr;
    }

    tile(int allpl, int id, int x, int y)
    {
        X = x;
        Y = y;
        safezone = false;
        DeterminateSafezone();
        allplayers = allpl;
        out = none;
        for (int i = 0; i < 4; i++)
        {
            ontile[i] = none;
        }
        std::fill_n(PlayersPawnCount, 4, 0);// A B C F
        render = " *";
        number_of_pawns = 0;
        bEmpty = true;
        ID = id;
    }

    void PlacePlayerOnTile(eEntityType p)
    {
        switch (p)
        {
        case PlayerA:
            render = " A";
            break;
        case PlayerB:
            render = " B";
            break;
        case PlayerC:
            render = " C";
            break;
        case PlayerF:
            render = " F";
            break;
        default:
            break;
        }
        PlayersPawnCount[p] += 1;
        bEmpty = false;
        safezone = true;
        ontile[p] = p;
        number_of_pawns += 1;
    }


    void DeterminateSafezone()
    {
        if ((Y == 0 && X == 2))
        {
            safezone = true;
        }
        if ((Y == 10 && X == 0))
        {
            safezone = true;
        }
        if (allplayers > 2)
        {
            if ((Y == 4 && X == 0) && allplayers == 3)
            {
                safezone = true;
            }
            else if (((Y == 4 && X == 0) || (Y == 6 && X == 10)))
            {
                safezone = true;
            }
        }
    }

    void stepcheck(eEntityType p, int  noplayer = 0)
    {
        //chceck which players steped on the tile and how many players there are rn on the tile and decide which form the tile should take

        if (!safezone && PlayersPawnCount[p] == 0 && ontile[p] == none && number_of_pawns == 1)
        {
            FirePawn(p);
        }
        ontile[p] = p;
        PlayersPawnCount[p] += 1;
        number_of_pawns += 1;
        bEmpty = false;
    }

    void FirePawn(eEntityType SteppedOnTile)
    {
        for (int i = 0; i < 4; ++i)
        {
            if (PlayersPawnCount[i] == 1)
            {
                PlayersPawnCount[i] -= 1;
                if (PlayersPawnCount[i] == 0)
                {
                    setFired(i);
                    ontile[i] = none;
                }
                number_of_pawns -= 1;
                break;
            }
        }
    }

    void setFired(int i)
    {
        out = ontile[i];
    }

    eEntityType GetCapturedPlayer()
    {
        return out;
    }

    void RemovePawn(eEntityType toremove)
    {
        PlayersPawnCount[toremove] -= 1;
        if (PlayersPawnCount[toremove] == 0)
        {
            ontile[toremove] = none;
        }
        number_of_pawns -= 1;
        if (number_of_pawns == 0)
        {
            bEmpty = true;
        }
    }
    std::string tilerender()
    {
        eEntityType gr = greatest();
        switch (gr)
        {
        case PlayerA:
            render = " A";
            break;
        case PlayerB:
            render = " B";
            break;
        case PlayerC:
            render = " C";
            break;
        case PlayerF:
            render = " F";
            break;
        default:
            render = " *";
            break;
        }
        return render;
    }

    eEntityType greatest()
    {
        int index = -1;
        int greatest = 0;

        for (int i = 0; i < 4; ++i)
        {
            if (PlayersPawnCount[i] > greatest)
            {
                greatest = PlayersPawnCount[i];
                index = i;
            }
        }

        switch (index)
        {
        case 0:
            return PlayerA;
        case 1:
            return PlayerB;
        case 2:
            return PlayerC;
        case 3:
            return PlayerF;
        default:
            return none;
        }
    }

    eEntityType PawnOnTile(eEntityType pawn)
    {
        return ontile[pawn];
    }
};

