#pragma once
#include "Libraries.h"
#include <iomanip>
class Printer
{
private:

public:

	void print(std::vector<std::vector<std::string>> &board)
	{
        //'''os X'''
        std::cout << " ";
        for (int i = 0; i < 11; ++i)
        {
            if (i < 10)
            {

                std::cout << ' ' << i;
            }
            else
            {
                std::cout << ' ' << i - 10;
            }
        }
        std::cout << std::endl;
        for (int i = 0; i < 11; ++i)
        {
            if (i < 10)
            {

                std::cout << i;
            }
            else
            {
                std::cout << i - 10;
            }
            //sachovnica
            for (int j = 0; j < (int(board[i].size())); ++j)
            {
                if (board[i].size() != 11 and j < 1)
                {
                    std::cout << std::setw(10);
                }
                std::cout << board[i][j];
            }
            std::cout << std::endl;
        }
	}

    void print(std::string toPrint, bool InvalidPawn)
    {
        std::cout << std::endl;
        std::cout << "----------------------" << std::endl;
        std::cout << toPrint << std::endl;
        std::cout << "----------------------" << std::endl;
    }

    void print(std::string toPrint1, int i1, int i2, std::string toPrint2)
    {
        std::cout << toPrint1 << i1 << " + " << i2  << toPrint2 << std::endl;
    }

    void print(std::string toPrint)
    {
        std::cout << toPrint << std::endl;
    }

    void print(std::string toPrint, int i, bool endl = true)
    {
        std::cout << toPrint << i; 
        if (endl)
        {
            std::cout << std::endl;
        }
    }

    void print(std::string toPrint, eEntityType &e)
    {
        std::string entity;
        switch (e)
        {
        case PlayerA:
            entity = "Player A";
            break;
        case PlayerB:
            entity = "Player B";
            break;
        case PlayerC:
            entity = "Player C";
            break;
        case PlayerF:
            entity = "Player F";
            break;
        case ePawn:
            entity = "Pawn";
            break;
        default:
            entity = "none";
            break;
        }
        std::cout << toPrint << entity << std::endl;
    }

    void print(std::string toPrint,int i, std::string toPrint2)
    {
        std::cout << toPrint << i << toPrint2 << std::endl;
    }
};