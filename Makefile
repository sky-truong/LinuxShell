OBJ = myShell.o 
CC = gcc
CFLAGS = -std=gnu99 -Wpedantic

%.o: %.c
	$(CC) $(CFLAGS) -c -o $ $@ $<

myShell: $(OBJ)
	$(CC) $(CFLAGS) -o $ $@ $^

make: myShell 

clean:
	rm -f $(OBJ) myShell