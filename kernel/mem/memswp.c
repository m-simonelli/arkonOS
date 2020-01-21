#include <mem/memswp.h>
void memswp(mem_ptr_u8_t m1, mem_ptr_u8_t m2, size_t size) {
    byte_t tmp;
    for (count_t i = 0; i < size; i++) {
        tmp = *((mem_ptr_u8_t)((mem_addr_t)m1 + i));
        *((mem_ptr_u8_t)((mem_addr_t)m1 + i)) =
            *((mem_ptr_u8_t)((mem_addr_t)m2 + i));
        *((mem_ptr_u8_t)((mem_addr_t)m2 + i)) = tmp;
    }
}