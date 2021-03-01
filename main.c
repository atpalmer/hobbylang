#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

struct line {
    size_t size;
    char *buff;
};

struct line *line_new(void) {
    struct line *new = malloc(sizeof *new);
    new->size = 128;
    new->buff = malloc(sizeof *new->buff * new->size);
    return new;
}

void line_free(struct line *this) {
    free(this->buff);
    free(this);
}

void line_read(struct line *this, FILE *fp) {
    getline(&this->buff, &this->size, fp);
}

int main(void) {
    struct line *line = line_new();
    printf(">>> ");
    line_read(line, stdin);

    Parser *parser = parser_new(line->buff);
    double value = parser_next_line(parser);
    printf("%f\n", value);
    parser_free(parser);

    line_free(line);
}
