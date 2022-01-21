#include <stdio.h>
#include "syswrap.h"
#include "eval.h"
#include "object.h"
#include "mapobj.h"

int interactive(void) {
    Object *varmap = MapObject_empty();

    for(;;) {
        printf(">>> ");
        char *line = NULL;
        size_t linecap = 0;
        getline(&line, &linecap, stdin);
        if(line[0] == '\n')
            goto loopend;

        Object *value = eval_string(line, varmap);
        if(!value)
            exit(-1);
        Object_println(value);
        Object_set(varmap, "_", value);  /* steals value */

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

    Object *value = eval_stream(stream, NULL);
    if(!value)
        goto out;
    Object_println(value);
    Object_destroy(value);

out:
    printf("\n");
    fclose(stream);
    return 0;
}
