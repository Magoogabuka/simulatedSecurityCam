OPTS = -ansi -Wall

main: main.o
	gcc -o main main.o
        
ass2.o: main.c utils.h
	gcc -c main.c $(OPTS)
        
clean:
	rm *.o
