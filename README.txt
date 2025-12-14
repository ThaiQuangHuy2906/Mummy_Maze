################################################################################
#                           MUMMY MAZE CLONE                                   #
#                                                                              #
#  Course: Introduction to Computer Science                                    #
#  Class: 25C11 - Faculty of Information Technology - HCMUS                    #
#  Group ID: 11                                                                #
#  Academic Year: 2024-2025                                                    #
################################################################################

================================================================================
TABLE OF CONTENTS
================================================================================
I.    GROUP MEMBERS
II.   PROJECT OVERVIEW
III.  SYSTEM REQUIREMENTS
IV.   INSTALLATION & BUILD INSTRUCTIONS
V.    HOW TO PLAY
VI.   GAME FEATURES
VII.  PROJECT STRUCTURE
VIII. TECHNICAL DETAILS
IX.   TROUBLESHOOTING
X.    CREDITS & REFERENCES

================================================================================
I. GROUP MEMBERS
================================================================================
  Student ID      Name                      Role
  ----------      ----                      ----
  24127177        Thái Quang Huy           Developer
  24127205        Nguyễn Văn Minh          Developer
  24127249        Mai Phương Thùy          Developer
  25127444        Lê Kim Phúc Nhật         Developer

================================================================================
II. PROJECT OVERVIEW
================================================================================
Mummy Maze Clone is a modern recreation of the classic PopCap puzzle game
"Mummy Maze Adventure". Built with C++ and SFML (Simple and Fast Multimedia 
Library), this game challenges players to navigate an Explorer through 
increasingly complex mazes while avoiding cunning enemies.

KEY HIGHLIGHTS:
  • Classic puzzle gameplay with modern graphics and sound
  • Multiple difficulty levels with intelligent AI pathfinding
  • 45+ hand-crafted levels across three maze sizes
  • Star rating system for replayability
  • Full save/load and undo functionality

EDUCATIONAL OBJECTIVES:
This project demonstrates proficiency in:
  - Object-Oriented Programming (OOP) principles and design patterns
  - Game development fundamentals (game loop, state management, rendering)
  - Artificial Intelligence (BFS pathfinding, heuristic algorithms)
  - Data structures and file I/O operations
  - C++ programming and SFML library integration

================================================================================
III. SYSTEM REQUIREMENTS
================================================================================
REQUIRED SOFTWARE:
  • Operating System: Windows 10/11 (64-bit)
  • IDE: Visual Studio Code (latest version recommended)
  • Compiler: MinGW-w64 (GCC/G++ 11.0 or higher)
  • Extensions: C/C++ Extension Pack (Microsoft)

HARDWARE REQUIREMENTS:
  • Processor: 1 GHz or faster
  • RAM: 512 MB minimum (1 GB recommended)
  • Graphics: DirectX 9 compatible with 256 MB VRAM
  • Storage: 150 MB available space
  • Display: 1024x768 minimum resolution

INCLUDED LIBRARIES:
  • SFML 2.6.2 (pre-configured in /lib directory)
  • All required DLL files (included in /build directory)

================================================================================
IV. INSTALLATION & BUILD INSTRUCTIONS
================================================================================
STEP 1: SETUP DEVELOPMENT ENVIRONMENT
--------------------------------------
1. Install Visual Studio Code from: https://code.visualstudio.com/
2. Install MinGW-w64 (GCC compiler):
   - Download from: https://www.msys2.org/
   - Install to: C:\msys64\mingw64 (recommended)
   - Add to PATH: C:\msys64\mingw64\bin

3. Install VS Code Extensions:
   - Open VS Code
   - Go to Extensions (Ctrl+Shift+X)
   - Search and install: "C/C++" by Microsoft

NOTE: If your MinGW is installed in a different location, update:
      - .vscode/c_cpp_properties.json (line 13-14)
      - .vscode/launch.json (line 15)

STEP 2: OPEN PROJECT
--------------------
1. Extract the project archive to your desired location
2. Open Visual Studio Code
3. File → Open Folder → Select the project root directory
4. Wait for VS Code to load the workspace configuration

STEP 3: BUILD THE PROJECT
--------------------------
METHOD 1 (Keyboard Shortcut):
  - Press: Ctrl + Shift + B

