#include "demofile.h"

#include <stdint.h>
// string hex to char number
uint8_t hex_to_char(const uint8_t data[2])
{
    uint8_t number;
    number = (data[0] - ((data[0] < 58) ? 48 : 55)) * 16;
    number = number + (data[1] - ((data[0] < 58) ? 48 : 55));
    return number;
}

// char number to string hex (FF) (Only big letters!)
void char_to_hex(uint8_t retstr[2], uint8_t data) {
    const uint8_t digits[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

    if (data < 16) {
        retstr[0] = '0';
        retstr[1] = digits[data];
    } else {
        retstr[0] = digits[(data & 0xF0) >> 4];
        retstr[1] = digits[(data & 0x0F)];
    }
}