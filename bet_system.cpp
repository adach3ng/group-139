// bettingsetting.cpp
#include "bettingsetting.h"

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
    if (amount <= 50) {
        multiplier = 1.0f + (amount / 100.0f);
    } else {
        multiplier = 1.5f + ((amount - 50) / 200.0f);
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
              << "10-50:  1.0x - 1.5x\n"
              << "51-100: 1.5x - 2.0x\n"
              << ">100:   2.0x 以上\n"
              << "Each bet must not be less than the base amount" << baseBet << std::endl;
}
