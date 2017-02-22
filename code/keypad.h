 /*
 * Keypad Driver
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 
 keypad pin
 0 x x x x x x x x 0 <--- positions on the keypad board
 1 2 3 4 5 6 7 8   <--- pins used from left to right
 PC0 ...................          x         <--- PC0 to pin 5    (activates row 1)
 PC1 ...................            x       <--- PC1 to pin 6    (activates row 2)
 PC2 ...................              x     <--- PC2 to pin 7    (activates row 3)
 PC3 ...................                x   <--- PC3 to pin 8    (activates row 4)
 PC4 ...................   x                <--- PC4 to pin 1    (activates column 1)
 PC5 ...................     x              <--- PC5 to pin 2    (activates column 2)
 PC6 ...................       x            <--- PC6 to pin 3    (activates column 3)
 PC7 ...................         x          <--- PC7 to pin 4    (activates column 4)
 
 PC4 PC5 PC6 PC7
 
 PC0   1 | 2 | 3 | A   <--- row 1
 PC1   4 | 5 | 6 | B   <--- row 2
 PC2   7 | 8 | 9 | C   <--- row 3
 PC3   * | 0 | # | D   <--- row 4
 
 ^   ^   ^   ^
 |   |   |   |
 col  1   2   3   4
 */

#ifndef KEYPAD_H
#define KEYPAD_H

#include "bit.h"

unsigned char GetKeypadKey()
{
	PORTC = 0xEF;
	asm("nop");
if (GetBit(PINC, 0) == 0) { return('1'); }
		if (GetBit(PINC, 1) == 0) { return('4'); }
		if (GetBit(PINC, 2) == 0) { return('7'); }
		if (GetBit(PINC, 3) == 0) { return('*'); }
         
    PORTC = 0xDF;
    asm("nop");
		if (GetBit(PINC, 0) == 0) { return('2'); }
		if (GetBit(PINC, 1) == 0) { return('5'); }
		if (GetBit(PINC, 2) == 0) { return('8'); }
		if (GetBit(PINC, 3) == 0) { return('0'); }
         
    PORTC = 0xBF;
    asm("nop");
		if (GetBit(PINC, 0) == 0) { return('3'); }
		if (GetBit(PINC, 1) == 0) { return('6'); }
		if (GetBit(PINC, 2) == 0) { return('9'); }
		if (GetBit(PINC, 3) == 0) { return('#'); }
         
    PORTC = 0x07;
    asm("nop");
		if (GetBit(PINC, 0) == 0) { return('A'); }
		if (GetBit(PINC, 1) == 0) { return('B'); }
		if (GetBit(PINC, 2) == 0) { return('C'); }
		if (GetBit(PINC, 3) == 0) { return('D'); }
         
    return('\0');
}

#endif //KEYPAD_H
