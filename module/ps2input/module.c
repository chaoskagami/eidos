#include <stdint.h>
#include <string.h>
#include <module.h>
#include <module_console.h>

#include <idt.h>

char avail_buf[256];
int avail_cnt = 0;

void ps2input_int(struct interrupt_frame* esp) {
    do {
        if (inb(0x60) != 0) {
            avail_buf[avail_cnt] = inb(0x60);
            if (avail_buf[avail_cnt]) {
                ++avail_cnt;
                return;
            }
        }
    } while(1);
}

char charmap[256] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    '-', '=', '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
    'o', 'p', '[', ']', '\n', 0, 'a', 's', 'd', 'f', 'g', 'h',
    'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', ' ',
};

char ps2input_rb() {
    if (!avail_cnt)
        return 0;

    --avail_cnt;

    char r = charmap[avail_buf[0]];

    memcpy(&avail_buf[0], &avail_buf[1], 255);

    return r;
}

int ps2input_avail() {
    return avail_cnt;
}

struct input ps2input_module = {
    .in    = ps2input_rb,
    .avail = ps2input_avail
};

void ps2input_init() {
    register_int_handler(33, ps2input_int);
    console_reg_read(&ps2input_module);
}

void ps2input_deinit() {
    console_unreg_read(&ps2input_module);
    // Stub.
}
