#pragma once
#include "Libraries.h"
#include "Enums.cpp"
#include "Player.cpp"
#include "RNG.cpp"
#include "Pawn.cpp"
#include "AI.cpp"
#include "GameMechanics.cpp"
#include "LudoBoard.cpp"
#include "Rules.cpp"
#include "InputHandler.cpp"

class Game : public LudoGameMechanics, public Rules
{
private:
    int NumberOfParticipants;

    Board GameBoard;

    int Round;
    tile* ht;
    AI players[4];
    AI* PlayerOnTheMove;
    eEntityType Winner;
    tile* ftile;

    std::optional<unsigned int> UserChoice;
    Printer printer;

    int userinput;
    int noplayers;
    int nohumans;
public:

    Game()
    {
        NumberOfParticipants = 0;
        GameBoard = Board();
        Round = 1;
        ht = nullptr;
        PlayerOnTheMove = nullptr;
        Winner = none;
        ftile = nullptr;
        UserChoice = 0;
        printer = Printer();
        userinput = 0;
        noplayers = 0;
        nohumans = 0;
    }

    void DeafultDisplay()
    {
        bool ok = false;
        DisplayGameOptions();

        unsigned int UserInput;

        do
        {
            UserInput = GetUserInput("Choose option as was described abow: ");
            if (UserInput == 5)
            {
                DisplayRules();
            }
            else if (UserInput > 5)
            {
                printer.print("Invalid input detected! Input > 5!");
            }
            else
            {
                printer.print("are u sure u want to start ");
                switch (UserInput)
                {
                case 1:
                    printer.print("Players vs AI game ?");
                    break;
                case 2:
                    printer.print("2 Player game ?");
                    break;
                case 3:
                    printer.print("3 Player game ?");
                    break;
                case 4:
                    printer.print("4 Player game ?");
                    break;
                default:
                    printer.print("AI vs AI game ?");
                    break;
                }
            }
            SetHumnaPlayers(UserInput);
            SetPlayers(UserInput);
            UserInput = GetUserInput("Type 1 for YES.");
            if (UserInput == 1)
            {
                ok = true;
            }
        } while (!ok);
    }

    void SetPlayers(int players)
    {
        switch (players)
        {
        case 3:
            noplayers = 3;
            break;
        case 4:
            noplayers = 4;
            break;
        default:
            noplayers = 2;
            break;
        }
    }

    void SetHumnaPlayers(int humans)
    {
        nohumans = humans;
    }

    void SetUpGame()
    {
        std::string splayer[4] = { " A"," B"," C"," F" };
        eEntityType ePlayer[4] = { PlayerA,PlayerB,PlayerC,PlayerF };

        int PawnsCoordsX[4][4] = { /*A*/{1,1,1,5}, /*B*/{1,1,1,5} ,/*C*/ {9,8,7,6} , /*F*/{1,2,3,4}}; //ArrayIndex

        int PawnsCoordsY[4][4] = { /*A*/{1,2,3,4}, /*B*/{9,8,7,6}, /*C*/{5,5,5,5}, /*F*/{5,5,5,5}}; //Array

        NumberOfParticipants = noplayers;
        GameBoard = Board(NumberOfParticipants);
        ftile = GameBoard.GetFirstTile();

        for (int player = 0; player < NumberOfParticipants; ++player)
        {
            if (player == 0 && nohumans == 1 || nohumans >= 2)
            {
                Player tmp = Player(splayer[player], ePlayer[player], true, ftile);
                players[player] = tmp;
            }
            else
            {
                AI tmp = AI(splayer[player], ePlayer[player], true, ftile);
                players[player] = AI(splayer[player], ePlayer[player], true, ftile);
            }

            for (int pawn = 0; pawn < 4; pawn++)
            {
                players[player].SetHomeCoordinates(PawnsCoordsX[player][pawn], PawnsCoordsY[player][pawn], pawn);
            }
            
        }
        //ht and stuff
        ht = ftile;

        for (int i = 0; i < 23; ++i)
        {
            if (i == 2)
            {

                players[0].Spawn(ht);
            }
            else if (i == 12 && nohumans >= 3)
            {
                players[2].Spawn(ht);
            }
            else if (i == 22)
            {
                players[1].Spawn(ht);
            }
            ht = ht->next;
        }

        if (nohumans == 4)
        {
            for (int j = 23; j < 33; ++j)
            {
                if (j == 32)
                {
                    players[3].Spawn(ht);
                }
                ht = ht->next;
            }
        }

    }

