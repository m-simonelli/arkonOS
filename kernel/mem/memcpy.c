#include <mem/memcpy.h>
void memcpy(mem_ptr_u8_t dst, const mem_ptr_t src, size_t size) {
    for (count_t i = 0; i < size; i++)
        *((mem_ptr_u8_t)((mem_addr_t)dst + i)) =
            *((mem_ptr_u8_t)((mem_addr_t)src + i));
}