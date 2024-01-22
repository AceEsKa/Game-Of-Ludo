#pragma once
#include <iostream>
#include "Ludoboard.cpp"
#include "AI.cpp"
#include "Printer.cpp"
#include <fstream>


class Rules
{
private:
	Board LudoBoard;
	Printer printer;
public:
	Rules() 
	{
		printer = Printer();
		LudoBoard = Board();
	};

	void DisplayRules()
	{
		std::ifstream frules;
		frules.open("Rules.txt", std::ios::in);
		std::string fline;
		if (frules.is_open())
		{
			while (frules)
			{
				std::getline(frules,fline);
				printer.print(fline);
			}
		}
		frules.close();
		tutorial();
	}

	void tutorial()
	{
		printer.print("Below will be displayed tutorial board: ");
		system("pause");
		LudoBoard.Tutorial();
		printer.print(LudoBoard.getBoard());
		printer.print("--> E <-- represents entrence to players home tiles --> D <-- and --> S <-- Represents safe tiles");
		printer.print("WARINING: be wary that E and S signes will not be displayed in real game and are displayed here only for better understending instead regular tiles will be displayed represented by --> * <--");
		system("pause");
	}

	bool CheckValidPawn(int movepawnno)
	{
		if (movepawnno > 4 || movepawnno < 1)
		{
			std::cout << std::endl;
			printer.print("----------------------");
			printer.print("Invalid pawn!");
			printer.print("----------------------");
			return false;
		}
		else {
			return true;
		}
	}

	bool CheckMove(int rn, int DiceRoll, int user, Pawn* pawntomove, int pawn, bool HomePositionStatus)
	{
		bool LegalMove = false;

		printer.print("Choose which pawn to move ", rn, DiceRoll - rn, " tiles with or place in game.");
		//get valid pawn less than 4, greater than 1
		if (!LegalMove)
		{
			printer.print("Move is illegal, try to move differnt pawn!");
			printer.print("----------------------");
		}
	}
};
