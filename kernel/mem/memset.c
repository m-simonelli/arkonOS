#include <mem/memset.h>
void memset(mem_ptr_u8_t addr, byte_t d, size_t size) {
    for (count_t i = 0; i < size; i++) addr[i] = d;
}