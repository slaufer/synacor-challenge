CC=gcc
OBJECTS=vm.o inst.o program.o const.o instdebug.o
BINARY=scvm
DASM_OBJECTS=dasm.o program.o const.o
DASM_BINARY=scdasm

%.o: %.c 
	$(CC) -c -o $@ $< ${CFLAGS}

default: $(OBJECTS) dasm
	$(CC) -o $(BINARY) $(OBJECTS) ${CFLAGS}

dasm: $(DASM_OBJECTS)
	$(CC) -o $(DASM_BINARY) $(DASM_OBJECTS) ${CFLAGS}

clean:
	rm -f $(BINARY) $(OBJECTS) $(DASM_BINARY) $(DASM_OBJECTS)
