#include <stdio.h>
#include "syswrap.h"
#include "interpreter.h"

int interactive(void) {
    Interpreter *interp = interpreter_new();

    for(;;) {
        printf(">>> ");
        char *line = NULL;
        size_t linecap = 0;
        size_t bytes_read = getline(&line, &linecap, stdin);
        if(line[0] == '\n')
            goto loopend;

        FILE *stream = fmemopen(line, bytes_read, "r");
        double value = 0.0;
        if(!interpreter_parse_line(interp, stream, &value))
            exit(-1);
        printf("%f\n", value);
        fclose(stream);

loopend:
        if(line)
            free(line);
    }

    printf("\n");
    interpreter_free(interp);
    return 0;
}

int main(int argc, const char **argv) {
    if(argc < 2)
        return interactive();

    FILE *stream = fopen_or_die(argv[1], "r");

    Interpreter *interp = interpreter_new();

    for(;;) {
        double value;
        if(!interpreter_parse_line(interp, stream, &value))
            break;
        printf("%f\n", value);
    }

    printf("\n");
    interpreter_free(interp);
    fclose(stream);
    return 0;
}
