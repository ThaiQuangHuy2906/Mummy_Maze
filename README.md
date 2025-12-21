# 🎮 Mummy Maze Clone

[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![SFML](https://img.shields.io/badge/SFML-2.6.2-green.svg)](https://www.sfml-dev.org/)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

> A modern recreation of the classic PopCap puzzle game "Mummy Maze Adventure" built with C++ and SFML.

**Course:** Introduction to Computer Science
**Class:** 25C11 - Faculty of Information Technology - HCMUS
**Group:** 11
**Academic Year:** 2025-2026

---

## 👥 Team Members

| Student ID | Name | Role |
|------------|------|------|
| 24127177 | Thái Quang Huy | Developer |
| 24127205 | Nguyễn Văn Minh | Developer |
| 24127249 | Mai Phương Thùy | Developer |
| 25127444 | Lê Kim Phúc Nhật | Developer |

---

## 📖 About The Project

Mummy Maze Clone is an educational recreation of PopCap's classic puzzle game. Navigate your Explorer through intricate mazes while avoiding deadly enemies, collecting keys, and reaching the exit stairs!

### ✨ Key Features

- 🎯 **45+ Hand-Crafted Levels** across 3 difficulty modes
- 🤖 **Smart AI System** with BFS pathfinding (Hard mode)
- ⭐ **Star Rating System** based on performance
- 💾 **Save/Load & Undo** functionality
- 🎵 **Sound Effects & Background Music**
- 🎨 **Beautiful Sprite-Based Graphics** (SFML)
- 🕹️ **Multiple Control Options** (Keyboard + Mouse)

### 🎮 Game Elements

| Element | Description |
|---------|-------------|
| 🧍 **Explorer** | YOU! Navigate to the stairs to win |
| 👻 **Mummy** | Moves 2 steps per turn, highly dangerous |
| 🦂 **Scorpion** | Moves 1 step per turn, steady pursuer |
| 🧱 **Wall** | Impassable obstacle |
| ❌ **Trap** | Instant death if stepped on |
| 🚪 **Gate** | Opens/closes with matching Key |
| 🔑 **Key** | Collect to toggle Gates |
| 🪜 **Stairs** | The exit! Reach this to win |

---

## 🚀 Getting Started

### Prerequisites

- **Windows 10/11** (64-bit)
- **Visual Studio Code** (latest version)
- **MinGW-w64** (GCC 11.0+)
- **C/C++ Extension** for VS Code

> **Note:** SFML 2.6.2 is already included in the `/lib` directory!

### Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/yourusername/mummy-maze-clone.git
   cd mummy-maze-clone
   ```

2. **Install MinGW-w64**
   - Download from [MSYS2](https://www.msys2.org/)
   - Install to `C:\msys64\mingw64` (recommended)
   - Add to PATH: `C:\msys64\mingw64\bin`

3. **Open in VS Code**
   ```bash
   code .
   ```

4. **Build the project**
   - Press `Ctrl + Shift + B`
   - Or: `Terminal` → `Run Build Task` → `Build C++/SFML Project`

5. **Run the game**
   - Press `F5` (Debug mode)
   - Or: Press `Ctrl + F5` (Run without debugging)

---

## 🎯 How To Play

### Controls

| Input | Action |
|-------|--------|
| **Arrow Keys** | Move Explorer (↑ ↓ ← →) |
| **Mouse Click** | Click adjacent cell to move |
| **Spacebar** | Skip turn (wait in place) |
| **Undo Button** | Undo last move |
| **ESC** | Pause/Resume game |

### Difficulty Modes

- 🟢 **Easy (6x6)** - Enemies move randomly
- 🟡 **Medium (8x8)** - Enemies use heuristic tracking
- 🔴 **Hard (10x10)** - Enemies use BFS pathfinding

### Star Rating

- ⭐⭐⭐ **Excellent** - Completed in optimal moves
- ⭐⭐ **Good** - Completed with some extra moves
- ⭐ **Clear** - Level completed

---

## 🏗️ Project Structure

```
mummy-maze-clone/
├── .vscode/              # VS Code configuration
├── assets/               # Game assets
│   ├── audio/           # Sound effects & music
│   ├── fonts/           # TTF font files
│   └── images/          # Sprites & textures
├── build/               # Build output (executables)
├── include/             # Header files (.h)
├── levels/              # Level data
│   ├── agents/          # Enemy spawn configs
│   └── maze/            # Maze layouts
├── lib/                 # External libraries
│   └── SFML-2.6.2/     # SFML library
├── src/                 # Source files (.cpp)
├── .gitignore          # Git ignore rules
├── LICENSE             # MIT License
└── README.md           # This file
```

---

## 🛠️ Technical Details

### Built With

- **Language:** C++17
- **Graphics Library:** SFML 2.6.2
- **Compiler:** MinGW-w64 GCC
- **IDE:** Visual Studio Code

### Design Patterns

- **State Pattern** - Game state management
- **Singleton Pattern** - AssetManager, AudioManager
- **Component Pattern** - Character system

### Algorithms

- **Breadth-First Search (BFS)** - Hard mode AI pathfinding
- **Heuristic Movement** - Medium mode AI
- **Manhattan Distance** - Distance calculation

---

## 📸 Screenshots

<img width="743" height="759" alt="image" src="https://github.com/user-attachments/assets/51433f04-f15d-4db1-9783-1dc43ed03be4" />

---

## 🎓 Educational Objectives

This project demonstrates:

- ✅ Object-Oriented Programming (OOP)
- ✅ Data Structures (Vector, Queue, Stack, Map)
- ✅ Algorithm Implementation (BFS, Heuristics)
- ✅ Game Development Fundamentals
- ✅ File I/O Operations
- ✅ Memory Management (Smart Pointers)

---

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

**Important:** This is an educational recreation of "Mummy Maze Adventure" by PopCap Games. All credit for the original game concept goes to PopCap Games. This is a non-commercial, educational project.

---

## 🙏 Acknowledgments

- **PopCap Games** - Original game concept
- **HCMUS** - Educational support
- **Course Instructors** - Guidance and feedback
- **SFML Community** - Excellent documentation

---

## 📧 Contact

For questions or feedback, please contact any team member through the course portal.

---

<div align="center">

**Made with ❤️ by Group 11**

*Faculty of Information Technology - HCMUS*

</div>
