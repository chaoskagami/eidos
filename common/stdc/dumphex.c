#include <string.h>
#include <stdint.h>

int dumphex(char* buf, const char* index, unsigned int num) {
    uint8_t *num_8 = (uint8_t *)&num;
    char *obuf = buf;
    for (int i = 3; i >= 0; i--) {
        uint8_t high = (num_8[i] >> 4) & 0xf;
        uint8_t low = num_8[i] & 0xf;

        *buf = (index)[high]; buf++;
        *buf = (index)[low];  buf++;
    }

    return buf - obuf;
}

