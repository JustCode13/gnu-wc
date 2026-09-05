#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t count_chrs(char *line);

void print_file_info(size_t file_count, char *files, size_t *char_count,
                     size_t *word_count, size_t *line_count);

size_t words_count(char *line);

FILE *open_file(char *file);

char *read_next_line(char *line, FILE *fptr);

int validate_options(char *opt, char *options, size_t opt_len);

void print_wc_result(char options[], size_t opt_len, size_t char_count,
                     size_t word_count, size_t line_count, char *file);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("USAGE: %s [-options] [-files]\n", argv[0]);
        return -1;
    }

    bool is_options = false;

    if (argv[1][0] == '-') {
        size_t opt_len = strlen(argv[1]) - 1;

        char options[opt_len];

        if (validate_options(argv[1], options, opt_len) != 0) {
            return -1;
        }

        is_options = true;
    }

    char *file = is_options ? argv[2] : argv[1];

    FILE *file_ptr = open_file(file);

    char line[256];

    size_t file_count = is_options ? argc - 2 : argc - 1;

    char *files[file_count];
    FILE *files_ptr[file_count];

    for (size_t i = 0; i < file_count; i++) {
        files[i] = argv[2 + i];
        files_ptr[i] = open_file(files[i]);

        if (files_ptr[i] == NULL) {
            return 1;
        }
    }

    for (size_t i = 0; i < file_count; i++) {
        size_t char_count = 0;
        size_t word_count = 0;
        size_t line_count = 0;

        while (fgets(line, sizeof(line), files_ptr[i])) {
            char_count += count_chrs(line);
            line_count++;
            word_count += words_count(line);
        }

        printf("%zu %zu %zu %s\n", char_count, word_count, line_count,
               files[i]);
    }

    return 0;
}

void print_wc_result(char options[], size_t opt_len, size_t char_count,
                     size_t word_count, size_t line_count, char *file) {
    for (size_t i = 0; i < opt_len; i++) {
        printf("%c", options[i]);
    }
}

size_t words_count(char *line) {
    size_t line_length = strlen(line);
    char ch;
    size_t word_count = 0;

    bool inside_word = false;

    for (size_t i = 0; i < line_length; i++) {
        if (!isspace(line[i])) {
            inside_word = true;
        } else if (isspace(line[i]) && inside_word) {
            word_count++;
            inside_word = false;
        }
    }

    if (inside_word) {
        word_count++;
    }

    return word_count;
}

size_t count_chrs(char *line) {
    if (line == NULL) {
        printf("Error reading characters\n");
        return 0;
    }

    size_t current_char_count = strlen(line);

    return current_char_count;
}

int validate_options(char *opt, char *options, size_t opt_len) {

    if (opt_len == 0) {
        printf("Invalid options: Valid options are -l, -w, -c, -m\n");
        return -1;
    }

    for (size_t i = 0; i < opt_len; i++) {
        if (!strchr("lcwm", opt[i + 1])) {
            printf("Invalid options: Valid options are -l, -w, -c, -m\n");
            return -1;
        }

        options[i] = opt[i + 1];
    }

    return 0;
}

FILE *open_file(char *file) {
    FILE *fptr = fopen(file, "r");

    if (fptr == NULL) {
        printf("Error opening file: %s\n", file);
    }

    return fptr;
}

char *read_next_line(char *line, FILE *fptr) {
    fgets(line, sizeof(line), fptr);

    if (line == NULL) {
        return NULL;
    }

    return line;
}
