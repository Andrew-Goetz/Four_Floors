CC:= gcc
CFLAGS:= -O3 -g #-g -Wall -pedantic -O3
TARGET:= four_floors
OBJECT_FILES:= src/utility.o src/character.o src/potions.o src/items.o src/spells.o src/actions.o src/combat.o src/levels.o

all:			src/main.o $(OBJECT_FILES)
		$(CC) src/main.o $(OBJECT_FILES) -o $(TARGET)

main.o:			src/defs.h $(OBJECT_FILES)

utility.o:		src/utility.c defs.h constants.h

character.o:	src/character.c src/defs.h src/constants.h src/utility.o

potions.o:		src/potions.c src/defs.h src/constants.h src/character.o src/utility.o

items.o:		src/items.c src/defs.h src/constants.h src/character.o src/utility.o

spells.o:		src/spells.c src/defs.h src/constants.h src/character.o src/utility.o

actions.o:		src/actions.c src/defs.h src/constants.h src/character.o src/utility.o src/potions.o src/items.o src/spells.o

combat.o:		src/combat.c src/defs.h src/constants.h src/character.o src/utility.o src/potions.o src/items.o src/spells.o src/actions.o

levels.o:		src/levels.c src/defs.h src/constants.h src/character.o src/utility.o src/potions.o src/items.o src/spells.o src/actions.o src/combat.o

.PHONY:		clean
clean:
	rm -f src/*.o $(TARGET)