    void StartGame()
    {
        printer.print("----------------------");
        GameBoard.print(players, noplayers);
        printer.print("----------------------");
        int playersFinished;
        while (true)
        {
            playersFinished = 0;
            for (int player = 0; player < NumberOfParticipants; player++)
            {
                printer.print("Round No. ", Round);
                PlayRound(player);
                printer.print("----------------------");
                if (players[player].GetNoPawnsHome() == 4)
                {
                    printer.print("Player " + players[player].Player::GetName() + " already won!");
                    ++playersFinished;
                    if (Winner == none)
                    {
                        Winner = players[player].GetEntityEType();
                    }
                }
                if (playersFinished == noplayers - 1 )
                {
                    break;
                }
            }
            if (playersFinished == noplayers - 1)
            {
                break;
            }
            ++Round;
        }
        printer.print("Game finished!");
        AnnounceWinner();
    }

    void AnnounceWinner()
    {
        printer.print("Winner: ", Winner);
    }

    void PlayRound(int &tomove)
    {
        PlayerOnTheMove = &players[tomove];
        bool NoAvailableMoveForThisRound = false;
        bool movedone = false;
        bool bothrolls = false;
        int rolls = 1;
        printer.print("Player: " + PlayerOnTheMove->GetName());
        printer.print("----------------------");
        int roll1 = RNG(1, 6);
        int roll2 = 0;
        printer.print("Rools: ",roll1,false);
        if (roll1 >= 6)
        {
            roll2 = RNG(1, 6);
            printer.print(", ", roll2, false);
            rolls = 2;
        }
        std::cout << std::endl;
        PlayerOnTheMove->CheckPawnCaptured();

        //Human
        if (nohumans == 1 && tomove == 0 || nohumans >= 2)
        {
            int tmproll = roll1;
            for (int i = 0; i < rolls; ++i)
            {
                do
                {
                    PromptUserInput(tomove, tmproll);
                    if (GetAvailableMovesStatus(PlayerOnTheMove, tmproll) != PawnsMovePossible)
                    {
                        printer.print("No Pawn can legally move...Next player to move!");
                        printer.print("----------------------");
                        return;
                    }
                } while (CheckValidPawn(userinput) != true || ValidateMove(PlayerOnTheMove, tmproll, (userinput-1),true) != true);
                PlayerOnTheMove->Player::Move(tmproll, userinput);
                GameBoard.print(players, noplayers);
                printer.print("----------------------");
                tmproll = roll2;
            }
        }
        else
        {
            PlayerOnTheMove->Move(roll1 + roll2);
            GameBoard.print(players, noplayers);
            printer.print("----------------------");
        }

    }// 

    void PromptUserInput(int &user, int &roll)
    {
        printer.print("Choose which pawn to move ", roll, " tiles with or which pawn to place in game.");
        UserChoice = players[user].MovemantHandler();
        userinput = UserChoice.value_or(-1);
        if (userinput == -1) 
        {
            printer.print("Unexpected Error...terminating program!");
            exit(EXIT_FAILURE);
        }
    }

    void DisplayGameOptions()
    {
        std::ifstream fOptions;
        fOptions.open("GameOptions.txt", std::ios::in);
        std::string fline;
        if (fOptions.is_open())
        {
            while (fOptions)
            {
                std::getline(fOptions, fline);
                printer.print(fline);
            }
        }
        fOptions.close();
    }
};
