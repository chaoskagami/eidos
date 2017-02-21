#include <stdio.h>

void printf(const char* fmt, ...) {
    char buf[4096];
    va_list ap;
    va_start(ap, fmt);

    vsprintf(buf, fmt, ap);

    va_end(ap);

    puts(buf);
}

