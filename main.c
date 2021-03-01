#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

struct line {
    size_t size;
    char *buff;
};

int main(void) {
    struct line line = {
        .size = 128,
        .buff = malloc(sizeof(char) * 128),
    };
    printf(">>> ");
    getline(&line.buff, &line.size, stdin);

    Parser *parser = parser_new(line.buff);
    double value = parser_next_line(parser);
    printf("%f\n", value);
    parser_free(parser);

    free(line.buff);
}
