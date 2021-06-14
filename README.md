# Four_Floors
Four Floors is a command-line turnbased rpg written in C.

It is compatible with both linux-based systems and Windows.

## Windows
This game can be run from Microsoft Visual Studio or with any C compiler.
To compile with clang (what was officially tested), navigate to the downloaded directory and run:
`clang .\main.c -o .\four-floors.exe`.
Then, run the .exe with `.\four-floors.exe` or just open it from file explorer.

## Linux
Clang can work for linux as well, but gcc is also tested and is likely already on your system.
Install gcc with package manager of choice; for Ubuntu/Debian this is:
`sudo apt install gcc`, and for Manjaro/Arch this is `sudo pacman -S gcc`.
Once installed, navigate to the downloaded directory and run:
`gcc main.c -o four-floors-linux`.
Then, run the executable with `./four-floors-linux`.
