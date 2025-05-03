# group-139
# Slot Machine Game 🎰

## Team Members
Ke Wong 3036389634
Chan Chun Hin, 3036327442
Cheng Jia Xi 3036452340
Zhang Xiaowei 3036389842
CHEONG LokIan 3036445880
Qiu Anchenxi 3036094461
---

## Application Description
A terminal-based slot machine game with dynamic betting systems, difficulty levels, and upgradable symbols. Players can:
- Choose difficulty levels affecting starting balance and odds
- Place bets and spin reels to win combinations
- Upgrade symbols to influence probabilities
- Experience random tax events and luck-based mechanics
- Aim for the Jackpot (7️⃣7️⃣7️⃣) worth $1,000,000!

---

## Key Features
### 🎚️ Difficulty System
| Difficulty | Balance | Payout Mod | Tax Mod | Symbol Distribution          |
|------------|---------|------------|---------|------------------------------|
| **Easy**   | $200    | 1.2x       | 0.8x    | Balanced odds                |
| **Medium** | $150    | 1.0x       | 1.0x    | Reduced rare symbols         |
| **Hard**   | $120    | 0.8x       | 1.1x    | Chaotic 0-4 copies per symbol|

### 💰 Betting System
#### Multiplier Logic
```c++
// Core calculation in bet_system.cpp
if (amount <= 40) {
    multiplier = 1.0f + ((amount - 10) / 60.0f);  // 10→1.0x, 40→1.5x
} else if (amount <= 80) {
    multiplier = 1.5f + ((amount - 40) / 80.0f);  // 40→1.5x, 80→2.0x
} else {
    multiplier = 2.0f;  // Flat 2.0x for >80
}

// Final payout formula
payout = base_win × multiplier × difficulty_payout_mod
Example:
1.5 + (50-40)/80 = 1.625x × Medium (1.0x) → 1.625x total multiplier

### 🎰 Game Mechanics
- 7 Unique symbols with 20+ winning combinations
- Progressive tax system (10% + scaling)
- Luck-based reroll system
- Symbol upgrades ($20 per upgrade):
  - Add copies of symbols to influence odds
  - Boost payout multipliers
  - Increase luck stat

### 🕹️ UX Features
- Animated spinning effects
- ASCII art interface
- Randomized failure/success messages
- Adjustable speed control

---

## Technical Specifications
### Non-Standard Libraries
- **C++ Standard Libraries Used**:
  - `<chrono>`: For timing delays
  - `<thread>`: For animation pacing
  - `<vector>/<map>`: Data structures

---

## Compilation & Execution
### Requirements
- C++11+ compiler
- macOS/Linux terminal

### Step-by-Step Guide
1. **Environment Setup**
    # Linux
    sudo apt update && sudo apt install build-essential git
    
    # macOS
    brew install git  # Requires Homebrew (https://brew.sh)

2. **Clone & Enter Repository**
    git clone https://github.com/adach3ng/group-139.git
    cd slot-machine-game

3. **Compile program**
    # Universal command with C++11 and threading support
    g++ -std=c++11 *.cpp -o slot-machine -pthread
    
    # Troubleshooting
    g++-11 -std=c++11 *.cpp -o slot-machine -pthread  # If default compiler fails

4. **Launch game**
    ./slot-machine

