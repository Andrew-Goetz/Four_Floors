CC:= gcc
CFLAGS:= -g -Wall -pedantic -O3
TARGET:= four_floors
OBJECT_FILES:= utility.o character.o potions.o items.o spells.o actions.o combat.o levels.o

all:			main.o $(OBJECT_FILES)
		$(CC) main.o $(OBJECT_FILES) -o $(TARGET)

main.o:			$(OBJECT_FILES)

utility.o:		utility.c defs.h constants.h

character.o:	character.c defs.h constants.h utility.o

potions.o:		potions.c defs.h constants.h character.o utility.o

items.o:		items.c defs.h constants.h character.o utility.o

spells.o:		spells.c defs.h constants.h character.o utility.o

actions.o:		actions.c defs.h constants.h character.o utility.o potions.o items.o spells.o

combat.o:		combat.c defs.h constants.h character.o utility.o potions.o items.o spells.o actions.o

levels.o:		levels.c defs.h constants.h character.o utility.o potions.o items.o spells.o actions.o combat.o

.PHONY:		clean
clean:
	rm -f *.o $(TARGET)
