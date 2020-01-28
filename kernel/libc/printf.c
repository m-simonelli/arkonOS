#include <libc/printf.h>

#define LEAD_ZERO       0x1
#define LONG_NUM        0x2
#define HALF_NUM        0x4
#define SIZE_T_NUM      0x8
#define LONG_LONG_NUM   0x10
#define HALF_HALF_NUM   0x20
#define ALT_FLAG        0x40

void vsprintf(__attribute__((unused))char *str, void (*putchar)(char), const char *fmt, va_list ap){
    count_t i;
    size_t n;
    char nbuf[32];
    char c;
    unsigned char uc;
    const char *s;
    int flags = 0;
    size_t npad_bytes = 0;
    for(;;){
        while((c = *fmt++) != 0){
            /* Break on format character*/
            if(c == '%') break;
            /* If not a format character, just print it */
            if(c == '\t'){
                putchar(' ');
                putchar(' ');
                putchar(' ');
                putchar(' ');
            } else {
                putchar(c);
            }
        }
        if(c == 0) break;

next_format_char:
        i = 0;
        while(i++ < 31) nbuf[i] = 0;
        /* Sanity check that the character after the '%' exists */
        if ((c = *fmt++) == 0) break;
        switch(c) {
            /* %0 - %9 are treated as amount of padding 0's */
            case '0':
                flags |= LEAD_ZERO;
                goto next_format_char;
            case '1':
                /* fall through */
            case '2':
                /* fall through */
            case '3':
                /* fall through */
            case '4':
                /* fall through */
            case '5':
                /* fall through */
            case '6':
                /* fall through */
            case '7':
                /* fall through */
            case '8':
                /* fall through */
            case '9':
                npad_bytes = (c - '0');
                goto next_format_char;
            case '*':
                npad_bytes = va_arg(ap, int);
                goto next_format_char;
            case '.':
                goto next_format_char;
            case '%':
                putchar(c);
                break;
            case '#':
                flags |= ALT_FLAG;
                goto next_format_char;
            case 'c':
                uc = (unsigned char) va_arg(ap, int);
                putchar(uc);
                break;
            case 's':
                s = va_arg(ap, const char *);
                if(s == 0) s = "<null>";
                for(i = 0; s[i] != 0; i++) putchar(s[i]);
                break;
            case 'l':
                if (flags & LONG_NUM) flags |= LONG_LONG_NUM;
                flags |= LONG_NUM;
                goto next_format_char;
            case 'h':
                if (flags & HALF_NUM) flags |= HALF_HALF_NUM;
                flags |= HALF_NUM;
                goto next_format_char;
            case 'z':
                flags |= SIZE_T_NUM;
                goto next_format_char;
            case 'i':
                /* fall through */
            case 'd':
                i = 0;
                n = (flags & LONG_LONG_NUM) ? (size_t)va_arg(ap, long long) :
					(flags & LONG_NUM) ? (size_t)va_arg(ap, long) : 
					(flags & HALF_HALF_NUM) ? (size_t)(signed char)va_arg(ap, int) :
					(flags & HALF_NUM) ? (size_t)(short)va_arg(ap, int) :
					(flags & SIZE_T_NUM) ? va_arg(ap, size_t) :
                    (size_t)va_arg(ap, int);
                do_itoa(n, (char *)&nbuf, 10, 1);
                if(npad_bytes > strlen((char *)&nbuf)){
                    npad_bytes -= strlen((char *)&nbuf);
                    for(; npad_bytes > 0; npad_bytes--) putchar('0');
                }
                for(i = 0; nbuf[i] != 0; i++) putchar(nbuf[i]);
                flags = 0;
                npad_bytes = 0;
                break;
            case 'u':
                i = 0;
                n = (flags & LONG_LONG_NUM) ? (size_t)va_arg(ap, unsigned long long) :
					(flags & LONG_NUM) ? (size_t)va_arg(ap, unsigned long) : 
					(flags & HALF_HALF_NUM) ? (size_t)(signed char)va_arg(ap, unsigned int) :
					(flags & HALF_NUM) ? (size_t)(short)va_arg(ap, unsigned int) :
					(flags & SIZE_T_NUM) ? va_arg(ap, size_t) :
                    (size_t)va_arg(ap, int);
                do_itoa(n, (char *)&nbuf, 10, 0);
                if(npad_bytes > strlen((char *)&nbuf)){
                    npad_bytes -= strlen((char *)&nbuf);
                    for(; npad_bytes > 0; npad_bytes--) putchar('0');
                }
                for(i = 0; nbuf[i] != 0; i++) putchar(nbuf[i]);
                flags = 0;
                npad_bytes = 0;
                break;
            case 'p':
                flags |= LONG_NUM | ALT_FLAG;
                goto hex;
            case 'X':
                /* Not implemented */
                /* fall through */
hex:
            case 'x':
                n = (flags & LONG_LONG_NUM) ? (size_t)va_arg(ap, unsigned long long) :
					(flags & LONG_NUM) ? (size_t)va_arg(ap, unsigned long) : 
					(flags & HALF_HALF_NUM) ? (size_t)(signed char)va_arg(ap, unsigned int) :
					(flags & HALF_NUM) ? (size_t)(short)va_arg(ap, unsigned int) :
					(flags & SIZE_T_NUM) ? va_arg(ap, size_t) :
                    (size_t)va_arg(ap, unsigned int);
                do_itoa(n, (char *)&nbuf, 16, 0);
                if (flags & ALT_FLAG){
                    putchar('0');
                    putchar('x');
                }
                if(npad_bytes > strlen((char *)&nbuf)){
                    npad_bytes -= strlen((char *)&nbuf);
                    for(; npad_bytes > 0; npad_bytes--) putchar('0');
                }
                for(i = 0; nbuf[i] != 0; i++) putchar(nbuf[i]);
                flags = 0;
                npad_bytes = 0;
                break;
            case 'I':   /* IP Addresses */
                /* Not implemented */
                /* fall through */
            case 'M':   /* Ethernet MAC addresses */
                /* Not implemented */
                /* fall through */
            case 'n':
                /* Not implemented */
                /* fall through */
            default:
                putchar('%');
                putchar(c);
                break;
        }
    }
}