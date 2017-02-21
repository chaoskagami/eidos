#ifndef _STDIO_H
#define _STDIO_H 1

#include <sys/cdefs.h>
#include <stddef.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

int    dumphex   (char* buf, const char* index, unsigned int num);
void   putc      (char c);
void   puts      (char* str);
void   vsprintf  (char *buffer, const char* fmt_con, va_list ap);
void   printf    (const char* fmt_con, ...);

#ifdef __cplusplus
}
#endif

#endif
