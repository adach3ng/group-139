// bet_system.h
#pragma once
#include <iostream>

class BetSystem {
private:
    int baseBet;    // base betting amount
    float multiplier; // dynamic reward ratio

public:
    BetSystem(int defaultBet = 10);
    
    // set betting amount
    bool placeBet(int& balance, int amount);
    
    // calculate actural rewatd
    int calculatePayout(int basePayout) const;
    
    // get current betting ratio
    float getMultiplier() const;
    
    // show the instruction
    void showBetHelp() const;

    //return value of base bet inputted by player
    int getBaseBet() const;
};
