#include "slot_io.h"
#include <fstream>
#include <iostream>
#include <sstream>

bool SlotIO::saveGame(const std::string& filename, 
                     int balance, 
                     int winStreak, 
                     int loops, 
                     int luck,
                     float payoutModifier,
                     float taxModifier,
                     const std::vector<std::string>& symbolSet,
                     int difficultyLevel) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error saving game to " << filename << std::endl;
        return false;
    }

    // Save basic stats
    outFile << balance << "\n"
            << winStreak << "\n"
            << loops << "\n"
            << luck << "\n"
            << payoutModifier << "\n"
            << taxModifier << "\n"
            << difficultyLevel << "\n";

    // Save symbol set
    for (const auto& symbol : symbolSet) {
        outFile << symbol << " ";
    }
    outFile << "\n";

    return true;
}

bool SlotIO::loadGame(const std::string& filename, 
                     int& balance, 
                     int& winStreak, 
                     int& loops, 
                     int& luck,
                     float& payoutModifier,
                     float& taxModifier,
                     std::vector<std::string>& symbolSet,
                     int& difficultyLevel) {
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "No saved game found at " << filename << std::endl;
        return false;
    }

    // Load basic stats
    inFile >> balance
           >> winStreak
           >> loops
           >> luck
           >> payoutModifier
           >> taxModifier
           >> difficultyLevel;

    // Load symbol set
    std::string symbolsLine;
    std::getline(inFile, symbolsLine); // Clear newline
    std::getline(inFile, symbolsLine);
    std::istringstream symbolStream(symbolsLine);
    symbolSet.clear();
    
    std::string symbol;
    while (symbolStream >> symbol) {
        symbolSet.push_back(symbol);
    }

    return true;
}
