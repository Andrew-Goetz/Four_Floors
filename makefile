CC = gcc
OBJECT_FILES = main.o
EXECUTABLE = four_floors

all:	$(OBJECT_FILES)
		$(CC) $(OBJECT_FILES) -o $(EXECUTABLE)

main.o:	

clean:
		rm -f *.o $(EXECUTABLE)
