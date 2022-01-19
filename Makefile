P=calc
TESTP=test
OBJECTS=token.o parser.o ast.o interpreter.o mapobj.o syswrap.o doubleobj.o
MAIN_OBJECTS=main.o
TEST_OBJECTS=test_interpreter.o test_lexer.o
CFLAGS=-Wall -Wextra -pedantic -g

$P: $(OBJECTS) $(MAIN_OBJECTS)
	$(CC) -o $P $(OBJECTS) $(MAIN_OBJECTS)

test_%: $(OBJECTS) test_%.o
	$(CC) -o $(TESTP) $?
	"./$(TESTP)"
	rm "./$(TESTP)"

test: test_lexer test_interpreter

.PHONY: clean all run install

clean:
	-rm $(OBJECTS) $(MAIN_OBJECTS) $(TEST_OBJECTS) $(TESTP) $P

all: clean $P test

run: all
	"./$P" "samples/sample3.calc"

install: all
	mv ./calc /usr/local/bin
