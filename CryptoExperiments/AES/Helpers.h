#ifndef HELPERS_H
#define HELPERS_H

#define BYTE_N(x, n) ((x >> 8 * n) & 0x000000FFU)
#define BYTE0(x) BYTE_N(x, 0)
#define BYTE1(x) BYTE_N(x, 1)
#define BYTE2(x) BYTE_N(x, 2)
#define BYTE3(x) BYTE_N(x, 3)

#define CROTATE_LEFT(x, cnt) ((x << cnt) | (x >> (32 - cnt)))

unsigned int ConstructUint(unsigned char b3, unsigned char b2, unsigned char b1, unsigned char b0);

#endif // HELPERS_H