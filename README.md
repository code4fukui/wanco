# Wanco (わんこ大喧嘩)

> 日本語のREADMEはこちらです: [README.ja.md](README.ja.md)

A simple terminal-based tower defense game written in C, where you deploy dog-like "wanco" units to attack an enemy base. This project serves as a foundation for building games that run in a standard terminal.

```
 [##########]

    _
   / \
  / _ \
  | x |
  | = |
  | _ |
  |! !|
  ||_||


                                  /----\
                                 | O  O |
                                 |  ==  |
                                  \----/
                                                                              _
                                                                             / \
                                                                            / _ \
                                                                            | + |
                                                                            | = |
                                                                            | _ |
                                                                            |! !|
                                                                            ||_||
 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^




                                                                ------- =======
                                                                 wanco  |noppo|
                                                                ------- =======
```

## Gameplay

Deploy units from your base on the right to destroy the enemy base on the left. Use the selection and production keys to spawn different types of "wanco" units, each with different stats.

### Controls

-   **Tab**: Select unit type
-   **Space**: Deploy selected unit
-   **ESC**: Exit the game

## Getting Started

### Requirements

-   A POSIX-compliant system (e.g., Linux, macOS)
-   GCC or a compatible C compiler

### Installation & Running

Clone the repository, then compile and run the main game file:

```sh
gcc wanco.c && ./a.out
```

## Included Games & Utilities

This repository contains several other small games and tools built with the same library components.

-   **Nyanko Kawakudari (にゃんこかわくだり)**: A simple avoidance game.
    ```sh
    gcc kawakudari.c && ./a.out
    ```

-   **AtoZ Hayauchi Game (AtoZはやうちゲーム)**: A typing speed game.
    ```sh
    gcc a2z.c && ./a.out
    ```

-   **Keycode Check Tool (キーコードチェックツール)**: A utility to display keycodes.
    ```sh
    gcc keycheck.c && ./a.out
    ```

## Project Structure

The project is split into the main game logic and a set of reusable libraries.

### Main Game Files

-   [wanco.c](wanco.c): Main game loop and initialization.
-   [wanco.h](wanco.h): Core game functions and object definitions.
-   [wanco_res.h](wanco_res.h): ASCII art resources for all game entities.

### Library Files

-   [lib/Screen.h](lib/Screen.h): Handles terminal screen buffer and rendering.
-   [lib/Image.h](lib/Image.h): Defines the structure for ASCII art images.
-   [lib/Sprite.h](lib/Sprite.h): Manages sprites, their positions, and images.
-   [lib/Keyboard.h](lib/Keyboard.h): Handles non-blocking keyboard input.
-   [lib/Button.h](lib/Button.h): Implements button UI elements.
-   [lib/ButtonManager.h](lib/ButtonManager.h): Manages a collection of buttons.
-   [lib/Mover.h](lib/Mover.h): Defines logic for sprite movement patterns (e.g., falling).
-   [lib/MoverManager.h](lib/MoverManager.h): Manages all active movers in the game.
