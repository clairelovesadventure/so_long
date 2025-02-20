
# So Long
A simple 2D game project using MiniLibX graphics library.

## Description
This project is a small 2D game where players need to collect all items and reach the exit to win. It's built using the MiniLibX library to handle graphics, window management, and events.

## Requirements
- macOS or Linux
- GCC compiler
- Make
- MiniLibX library

## Installation

### 1. Clone the Repository
```bash
git clone <repository_url>
cd so_long
```

### 2. Install Dependencies
#### On macOS:
```bash
brew install xquartz
```

#### On Linux:
```bash
sudo apt-get update
sudo apt-get install gcc make xorg libxext-dev libbsd-dev
```

### 3. Build the Project
```bash
make
```

## Running the Game
```bash
./so_long maps/map1.ber
```

## Controls
- Movement: W(up) A(left) S(down) D(right)
- Exit: ESC or click window close button

## Map Format
Maps must have a .ber extension and contain the following elements:
- 0: Empty space
- 1: Wall
- C: Collectible
- E: Exit
- P: Player starting position

Example map:
```
1111111111111
10010000000C1
1000011111001
1P0011E000001
1111111111111
```

## Map Rules
- Maps must be rectangular
- Maps must be surrounded by walls
- Must have exactly one player starting position (P)
- Must have exactly one exit (E)
- Must have at least one collectible (C)
- Must have a valid path to collect all items and reach the exit

## Error Handling
The program will display "Error" followed by a specific error message if any map configuration is invalid.

## Cleaning Up
```bash
make clean    # Remove object files
make fclean   # Remove object files and executable
make re       # Recompile the entire project
