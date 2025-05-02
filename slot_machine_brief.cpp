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
BetSystem betSystem(10); //debug: defining systems at start instead of in main
DifficultySystem difficulty;

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
int balance = 100; // intitial balance (debug note: moved this ahead so balance is defined in level)
void level(int &win, std::vector<std::string> &symbolSet){ //changed from one loop to function, remember to test
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
            std::this_thread::sleep_for(std::chrono::milliseconds(3000));
            chance = 0;
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
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); //(originally 300)
            }
        }
        std::cout << std::endl;
        for (const auto& reel : reels) {
            std::cout << reel << "| ";
        }
        std::cout << std::endl;

        int baseWin = checkWinningCombination(reels);
        if (baseWin > 0) {
            float difficultyModifier = difficulty.getModifier(); //debug: defined difficultyModifier
            int payout = static_cast<int>(betSystem.calculatePayout(baseWin) * difficultyModifier);
            balance += payout;
            std::cout << "WIN! Base: $" << baseWin 
                      << " × " << betSystem.getMultiplier() 
                      << " × Difficulty: " << difficultyModifier 
                      << " → Total: $" << payout << "!\n";
            
            if (baseWin == 1000000*win) {
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

        if (balance < betSystem.getBaseBet()) { //debug added getBaseBet function
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            std::cout << "\n⚠️ Insufficient funds for minimum bet. Game Over! ⚠️\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(3000));
            chance = 0;
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500)); //upgrade phase (originally 1500)
        /*note for game balancing. Increase the number of occurance in symbolSet to increase the odds of a symbol.
        Easy mode is actually hard rn since the symbolSet is just one of each symbol in normal while in easy it is
        three of each symbol, this diluting the pool for upgrades*/

        /*additional ideas: if you played dnd you probably know this, rolling with advantage is something we can 
        implement easily. Have an upgrade that gives the player a chance to roll with advantage, that is they roll
        twice and get the better result of the two rolls*/

        std::cout << "upgrade time!" << std::endl;
        int upgrade_choice;
        std::cin >> upgrade_choice;

        if (upgrade_choice >= 1 && upgrade_choice <= 3)
        {
            std::cout << "good choice" << std::endl;
            for (int i = 0; i < 3; i++)
            {
                symbolSet.push_back(symbols[upgrade_choice-1]);
            }
        }

        if (upgrade_choice == 4 || upgrade_choice == 3)
        {
            std::cout << "good choice" << std::endl;
            for (int i = 0; i < 2; i++)
            {
                symbolSet.push_back(symbols[upgrade_choice-1]);
            }
        }

        if (upgrade_choice == 6)
        {
            std::cout << "a gambling man, huh?" << std::endl;
            symbolSet.push_back(symbols[upgrade_choice-1]);
        }

/*        for(int i = 0; i < symbolSet.size(); i++) //for seeing the symbolSet
        {
            std::cout << symbolSet[i] << std::endl;
        }
        std::cout << std::endl;*/
    }
}
int main() {
    srand(static_cast<unsigned int>(time(0)));
    std::cout<<"start";

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

    std::cout << "\n🎮 Difficulty Selected: " << difficultyName << "\n";
    std::cout << "🏦 Starting Balance: $" << balance << "\n";
    std::cout << "💸 Payout Modifier: x" << difficultyModifier << "\n";

    std::cout << "🎰 Welcome to the Enhanced Slot Machine! 🎰\n";
    betSystem.showBetHelp();
    while(chance!=0){
        level(win, symbolSet); //debug: a quick patch for symbolSet
    }
    if (chance == 0){
        std::cout << "Good game, your win streak was" << win; //debug: added std
    }
    return 0;
}
