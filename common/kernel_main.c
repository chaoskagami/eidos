#include <stdint.h>
#include <module.h>
#include <module_console.h>

#include <stdio.h>

int kernel_main(struct multiboot *mboot_ptr)
{
    console_init();

    printf("1%c%% %s %X\n", 'c', "whoop", 0xDEADBEEF);

    console_deinit();

    while(1) {
        uint64_t ticks = clock();
        printf("%X%X\r", ((uint32_t*)&ticks)[1], ((uint32_t*)&ticks)[0]);
        asm("hlt");
    }

    return 0xDEADBEEF;
}
