PROJECT = four-floors

CC = clang

$(PROJECT):	main.o
	$(CC) main.o -o $@

main.o: main.c

clean:
	rm -f *.o $(PROJECT)
