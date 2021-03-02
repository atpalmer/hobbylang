P=calc
TESTP=test
OBJECTS=lexer.o parser.o
MAIN_OBJECTS=main.o
TEST_OBJECTS=test.o
CFLAGS=-Wall -Wextra -pedantic -g

$P: $(OBJECTS) $(MAIN_OBJECTS)
	$(CC) -o $P $(OBJECTS) $(MAIN_OBJECTS)

test: clean $(OBJECTS) $(TEST_OBJECTS)
	$(CC) -o $(TESTP) $(OBJECTS) $(TEST_OBJECTS)
	"./$(TESTP)"

.PHONY: clean all run install

clean:
	-rm $(OBJECTS) $(TEST_OBJECTS) $(TESTP) $P

all: clean $P test

run: all
	"./$P"

install: all
	mv ./calc /usr/local/bin
