# Four_Floors
Four Floors is a command-line turn-based RPG written in C.

It is compatible with both Linux-based systems and Windows.

## Windows
This game can be run from Microsoft Visual Studio or with any C compiler.
To compile with clang (what was officially tested for windows), navigate to the downloaded directory and run:
`clang .\main.c -o .\four-floors.exe`.
Then, run the .exe with `.\four-floors.exe`, or just open it from the file explorer.

## Linux
Clang can work for Linux as well, but gcc is also tested and is likely already on your system.
Install gcc with package manager of choice; for Ubuntu/Debian this is:
`sudo apt install gcc`. For Manjaro/Arch this is: `sudo pacman -S gcc`.

Once program is downloaded, navigate to the downloaded directory and run:
`gcc main.c -o four-floors`.
Then, run the executable with `./four-floors`.

## Checklist

- [x] Basic functionality
- [ ] Refactor code into separate files
- [x] Level 0
- [x] Level 1
- [ ] Spells/Items that effect game for multiple turns (ie: buffs)
- [x] Level 2
- [ ] Level 3
- [ ] Level 4
- [ ] Level 5
- [ ] Ending
- [ ] Save Files
- [ ] Finishing Touches/Balancing
