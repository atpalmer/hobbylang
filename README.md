# hobbylang

Reads input from a file or an interactive console and computes results using double floating-point arithmatic. Supports parentheses and variables.

## Sample

Sample program:

```
    alfa = 1
    bravo = 7
    alfa = 5
    alfa * bravo
```

Expected output is the evaluation of last line of the program block:

```
    35.000000
```


More sample programs are available in [/samples](./samples).

## Build

From the project directory, run `make`.

## Running a program

Run `hobby [path]`. For example, to run `/samples/sample1.hb`, run `./hobby ./samples/sample1.hb` from the project directory after running `make`.

## Interactive mode

To run in interactive mode, simply run `hobby` with no arguments.

Enter `Ctrl+D` to exit.

