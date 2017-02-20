#include <stdint.h>
#include <module.h>
#include <module_console.h>

int kernel_main(struct multiboot *mboot_ptr)
{
    console_init();

    kwrite("1\n");
    kwrite("2\n");
    kwrite("3\n");
    kwrite("4\n");
    kwrite("5\n");
    kwrite("6\n");
    kwrite("7\n");
    kwrite("8\n");
    kwrite("9\n");
    kwrite("10\n");
    kwrite("11\n");
    kwrite("12\n");
    kwrite("13\n");
    kwrite("14\n");
    kwrite("15\n");
    kwrite("16\n");
    kwrite("17\n");
    kwrite("18\n");
    kwrite("19\n");
    kwrite("20\n");
    kwrite("21\n");
    kwrite("22\n");
    kwrite("23\n");
    kwrite("24\n");
    kwrite("25\n");
    kwrite("26\n");

    console_deinit();

    return 0xDEADBEEF;
}
