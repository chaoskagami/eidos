#include <stdio.h>
#include <string.h>

void vsprintf(char *buffer, const char* fmt_con, va_list ap) {
    char* fmt = (char*)fmt_con;
    char* buf = buffer;
    while(*fmt != 0) {
        if (*fmt == '%') {
            // Format string.
            ++fmt;
            switch (*fmt) {
                case 'c':
                    *buf = (char)va_arg(ap, int);
                    buf++;
                    fmt++;
                    break;
                case '%':
                    *buf = '%';
                    buf++;
                    fmt++;
                    break;
                case 's':
                    buf += strcatlen(buf, va_arg(ap, char*));
                    fmt++;
                    break;
                case 'x':
                    buf += dumphex(buf, "0123456789abcdef", va_arg(ap, unsigned int));
                    ++fmt;
                    break;
                case 'X':
                    buf += dumphex(buf, "0123456789ABCDEF", va_arg(ap, unsigned int));
                    ++fmt;
                    break;
                case 'd':
                case 'u':
                default:
                    ++fmt;
                    break;
            }
        } else {
            *buf = *fmt;
            ++buf;
            ++fmt;
        }
    }

    *buf = 0;
}
