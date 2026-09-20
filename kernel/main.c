#include <text/text.h>

void kernel_main(void)
{
    text_init();
    text_write_line("CookieOS Flex");
    for (;;)
    {
        __asm__ volatile ("hlt");
    }
}