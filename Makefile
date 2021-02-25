P=calc
OBJECTS=main.o lexer.o parser.o

$P: $(OBJECTS)
	$(CC) -o $P $(OBJECTS)


clean:
	rm $(OBJECTS) $P
