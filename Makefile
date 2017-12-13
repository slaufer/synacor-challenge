CC=gcc
CFLAGS=
OBJECTS=vm.o inst.o

%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS)

default: $(OBJECTS)
	$(CC) -o vm $(OBJECTS) $(CFLAGS)
	
