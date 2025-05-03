#pragma once

#include <string>
#include <vector>
#include <map>

class SlotIO {
public:
    
    static bool saveGame(const std::string& filename, 
                        int balance, 
                        int winStreak, 
                        int loops, 
                        int luck,
                        float payoutModifier,
                        float taxModifier,
                        const std::vector<std::string>& symbolSet,
                        int difficultyLevel);
    static bool loadGame(const std::string& filename, 
                        int& balance, 
                        int& winStreak, 
                        int& loops, 
                        int& luck,
                        float& payoutModifier,
                        float& taxModifier,
                        std::vector<std::string>& symbolSet,
                        int& difficultyLevel);
};
