#include <stdio.h>
#include "syswrap.h"
#include "interpreter.h"
#include "object.h"
#include "mapobj.h"

int interactive(void) {
    Object *varmap = MapObject_empty();

    for(;;) {
        printf(">>> ");
        char *line = NULL;
        size_t linecap = 0;
        size_t bytes_read = getline(&line, &linecap, stdin);
        if(line[0] == '\n')
            goto loopend;

        FILE *stream = fmemopen(line, bytes_read, "r");
        Object *value = interpreter_eval(stream, varmap);
        if(!value)
            exit(-1);
        Object_println(value);
        Object_set(varmap, "_", value);  /* steals value */
        fclose(stream);

loopend:
        if(line)
            free(line);
    }

    printf("\n");
    Object_destroy(varmap);
    return 0;
}

int main(int argc, const char **argv) {
    if(argc < 2)
        return interactive();

    FILE *stream = fopen_or_die(argv[1], "r");

    Object *value = interpreter_eval(stream, NULL);
    if(!value)
        goto out;
    Object_println(value);
    Object_destroy(value);

out:
    printf("\n");
    fclose(stream);
    return 0;
}
