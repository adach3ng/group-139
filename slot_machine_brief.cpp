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
        //full board type
        {{"🍒", "🍒", "🍒"}, 30},
        {{"🍋", "🍋", "🍋"}, 30},
        {{"🍊", "🍊", "🍊"}, 50},
        {{"🍑", "🍑", "🍑"}, 50},
        {{"🔔", "🔔", "🔔"}, 80},
        {{"🍫", "🍫", "🍫"}, 80},
        {{"7️⃣", "7️⃣", "7️⃣"}, 1000000}, // Jackpot 
        //proposed synergy 🍒+🔔, 🍋+🍊, 🍑+🍫 (basic+rare, basic+uncommon, uncommon+rare)
        //(Note 7️⃣ has no synergy, so when the player start upgrading 7️⃣, they would disrupt their other synergy)
        //🍒+🔔
        {{"🔔", "🍒", "🔔"}, 200},
        {{"🔔", "🔔", "🍒"}, 80},
        {{"🍒", "🔔", "🔔"}, 80},
        {{"🍒", "🍒", "🔔"}, 50},
        {{"🍒", "🔔", "🍒"}, 50},
        {{"🔔", "🍒", "🍒"}, 50},
        //🍋+🍊
        {{"🍋", "🍊", "🍋"}, 100},
        {{"🍋", "🍋", "🍊"}, 30},
        {{"🍊", "🍋", "🍋"}, 30},
        {{"🍊", "🍊", "🍋"}, 50},
        {{"🍊", "🍋", "🍊"}, 50},
        {{"🍋", "🍊", "🍊"}, 50},
        //🍑+🍫
        {{"🍑", "🍫", "🍫"}, 150},
        {{"🍫", "🍑", "🍫"}, 70},
        {{"🍫", "🍫", "🍑"}, 70},
        {{"🍫", "🍑", "🍑"}, 70},
        {{"🍑", "🍫", "🍑"}, 70},
        {{"🍑", "🍑", "🍫"}, 70},
          
    };
    /*{{"🍒", "🍒", "🍒"}, 10}, og values
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
        {{"7️⃣", "7️⃣", "7️⃣"}, 1000000}  // Jackpot */

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

int loops = 0; //loop++ per slot machine spin
int win = 0; //win++ if user wins
int chance = 1; // chance = 0 then gg
int balance = 200; // intitial balance (debug note: moved this ahead so balance is defined in level)
void level(int &win, std::vector<std::string> &symbolSet){ //changed from one loop to function, remember to test
    while (true) {
        int p1 = rand() % 20;
        if (p1 <= loops-5 && p1 % 2 == 0) //rob the player with increasing probability that caps at 50%
        {
            std::cout << "\nTIME TO PAY YOUR GAMBLING TAXES!" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            balance -= balance * 0.1 + loops;
            std::cout << "The IRS took away $" << balance * 0.1 + loops << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        }

        std::cout << "\nBalance: $" << balance << std::endl;
        
        if (balance < betSystem.getBaseBet()) { //check if player is broke
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            std::cout << "\n ⚠️ Insufficient funds for minimum bet. Game Over! ⚠️\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            chance = 0;
            break;
        }

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
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
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
            std::this_thread::sleep_for(std::chrono::milliseconds(300-loops*2));
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

        std::this_thread::sleep_for(std::chrono::milliseconds(500-loops*3)); //entering upgrade phase (originally 1500)
        /*note for game balancing. Increase the number of occurance in symbolSet to increase the odds of a symbol.
        Easy mode is actually hard rn since the symbolSet is just one of each symbol in normal while in easy it is
        three of each symbol, this diluting the pool for upgrades*/

        /*additional ideas: if you played dnd you probably know this, rolling with advantage is something we can 
        implement easily. Have an upgrade that gives the player a chance to roll with advantage, that is they roll
        twice and get the better result of the two rolls*/

        std::cout << "Upgrade time!" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1500-loops*10));
        std::cout << "\nChoices: (Type in corresponding number, everything costs $20)" << std::endl; //display available upgrades
        std::this_thread::sleep_for(std::chrono::milliseconds(500-loops*3));
        for (int i = 0; i < symbols.size(); i++)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100-loops));
            std::cout << i+1 << ": " << symbols[i] << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000-loops*10));
        std::cout << "\nWhat is your choice?" << std::endl;
        int upgrade_choice;
        std::cin >> upgrade_choice;

        if (upgrade_choice == 1 || upgrade_choice == 2) //add four copies of basic
        {
            std::cout << "good choice" << std::endl;
            balance -= 20;
            for (int i = 0; i < 4; i++)
            {
                symbolSet.push_back(symbols[upgrade_choice-1]);
            }
        }

        else if (upgrade_choice == 3 || upgrade_choice == 4) //add three copies of uncommon
        {
            std::cout << "good choice" << std::endl;
            balance -= 20;
            for (int i = 0; i < 3; i++)
            {
                symbolSet.push_back(symbols[upgrade_choice-1]);
            }
        }

        else if (upgrade_choice == 5 || upgrade_choice == 6) //add two copies of basic
        {
            std::cout << "good choice" << std::endl;
            balance -= 20;
            for (int i = 0; i < 2; i++)
            {
                symbolSet.push_back(symbols[upgrade_choice-1]);
            }
        }

        else if (upgrade_choice == 7) //add one copy of jackpot
        {
            std::cout << "a gambling man, huh?" << std::endl;
            balance -= 20;
            symbolSet.push_back(symbols[upgrade_choice-1]);
        }

        else
        {
            std::cout << "you leave the upgrade shop" << std::endl;
        }

        loops++;
        std::cout << symbolSet.size() << "\n";
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
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    std::cout << "🎰 Welcome to the Enhanced Slot Machine! 🎰\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    betSystem.showBetHelp();
    while(chance!=0){
        level(win, symbolSet); //debug: added symbolSet into level function
    }
    if (chance == 0){
        std::cout << "Good game, your win streak was " << win;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    return 0;
}
