#include <stdio.h>
#include "syswrap.h"
#include "interpreter.h"

int interactive(void) {
    Interpreter *interp = interpreter_from_stream(NULL);

    for(;;) {
        printf(">>> ");
        char *line = NULL;
        size_t linecap = 0;
        size_t bytes_read = getline(&line, &linecap, stdin);
        if(line[0] == '\n')
            goto loopend;

        interp->parser->lexer->stream = fmemopen(line, bytes_read, "r");
        double value = 0.0;
        if(!interpreter_parse_line(interp, &value))
            exit(-1);
        printf("%f\n", value);
        fclose(interp->parser->lexer->stream);

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

    FILE *fp = fopen_or_die(argv[1], "r");

    Interpreter *interp = interpreter_from_stream(fp);

    for(;;) {
        double value;
        if(!interpreter_parse_line(interp, &value))
            break;
        printf("%f\n", value);
    }

    printf("\n");
    interpreter_free(interp);
    fclose(fp);
    return 0;
}
