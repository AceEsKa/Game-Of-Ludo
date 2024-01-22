#pragma once
#include "Libraries.h"
#include <ctime>
#include <cstdlib>
#include <iostream>

//make into class

std::random_device r;
std::shuffle_order_engine<std::minstd_rand0, 256> generator(r());
//for debug
int arr[] = { 3,5,5,5,2,4,5,4,5,4,2,1,6,4,5,4,3,6,3,5,6,2,6,6,6,6,6,4,2,1,2,5,4,4,2,5,4,2,4,6,5,4,3,6,2,2,1,3,4,4,4,6,5,5,3,1,4,2,5,2,4,1,4,2,6,6, 6,2,6,6,6,6,6,3 };
int iterator = 0;

int RNG(int start, int finish)
{
    int roll;

    std::uniform_int_distribution<int> dist(1, 6);
    roll = dist(generator);
    return roll;
}
