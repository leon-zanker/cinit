CC = clang

all: cinit

cinit:
	$(CC) main.c -o cinit -Wall -Werror

clean:
	rm -rf cinit

.PHONY: all clean