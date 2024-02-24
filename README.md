# cinit

`cinit` is a simple initialization tool for small multi-file C projects. It is currently only tested on Linux, though it should support macOS as well.

This application is meant to work well with `clang`, `clangd` and `lldb`, as it has functionality specifically for `clangd` and the default `Makefile` uses `clang` as the compiler and `lldb` as the debugger, though you can easily switch out the `COMPILER` variable, with that being the only change necessary to use another compiler of your choice and the `DEBUGGER` variable, with that being the only change necessary to use another debugger of your choice.

## Features

`cinit` will initialize the following project structure ("project" will be the name for your project):

``` shell
$ cinit project
# initialization output
$ cd project
$ tree
.
├── bin
├── build
├── include
│   └── project.h
├── LICENSE.txt
├── Makefile
├── README.md
└── src
    └── main.c
```

The `Makefile` supports that the `src` directory can contain multiple C source files, as long as they are not located inside subfolders. The same is true for the `include` folder and header files, respectively. You can also specify multiple, space-separated packages/dependencies/libraries that should be looked up using `pkg-config` during the compilation process. If you need intellisense from `clangd` for these packages/dependencies/libraries you can run the `make commands` command to generate a `compile_commands.json` file for it.

The possible `Makefile` commands are:

- `make`: This builds the project using the variables specified in the `Makefile`.
- `make run`: This runs ./bin/project where project is the name specified at initialization.
- `make debug`: This runs the debugger specified in the `Makefile` on the executable. The default debugger is `lldb`.
- `make clean`: This will delete everything inside `bin` and `build` as well as the whole `.cache` directory, should it exist.
- `make commands`: This will create a `compile_commands.json` file using `bear` (this enables intellisense with clangd when using third party dependencies). `bear` has to be installed seperately for this to work.

`cinit` will also initialize an MIT License and a template for a README file (please don't forget to enter your name instead of [your name]).

It will also initialize a simple header file with `#pragma once` that has the same name as the project and a main.c file with a hello world program to test the development environment with `make` followed by `make run` after initializing the project.

NOTES: Before generating/regenerating the `compile_commands.json` file with `make commands`, please run `make clean`, so `bear` can catch all dependencies correctly. Also, `pkg-config` will output an error telling you to specify package names when the `PACKAGES` variable in the `Makefile` is empty. This does not impact the compilation of programs that don't use third party dependencies, so it is completely safe to leave the `PACKAGES` variable empty.

## Installation

To install and use `cinit` you will need the following:

- `make`
- `pkg-config`
- A C compiler - the default is `clang`, but you can specify another one such as `gcc` with the `CC` variable in the `Makefile`
- `lldb` or another debugger (only if you plan on using the `make debug` command)
- `bear` (only if you need to generate `compile_commands.json` files)

To install `cinit`, clone the repository to a location of your choice, `cd` into the directory and run the following commands to create a symbolic link to the executable. Switch out "location/in/your/path" for the directory you want to put the symbolic link in:

```shell
$ make
$ sudo ln -s "$(pwd)/cinit" "location/in/your/path/cinit"
```

A common place to put symbolic links to programs on Linux is "/usr/local/bin", though you may put it in any directory in your PATH or use another method of making it available globally.

Copyright (C) 2024 Leon Zanker
