#include <stdio.h>
#include "syswrap.h"
#include "interpreter.h"

struct line {
    size_t size;
    char *buff;
};

struct line *line_new(void) {
    struct line *new = malloc_or_die(sizeof *new);
    new->size = 128;
    new->buff = malloc_or_die(sizeof *new->buff * new->size);
    return new;
}

void line_free(struct line *this) {
    free(this->buff);
    free(this);
}

int line_read(struct line *this, FILE *fp) {
    return getline(&this->buff, &this->size, fp);
}

int main(int argc, const char **argv) {
    FILE *fp = argc < 2 ? stdin : fopen_or_die(argv[1], "r");

    struct line *line = line_new();
    Interpreter *interp = interpreter_new("");

    for(;;) {
        if(fp == stdin)
            printf(">>> ");
        if(line_read(line, fp) < 0)
            break;
        if(*line->buff == '\n')
            continue;
        interpreter_set_buff(interp, line->buff);
        double value = interpreter_parse_line(interp);
        printf("%f\n", value);
    }

    printf("\n");
    line_free(line);
    interpreter_free(interp);
    if(fp != stdin)
        fclose(fp);
    return 0;
}
