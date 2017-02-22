//  bit.h

#ifndef _bit_h
#define _bit_h

// Bit-access functions
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b)
{
    return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k)
{
    return ((x & (0x01 << k)) != 0);
}

#endif
