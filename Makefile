P=calc
OBJECTS=main.o lexer.o parser.o
CFLAGS=-Wall -Wextra -pedantic -g

$P: $(OBJECTS)
	$(CC) -o $P $(OBJECTS)

.PHONY: clean all

clean:
	rm $(OBJECTS) $P

all: clean $P
