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
BetSystem betSystem(10); //initializing systems
DifficultySystem difficulty;
std::map<std::vector<std::string>, int> payouts = { //defining winning boards
    //full board type win
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

//generate and return a reel with three symbols randomly using odds given by symboSet
std::vector<std::string> spinReels(const std::vector<std::string>& symbolSet) { 
    std::vector<std::string> reels;
    for (int i = 0; i < 3; ++i) {
        int index = rand() % symbolSet.size();
        reels.push_back(symbolSet[index]);
    }
    return reels;
}

//look through payouts to check if one of the keys matches the input reel
int checkWinningCombination(const std::vector<std::string>& reels) {
    auto it = payouts.find(reels);
    if (it != payouts.end()) {
        return it->second;
    }
    return 0;
}

//Messages printed when no winning combo
std::vector<std::string> sadMessages = {
    "😢 So close!",
    "😞 Better luck next time!",
    "💪 Almost there, don't give up!",
    "🍀 Luck wasn't on your side!",
    "🤡 Womp womp.",
    "Nice try diddy!",
    "😐 Bro typed 'spin' and got humbled."
};

//main game vatiables
int loops = 1; //loop++ per slot machine spin
int win = 0; //win++ if user wins, reset to zero when use loses
int winStreak = 0; //record max value of win
int balance = 200; //available balance for player to spend gambling
float payoutModifier = 1.0f; //a multiplier on payout affected by difficulty and upgrades
double winMult = 1.0f; //win streak multiplier
float taxModifier = 1.0f; //a multiplier on tax payment affected by difficulty
int luck = 110; //determines how rigged the slot machine is, player has base luck of 10 + 100 beginner's luck 
double timerMult = 1; //multipler of time of pauses in the game
bool showRig = false; //determin if rerolls made by rigged rolls are shown

void item1(int &balance){ // random items 
    balance+=500;
}
void item2(int &luck){
    luck+=5;
}
void item3(int &balance){
    balance+=5000;
}

void level(std::vector<std::string> &symbolSet){ //main game loop
    while (true) {
        std::cout << "\nBalance: $" << balance << std::endl; //output balance
        
        if (balance < betSystem.getBaseBet()) { //check if player is broke
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            std::cout << "\n ⚠️ Insufficient funds for minimum bet. Game Over! ⚠️\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            break;
        }

        int amount; //get user's bet amount
        std::cout << "Enter bet amount (0 to quit): ";
        std::cin >> amount;

        //cheats
        if (amount == 777) { 
            std::cout << "MANIFEST DESTINY\n";
            luck += 77777;
            continue;
        }
        if (amount == -777) {
            std::cout << "UNMANIFEST DESTINY\n";
            luck -= 77777;
            continue;
        }

        //invalid inputs
        if (std::cin.fail() || amount < 0) { 
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input! Please enter a positive number.\n";
            continue;
        }

        //quit game
        if (amount == 0) { 
            std::cout << "Cash out with $" << balance << ". Goodbye!\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            break;
        }

        //run placeBet and ignore cin if amount is less that required amount or more than available balance
        if (!betSystem.placeBet(balance, amount)) { 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        // Successful bet placed
        std::vector<std::string> reels = spinReels(symbolSet); //spin the reels
        for (int i = 1; i < 4; i++){
            std::cout << "\nSpinning";
            for (int j = 0; j < i; j++) {
                std::cout << ".";
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(300*timerMult)));
            }
        }
        int baseWin = checkWinningCombination(reels); //get the base payout amount

        //roll with advantage multiple times if lucky enough
        int repeats = 0;
        int rerollProb = rand() % 20;
        
        std::vector<std::string> jackpot = {"7️⃣", "7️⃣", "7️⃣"};
        bool riggedJackpot = false;
        while (luck >= rerollProb) {
            std::vector<std::string> rerollReels = spinReels(symbolSet);
            if (showRig)
            {
                std::cout << "rigged!\n";
                for (const auto& reel : reels) { //outputting rigged reels acording to player setting
                    std::cout << reel << "| ";
                }
                for (const auto& rerollReels : rerollReels) { 
                    std::cout << rerollReels << "| ";
                }
                std::cout << "\n";
            }
            if (checkWinningCombination(rerollReels) > baseWin) //chooses the better outcome of the two reels
            {
                reels = rerollReels;
                baseWin = checkWinningCombination(rerollReels);

                if (reels == jackpot && !riggedJackpot) { //rig jackpot
                    std::vector<std::string> rerollReels = spinReels(symbolSet);
                    reels = rerollReels;
                    baseWin = checkWinningCombination(rerollReels);
                    riggedJackpot = true;
                }
            }
            int addProb = rand() % 20; //additional reroll chance
            rerollProb += addProb;
        }
        
        //print out reels
        std::cout << std::endl;
        for (const auto& reel : reels) { 
            std::cout << reel << "| ";
        }
        std::cout << std::endl;

        if (baseWin > 0) { //when player wins
            winMult = 1; //win streak multiplier calculation, multiply payout by 1.2 per 5 win streak
            for (int i = 0; i < win/5; i++)
            {     
                winMult *= 1.2;
            }

            //calculate payout with total multipliers
            int payout = static_cast<int>(betSystem.calculatePayout(baseWin) * payoutModifier * winMult);
            balance += payout;
            std::cout << "WIN! Base: $" << baseWin 
                      << " × " << betSystem.getMultiplier() 
                      << " × Payout Multiplier: " << payoutModifier 
                      << " × Win Streak Multiplier: " << winMult
                      << " → Total: $" << payout << "!\n";
            
            //increment win streak.
            win++;
            if (win > 1)
            {
                std::cout << "Current Win Streak: " << win << std::endl;
            }
            if (winStreak < win)
            {
                winStreak++;
            }

            //check if it is a jack pot, special dialogues
            if (baseWin >= 1000000) {
                std::cout << "💰 JACKPOT!!! MILLIONAIRE STATUS ACHIEVED! 💰\n";
                std::cout << "Enter Yes/No to continue to second level ";
                std::string response;;
                std::cin >> response;
                if (response == "Yes" || response == "yes") {
                    std::cout << "You are now a millionaire! But you got robbed by the IRS when entering the next level, good luck!\n";
                    balance -= 1000000;
                } else {
                    std::cout << "You chose not to continue. Goodbye!\n";
                    break;
                }
            } 

            if (win % 3 == 0) //get 1 random item per 3 wins
            { 
                int randomnumber = rand() % 11;
                if (randomnumber < 7){
                    item1(balance);
                    std::cout<< "you just got a common item, small pot of gold";
                }
                else if (randomnumber >=7 && randomnumber <10){
                    item2(luck);
                    std::cout<< "you just got a rare item, four leaf clover!";
                }
                else{
                    item3(balance);
                    std::cout<< "you just got an epic item, gold rain!";
                }
            }

        }else { //when player loses
            std::cout << sadMessages[rand() % sadMessages.size()] << "\n";
            std::cout << "No winning combination. Try again!\n";
            win = 0;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1500*timerMult))); //entering upgrade phase (originally 1500)
        std::cout << "Upgrade time!" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1500*timerMult)));

        //display available upgrades
        std::cout << "\nChoices: (Type in corresponding number, everything costs $20)" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(500*timerMult)));

        //symbol upgrades
        for (int i = 0; i < symbols.size(); i++) 
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(100*timerMult)));
            std::cout << i+1 << ": " << symbols[i] << std::endl;
        }

        //special upgrades
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(100*timerMult)));
        std::cout << "8: 📈" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(100*timerMult)));
        std::cout << "9: 🍀" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(100*timerMult))); 
        
        //get player's choice of upgrade
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

        else if (upgradeChoice == 9) //increase luck for reroll chance
        {
            std::cout << "feeling lucky?" << std::endl;
            balance -= 20;
            luck += 1;
        }

        else
        {
            std::cout << "you leave the upgrade shop" << std::endl;
        }

        if (loops == 5) //remove beginner's luck after 5 rolls
        {
            luck -= 100;
        }

        int p1 = rand() % 20;
        if (p1 < loops-5 && p1 % 3 == 0) //robs the player with increasing probability over time that caps at 33%
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

    //game configuration
    //pause time
    std::cout << "\n\npause period? (0-100): ";
    std::cin >> timerMult;
    if (timerMult > 100)
    {
        timerMult = 1;
    }
    else if (std::cin.fail() || timerMult == 0)
    {
        timerMult = 0;
        std::cout << "in a hurry?\n";
    }
    else
    {
        timerMult /= 100;
    }

    //show rigged rolls
    std::string showRigInput;
    std::cout << "\nshow rigged result? (y/n): ";
    std::cin >> showRigInput;
    if (showRigInput == "y" || showRigInput == "Y")
    {
        std::cout << "It always has been rigged\n";
        showRig = true;
    }

    //difficulty
    int choice;
    std::cout << "\nSelect Difficulty:\n"
              << "1. Easy   (More wins, higher payout)\n"
              << "2. Medium (Balanced)\n"
              << "3. Hard   (Low chance, tough odds)\n"
              << "Enter choice [1-3]: ";
    std::cin >> choice;
    difficulty.selectDifficulty(choice);

    //initialize base on difficulty
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
    std::cout << R"(
        ⠀⠀ ⠀⣀⣤⣴⣶⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣶⣤⣤⣀⠀⠀⠀⠀⠀
        ⠀⠀⠀ ⠀⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⠀⠀⠀⠀
        ⠀⠀⠀⠀⢸⣿⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⣿⡇⠀⠀
        ⠀⠀⠀⠀⢸⣿⠀⢸⣿⣿⡇ ⢸⣿⣿⡇ ⢸⣿⣿⡇ ⣿⡇⠀
        ⠀⠀⠀⠀⢸⣿⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣿⡇
        ⠀⠀⠀⠀⠈⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉
          ⠀⢀⣴⣿⠟⠛⠛⠛⢿⣿⡿⠛⠛⠛⢿⣿⡿⠛⠛⠛⠛⢿⣦⡀⠀⠀
            ⣤⣾⣷⣦⣤⣤⣾⣿⣷⣦⣤⣤⣤⣾⣷⣦⣤⣤⣶⣿⣷⣦)";
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(3000*timerMult)));

    betSystem.showBetHelp(); //show note on how betting system works
    level(symbolSet); //main game loop
    std::cout << "Good game, your win streak was " << winStreak; //show highest win streak at end of game
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    return 0;
}