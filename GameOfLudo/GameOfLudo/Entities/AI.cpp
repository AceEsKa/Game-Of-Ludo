#pragma once
#include "Libraries.h"
#include "Enums.cpp"
#include "Pawn.cpp"
#include "Player.cpp"
#include "Entity.cpp"

class AI : public Player
{
protected:
    tile* firsttile;
    MovePriority Dicision[4][3] = { {Enoaction,Enoaction,Enoaction }, { Enoaction,Enoaction,Enoaction }, {Enoaction,Enoaction,Enoaction } };
    MovePriority BestMoveDecisions[3];
    int BestPawns[2];
    LeagalMoveStatus Status;
public:
    AI() : Player()
    {
        firsttile = nullptr;
        std::fill_n(BestMoveDecisions,3,Enoaction);
        std::fill_n(BestPawns,2,-1);
        Status = PawnsMoveNotPossible;
    }

    AI(std::string p, eEntityType ep, bool IsPlaying, tile* ftile) : Player(p, ep, IsPlaying, ftile)
    {
        firsttile = ftile;
        std::fill_n(BestMoveDecisions, 3, Enoaction);
        std::fill_n(BestPawns, 2, -1);
        Status = PawnsMoveNotPossible;
    }

    virtual void Move(int AmountToMove, int no_pawn = -1)
    {
        int dice = AmountToMove;

        DetermineBestMove(dice);

        if (Status == PawnsMoveNotPossible)
        {
            return;
        }

        int rolls = (dice > 6 ? 2 : 1);
        bool OnBoard;
        bool AtHome;
        int HomePosition;
        int j;

        for (int i = 0; i < rolls; ++i)
        {
            OnBoard = player_pawns[BestPawns[i]].GetLocation();
            AtHome = player_pawns[BestPawns[i]].AtHome();
            dice = dice > 6 && i == 0 ?  6 : dice;
            HomePosition = player_pawns[BestPawns[i]].GetTileNumber() + dice - 41;
            j = AmountToMove > 6 ? i + 1 : i;

            if(BestPawns[i] == -1)
            {
                break;
            }
            if (!OnBoard)
            {
                player_pawns[BestPawns[i]].placeonboard();
                no_PawnsInGame += 1;
            }
            else if(HomePosition >= 0 && HomePosition < 4 && home[HomePosition] != taken)
            {
                player_pawns[BestPawns[i]].GoHome(EntityType, HomeCoordX[HomePosition], HomeCoordY[HomePosition], dice);
                no_PawnsHome += 1;
                home[HomePosition] = taken;
            }
            else if (Dicision[BestPawns[i]][j] != Ehometile && Dicision[BestPawns[i]][j] != Enoaction )
            {
                player_pawns[BestPawns[i]].Move(dice, EntityType);
            }
            dice = AmountToMove - 6;
        }

    }

    tile *tomoveon(Pawn &p, int dice)
    {
        tile *tmp;
        int NumberOfTilesToMove = dice;
        if (!p.inGame() && NumberOfTilesToMove >= 6)
        {
            tmp = p.GetSpawnTile();
            NumberOfTilesToMove -= 6;
        }
        else
        {
            tmp = p.Get_current_tile();
        }

        for (int k = 0; k < NumberOfTilesToMove; ++k)
        {
            if (tmp->next == nullptr)
            {
                tmp = firsttile;
            }
            else
            {
                tmp = tmp->next;
            }
        }

        return tmp;
    }//tomoveon

    MovePriority EvaluateAction(Pawn &PawnToMove, int roll, int Pawn)
    {   
        bool atHome = PawnToMove.AtHome();
        bool onBoard = PawnToMove.GetLocation();
        if (atHome || !(onBoard) && roll < 6)
        {
            return Enoaction;
        }
        if (PawnToMove.CheckOutOfRange(roll))
        {
            return Enoaction;
        }
        tile *t = tomoveon(PawnToMove, roll);
        bool safezone = t->safezone;

        if (t->number_of_pawns == 1 && !safezone && t->PawnOnTile(EntityType) != EntityType)
        {
            return EPawnCapture;
        }
        else if (CheckHome(roll, Pawn + 1) == HomeTile)
        {
            return Ehometile;
        }
        else if (roll >= 6 && !(atHome) && !(onBoard))
        {
            return Eplacepawnonboard;
        }
        else if (safezone)
        {
            return Esafetile;
        }
        else
        {
            return Emovefarthestpawn;
        }
    }//actiontotake


