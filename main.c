#include <stdio.h>
#include "syswrap.h"
#include "interpreter.h"
#include "object.h"

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
        Object *value = interpreter_parse_line(interp, stream);
        if(!value)
            exit(-1);
        Object_println(value);
        Object_destroy(value);
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
        Object *value = interpreter_parse_line(interp, stream);
        if(!value)
            break;
        Object_println(value);
        Object_destroy(value);
    }

    printf("\n");
    interpreter_free(interp);
    fclose(stream);
    return 0;
}
