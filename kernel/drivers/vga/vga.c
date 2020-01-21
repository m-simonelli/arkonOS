#include <drivers/io/ports.h>
#include <drivers/vga/vga.h>
#include <inttypes.h>
#include <mem/memcpy.h>

/* VGA base address */
vga_mem_ptr_t vga_mem = (uint8_t *)VGA_MEM_LOW;

/* The current position of the cursor, contains an x and y value, see vga.h */
vga_cursor_pos_t vga_cursor_pos = {0, 0};

void vga_init() {
    /* Disable the hardware cursor*/
    port_byte_out(VGA_CTRL_REG, 0x0A);
    port_byte_out(VGA_DATA_REG, 0x20);
    /* Clear the screen */
    vga_clear_screen();
    /* Write to VGA (and serial if enabled) that VGA has been initalized */
    vga_log("vga init\n");
}

void vga_clear_screen() {
    vga_screen_size_t screen_size = 2 * VGA_MAX_COLS * VGA_MAX_ROWS;

    /* Null the screen so it's empty */
    for (vga_screen_size_t i = 0; i < screen_size; i++) {
        vga_mem[i] = 0;
    }
}

void vga_scroll_line() {
    /* Iterate over every row and memcpy it to the row before it */
    for (count32_t i = 1; i < VGA_MAX_ROWS; i++) {
        memcpy((uint8_t *)(VGA_MEM_LOW + 2 * VGA_X_Y_TO_OFFSET(0, i - 1)),
               (void *)(VGA_MEM_LOW + 2 * VGA_X_Y_TO_OFFSET(0, i)),
               2 * VGA_MAX_COLS);
    }
    /* Clear the bottom row */
    uint8_t *bottom_row =
        (uint8_t *)(VGA_MEM_LOW + 2 * VGA_X_Y_TO_OFFSET(0, VGA_MAX_ROWS - 1));
    for (count16_t i = 0; i < VGA_MAX_COLS * 2; i++) bottom_row[i] = 0;

    /* Decrease the cursor's y value by 1 so the cursor is on the last line */
    vga_cursor_pos.y -= 1;
    /* Set the cursor's x value to 0 so it's at the start of the last line */
    vga_cursor_pos.x = 0;
}

void vga_print_char(char c, char attr) {
    /* If the attribute is 0 (black on black), set it to white on black */
    if (!attr)
        attr = VGA_COL_FOREGROUND_WHITE |
               VGA_COL_BACKGROUND_BLACK; /* default to white on black */

    uint16_t off;
    if (c == '\n') {
        /* If the character is a new line, increase the cursor's y value */
        vga_cursor_pos.y += 1;
        vga_cursor_pos.x = 0;

        /* Calculate the offset */
        off = VGA_X_Y_TO_OFFSET(vga_cursor_pos.x, vga_cursor_pos.y);
    } else {
        /* Calculate the offset */
        off = VGA_X_Y_TO_OFFSET(vga_cursor_pos.x, vga_cursor_pos.y);

        /* Write to vga mem at offset * 2
         * The `2 * off` is because VGA memory uses 2 bytes per character,
         * one for attributes, and another for the ASCII code. The offset
         * calculation gives us a value in words, not bytes. Hence we have
         * to multiply by 2 first
         */

        /* Write the ASCII code */
        vga_mem[2 * off] = c;

        /* Write the attribute*/
        vga_mem[2 * off + 1] = attr;

        /* Increase the cursor's x value */
        vga_cursor_pos.x++;

        /* Increase off by 1 as we have only moved by 1 character */
        off++;
    }
    /* If the offset is past the end of the screen, scroll */
    if (off >= VGA_MAX_ROWS * VGA_MAX_COLS) vga_scroll_line();
}

void vga_print(char *s) {
    /* Offset to the current character */
    count32_t i = 0;

    /* While the current character is not a null byte */
    while (s[i]) {
        /* Print the character, and increase i */
        vga_print_char(s[i++],
                       VGA_COL_FOREGROUND_WHITE | VGA_COL_BACKGROUND_BLACK);
    }
    /* If serial logging is enabled, then also write the message to serial */
#if LOG_TO_SERIAL == 1
    serial_print(s);
#endif
}

void vga_log(char *s) {
    /* Prepend "[info] >" to the start of the message, this is to distinguish
     * logs, warns, and errors
     */
    vga_print("[info] >");

    /* Print it */
    vga_print(s);
}