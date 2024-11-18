#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

const char* usage_help = \
    "Usage: %s [OPTION...] [FILE...]\n"
    "Try `%s --help` for more information.\n";
const char* help = \
    "GNU/Linux cat utility.\n"
    "Version: 0.0.1.\n"
    "LICENSE: MIT License.\n"
    "\n"
    "Description: A simple utility to concatenate and display file contents.\n"
    "Flags:\n"
    "-h : Prints help.\n"
    "-b : Number non-blank lines.\n"
    "-n : Number all lines.\n"
    "-E : Show `$` at the end of each line.\n"
    "-T : Show `^I` for tabs.\n"
    "-v : Show non-printing characters as `^` and `^[`.\n"
    "-e : Equivalent to `-vE` (non-printing characters and `$`).\n"
    "-t : Equivalent to `-vT` (non-printing characters and tabs).\n"
    "-A : Equivalent to `-vET` (non-printing characters, `$`, and tabs).\n"
    "-s : Squeeze blank lines.";


typedef enum {
    NUMBER_NONBLANK_LINES = (1 << 0),  // -b
    NUMBER_ALL_LINES      = (1 << 1),  // -n
    SHOW_NEW_LINES        = (1 << 2),  // -E
    SHOW_NON_PRINTING     = (1 << 3),  // -v
    SHOW_TABS             = (1 << 4),  // -T
    SQUEEZE_BLANK         = (1 << 5),  // -s
} Options;


void print_help(void) {
    puts(help);
}


int process_flag(int argc, char* argv[]) {
    int opt   = 0;
    int flags = 0;
    opterr    = 0;

    while ((opt = getopt(argc, argv, ":::bnEveTtAhs")) != -1) {
        switch (opt) {
            case 'b':
                flags |= NUMBER_NONBLANK_LINES;
                break;
            case 'n':
                flags |= NUMBER_ALL_LINES;
                break;
            case 'E':
                flags |= SHOW_NEW_LINES;
                break;
            case 'v':
                flags |= SHOW_NON_PRINTING;
                break;
            case 'e':
                flags |= (SHOW_NON_PRINTING | SHOW_NEW_LINES);
                break;
            case 'T':
                flags |= SHOW_TABS;
                break;
            case 't':
                flags |= (SHOW_NON_PRINTING | SHOW_TABS);
                break;
            case 'A':
                flags |= (SHOW_NON_PRINTING | SHOW_NEW_LINES | SHOW_TABS);
                break;
            case 's':
                flags |= SQUEEZE_BLANK;
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


void print_ctrl_char(int ch) {
    if (ch >= 128) {
        putchar('^');
        putchar('[');
        putchar(ch - 128);
    } else {
        putchar('^');
        putchar(ch + '@');
    }
}


void process_file(FILE* fd, int flags) {
    int    ch          = 0;
    size_t line_size   = 0;
    size_t line_number = 1;
    size_t blank_lines = 0;
    bool   is_new_line = true;

    while ((ch = getc(fd)) != EOF) {
        ++line_size;

        if (is_new_line) {
            if ((flags & NUMBER_NONBLANK_LINES) && (ch != '\n')) {
                printf("%6zu\t", line_number);
                ++line_number;
            } else if (flags & NUMBER_ALL_LINES) {
                printf("%6zu\t", line_number);
                ++line_number;
            }
        }

        if (ch == '\n') {
            if (line_size == 1) {
                ++blank_lines;
            }
            if (flags & SHOW_NEW_LINES) {
                putchar('$');
            }
            if (!(flags & SQUEEZE_BLANK) || blank_lines < 2) {
                putchar(ch);
            }
            is_new_line = true;
            line_size = 0;
        } else {
            blank_lines = 0;
            is_new_line = false;

            if (iscntrl(ch) || ch >= 128) {
                if (flags & SHOW_NON_PRINTING) {
                    print_ctrl_char(ch);
                } else if ((ch == '\t') && (flags & SHOW_TABS)) {
                    print_ctrl_char(ch);
                } else {
                    putchar(ch);
                }
            } else {
                putchar(ch);
            }
        }
    }
}


int main(int argc, char* argv[]) {
    int flags = process_flag(argc, argv);

    if (optind >= argc) {
        fprintf(stderr, usage_help, argv[0], argv[0]);
        return 1;
    }

    for (; optind < argc; ++optind) {
        FILE* fd = fopen(argv[optind], "r");
        if (fd == NULL) {
            fprintf(stderr, "Cannot open file: %s.\n", argv[optind]);
            return 1;
        }

        process_file(fd, flags);

        fclose(fd);
    }

    return 0;
}