METHOD 2 (Menu):
  - Terminal → Run Build Task → "Build C++/SFML Project"

METHOD 3 (Command Palette):
  - Press: Ctrl + Shift + P
  - Type: "Tasks: Run Build Task"
  - Select: "Build C++/SFML Project"

BUILD OUTPUT:
  ✓ Executable: build/mummymaze.exe
  ✓ Debug symbols included for debugging
  ✓ SFML DLLs already present in build folder

STEP 4: RUN THE GAME
--------------------
METHOD 1 (Debug Mode):
  - Press F5 to start debugging
  - Breakpoints and step-through debugging available

METHOD 2 (Run Without Debugging):
  - Press Ctrl + F5
  - Faster startup, no debugging overhead

METHOD 3 (Direct Execution):
  - Navigate to /build folder
  - Double-click: mummymaze.exe

IMPORTANT: The game automatically loads assets from the workspace root.
           Do not move the executable outside the build folder unless you
           copy the entire project structure.

================================================================================
V. HOW TO PLAY
================================================================================
GAME OBJECTIVE
--------------
Guide your Explorer through the maze to reach the Stairs (Exit) while avoiding
deadly enemies and obstacles. Collect Keys to open Gates and plan your moves
carefully - one wrong step could be your last!

CONTROLS
--------
┌─────────────────────────────────────────────────────────────────────────┐
│ MOVEMENT                                                                │
├─────────────────────────────────────────────────────────────────────────┤
│  Arrow Keys          Move Explorer (Up/Down/Left/Right)                │
│  Mouse Left Click    Click adjacent cell to move                       │
├─────────────────────────────────────────────────────────────────────────┤
│ ACTIONS                                                                 │
├─────────────────────────────────────────────────────────────────────────┤
│  Spacebar           Skip turn (wait in place)                          │
│  Undo Button        Undo last move (top-right UI)                      │
├─────────────────────────────────────────────────────────────────────────┤
│ SYSTEM                                                                  │
├─────────────────────────────────────────────────────────────────────────┤
│  ESC                Pause/Resume game                                   │
│  Mouse              Navigate menus and select options                   │
└─────────────────────────────────────────────────────────────────────────┘

