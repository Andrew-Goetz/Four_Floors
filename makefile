CC = gcc
FLAGS = -Wall -Wextra -g
OBJECT_FILES = actions.o combat.o levels.o spells.o character.o items.o main.o potions.o saves.o utility.o
EXECUTABLE = four_floors

all:	$(OBJECT_FILES)
		$(CC) $(FLAGS) $(OBJECT_FILES) -o $(EXECUTABLE)
actions.o:	constants.h defs.h actions.c
		$(CC) $(FLAGS) -c actions.c
combat.o:	constants.h defs.h combat.c
		$(CC) $(FLAGS) -c combat.c
levels.o:	constants.h defs.h levels.c
		$(CC) $(FLAGS) -c levels.c
spells.o:	constants.h defs.h spells.c
		$(CC) $(FLAGS) -c spells.c
character.o:	constants.h defs.h character.c
		$(CC) $(FLAGS) -c character.c
items.o:	constants.h defs.h items.c
		$(CC) $(FLAGS) -c items.c
main.o:	constants.h defs.h main.c
		$(CC) $(FLAGS) -c main.c
potions.o:	constants.h defs.h potions.c
		$(CC) $(FLAGS) -c potions.c
saves.o:	constants.h defs.h saves.c
		$(CC) $(FLAGS) -c saves.c
utility.o:	constants.h defs.h utility.c
		$(CC) $(FLAGS) -c utility.c
clean:
		rm -f *.o $(EXECUTABLE)
