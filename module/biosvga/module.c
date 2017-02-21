#include <stdint.h>
#include <string.h>
#include <module.h>
#include <module_console.h>

#include <utils.h>

#define WIDTH  80
#define HEIGHT 25

int x, y;

uint16_t *video_mem = (uint16_t*)0xC00B8000;

void cursor(uint16_t x, uint16_t y) {
    uint16_t loc = y * 80 + x;
    outb(0x3D4, 14);
    outb(0x3D5, loc >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, loc);
}

void biosvga_wb(char c) {
    uint8_t color_bg = 0;
    uint8_t color_fg = 0xF;
    uint8_t attr = (color_bg << 4) | (color_fg & 0xF);
    uint16_t write = (attr << 8) | c;

    switch(c) {
        case '\n':
            y++;
        case '\r':
            x = 0;
            break;
        case '\b':
            if (x) {
                --x;
                video_mem[y * 80 + x] = (attr << 8) | ' ';
            }
            break;
        case '\t':
            x += 4 - (x % 4);
            break;
        case ' ' ... '~':
            video_mem[y * 80 + x] = write;
            ++x;
            break;
    }

    if (x >= WIDTH) {
        x = 0;
        ++y;
    }

    if (y >= HEIGHT) {
        memcpy(video_mem, &video_mem[80], (HEIGHT - 1) * 80 * sizeof(uint16_t));
        --y;
        memset(&video_mem[y * 80], 0, 160);
    }

    cursor(x, y);
}

struct output biosvga_module = {
    .out    = biosvga_wb
};

void biosvga_init() {
    x = y = 0;
    console_reg_write(&biosvga_module);
}

void biosvga_deinit() {
    console_unreg_write(&biosvga_module);
    // Stub.
}
