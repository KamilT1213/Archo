# Archo Demo

Demo of digging and extraction game set in space, Hyper casual, rpg-like.

---

## Requirements

- Visual Studio or VS Code
- CMake versions 3.10 - 3.29
- System that supports OpenGL version 4.6 or better
- Mouse for input

---

## How to Setup
&nbsp;

1. Clone Repository: [https://github.com/KamilT1213/Archo.git](https://github.com/KamilT1213/Archo.git)

&nbsp;
### For Visual Studio

2. Execute: "runSetup.exe"

&nbsp;

3. Locate .sln in build folder

&nbsp;

4. Build and run

&nbsp;
### For VS Code

2. Install cmake tools extension

&nbsp;

3. Right click CMakeLists in directory and build all projects

&nbsp;

4. Place assets folder in the same directory as the game's exe and then run

### Finally

The output of both need to have the assets folder placed in the same directory as the exe if the game is being run stand-alone

---

## Project Layout

The project is broken up into two main projects.

1. Game/Application itself that contains code directly related to the game
&nbsp;
2. Engine/DemonRenderer, heavily modified renderer that I used, optimized and upgraded.

Shader Programs are stored in the asset folder and are compiled during runtime.

