#include <text/text.h>

#define VGA_MEMORY ((volatile unsigned short *)0xB8000)

#define VGA_WIDTH  80
#define VGA_HEIGHT 25

#define VGA_COLOR  0x07

static unsigned int cursor_x = 0;
static unsigned int cursor_y = 0;

static void text_scroll(void)
{
    if (cursor_y < VGA_HEIGHT)
        return;

    for (unsigned int y = 1; y < VGA_HEIGHT; y++)
    {
        for (unsigned int x = 0; x < VGA_WIDTH; x++)
        {
            VGA_MEMORY[(y - 1) * VGA_WIDTH + x] =
                VGA_MEMORY[y * VGA_WIDTH + x];
        }
    }

    for (unsigned int x = 0; x < VGA_WIDTH; x++)
    {
        VGA_MEMORY[(VGA_HEIGHT - 1) * VGA_WIDTH + x] =
            ((unsigned short)VGA_COLOR << 8) | ' ';
    }

    cursor_y = VGA_HEIGHT - 1;
}

static void text_newline(void)
{
    cursor_x = 0;
    cursor_y++;

    text_scroll();
}

void text_init(void)
{
    cursor_x = 0;
    cursor_y = 0;

    for (unsigned int y = 0; y < VGA_HEIGHT; y++)
    {
        for (unsigned int x = 0; x < VGA_WIDTH; x++)
        {
            VGA_MEMORY[y * VGA_WIDTH + x] =
                ((unsigned short)VGA_COLOR << 8) | ' ';
        }
    }
}

void text_putc(char c)
{
    if (c == '\n')
    {
        text_newline();
        return;
    }

    if (c == '\r')
    {
        cursor_x = 0;
        return;
    }

    VGA_MEMORY[cursor_y * VGA_WIDTH + cursor_x] =
        ((unsigned short)VGA_COLOR << 8) | (unsigned char)c;

    cursor_x++;

    if (cursor_x >= VGA_WIDTH)
        text_newline();
}

void text_write(const char *str)
{
    while (*str)
    {
        text_putc(*str);
        str++;
    }
}

void text_write_line(const char *str)
{
    text_write(str);
    text_putc('\n');
}