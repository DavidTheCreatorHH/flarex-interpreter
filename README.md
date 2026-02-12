# flarex-interpreter
The FlareX Programming language Interpreter.

This is the repository for the FlareX Programming Language interpreter i made.
It is in Unstable Development, so definitely NOT for real world use.

You can build it using make and run it, the building is currently only supported on Linux.

# Features

The Interpreter can read a file and generate bytecode from it,
for more information, run:
"./fx -h"
and for the guide:
"./fx -g".

It is able to read the source code and compile it into the FlareX Bytecode file,
which can then be executed by the FlareX Runner.

The language itself has simple instructions:

out <something> For printing a certain object on the terminal
  - Supports variables via the . prefix, intergers, and strings
var <type> <name> = <value> for storing variables.
  - Currently supports intergers and strings
ext <exit_code> For terminating the program and setting the exit code.
  - The argument is always an interger

There is currently no way to distuingish between unsigned, signed, long, long long and short intergers,
and strings are always multiple characters.

If you encounter any errors or have a new feature in mind, please contact me!
