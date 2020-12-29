PROJECT = four-floors

CC = gcc

$(PROJECT):	main.o
	$(CC) main.o -o $@

main.o: main.c

clean:
	rm -f *.o $(PROJECT)
