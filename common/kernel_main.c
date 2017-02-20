#include <stdint.h>
#include <module.h>
#include <module_console.h>

int kernel_main(struct multiboot *mboot_ptr)
{
    console_init();

    kprintf("1%c%% %s %X\n", 'c', "whoop", 0xDEADBEEF);

    console_deinit();

    while(1) {
        uint64_t clock = kclock();
        kprintf("%X%X\r", ((uint32_t*)&clock)[1], ((uint32_t*)&clock)[0]);
        asm("hlt");
    }

    return 0xDEADBEEF;
}
