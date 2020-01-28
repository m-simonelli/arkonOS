#ifndef _drivers_vga_h
#define _drivers_vga_h
#include <drivers/serial/serial.h>
#include <inttypes.h>

/* Handy macro to convert an x,y position to VGA offset */
#define VGA_X_Y_TO_OFFSET(x, y) ((y)*VGA_MAX_COLS + (x))

/* Memory addresses */
#define VGA_MEM_LOW 0xb8000
#define VGA_MEM_HIGH 0xbffff

/* Row and column count */
#define VGA_MAX_ROWS 25
#define VGA_MAX_COLS 80

/* Port addresses */
#define VGA_CTRL_REG 0x3d4
#define VGA_DATA_REG 0x3d5

/* typedefs */
typedef unsigned short vga_screen_size_t;
typedef unsigned char vga_cursor_scanline_t;
typedef uint8_t *vga_mem_ptr_t;

/* Struct for storing cursor position */
typedef struct vga_cursor_pos {
    uint16_t x;
    uint16_t y;
} vga_cursor_pos_t;

/* Colours */
/* Foreground colours (i.e. colour of the actual text */
#define VGA_COL_FOREGROUND_BLACK 0x0
#define VGA_COL_FOREGROUND_BLUE 0x1
#define VGA_COL_FOREGROUND_GREEN 0x2
#define VGA_COL_FOREGROUND_CYAN 0x3
#define VGA_COL_FOREGROUND_RED 0x4
#define VGA_COL_FOREGROUND_MAGENTA 0x5
#define VGA_COL_FOREGROUND_BROWN 0x6
#define VGA_COL_FOREGROUND_LIGHT_GRAY 0x7
#define VGA_COL_FOREGROUND_DARK_GRAY 0x8
#define VGA_COL_FOREGROUND_LIGHT_BLUE 0x9
#define VGA_COL_FOREGROUND_LIGHT_GREEN 0xa
#define VGA_COL_FOREGROUND_LIGHT_CYAN 0xb
#define VGA_COL_FOREGROUND_LIGHT_RED 0xc
#define VGA_COL_FOREGROUND_LIGHT_MAGENTA 0xd
#define VGA_COL_FOREGROUND_YELLOW 0xe
#define VGA_COL_FOREGROUND_WHITE 0xf

/* Background colours (colour of everything but the text) */
#define VGA_COL_BACKGROUND_BLACK 0x00
#define VGA_COL_BACKGROUND_BLUE 0x10
#define VGA_COL_BACKGROUND_GREEN 0x20
#define VGA_COL_BACKGROUND_CYAN 0x30
#define VGA_COL_BACKGROUND_RED 0x40
#define VGA_COL_BACKGROUND_MAGENTA 0x50
#define VGA_COL_BACKGROUND_BROWN 0x60
#define VGA_COL_BACKGROUND_LIGHT_GRAY 0x70
#define VGA_COL_BACKGROUND_DARK_GRAY 0x80
#define VGA_COL_BACKGROUND_LIGHT_BLUE 0x90
#define VGA_COL_BACKGROUND_LIGHT_GREEN 0xa0
#define VGA_COL_BACKGROUND_LIGHT_CYAN 0xb0
#define VGA_COL_BACKGROUND_LIGHT_RED 0xc0
#define VGA_COL_BACKGROUND_LIGHT_MAGENTA 0xd0
#define VGA_COL_BACKGROUND_YELLOW 0xe0
#define VGA_COL_BACKGROUND_WHITE 0xf0

/*!
    @function vga_print
    Print null terminated string `s` to vga

    @param s
    A pointer to a null terminated string that will be printed to VGA
!*/
void vga_print(char *s);

/*!
    @function vga_print_color
    Print null terminated string `s` to vga with attributes `attr`

    @param s
    A pointer to a null terminated string that will be printed to VGA

    @param attr
    The attributes for the text to have
!*/
void vga_print_color(char *s, uint8_t attr);

/*!
    @function vga_clear_screen
    Clears the screen to black
!*/
void vga_clear_screen();

/*!
    @function vga_print_char
    Print a character to VGA

    @param c
    The ASCII character to be printed

    @param attr
    The colour attribute of the character, use the VGA colours defined above
    If 0 the function will default to white text on a black background
!*/
void vga_print_char(char c, char attr);

/*!
    @function vga_init
    Clear the screen and disable the hardware cursor
!*/
void vga_init();

/*!
    @function vga_log
    Same behaviour as vga_print, but prepends "[info] >" to the string

    @param s
    A pointer to a null terminated string that will be printed to VGA
!*/
void vga_log(char *s);

/*!
    @function vga_scroll_line
    Scrolls all lines by 1 line

    @discussion
    This is used by vga_print_char when the cursor is out of bounds, this
    shouldn't be called directly.
!*/
void vga_scroll_line();

/*!
    @function vga_print_xxx
    Print value `val` to vga with base `base`

    @param val
    The value to be printed to vga

    @param base
    The base of the val
!*/
void vga_print_u8(uint8_t val, uint8_t base);
void vga_print_u16(uint16_t val, uint8_t base);
void vga_print_u32(uint32_t val, uint8_t base);
void vga_print_u64(uint64_t val, uint8_t base);
void vga_print_i8(int8_t val, uint8_t base);
void vga_print_i16(int16_t val, uint8_t base);
void vga_print_i32(int32_t val, uint8_t base);
void vga_print_i64(int64_t val, uint8_t base);

/*!
    @function vga_print_from_address

    @param addr
    The address where to print from

    @param cnt
    The amount of bytes to print
!*/
void vga_print_from_address(mem_ptr_t addr, count_t cnt);
#endif /* _drivers_vga_h */