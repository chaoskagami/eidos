#include <stdarg.h>

typedef void     (*module_out_f)    (char);
typedef char     (*module_in_f)     (void);
typedef int      (*module_avail_f)  (void);

struct output {
    module_out_f    out;
};

struct input {
    module_in_f     in;
    module_avail_f  avail;
};

void console_reg_write(struct output*);
void console_unreg_write(struct output*);

void console_reg_read(struct input*);
void console_unreg_read(struct input*);

void console_init();
void console_deinit();
