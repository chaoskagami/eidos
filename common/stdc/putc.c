#include <stdio.h>

void put_handler(char);

void putc(char c) {
    put_handler(c);
}
