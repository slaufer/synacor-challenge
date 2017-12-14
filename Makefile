CC=gcc
OBJECTS=vm.o inst.o program.o const.o
BINARY=scvm

%.o: %.c 
	$(CC) -c -o $@ $< ${CFLAGS}

default: $(OBJECTS)
	$(CC) -o $(BINARY) $(OBJECTS) ${CFLAGS}

clean:
	rm -f $(BINARY) $(OBJECTS)