GAME ELEMENTS
-------------
  [P] Explorer       - YOU! Navigate to the stairs to win
  [M] White Mummy    - Moves 2 steps per turn, highly dangerous
  [M] Red Mummy      - Moves 2 steps, more aggressive tracking
  [S] White Scorpion - Moves 1 step per turn, steady pursuer
  [S] Red Scorpion   - Moves 1 step, enhanced AI behavior
  [#] Wall           - Impassable obstacle
  [X] Trap           - Instant death if stepped on
  [G] Gate           - Blocks path, opens with matching Key
  [K] Key            - Collect to open Gates of same color
  [^] Stairs         - The exit! Reach this to complete the level

DIFFICULTY LEVELS
-----------------
  🌟 EASY MODE
     • Enemies move randomly or follow simple patterns
     • Great for learning game mechanics
     • Recommended for first-time players

  ⭐⭐ MEDIUM MODE
     • Enemies prioritize horizontal/vertical tracking
     • Moderate challenge with predictable patterns
     • Good for intermediate players

  ⭐⭐⭐ HARD MODE
     • Enemies use BFS algorithm for optimal pathfinding
     • They ALWAYS find the shortest path to you
     • Ultimate challenge for expert players

STAR RATING SYSTEM
------------------
Complete levels efficiently to earn up to 3 stars:
  ⭐⭐⭐ Excellent - Completed in optimal moves
  ⭐⭐   Good     - Completed with some extra moves
  ⭐     Clear   - Level completed (any move count)

Your star rating is based on how efficiently you solve each puzzle!

TIPS & STRATEGIES
-----------------
  1. Plan ahead - Think several moves in advance
  2. Use corners and walls to trap or avoid enemies
  3. The Spacebar (wait) can be strategic - use it wisely
  4. Collect all Keys before approaching Gates
  5. In Hard mode, enemies are relentless - stay mobile!
  6. Undo is unlimited - don't be afraid to experiment
  7. Some levels have multiple solutions - find the optimal one!

================================================================================
VI. GAME FEATURES
================================================================================
CORE FEATURES
-------------
  ✓ 45+ Hand-Crafted Levels
    - 15 levels each for 6x6, 8x8, and 10x10 maze sizes
    - Progressive difficulty curve
    - Unique puzzle design for each level

  ✓ Multiple Enemy Types
    - White/Red Mummies (fast, 2 steps per turn)
    - White/Red Scorpions (steady, 1 step per turn)
    - Each with distinct AI behavior patterns

  ✓ Interactive Elements
    - Color-coded Keys and Gates system
    - Deadly Traps to avoid
    - Stairs as the goal destination

  ✓ Three Difficulty Modes
    - Easy: Random/simple movement AI
    - Medium: Heuristic-based tracking
    - Hard: BFS optimal pathfinding

  ✓ Polished User Interface
    - Main menu with level selection
    - Pause menu with save/load options
    - In-game HUD showing moves and stars
    - Smooth animations and transitions

  ✓ Game State Management
    - Save progress at any time
    - Load saved games and continue
    - Unlimited undo functionality
    - Persistent progress tracking

ADVANCED FEATURES
-----------------
  ✓ Smart AI Implementation
    - Breadth-First Search (BFS) pathfinding in Hard mode
    - Heuristic movement in Medium mode
    - Optimized for real-time performance

  ✓ Modern Graphics (SFML)
    - Sprite-based rendering instead of ASCII
    - Smooth character animations
    - Visual effects for gates, traps, and stairs
    - Particle effects and transitions

  ✓ Audio System
    - Background music for different game states
    - Sound effects for movements, key collection, etc.
    - Victory/defeat audio cues
    - Volume controls

  ✓ Star Rating & Progression
    - Performance-based scoring system
    - Unlock progression through levels
    - Replay levels to improve ratings

  ✓ Robust Architecture
    - State pattern for game state management
    - Observer pattern for event handling
    - Modular asset management system
    - Clean separation of concerns (MVC-like structure)

================================================================================
VII. PROJECT STRUCTURE
================================================================================
PROJECT ROOT
  │
  ├── .vscode/                    VS Code Configuration
  │   ├── c_cpp_properties.json   C++ IntelliSense config
  │   ├── launch.json             Debugger configuration
  │   └── tasks.json              Build task definition
  │
  ├── assets/                     Game Assets
  │   ├── audio/                  Sound effects and music
  │   ├── fonts/                  TTF font files
  │   └── images/                 Sprites and textures
  │
  ├── build/                      Build Output
  │   ├── mummymaze.exe          Compiled executable
  │   └── *.dll                   SFML runtime libraries
  │
  ├── include/                    Header Files (.h)
  │   ├── Animation.h            Animation system
  │   ├── AppTypes.h             Application-wide types
  │   ├── AssetManager.h         Asset loading/management
  │   ├── AudioManager.h         Sound/music management
  │   ├── Character.h            Player and enemy classes
  │   ├── GameLogic.h            Core game rules and AI
  │   ├── GameLoop.h             Main game loop
  │   ├── GameRenderer.h         Rendering system
  │   ├── GameState.h            State management
  │   ├── GameTypes.h            Game-specific types
  │   ├── GateAnimation.h        Gate animation handler
  │   ├── LevelSelect.h          Level selection screen
  │   ├── MainMenu.h             Main menu interface
  │   ├── StairsSpriteSheet.h    Stairs sprite handler
  │   └── WallSpriteSheet.h      Wall sprite handler
  │
  ├── levels/                     Level Data
  │   ├── agents/                Enemy spawn configurations
  │   │   ├── map6_*.txt         6x6 enemy layouts
  │   │   ├── map8_*.txt         8x8 enemy layouts
  │   │   └── map10_*.txt        10x10 enemy layouts
  │   └── maze/                   Maze tile configurations
  │       ├── map6_*.txt         6x6 maze layouts
  │       ├── map8_*.txt         8x8 maze layouts
  │       └── map10_*.txt        10x10 maze layouts
  │
  ├── lib/                        External Libraries
  │   └── SFML-2.6.2/            SFML library files
  │       ├── bin/               DLL files
  │       ├── include/           Header files
  │       ├── lib/               Static/import libraries
  │       └── doc/               Documentation
  │
  ├── src/                        Source Files (.cpp)
  │   ├── main.cpp               Program entry point
  │   ├── Animation.cpp          Animation implementation
  │   ├── AssetManager.cpp       Asset manager implementation
  │   ├── AudioManager.cpp       Audio manager implementation
  │   ├── Character.cpp          Character implementation
  │   ├── GameLogic.cpp          Game logic implementation
  │   ├── GameLoop.cpp           Game loop implementation
  │   ├── GameRenderer.cpp       Rendering implementation
  │   ├── GameState.cpp          State implementation
  │   ├── GateAnimation.cpp      Gate animation implementation
  │   ├── LevelSelect.cpp        Level select implementation
  │   ├── MainMenu.cpp           Main menu implementation
  │   ├── StairsSpriteSheet.cpp  Stairs sprite implementation
  │   └── WallSpriteSheet.cpp    Wall sprite implementation
  │
  ├── BFS.txt                     BFS algorithm documentation
  ├── README.txt                  This file
  ├── Report.pdf                  Detailed project report
  ├── save_progress.txt           Save game data file
  └── video.txt                   Demo video link

FILE NAMING CONVENTIONS:
  • Level files: map{size}_{number}.txt (e.g., map6_1.txt)
  • Size options: 6 (6x6), 8 (8x8), 10 (10x10)
  • Each level has two files: one in /maze and one in /agents

================================================================================
VIII. TECHNICAL DETAILS
================================================================================
PROGRAMMING LANGUAGE & STANDARDS
---------------------------------
  • Language: C++17
  • Compiler: MinGW-w64 GCC 11.0+
  • Build System: VS Code Tasks with direct G++ compilation

LIBRARIES & FRAMEWORKS
----------------------
  • SFML 2.6.2 (Simple and Fast Multimedia Library)
    - Graphics: Rendering, sprites, textures
    - Window: Window management, input handling
    - Audio: Sound effects and music playback
    - System: Threading, timing utilities

DESIGN PATTERNS IMPLEMENTED
----------------------------
  1. State Pattern
     - GameState, MainMenu, LevelSelect, GameLoop
     - Clean state transitions and management

  2. Singleton Pattern
     - AssetManager: Centralized asset loading
     - AudioManager: Global audio control

  3. Component Pattern
     - Character components (position, movement, type)
     - Animation components

  4. Game Loop Pattern
     - Fixed time step update loop
     - Separate update and render phases

ALGORITHMS & DATA STRUCTURES
-----------------------------
  • Breadth-First Search (BFS)
    - Used in Hard mode for optimal enemy pathfinding
    - Queue-based implementation
    - Path reconstruction for movement

  • Heuristic Movement
    - Manhattan distance calculation
    - Priority-based direction selection
    - Used in Medium mode

  • Data Structures:
    - std::vector: Dynamic arrays for entities, states
    - std::queue: BFS frontier management
    - std::map: Asset storage and retrieval
    - std::stack: Undo history management

CODE ARCHITECTURE
-----------------
The project follows a modular, layered architecture:

  PRESENTATION LAYER
    └── GameRenderer, MainMenu, LevelSelect
        ↓
  APPLICATION LAYER
    └── GameLoop, GameState
        ↓
  LOGIC LAYER
    └── GameLogic, Character, AI Algorithms
        ↓
  DATA LAYER
    └── Level files, Save files
        ↓
  INFRASTRUCTURE LAYER
    └── AssetManager, AudioManager, SFML

KEY CLASSES:
  • GameLoop: Main game state, handles gameplay
  • GameLogic: Core game rules, victory conditions, AI
  • Character: Base class for Explorer and Enemies
  • GameRenderer: Handles all rendering operations
  • AssetManager: Loads and manages textures, fonts
  • AudioManager: Manages sound effects and music

FILE I/O OPERATIONS
-------------------
  • Level Loading:
    - Parse maze layout from /levels/maze/*.txt
    - Parse enemy positions from /levels/agents/*.txt
    - Validation and error handling

  • Save/Load System:
    - Serialize game state to save_progress.txt
    - Include: level, player position, enemy positions, keys, moves
    - Restore complete game state on load

PERFORMANCE OPTIMIZATIONS
--------------------------
  • Asset caching: Load once, reuse throughout game
  • Sprite batching: Minimize draw calls
  • Delta time: Frame-independent movement
  • Efficient pathfinding: Early exit conditions, visited tracking

================================================================================
IX. TROUBLESHOOTING
================================================================================
COMMON ISSUES & SOLUTIONS
-------------------------

ISSUE: Build fails with "g++ not found"
SOLUTION:
  1. Verify MinGW is installed: Open cmd and type "g++ --version"
  2. Add to PATH: C:\msys64\mingw64\bin (or your install path)
  3. Restart VS Code after PATH changes

ISSUE: Missing SFML DLLs when running
SOLUTION:
  • SFML DLLs should be in /build folder
  • Copy from: lib/SFML-2.6.2/bin/*.dll
  • Required: sfml-graphics-2.dll, sfml-window-2.dll, 
              sfml-system-2.dll, sfml-audio-2.dll

ISSUE: Game cannot find assets (textures, sounds)
SOLUTION:
  • Always run from VS Code (F5 or Ctrl+F5)
  • Or run executable from /build folder without moving it
  • Game uses relative paths from workspace root

ISSUE: Compiler errors with "undefined reference"
SOLUTION:
  • Check SFML library paths in tasks.json
  • Ensure -lsfml-graphics, -lsfml-window, etc. are in correct order
  • Rebuild project: Delete /build folder and rebuild

ISSUE: IntelliSense errors (red squiggles) but builds fine
SOLUTION:
  • Update c_cpp_properties.json with correct include paths
  • Reload VS Code window: Ctrl+Shift+P → "Reload Window"

ISSUE: Game crashes on launch
SOLUTION:
  • Verify all DLLs are present in /build
  • Check assets folder is intact
  • Run in debug mode (F5) to see error details

ISSUE: Audio not playing
SOLUTION:
  • Check audio files exist in /assets/audio
  • Verify sfml-audio-2.dll is in /build
  • Check system volume and audio device

NEED MORE HELP?
---------------
  • Review the detailed project report: Report.pdf
  • Watch the demo video: See link in video.txt
  • Check BFS.txt for algorithm documentation
  • Contact group members (see Section I for details)

================================================================================
X. CREDITS & REFERENCES
================================================================================
ORIGINAL GAME
-------------
  "Mummy Maze Adventure" by PopCap Games
  • This project is an educational recreation for academic purposes only
  • All credit for original game design goes to PopCap Games
  • This is a non-commercial, educational project

DEVELOPMENT TEAM
----------------
  Group 11 - Class 25C11
  Faculty of Information Technology
  Ho Chi Minh City University of Science (HCMUS)

  Under the guidance of course instructors for:
  "Introduction to Computer Science" course

LIBRARIES & TOOLS
-----------------
  • SFML 2.6.2
    Website: https://www.sfml-dev.org/
    License: zlib/png license

  • MinGW-w64 GCC Compiler
    Website: https://www.mingw-w64.org/

  • Visual Studio Code
    Website: https://code.visualstudio.com/

LEARNING RESOURCES
------------------
  • C++ documentation: https://cppreference.com/
  • SFML tutorials: https://www.sfml-dev.org/tutorials/
  • Game Programming Patterns: http://gameprogrammingpatterns.com/

ASSET SOURCES
-------------
  • Sprites: Custom created / Modified from PopCap original style
  • Fonts: Free for educational use
  • Sound Effects: Free/Royalty-free sources
  (Detailed attribution in Report.pdf)

SPECIAL THANKS
--------------
  • Course instructors for guidance and support
  • HCMUS for providing educational resources
  • PopCap Games for the original game concept
  • SFML community for excellent documentation

================================================================================
PROJECT INFORMATION
================================================================================
  Version: 1.0
  Last Updated: December 2024
  Academic Year: 2024-2025
  Course: Introduction to Computer Science
  Class: 25C11
  
  For more information, please refer to Report.pdf

================================================================================
                            END OF DOCUMENTATION
================================================================================