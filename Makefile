OBJ = array.o myShell.o 
DEPS = array.h
CC = gcc
CFLAGS = -std=gnu99 -Wpedantic

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -g -c -o $ $@ $<

myShell: $(OBJ)
	$(CC) $(CFLAGS) -o $ $@ $^

make: myShell 

clean:
	rm -f $(OBJ) myShell