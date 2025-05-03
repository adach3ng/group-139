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
    {{"🍋", "🍋", "🍊"}, 40},
    {{"🍊", "🍋", "🍋"}, 40},
    {{"🍊", "🍊", "🍋"}, 60},
    {{"🍊", "🍋", "🍊"}, 60},
    {{"🍋", "🍊", "🍊"}, 60},
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

std::vector<std::string> spinReels(const std::vector<std::string>& symbolSet) {
    std::vector<std::string> reels;
    for (int i = 0; i < 3; ++i) {
        int index = rand() % symbolSet.size();
        reels.push_back(symbolSet[index]);
    }
    return reels;
}

int checkWinningCombination(const std::vector<std::string>& reels) {
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

int loops = 1; //loop++ per slot machine spin
int win = 0; //win++ if user wins
int winStreak = 0;
//int chance = 1; // chance = 0 then gg
int balance = 200; // intitial balance (debug note: moved this ahead so balance is defined in level)
float payoutModifier = 1.0f;
float taxModifier = 1.0f;
int luck = 110;
double timerMult = 1;
void level(std::vector<std::string> &symbolSet){
    while (true) {
        std::cout << "\nBalance: $" << balance << std::endl;
        
        if (balance < betSystem.getBaseBet()) { //check if player is broke
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            std::cout << "\n ⚠️ Insufficient funds for minimum bet. Game Over! ⚠️\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            //chance = 0;
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
            //chance = 0;
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
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(300*timerMult)));
            }
        }
        int baseWin = checkWinningCombination(reels);

        //roll with advantage
        int repeats = 0;
        int rerollProb = rand() % 20;
        while (luck >= rerollProb) {
            std::vector<std::string> rerollReels = spinReels(symbolSet);
            std::cout << "rigged!\n";
            for (const auto& reel : reels) { //checking rigged reels
                std::cout << reel << "| ";
            }
            for (const auto& rerollReels : rerollReels) { 
                std::cout << rerollReels << "| ";
            }
            std::cout << "\n";
            if (checkWinningCombination(rerollReels) > baseWin)
            {
                reels = rerollReels;
                baseWin = checkWinningCombination(rerollReels);
            }
            int addProb = rand() % 20; //additional reroll chance
            rerollProb += addProb;
        }

        //rig jackpot
        std::vector<std::string> jackpot = {"7️⃣", "7️⃣", "7️⃣"};
        if (reels == jackpot) {
            std::vector<std::string> rerollReels = spinReels(symbolSet);
            reels = rerollReels;
            baseWin = checkWinningCombination(rerollReels);
        }

        std::cout << std::endl;
        for (const auto& reel : reels) { //output reels
            std::cout << reel << "| ";
        }
        std::cout << std::endl;

        if (baseWin > 0) {
            double winMult = 1;
            for (int i = 0; i < win/5; i++)
            {     
                winMult *= 1.2;
            }
            int payout = static_cast<int>(betSystem.calculatePayout(baseWin) * payoutModifier * winMult);
            balance += payout;
            std::cout << "WIN! Base: $" << baseWin 
                      << " × " << betSystem.getMultiplier() 
                      << " × Payout Multiplier: " << payoutModifier 
                      << " × Win Streak Multiplier: " << winMult
                      << " → Total: $" << payout << "!\n";
            
            if (win > 1)
            {
                std::cout << "Current Win Streak: " << win << std::endl;
            }

            if (baseWin >= 800000) {
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
            win++;
            if (winStreak < win)
            {
                winStreak++;
            }
        }else {
            std::cout << sadMessages[rand() % sadMessages.size()] << "\n";
            std::cout << "No winning combination. Try again!\n";
            win = 0;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1500*timerMult))); //entering upgrade phase (originally 1500)

        std::cout << "Upgrade time!" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1500*timerMult)));
        std::cout << "\nChoices: (Type in corresponding number, everything costs $20)" << std::endl; //display available upgrades
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(500*timerMult)));

        for (int i = 0; i < symbols.size(); i++) //symbol upgrades
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(100*timerMult)));
            std::cout << i+1 << ": " << symbols[i] << std::endl;
        }

        //special upgrades
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(100*timerMult)));
        std::cout << "8: 📈" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(100*timerMult)));
        std::cout << "9: 🍀" << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(100*timerMult))); //get player's choice of upgrade
        std::cout << "\nWhat is your choice?" << std::endl;
        int upgradeChoice;
        std::cin >> upgradeChoice;

        if (upgradeChoice == 1 || upgradeChoice == 2) //add four copies of basic
        {
            std::cout << "good choice" << std::endl;
            balance -= 20;
            for (int i = 0; i < 4; i++)
            {
                symbolSet.push_back(symbols[upgradeChoice-1]);
            }
        }

        else if (upgradeChoice == 3 || upgradeChoice == 4) //add three copies of uncommon
        {
            std::cout << "good choice" << std::endl;
            balance -= 20;
            for (int i = 0; i < 3; i++)
            {
                symbolSet.push_back(symbols[upgradeChoice-1]);
            }
        }

        else if (upgradeChoice == 5 || upgradeChoice == 6) //add two copies of basic
        {
            std::cout << "good choice" << std::endl;
            balance -= 20;
            for (int i = 0; i < 2; i++)
            {
                symbolSet.push_back(symbols[upgradeChoice-1]);
            }
        }

        else if (upgradeChoice == 7) //add one copy of jackpot
        {
            std::cout << "a gambling man, huh?" << std::endl;
            balance -= 20;
            symbolSet.push_back(symbols[upgradeChoice-1]);
        }

        else if (upgradeChoice == 8) //increase payout multiplier
        {
            std::cout << "good investment" << std::endl;
            balance -= 20;
            payoutModifier += 0.05;
        }

        else if (upgradeChoice == 9) //icrease luck for reroll chance
        {
            std::cout << "feeling lucky?" << std::endl;
            balance -= 20;
            luck += 1;
        }

        else
        {
            std::cout << "you leave the upgrade shop" << std::endl;
        }

        if (loops == 5)
        {
            luck -= 100;
        }

        int p1 = rand() % 20;
        if (p1 < loops-5 && p1 % 3 == 0) //rob the player with increasing probability that caps at 33%
        {
            std::cout << "\nTIME TO PAY YOUR GAMBLING TAXES!" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            int tax = static_cast<int>((balance * 0.1 + loops) * taxModifier);
            balance -= tax;
            std::cout << "The IRS took away $" << tax << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        }
        loops++;
    }
}
int main() {
    srand(static_cast<unsigned int>(time(0)));
    std::cout<<"start";

    std::cout << "\n\nInput waiting time (0-100): ";
    std::cin >> timerMult;
    timerMult /= 100;
    if (std::cin.fail() || timerMult == 0)
    {
        timerMult = 0;
        std::cout << "in a hurry?\n";
    }

    int choice;
    std::cout << "Select Difficulty:\n"
              << "1. Easy   (More wins, higher payout)\n"
              << "2. Medium (Balanced)\n"
              << "3. Hard   (Low chance, tough odds)\n"
              << "Enter choice [1-3]: ";
    std::cin >> choice;
    difficulty.selectDifficulty(choice);

    balance = difficulty.getStartingBalance();
    payoutModifier = difficulty.getPayoutModifier();
    taxModifier = difficulty.getTaxModifier();
    std::string difficultyName = difficulty.getDifficultyName();
    std::vector<std::string> symbolSet = getSymbolSetForDifficulty(choice);

    std::cout << "\n🎮 Difficulty Selected: " << difficultyName << "\n";
    std::cout << "🏦 Starting Balance: $" << balance << "\n";
    std::cout << "💰 Payout Modifier: x" << payoutModifier << "\n";
    std::cout << "💸 Tax Modifier: x" << taxModifier << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(2000*timerMult)));

    std::cout << "🎰 Welcome to the Enhanced Slot Machine! 🎰\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(2000*timerMult)));

    betSystem.showBetHelp();
    //while(chance!=0){
    level(symbolSet);
    //}
    //if (chance == 0){
    std::cout << "Good game, your win streak was " << winStreak;
    //}
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    return 0;
}
