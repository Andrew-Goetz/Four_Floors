# Four_Floors

Four Floors is a command-line turn-based RPG written in C.

It is compatible with both Linux-based systems and Windows.

## Linux 

Install gcc and make with package manager of choice. For Debian-based distributions
this is: `apt install gcc make`. For Arch based distributions this is: `pacman -S gcc
make`. Run with `sudo` if needed.

Once done, open a terminal and type `make` in the downloaded git directory to compile
the program. Then, type ./four_floors to run the game.

## Windows

The game does support Windows (clang was tested with an older build of the game), but
there is currently not an easy way to build it (too lazy to learn cmake for now). It
is probably easiest to install Windows Subsystem for Linux to get it to run for now,
or use some other method of installing gcc and make on Windows.

## Other

For MacOS, BSD, and other Unix type operating systems, the code is untested but
probably still works. After installing gcc and make, follow the Linux instructions.
You should receive an error message if the program is unable to run.

## Other Notes

Have fun.

## Checklist

- [x] Basic functionality
- [x] Refactor code into separate files
- [x] Makefile to compile
- [ ] Makefile to compile on windows (with cmake probably)
- [x] Level 0
- [x] Level 1
- [x] Spells/Items that effect game for multiple turns (status effects)
- [x] Level 2
- [ ] Enemy casts spells
- [ ] Level 3
- [ ] Buffs (defense increase and attack/health increase effects)
- [ ] Level 4
- [ ] Enemies have different attack types, chosen at random
- [ ] Level 5
- [ ] Ending
- [ ] Save Files
- [ ] Finishing Touches/Testing/Balancing
- [x] Colors!

## Licensing

Feel free to use this code for any non-profit endeavours as long as the author is
properly accredited. For any other uses for the code, please reach out via email.
