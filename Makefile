OBJ = myShell.o 
CC = gcc
CFLAGS = -std=c99 -Wall -pedantic

%.o: %.c
	$(CC) $(CFLAGS) -c -o $ $@ $<

myShell: $(OBJ)
	$(CC) $(CFLAGS) -o $ $@ $^

make: myShell 

clean:
	rm -f $(OBJ) myShell