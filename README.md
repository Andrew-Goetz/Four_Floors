# Four_Floors

Four Floors is a command-line turn-based RPG written in C.

It is compatible with both Linux-based systems and Windows.

## Windows

NOTE: code currently requires make, so building on Windows may prove difficult.
This game can be run from Microsoft Visual Studio or with any C compiler/IDE. To compile
with clang (what was officially tested for windows), navigate to the downloaded
directory and run: `clang .\main.c -o .\four-floors.exe`. Then, run the .exe with
`.\four-floors.exe`.

## Linux 

Clang can work for Linux as well, but gcc is recommended and is likely already on
your system. Install gcc with package manager of choice; for Ubuntu/Debian this is:
`sudo apt install gcc`. For Manjaro/Arch this is: `sudo pacman -S gcc`.

Once program is downloaded, navigate to the downloaded directory and run: `gcc main.c
-o four-floors`. Then, run the executable with `./four-floors`.

## UNIX

For MacOS, BSD, and other UNIX type operating systems, the code is untested but
probably works. Using gcc, follow the Linux instructions. You should receive an error
message if the program is unable to run.

## Other Notes

Have fun.

## Checklist

- [x] Basic functionality
- [x] Refactor code into separate files
- [ ] Makefile to compile
- [x] Level 0
- [x] Level 1
- [ ] Spells/Items that effect game for multiple turns (ie: buffs)
- [x] Level 2
- [ ] Enemy casts spells
- [ ] Level 3
- [ ] Level 4
- [ ] Level 5
- [ ] Ending
- [ ] Save Files
- [ ] Finishing Touches/Balancing
- [ ] Colors!

## Licensing

Feel free to use this code for any non-profit endeavours as long as the author is
properly accredited. For any other uses for the code, please reach out via email.
