#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>


#define DEFAULT_COLOR "\033[0m"
#define RED_COLOR     "\033[0;31m"


const char* usage_help = \
    "Usage: %s [OPTIONS...] PATTERNS [FILE...]\n"
    "Try `%s --help` for more information.\n";
const char* help = \
    "GNU/Linux grep utility.\n"
    "Version: 0.0.1.\n"
    "LICENSE: MIT License.\n"
    "\n"
    "Description: A simple utility to searches for PATTERNS in each FILE.\n"
    "Flags:\n"
    "-h : Prints help.\n";


typedef struct {
    size_t amount_patterns;
    char*  patterns[10];
} Context;


typedef enum {
    IGNORE_CASE = (1 << 0),  // -i
} Options;


void print_help(void) {
    puts(help);
}


int process_flag(int argc, char* argv[], Context* cx) {
    int opt   = 0;
    int flags = 0;
    // opterr    = 0;

    while ((opt = getopt(argc, argv, "e:ih")) != -1) {
        switch (opt) {
            case 'e':
                cx->patterns[cx->amount_patterns] = optarg;
                ++cx->amount_patterns;
                break;
            case 'i':
                flags |= IGNORE_CASE;
                break;
            case 'h':
                print_help();
                exit(EXIT_SUCCESS);
            default:
                fprintf(stderr, usage_help, argv[0], argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    return flags;
}


bool is_match(
    const char* line,
    size_t* position_in_line,
    const char* pattern,
    int flags
) {
    size_t current_pattern_pos = 0;

    do {
        int pattern_char;
        int line_char;
        
        if (flags & IGNORE_CASE) {
            pattern_char  = tolower(pattern[current_pattern_pos]);
            line_char = tolower(line[*position_in_line]);
        } else {
            pattern_char = pattern[current_pattern_pos];
            line_char = line[*position_in_line];
        }
    
        if (pattern_char != line_char) {
            return false;
        }
        ++(*position_in_line);
        ++current_pattern_pos;
    } while (pattern[current_pattern_pos] != '\0');

    return true;
}


void search_in_line(const char* line, int flags, Context* cx) {
    size_t position_in_line    = 0;
    size_t current_pos_in_line = 0;
    size_t render_position     = 0;
    size_t count_matched       = 0;

    while (line[position_in_line] != '\0') {
        current_pos_in_line = position_in_line;
        bool is_matched = false;
        size_t i = 0;
        for (; i < cx->amount_patterns; ++i) {
            is_matched = is_match(
                line,
                &position_in_line,
                cx->patterns[i],
                flags
            );

            if (is_matched) {
                break;
            }
            position_in_line = current_pos_in_line;
        }
    
        if (is_matched) {
            ++count_matched;
            while (render_position < current_pos_in_line) {
                putchar(line[render_position]);
                ++render_position;
            }
            printf("%s", RED_COLOR);
            while (render_position < position_in_line) {
                putchar(line[render_position]);
                ++render_position;
            }
            printf("%s", DEFAULT_COLOR);
            putchar(line[position_in_line]);
            ++render_position;
        }
        ++position_in_line;
    }

    if (count_matched > 0) {
        while (line[render_position] != '\0') {
            putchar(line[render_position]);
            ++render_position;
        }
    }
}


void search_in_file(FILE* file, int flags, Context* cx) {
    char buffer[BUFSIZ];

    while (fgets(buffer, BUFSIZ, file)) {
        search_in_line(buffer, flags, cx);
    }
}


int main(int argc, char* argv[]) {
    Context cx = {0};
    int flags = process_flag(argc, argv, &cx);

    if (optind >= argc) {
        fprintf(stderr, usage_help, argv[0], argv[0]);
        return 1;
    }

    if (cx.amount_patterns == 0) {
        cx.patterns[cx.amount_patterns] = argv[optind];
        ++cx.amount_patterns;
        ++optind;
    }

    FILE* fd = fopen(argv[optind], "r");
    if (fd == NULL) {
        fprintf(stderr, "Cannot open file: %s\n", argv[optind]);
        return 1;
    }

    search_in_file(fd, flags, &cx);

    fclose(fd);
    return 0;
}
