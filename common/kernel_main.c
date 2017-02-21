#include <stdint.h>
#include <module.h>
#include <module_console.h>

#include <stdio.h>

int kernel_main(struct multiboot *mboot_ptr)
{
    console_init();

    printf("1%c%% %s %X\n", 'c', "whoop", 0xDEADBEEF);

    while(1) {
        console_refresh();
        asm("hlt");
    }

    console_deinit();

    return 0xDEADBEEF;
}
