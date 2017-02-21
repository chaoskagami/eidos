#include <stdarg.h>

typedef void     (*module_out_f)    (char);

struct output {
    module_out_f     out;
};

void console_reg(struct output*);
void console_unreg(struct output*);
void console_init();
void console_deinit();
