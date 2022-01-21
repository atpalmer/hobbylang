P=hobby
TESTP=test
OBJECTS=token.o parser.o ast.o eval.o mapobj.o syswrap.o doubleobj.o
MAIN_OBJECTS=main.o
TEST_OBJECTS=test_eval.o test_lexer.o test_parser.o
CFLAGS=-Wall -Wextra -pedantic -g

$P: $(OBJECTS) $(MAIN_OBJECTS)
	$(CC) -o $P $(OBJECTS) $(MAIN_OBJECTS)

test_%: $(OBJECTS) test_%.o
	$(CC) -o $(TESTP) $?
	"./$(TESTP)"
	rm "./$(TESTP)"

test: test_lexer test_parser test_eval

.PHONY: clean run

clean:
	-rm $(OBJECTS) $(MAIN_OBJECTS) $(TEST_OBJECTS) $(TESTP) $P

run: clean $P test
	"./$P" "samples/sample1.hb"
	"./$P" "samples/sample2.hb"
	"./$P" "samples/sample3.hb"

interactive: clean $P
	"./$P"
