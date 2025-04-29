#pragma once
#include <string>
#include <vector>
#include <iostream>

class DifficultySystem {
private:
    float modifier;
    std::string difficultyName;

public:
    enum DifficultyLevel { EASY = 1, MEDIUM = 2, HARD = 3 };

    DifficultySystem() : modifier(1.0f), difficultyName("🟡 Medium") {}

    void selectDifficulty(int choice) {
        switch (choice) {
            case EASY:
                modifier = 1.2f;
                difficultyName = "🟢 Easy";
                break;
            case MEDIUM:
                modifier = 1.0f;
                difficultyName = "🟡 Medium";
                break;
            case HARD:
                modifier = 0.8f;
                difficultyName = "🔴 Hard";
                break;
            default:
                std::cout << "Invalid choice. Defaulting to Medium.\n";
                modifier = 1.0f;
                difficultyName = "🟡 Medium";
                break;
        }
    }

    float getModifier() const { return modifier; }
    std::string getDifficultyName() const { return difficultyName; }
};



inline std::vector<std::string> getSymbolSetForDifficulty(int choice) {
    std::vector<std::string> base = {"🍒", "🍋", "🍊", "🍑", "🔔", "🍫", "7️⃣"};
    std::vector<std::string> result;
    switch (choice) {
        case DifficultySystem::EASY:
            for (const auto& symbol : base) {
                for (int i = 0; i < 3; ++i)
                    result.push_back(symbol);
            }
            return result;
        case DifficultySystem::MEDIUM:
            return base;
        case DifficultySystem::HARD: { //chaotic symbol library 
            for (const auto& symbol : base) {
                int copies = 1 + rand() % 2;  // 1 or 2 copies
                for (int i = 0; i < copies; ++i)
                    result.push_back(symbol);
            }
            return result;
        default:
            return base;
    }
}
}
