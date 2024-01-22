#pragma once
#include "Libraries.h"
#include "Enums.cpp"
#include "Pawn.cpp"
#include "Tile.cpp"
#include "Entity.cpp"

class Player : Entity
{
protected:
    Pawn player_pawns[4];

    int no_PawnsInGame;
    int no_PawnsHome;

    eHomeTileStatus home[4];
    int HomeCoordX[4] = { 0,0,0,0 };
    int HomeCoordY[4] = { 0,0,0,0 };
    tile* hometile;
    eEntityType EntityType;
public:
    Player()
    {
        no_PawnsInGame = -1;
        no_PawnsHome = -1;
        EntityType = none;
        hometile = nullptr;
        std::fill_n(home, 4, freetile);

        Name = "";
        InGame = false;
    }

    Player(std::string name, eEntityType ep, bool IsPlaying, tile* firsttile)
    {
        no_PawnsInGame = 1;
        Name = name;
        no_PawnsHome = 0;
        EntityType = ep;
        InGame = IsPlaying;
        std::fill_n(home,4 , freetile);
        std::fill_n(player_pawns,4, Pawn(name, ep, firsttile));
    }

    bool *GetInGameStatus() override
    {
        return &InGame;
    }

    std::string GetName() override
    {
        return Name;
    }

    void SetHomeTile(tile* spawn)
    {
        hometile = spawn;
    }

    void Spawn(tile* spawn)
    {

        hometile = spawn;
        for (int i = 0; i < 4; ++i)
        {
            player_pawns[i].setSpawnTile(*hometile);
        }
        player_pawns[0].placeonboard();
    }

    void SetHomeCoordinates(int x, int y, int CoordPosition)
    {

        HomeCoordX[CoordPosition] = x;
        HomeCoordY[CoordPosition] = y;
    }

    /*void test()
    {
        no_PawnsHome = 3;
        for (int i = 1; i < 4; ++i)
        {
            home[i] = taken;
            player_pawns[i].GoHome(EntityType, HomeCoordX[i], HomeCoordY[i]);
        }
    }*/

    void CheckPawnCaptured()
    {
        if (no_PawnsInGame != 0)
        {
            for (int i = 0; i < 4; ++i)
            {
                bool onBoard = player_pawns[i].GetLocation();
                if (onBoard && player_pawns[i].CheckCaptured())
                {
                    player_pawns[i].RemovePawn();
                    no_PawnsInGame -= 1;
                }
            }
        }
    }

    virtual void Move(int AmountToMove, int no_pawn = -1) override
    {
        TileStatus moveoperation = CheckHome(AmountToMove, no_pawn-1);
        int HomePosition;
        bool onBoard = player_pawns[no_pawn - 1].GetLocation();
        //place pove new pawn on board
        if (!(onBoard) && AmountToMove >= 6)
        {
            player_pawns[no_pawn - 1].placeonboard();
            no_PawnsInGame += 1;
        }
        //just move
        else if (moveoperation != HomeTile && moveoperation != OutOfRange)
        {
            player_pawns[no_pawn - 1].Move(AmountToMove, EntityType);
        }
        //move to new home position
        else if (ValidateHomeMovemant(no_pawn - 1,AmountToMove))
        {
            moveAtHome(no_pawn-1, AmountToMove);
        }
        //move home
        else
        {
            HomePosition = player_pawns[no_pawn - 1].GetTileNumber() + AmountToMove - 41;
            player_pawns[no_pawn - 1].GoHome(EntityType, HomeCoordX[HomePosition], HomeCoordY[HomePosition], AmountToMove);
            no_PawnsHome += 1;
            home[HomePosition] = taken;
        }
    }

    void moveAtHome(int pawn, int dice)
    {
        if (ValidateHomeMovemant(pawn,dice))
        {
            int t = player_pawns[pawn].GetTileNumber() + dice - 41;
            home[t] = taken;
            if (t == 3)
            {
                player_pawns[pawn].setCanMove(false);
            }
            player_pawns[pawn].setPosition(HomeCoordX[t],HomeCoordY[t]);
            player_pawns[pawn].setTileNumber(t);
            t = player_pawns[pawn].GetTileNumber() - 41;
            home[t] = freetile;
        }
    }

    bool ValidateHomeMovemant(int pawn, int dice)
    {
        return dice <= 3 && player_pawns[pawn].GetTileNumber() + dice <= 44 && home[dice] == freetile ? true : false;
    }

    TileStatus CheckHome(int dice, int pawntomove) //pawntomove 
    {
        int TileID = player_pawns[pawntomove - 1].GetTileNumber();
        TileID += dice;
        if (TileID > 44)
        {
            return OutOfRange;
        }
        else
        {
            int homepos = TileID - 41;                              //tile IDs are starting from 1 but array indexing is form 0 therefore we cant have -40 we need to do additonal -1 therefore -41
            if (homepos >= 0)
            {
                if (home[homepos] == freetile)
                {
                    return HomeTile;
                }
                else
                {
                    return Occupied;
                }
            }
            else
            {
                return InRange;
            }
        }
    }

    int GetNoPawnsOnBoard()
    {
        return no_PawnsInGame;
    }

    Pawn &GetReference(int nopawn)
    {
        return player_pawns[nopawn ];
    }

    Pawn GetCopy(int nopawn)
    {
        return player_pawns[nopawn ];
    }

    //look for remake
    bool CanMove(int ipawn)
    {
        return player_pawns[ipawn].inGame();
    }

    bool &GetPawnHome(int ipawn)
    {
        return player_pawns[ipawn].AtHome();
    }

    eHomeTileStatus &GetHomePositionStatus(int position)
    {
        return home[position];
    }

    eEntityType GetEntityEType() 
    {
        return EntityType;
    }

    int GetNoPawnsHome()
    {
        return no_PawnsHome;
    }

    int &GetXhomeCoord(int i)
    {
        return HomeCoordX[i];
    }

    int &GetYhomeCoord(int i)
    {
        return HomeCoordY[i];
    }

    //Prevents invalid input, input other than int
    std::optional<unsigned int> MovemantHandler(std::string DisplayMessage = "Which pawn do u wish to move with ? ") override
    {
        unsigned int input;
        bool InvalidInput = false;
        do
        {
            InvalidInput = false;
            std::cout << DisplayMessage;
            std::cin >> input;
            try
            {
                if (std::cin.fail())
                {
                    throw "error";
                    std::cin.clear();
                    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
                }
            }
            catch (const char* error)
            {
                std::cerr << error << std::endl;
                InvalidInput = true;
                std::cin.clear();
                std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            }
            std::cout << std::endl;
        } while (InvalidInput);

        return input;
    }

    ~Player()
    {
        hometile = nullptr;
    }
}; //ClassPlayer

