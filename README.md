# ⚔️ Dungeon of Shadows

A turn-based dungeon crawler for the terminal, written in C++17.
Fight through 5 floors, pick upgrades, and take down the Shadow King.

## Features
- Turn-based combat: attack, heavy strike, potions, defend
- Random enemies that scale with each floor
- Upgrade choice after every victory
- Boss fight on floor 5

## Build & run (Windows, MSYS2 g++)
```bash
g++ -std=c++17 -O2 -Wall -o dungeon main.cpp
./dungeon.exe
```

## What I learned
- Structs and passing by reference
- Random number generation with `<random>`
- Input validation and game loops

## Roadmap
- [ ] Save/load system
- [ ] Items and inventory
- [ ] Coloured terminal output