#include <mem/memcpy.h>
void *memcpy(void *to, const void *from, size_t len) {
    for (count_t i = 0; i < len; i++)
        *((mem_ptr_u8_t)((mem_addr_t)to + i)) =
            *((mem_ptr_u8_t)((mem_addr_t)from + i));
    return to;
}