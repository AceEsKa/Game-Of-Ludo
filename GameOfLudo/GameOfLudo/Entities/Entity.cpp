#pragma once
#include <string>
#include <functional>
#include <iostream>
#include "Enums.cpp"
/// <summary>
/// pure virtual
/// </summary>
class Entity
{
private:

protected:
    bool InGame;
    std::string Name;
    eEntityType EntityType;

public:

    virtual bool *GetInGameStatus() = 0;


    virtual std::string GetName() = 0;


    virtual void Move(int AmountToMove, int no_pawn = -1) = 0;

    virtual std::optional<unsigned int> MovemantHandler(std::string DisplayMessage = "Message") = 0;

};