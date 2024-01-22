#pragma once
#include "Libraries.h"
#include "Pawn.cpp"
#include "Player.cpp"
#include "Rules.cpp"
class LudoGameMechanics
{
private:
    Printer printer;  
protected:
    LeagalMoveStatus Status;
public:

    LudoGameMechanics()
    {
        printer = Printer();
        Status = PawnsMoveNotPossible;
    }

    virtual bool ValidateMove(Player *PlayerToValidate, int DiceRoll, int pawn, bool ValidateExactMove = false)
    {
        int TileID = PlayerToValidate->GetReference(pawn).GetTileNumber();
        int NewTile = TileID + DiceRoll;
        bool bHome = PlayerToValidate->GetReference(pawn ).AtHome();
        bool moveAtHome = PlayerToValidate->ValidateHomeMovemant(pawn,DiceRoll);

        if (bHome)
        {
            if (moveAtHome)
            {
                return true;
            }
            if (ValidateExactMove)
            {
                printer.print("Home position already occupied or is out of range, choose different!");
            }
            return false;
        }
        else if (NewTile - 41 >= 0)
        {
            eHomeTileStatus H_status = PlayerToValidate->GetHomePositionStatus(NewTile - 41);
            if (H_status == taken)
            {
                if (ValidateExactMove) 
                {
                    printer.print("Pawns home position occupied, choose different!");
                }
                return false;
            }
        }

        bool PawnInGame = PlayerToValidate->GetReference(pawn).GetLocation();

        return ((PawnInGame && NewTile <= 44) || (!(PawnInGame) && DiceRoll >= 6)) ? true : NotOnBoardMessage();
    }//ValidateMove

    bool NotOnBoardMessage()
    {
        printer.print("Pawn is not on board and dice roll wasnt 6, choose different!");
        return false;
    }

    void CheckAllPawns(Player* PlayerOnTheMove, int DiceRoll)
    {
        for (int pawn = 0; pawn < 4; ++pawn)
        {
            bool PossibleMove = ValidateMove(PlayerOnTheMove, DiceRoll, pawn);
            if (PossibleMove)
            {
                SetStatus(PawnsMovePossible);;
                return;
            }
        }
        SetStatus(PawnsMoveNotPossible);
    }

    void SetStatus(LeagalMoveStatus status)
    {
        Status = status;
    }

    LeagalMoveStatus GetAvailableMovesStatus(Player* PlayerOnTheMove, int DiceRoll)
    {
        CheckAllPawns(PlayerOnTheMove, DiceRoll);

        return Status;
    }
};
