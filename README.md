# FlareX Interpreter

The official interpreter for the **FlareX Programming Language**.

> |> Status: Unstable development
> Not intended for production or real-world use.

---

## Overview

This repository contains the reference implementation of the FlareX interpreter.

It reads FlareX source code, compiles it into FlareX bytecode, and allows execution through the FlareX Runner.

Currently, building is supported on **Linux only**.

---

## Building

Use `make` to build the project:

`make`

After building, the *fx* binary can be used.

## Usage

Display help information:

`./fx -h` or `./fx -help`

Display the language guide:

`./fx -g` or `./fx -guide`

The interpreter supports compiling and execution:

  - It reads the FlareX source file (example.flx)
  - Compiles it into bytecode
  - Outputs it into a .fxb (FlareX Bytecode) file
  - Executes the bytecode file

## Features

FlareX currently has three main features:

out  -  Outputs a specified object onto the terminal.

*Currently supports signed intergers, strings and variables via the "." prefix*

var  -  Creates a variable

Var has two variables types:

int (signed int)
str (multi-character string, aka std::string in C++)

ext  -  Sets the exit code and terminates the program

Only an int is supported.
NOTE: Anything that is after the ext instruction will simply not be executed.
