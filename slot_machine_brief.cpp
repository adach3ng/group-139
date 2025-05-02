#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include "bet_system.h"
#include "difficulty_system.h"

std::vector<std::string> symbols = {"🍒", "🍋", "🍊", "🍑", "🔔", "🍫", "7️⃣"};  // Cherry, Lemon, Orange, Plum, Bell, Bar, Seven

std::vector<std::string> spinReels(const std::vector<std::string>& symbolSet) {
    std::vector<std::string> reels;
    for (int i = 0; i < 3; ++i) {
        int index = rand() % symbolSet.size();
        reels.push_back(symbolSet[index]);
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

std::vector<std::string> sadMessages = {
    "😢 So close!",
    "😞 Better luck next time!",
    "💪 Almost there, don't give up!",
    "🍀 Luck wasn't on your side!",
    "🤡 Womp womp.",
    "Nice try diddy!",
    "😐 Bro typed 'spin' and got humbled."
};

int win = 0; //win++ if user wins
int chance = 1; // chance = 0 then gg
int level(int &win){ //changed from one loop to function, remember to test
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
        std::vector<std::string> reels = spinReels(symbolSet);
        for (int i = 1; i < 4; i++){
            std::cout << "\nSpinning";
            for (int j = 0; j < i; j++) {
                std::cout << ".";
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            }
        }
        std::cout << std::endl;
        for (const auto& reel : reels) {
            std::cout << reel << "| ";
        }
        std::cout << std::endl;

        int baseWin = checkWinningCombination(reels);
        if (baseWin > 0) {
            int payout = static_cast<int>(betSystem.calculatePayout(baseWin) * difficultyModifier);
            balance += payout;
            std::cout << "WIN! Base: $" << baseWin 
                      << " × " << betSystem.getMultiplier() 
                      << " × Difficulty: " << difficultyModifier 
                      << " → Total: $" << payout << "!\n";
            
            if (baseWin == 1000000) {
                std::cout << "💰 JACKPOT!!! MILLIONAIRE STATUS ACHIEVED! 💰\n";
                win++;
                std::cout << "Enter Yes/No to continue to second level";
                std::string response;;
                std::cin >> response;
                if (response == "Yes" || response == "yes") {
                    std::cout << "You are now a millionaire! But you got robbed when entering the next level, good luck!\n";
                } else {
                    std::cout << "You chose not to continue. Goodbye!\n";
                    break;
                }
            } 
        }else {
            
            std::cout << sadMessages[rand() % sadMessages.size()] << "\n";
            std::cout << "No winning combination. Try again!\n";
        }

        if (balance < betSystem.getBaseBet()) {
            std::cout << "\n⚠️ Insufficient funds for minimum bet. Game Over! ⚠️\n";
            chance = 0;
            break;
        }
    }
}
int main() {
    srand(static_cast<unsigned int>(time(0)));
    BetSystem betSystem(10);

    DifficultySystem difficulty;

    int choice;
    std::cout << "Select Difficulty:\n"
              << "1. Easy   (More wins, higher payout)\n"
              << "2. Medium (Balanced)\n"
              << "3. Hard   (Low chance, tough odds)\n"
              << "Enter choice [1-3]: ";
    std::cin >> choice;
    difficulty.selectDifficulty(choice);

    float difficultyModifier = difficulty.getModifier();
    std::string difficultyName = difficulty.getDifficultyName();
    std::vector<std::string> symbolSet = getSymbolSetForDifficulty(choice);

    int balance = 100;

    std::cout << "\n🎮 Difficulty Selected: " << difficultyName << "\n";
    std::cout << "🏦 Starting Balance: $" << balance << "\n";
    std::cout << "💸 Payout Modifier: x" << difficultyModifier << "\n";

    std::cout << "🎰 Welcome to the Enhanced Slot Machine! 🎰\n";
    betSystem.showBetHelp();
    while(chance!=0){
        level(win);
    }
    if (chance == 0){
        cout << "Good game, your win streak was" << win;
    }
    return 0;
}
