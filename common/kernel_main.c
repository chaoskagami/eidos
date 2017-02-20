#include <stdint.h>
#include <module.h>
#include <module_console.h>

int kernel_main(struct multiboot *mboot_ptr)
{
    console_init();

    kprintf("1%c%% %s %X\n", 'c', "whoop", 0xDEADBEEF);

    console_deinit();

    asm volatile("int $4");
    asm volatile("int $3");

    while(1);

    return 0xDEADBEEF;
}
