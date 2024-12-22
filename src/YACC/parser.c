#include <stdio.h>

void print_error(const char *msg) {
    fprintf(stderr, "Error: %s\n", msg);
}
