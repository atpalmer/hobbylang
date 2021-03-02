# Double Calculator

Reads input from a file or an interactive console and computes results using double floating-point arithmatic. Supports parentheses and variables.

## Sample

Sample program:

```
    42
    x = 2 + 1
    -2 + x * 2 + 5 - 1
    -2 + x * (2 + 5) - 1
    alfa = 1
    bravo = 7
    alfa = 5
    alfa * bravo
```

Expected output (one line of output for every line of input):

```
    42.000000
    3.000000
    8.000000
    18.000000
    1.000000
    7.000000
    5.000000
    35.000000
```


More sample programs are available in [/samples](./samples).

## Build

From the project directory, run `make`.

## Running a program

Run `calc [path]`. For example, to run `/samples/sample1.calc`, run `./calc ./samples/sample1.calc` from the project directory after running `make`.

## Interactive mode

To run in interactive mode, simply run `calc` with no arguments.


