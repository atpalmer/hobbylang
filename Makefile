P=calc
OBJECTS=main.o lexer.o parser.o
CFLAGS=-Wall -Wextra -pedantic -g

$P: $(OBJECTS)
	$(CC) -o $P $(OBJECTS)

.PHONY: clean all run

clean:
	rm $(OBJECTS) $P

all: clean $P

run: all
	"./$P"
