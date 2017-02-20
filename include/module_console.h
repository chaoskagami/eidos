#include <stdarg.h>

typedef void     (*console_put_f)    (char,     char,  char,  uint16_t, uint16_t);
typedef void     (*console_get_f)    (char*,    char*, char*, uint16_t, uint16_t);
typedef void     (*console_cursor_f) (uint16_t, uint16_t);
typedef uint16_t (*console_width_f)  ();
typedef uint16_t (*console_height_f) ();
typedef void     (*console_refresh_f)();
typedef int      (*console_tty_f)    ();

struct console_module {
    console_put_f     put;
    console_get_f     get;
    console_cursor_f  cursor;
    console_width_f   width;
    console_height_f  height;
    console_refresh_f refresh;
    console_tty_f     tty;
};

void console_register_module(struct console_module*);
void console_unregister_module(struct console_module*);
void console_init();
void console_deinit();

void kputc(char);
void kwrite(char*);
void kvsprintf(char *buffer, const char* fmt_con, va_list ap);
void kprintf(const char* fmt, ...);

