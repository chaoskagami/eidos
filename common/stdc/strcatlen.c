#include <string.h>

int strcatlen(char* a, char* b) {
    char* o = b;
    while(*b != 0) {
        *a = *b;
        a++;
        b++;
    }
    return b - o;
}