    //OK
    void ResetDicisions()
    {
        //try to optimize
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                Dicision[i][j] = Enoaction;
            }
        }
    }

    void MovesOptions(int dice)
    {
        ResetDicisions();

        MovePriority Move;
        Pawn TmpPawn;//Maybe pointer
        bool atHome;
        bool onBoard;
        bool dummy = false;

        for (int i = 0; i < 4; ++i)
        {
            atHome = player_pawns[i].AtHome();
            onBoard = player_pawns[i].GetLocation();
            if ((!(atHome) && !(onBoard) && dice >= 6) || (!(atHome) && (onBoard)))
            {
                TmpPawn = GetReference(i);
                Move = EvaluateAction(TmpPawn, dice, i);
                Dicision[i][0] = Move;

                if (dice > 6)
                {
                    Move = EvaluateAction(TmpPawn, 6, i);
                    Dicision[i][1] = Move;
                    if (Move == Eplacepawnonboard)
                    {
                        TmpPawn.placeonboard(true);
                        dummy = true;
                    }
                    Move = EvaluateAction(TmpPawn, dice - 6, i);
                    Dicision[i][2] = Move;
                    if (dummy)
                    {
                        TmpPawn.RemovePawn();
                        dummy = false;
                    }
                }
            }
        }
    }

    int bestPawn(int j,int k, int pawnToSkip = -1) 
    {
        MovePriority BestMove, CurrentMove;
        BestMove = CurrentMove = Enoaction;
        int pawn = -1;
        for (int i = 0; i < 4; ++i)
        {
            if (pawnToSkip != i)
            {
                CurrentMove = Dicision[i][j];
                if (CurrentMove > BestMove)
                {
                    BestMove = CurrentMove;
                    pawn = i;
                }
            }
        }
        setDecisions(k, BestMove);
        return pawn;
    }

    void setDecisions(int k, MovePriority BestMove)
    {
        BestMoveDecisions[k] = BestMove;
    }

    void DetermineBestMove(int dice)
    {
        int pawn;
        MovesOptions(dice);
        if (dice < 6 ? true : false) 
        {
            pawn = bestPawn(0, 0);
            BestPawns[0] = pawn;
        }
        else
        {
            pawn = bestPawn(1,0);
            BestPawns[0] = pawn;
            setStatus(pawn);
            pawn = bestPawn(2,1);
            BestPawns[1] = pawn;
        }

        setStatus(pawn);
        int bestpawn;
        bool greaterThenSix = dice > 6 ? true : false;
        if (greaterThenSix && BestPawns[0] == BestPawns[1] && Status != PawnsMoveNotPossible)
        {
            if(BestMoveDecisions[0] == Ehometile || BestMoveDecisions[0] == Eplacepawnonboard || Dicision[BestPawns[0]][0] == Enoaction)
            {
                bestpawn = bestPawn(0,2);
                if (BestMoveDecisions[2] == EPawnCapture)
                {
                    BestPawns[0] = BestPawns[1] = bestpawn;
                }
                else
                {
                    bestpawn = bestPawn(2,2,BestPawns[0]);
                    if (bestpawn != -1)
                    {
                        BestPawns[1] = bestpawn;
                    }
                }
            }
            else if (BestMoveDecisions[1] == EPawnCapture)
            {
                BestPawns[0] = bestPawn(1,0 ,BestPawns[0]);
            }
        }
        else if (greaterThenSix && BestPawns[0] != BestPawns[1] && Status != PawnsMoveNotPossible)
        {
            bool onBoard = player_pawns[BestPawns[1]].GetLocation();
            bool atHome;
            bestpawn = bestPawn(0, 2);
            if (BestMoveDecisions[2] == EPawnCapture )
            {
                BestPawns[0] = BestPawns[1] = bestpawn;
            }
            else if (BestPawns[1] == EPawnCapture && !(onBoard))
            {
                BestPawns[0] = BestPawns[1] = bestpawn;
            }
            else if (dice == 12 && BestMoveDecisions[2] != EPawnCapture)
            {
                for (int i= 0; i < 4; ++i)
                {
                    if (i != BestPawns[0])
                    {
                        onBoard = player_pawns[i].GetLocation();
                        atHome = player_pawns[i].AtHome();
                        if (greaterThenSix && !(atHome) && !(onBoard))
                        {
                            BestPawns[1] = i;
                            break;
                        }
                    }
                }
            }
        }

    }

    void setStatus(int &pawn) 
    {
        if (pawn == -1 ? true : false)
        {
            Status = PawnsMoveNotPossible;
        }
        else
        {
            Status = PawnsMovePossible;
        }
    }

    AI(const Player& p) : Player(p) {}
    AI(Player&& p) : Player(std::move(p)) {}

    ~AI() {}
};