#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <time.h>

#define STD_DIR_MODE 0755

#define MAX_APP_NAME_LEN 40
#define PATH_BUFFER_SIZE 200

static void make_subdir(char* app_name, char* sub_dir) {
    char dir_path_buffer[MAX_APP_NAME_LEN + PATH_BUFFER_SIZE];
    sprintf(dir_path_buffer, "%s/%s", app_name, sub_dir);
    if (mkdir(dir_path_buffer, STD_DIR_MODE) == -1) {
        perror("ERROR: Failed to create directory\n");
        exit(EXIT_FAILURE);
    } else {
        printf("Created directory: %s/%s\n", app_name, sub_dir);
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        perror("You need to specify the project name.\n");
        return EXIT_FAILURE;
    } else if (argc > 2) {
        fprintf(stderr, "Too many arguments: expected one (the project name), got %d\n", argc - 1);
        return EXIT_FAILURE;
    }

    char* app_name = argv[1];
    if (strlen(app_name) > MAX_APP_NAME_LEN) {
        perror("Project name is too long.\n");
        return EXIT_FAILURE;
    } else if (strlen(app_name) <= 0) {
        perror("Project name is too short.\n");
        return EXIT_FAILURE;
    }
    if (strchr(app_name, '/')) {
        perror("Project name cannot contain '/'\n");
        return EXIT_FAILURE;
    }
    if (strchr(app_name, '.')) {
        perror("Project name cannot contain '.'\n");
        return EXIT_FAILURE;
    }

    DIR* cwd = opendir(".");
    if (cwd == NULL) {
        perror("ERROR: Could not open cwd\n");
        return EXIT_FAILURE;
    }

    struct dirent* entry;
    while ((entry = readdir(cwd)) != NULL) {
        if (strcmp(entry->d_name, app_name) == 0) {
            perror("The specified directory already exists.\n");
            closedir(cwd);
            return EXIT_FAILURE;
        }
    }
    closedir(cwd);

    if (mkdir(app_name, STD_DIR_MODE) == -1) {
        perror("ERROR: Failed to create directory\n");
        return EXIT_FAILURE;
    } else {
        printf("Created directory: %s\n", app_name);
    }

    make_subdir(app_name, "bin");
    make_subdir(app_name, "build");
    make_subdir(app_name, "include");
    make_subdir(app_name, "src");

    // create README.md
    {
        char file_path[PATH_BUFFER_SIZE];
        strcpy(file_path, app_name);
        char* file_name = "/README.md";
        strncat(file_path, file_name, 10);

        FILE* f = fopen(file_path, "w");
        if (f == NULL) {
            fprintf(stderr, "ERROR: Failed to create file: %s\n", file_name);
            return EXIT_FAILURE;
        } else {
            const char* file_fmt = "# %s\n"
                                     "\n"
                                     "\n";
            fprintf(f, file_fmt, app_name);
            fclose(f);
            printf("Created file: %s\n", file_name);
        }
    }

    // create LICENSE.txt
    {
        char file_path[PATH_BUFFER_SIZE];
        strcpy(file_path, app_name);
        char* file_name = "/LICENSE.txt";
        strncat(file_path, file_name, 12);

        FILE* f = fopen(file_path, "w");
        if (f == NULL) {
            fprintf(stderr, "ERROR: Failed to create file: %s\n", file_name);
            return EXIT_FAILURE;
        } else {
            char year[5];
            time_t now = time(NULL);
            struct tm *local_time = localtime(&now);
            strftime(year, sizeof (year), "%Y", local_time);

            const char* file_fmt = "MIT License\n"
                                   "\n"
                                   "Copyright (c) %s [your name]\n"
                                   "\n"
                                   "Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:\n"
                                   "\n"
                                   "The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.\n"
                                   "\n"
                                   "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n";
            fprintf(f, file_fmt, year);
            fclose(f);
            printf("Created file: %s\n", file_name);
        }
    }

    // create Makefile
    {
        char file_path[PATH_BUFFER_SIZE];
        strcpy(file_path, app_name);
        char* file_name = "/Makefile";
        strncat(file_path, file_name, 9);

        FILE* f = fopen(file_path, "w");
        if (f == NULL) {
            fprintf(stderr, "ERROR: Failed to create file: %s\n", file_name);
            return EXIT_FAILURE;
        } else {
            const char* file_fmt = "# Flexible C Project Makefile (FCPM)\n"
                                   "EXE_NAME = %s\n"
                                   "COMPILER = clang\n"
                                   "FLAGS = -Wall -Werror\n"
                                   "PACKAGES = \n"
                                   "\n"
                                   "# Commands:\n"
                                   "\n"
                                   "# make          => builds the executable\n"
                                   "# make run      => runs the executable (must be built first)\n"
                                   "# make clean    => removes build artifacts\n"
                                   "# make commands => builds compile_commands.json for 'clang' and 'clangd'\n"
                                   "\n"
                                   "# -----------------------------------------------------------------------------------------\n"
                                   "\n"
                                   "TARGET = bin/$(EXE_NAME)\n"
                                   "CFLAGS = -Iinclude $(FLAGS) $(shell pkg-config --cflags $(PACKAGES))\n"
                                   "LDFLAGS = $(FLAGS) $(shell pkg-config --libs $(PACKAGES))\n"
                                   "SRC_FILES = $(wildcard src/*.c)\n"
                                   "OBJ_FILES = $(patsubst src/%%.c,build/%%.o,$(SRC_FILES))\n"
                                   "\n"
                                   "all: $(TARGET)\n"
                                   "\n"
                                   "$(TARGET): $(OBJ_FILES)\n"
                                   "\t@mkdir -p bin\n"
                                   "\t$(COMPILER) $(OBJ_FILES) -o $@ $(LDFLAGS)\n"
                                   "\n"
                                   "build/%%.o: src/%%.c\n"
                                   "\t@mkdir -p build\n"
                                   "\t$(COMPILER) -c $< -o $@ $(CFLAGS)\n"
                                   "\n"
                                   "run:\n"
                                   "\t./bin/$(EXE_NAME)\n"
                                   "\n"
                                   "clean:\n"
                                   "\trm -rf bin/*\n"
                                   "\trm -rf build/*\n"
                                   "\trm -rf .cache\n"
                                   "\n"
                                   "commands:\n"
                                   "\tbear -- make\n"
                                   "\n"
                                   "# you may add your own install label here\n"
                                   "\n"
                                   ".PHONY: all clean run commands # don't forget to make the install label phony if you define one\n";
            fprintf(f, file_fmt, app_name);
            fclose(f);
            printf("Created file: %s\n", file_name);
        }
    }

    // create main.c
    {
        char file_path[PATH_BUFFER_SIZE];
        strcpy(file_path, app_name);
        char* file_name = "/src/main.c";
        strncat(file_path, file_name, 11);

        FILE* f = fopen(file_path, "w");
        if (f == NULL) {
            fprintf(stderr, "ERROR: Failed to create file: %s\n", file_name);
            return EXIT_FAILURE;
        } else {
            const char* file_fmt = "#include \"../include/%s.h\"\n"
                                   "#include <stdlib.h>\n"
                                   "#include <stdio.h>\n"
                                   "\n"
                                   "int main(void) {\n"
                                   "    printf(\"hello, world\\n\");\n"
                                   "\n"
                                   "    return EXIT_SUCCESS;\n"
                                   "}\n";
            fprintf(f, file_fmt, app_name);
            fclose(f);
            printf("Created file: %s\n", file_name);
        }
    }

    // create header file
    {
        char file_path[PATH_BUFFER_SIZE];
        strcpy(file_path, app_name);

        char file_name[MAX_APP_NAME_LEN + 11];
        const char* file_fmt = "/include/%s.h";
        sprintf(file_name, file_fmt, app_name);

        strncat(file_path, file_name, MAX_APP_NAME_LEN + 11);

        FILE* f = fopen(file_path, "w");
        if (f == NULL) {
            fprintf(stderr, "ERROR: Failed to create file: %s\n", file_name);
            return EXIT_FAILURE;
        } else {
            const char* file_fmt = "#pragma once\n"
                                   "\n"
                                   "\n";
            fprintf(f, file_fmt, app_name);
            fclose(f);
            printf("Created file: %s\n", file_name);
        }
    }

    printf("Project %s initialized successfully.\n", app_name);
    return EXIT_SUCCESS;
}
