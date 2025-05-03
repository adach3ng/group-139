# group-139
# Slot Machine Game 🎰

## Team Members
Ke Wong 3036389634
Ke Wong. 3036389634
Chan Chun Hin, 3036327442
Cheng Jia Xi 3036452340
Zhang Xiaowei 3036389842

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
- **3 Levels**: Easy/Medium/Hard with different:
  - Starting balances ($200/$150/$120)
  - Payout modifiers (1.2x/1.0x/0.8x)
  - Symbol probability distributions

### 💰 Betting System
- Dynamic multiplier based on bet amount:
  - 10-40: 1.0x-1.5x
  - 41-80: 1.5x-2.0x
  - >80: 2.0x
- Minimum bet enforcement

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
1. **Clone Repository**
   ```bash
   git clone https://github.com/yourusername/slot-machine-game.git
   cd slot-machine-game

