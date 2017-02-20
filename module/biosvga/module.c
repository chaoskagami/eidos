#include <stdint.h>
#include <module.h>
#include <module_console.h>

uint16_t *video_mem = (uint16_t*)0xB8000;

void biosvga_put(char c, char color_fg, char color_bg, uint16_t x, uint16_t y) {
    uint8_t attr = (color_bg << 4) | (color_fg & 0xF);
    uint16_t write = (attr << 8) | c;

    video_mem[y * 80 + x] = write;
}

void biosvga_get(char* c, char* color_fg, char* color_bg, uint16_t x, uint16_t y) {
    uint16_t write = video_mem[y * 80 + x];
    *c = (char)(write & 0xFF);
    *color_bg = (write << 12) & 0xF;
    *color_fg = (write << 8)  & 0xF;
}

void biosvga_cursor(uint16_t x, uint16_t y) {
    uint16_t loc = y * 80 + x;
    outb(0x3D4, 14);
    outb(0x3D5, loc >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, loc);
}

uint16_t biosvga_width() {
    return 80;
}

uint16_t biosvga_height() {
    return 25;
}

void biosvga_refresh() {

}

struct console_module biosvga_module = {
    .put    = biosvga_put,
    .get    = biosvga_get,
    .cursor = biosvga_cursor,
    .width  = biosvga_width,
    .height = biosvga_height,
    .refresh = biosvga_refresh
};

void biosvga_init() {
    console_register_module(&biosvga_module);
}

void biosvga_deinit() {
    console_unregister_module(&biosvga_module);
    // Stub.
}
