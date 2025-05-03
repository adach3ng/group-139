// bettingsetting.cpp
#include "bet_system.h"

BetSystem::BetSystem(int defaultBet) : baseBet(defaultBet), multiplier(1.0f) {}

bool BetSystem::placeBet(int& balance, int amount) {
    if (amount < baseBet) {
        std::cout << "Lowest Bet Is: " << baseBet << "!" << std::endl;
        return false;
    }
    
    if (amount > balance) {
        std::cout << "Insufficient balance! Current balance: " << balance << std::endl;
        return false;
    }

    // Payout Ratio(Unlinear)
    if (amount <= 40) { //50
        multiplier = 1.0f + (amount - 10 / 60.0f); //100
    } else if (amount <= 80) {
        multiplier = 1.5f + ((amount - 40) / 80.0f); //((amount - 50) / 200.0f)
    } else {
        multiplier = 2.0f;
    }

    balance -= amount;
    return true;
}

int BetSystem::calculatePayout(int basePayout) const {
    return static_cast<int>(basePayout * multiplier);
}

float BetSystem::getMultiplier() const {
    return multiplier;
}

void BetSystem::showBetHelp() const {
    std::cout << "\n---Betting system description---\n"
              << "The betting amount affects the bonus multiplier ratio:\n"
              << "10-40:  1.0x - 1.5x\n" //"10-50:  1.0x - 1.5x\n"
              << "41-80: 1.5x - 2.0x\n" //"51-100: 1.5x - 2.0x\n"
              << ">80:   2.0x\n" //">100:   2.0x\n"
              << "Each bet must not be less than the base amount" << baseBet << std::endl;
}

int BetSystem::getBaseBet() const {
    return baseBet;
}