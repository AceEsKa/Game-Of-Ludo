#pragma once
#include "Libraries.h"

/*enum pawntomove
{
    first, second, third, last
};*/

enum eEntityType
{
    none = -1,
    PlayerA, PlayerB, PlayerC, PlayerF, ePawn
};

enum eHomeTileStatus
{
    DefaultStatus = -1, taken = 0, freetile = 1
};


//AI
enum MovePriority
{
    Enoaction, Emovefarthestpawn, Esafetile, Eplacepawnonboard, Ehometile, EPawnCapture
};

enum TileStatus
{
    OutOfRange, InRange, Occupied, HomeTile,
};

enum LeagalMoveStatus
{
    PawnsMoveNotPossible, PawnsMovePossible
};

