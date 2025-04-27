#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cstdlib>
#include <ctime>
#include "bet_system.h"

std::vector<std::string> symbols = {"🍒", "🍋", "🍊", "🍑", "🔔", "🍫", "7️⃣"};  // Cherry, Lemon, Orange, Plum, Bell, Bar, Seven

std::vector<std::string> spinReels() {
    std::vector<std::string> reels;
    for (int i = 0; i < 3; ++i) {
        int index = rand() % symbols.size();
        reels.push_back(symbols[index]);
    }
    return reels;
}

int checkWinningCombination(const std::vector<std::string>& reels) {
    // Define payouts for each winning combination
    std::map<std::vector<std::string>, int> payouts = {
        {{"🍒", "🍒", "🍒"}, 10},
        {{"🍋", "🍋", "🍋"}, 5},
        {{"🍊", "🍊", "🍊"}, 4},
        {{"🍑", "🍑", "🍑"}, 3},
        {{"🔔", "🔔", "🔔"}, 20},
        {{"🍫", "🍫", "🍫"}, 50},
        {{"🍒", "🍒", "🍋"}, 2},
        {{"🍋", "🍋", "🍊"}, 2},
        {{"🍊", "🍊", "🍑"}, 2},
        {{"🍑", "🍑", "🔔"}, 2},
        {{"🔔", "🔔", "🍫"}, 10},
        {{"🍒", "🍒", "🍊"}, 2},
        // Additional combinations for easier wins
        {{"🍒", "🍋", "🍒"}, 2},
        {{"🍒", "🍊", "🍒"}, 2},
        {{"🍋", "🍒", "🍋"}, 2},
        {{"🍋", "🍑", "🍋"}, 2},
        {{"🍊", "🍒", "🍊"}, 2},
        {{"🍊", "🔔", "🍊"}, 2},
        {{"🍑", "🍒", "🍑"}, 2},
        {{"🍑", "🍫", "🍑"}, 2},
        {{"🔔", "🍒", "🔔"}, 5},
        {{"🔔", "🍋", "🔔"}, 5},
        {{"🍫", "🍒", "🍫"}, 10},
        {{"🍫", "🍋", "🍫"}, 10},
        {{"7️⃣", "7️⃣", "7️⃣"}, 1000000}  // Jackpot 
    };

    auto it = payouts.find(reels);
    if (it != payouts.end()) {
        return it->second;
    }
    return 0;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    BetSystem betSystem(10);
    int balance = 100;

    std::cout << "🎰 Welcome to the Enhanced Slot Machine! 🎰\n";
    betSystem.showBetHelp();

    while (true) {
        std::cout << "\nBalance: $" << balance << std::endl;
        
        int amount;
        std::cout << "Enter bet amount (0 to quit): ";
        std::cin >> amount;

        if (std::cin.fail() || amount < 0) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input! Please enter a positive number.\n";
            continue;
        }

        if (amount == 0) {
            std::cout << "Cash out with $" << balance << ". Goodbye!\n";
            break;
        }

        if (!betSystem.placeBet(balance, amount)) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        // Successful bet placed
        std::vector<std::string> reels = spinReels();
        std::cout << "\nSpinning: ";
        for (const auto& reel : reels) {
            std::cout << reel << " ";
        }
        std::cout << std::endl;

        int baseWin = checkWinningCombination(reels);
        if (baseWin > 0) {
            int payout = betSystem.calculatePayout(baseWin);
            balance += payout;
            std::cout << "WIN! Base: $" << baseWin 
                      << " × " << betSystem.getMultiplier() 
                      << " → Total: $" << payout << "!\n";
            
            if (baseWin == 1000000) {
                std::cout << "💰 JACKPOT!!! MILLIONAIRE STATUS ACHIEVED! 💰\n";
            }
        } else {
            std::cout << "No winning combination. Try again!\n";
        }

        if (balance < betSystem.getBaseBet()) {
            std::cout << "\n⚠️ Insufficient funds for minimum bet. Game Over! ⚠️\n";
            break;
        }
    }

    return 0;
}
