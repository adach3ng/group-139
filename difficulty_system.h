#pragma once
#include <string>
#include <vector>
#include <iostream>

class DifficultySystem {
private:
    int startingBalance;
    float payoutModifier;
    float taxModifier;
    std::string difficultyName;

public:
    enum DifficultyLevel { EASY = 1, MEDIUM = 2, HARD = 3 };

    DifficultySystem() : payoutModifier(1.0f), difficultyName("🟡 Medium") {}

    void selectDifficulty(int choice) {
        switch (choice) {
            case EASY:
                startingBalance = 200;
                payoutModifier = 1.2f;
                taxModifier = 0.8f;
                difficultyName = "🟢 Easy";
                break;
            case MEDIUM:
                startingBalance = 150;
                payoutModifier = 1.0f;
                taxModifier = 1.0f;
                difficultyName = "🟡 Medium";
                break;
            case HARD:
                startingBalance = 120;
                payoutModifier = 0.8f;
                taxModifier = 1.1f;
                difficultyName = "🔴 Hard";
                break;
            default:
                std::cout << "Invalid choice. Defaulting to Medium.\n";
                startingBalance = 150;
                payoutModifier = 1.0f;
                taxModifier = 1.0f;
                difficultyName = "🟡 Medium";
                break;
        }
    }

    std::string getDifficultyName() const { return difficultyName; }
    int getStartingBalance() const { return startingBalance; }
    float getPayoutModifier() const { return payoutModifier; }
    float getTaxModifier() const { return taxModifier; }
};



inline std::vector<std::string> getSymbolSetForDifficulty(int choice) {
    std::vector<std::string> base = {"🍒", "🍋", "🍊", "🍑", "🔔", "🍫", "7️⃣"};
    std::vector<std::string> result;
    switch (choice) {
        case DifficultySystem::EASY:
            for (const auto& symbol : base) { //equal odds
                for (int i = 0; i < 2; ++i)
                    result.push_back(symbol);
            }
            return result;
        case DifficultySystem::MEDIUM: //higher rarity have less odds
            for (int i = 3; i < 10; i++)
            {
                for (int j = 0; j < i/2; j++)
                {
                    result.push_back(base[9-i]);
                }
            }
            return result;
        case DifficultySystem::HARD: { //chaotic symbol library 
            for (const auto& symbol : base) {
                int copies = 1 + rand() % 4;  // 1 to 4 copies of each
                for (int i = 0; i < copies; ++i)
                    result.push_back(symbol);
            }
            return result;
        default:
            return base;
    }
}
}
