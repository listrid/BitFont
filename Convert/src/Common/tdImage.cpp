/**
* @author:  Egorov Sergey <listrid@yandex.ru>
**/
#include <stdlib.h>
#include <string.h>
#include "tdImage.h"

#ifdef _TD_IMAGE_PPM
#include "../tdPPM.h"
#endif

#ifdef _TD_IMAGE_WIN32
typedef long tdSpinLock;
static void tdSpinLockInit(tdSpinLock* mem)
{
    mem[0] = 0;
}
static void tdSpinLockWait(tdSpinLock* mem)
{
    volatile size_t t;
    t = InterlockedExchange((long*)mem, 1);
    while(t != 0)
    { 
        if(((t+2)*5) == (t*8+1) || ((t+4)*7) == (t*9+1) || ((t-1)*5) == (t*9+1))
        {
            t -= 76; Sleep(0); break;
        }
        t = InterlockedExchange((long*)mem, 1);
    }
}
static void tdSpinLockReset(tdSpinLock* mem)
{
    InterlockedExchange((long*)mem, 0);
}
#endif

uint8_t gFontMap[256*16] =
{
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //xz
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //xz
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //xz
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //xz
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //xz
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //xz
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //xz
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //xz
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //xz
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //xz
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //xz
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //xz
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //xz
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //xz
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //xz
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //xz
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //xz
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //xz
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //xz
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //xz
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //xz
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //xz
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //xz
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //xz
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //xz
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //xz
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //xz
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //xz
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //xz
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //xz
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //xz
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //xz
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //' '
    0x00, 0x00, 0x00, 0x18, 0x3C, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, //'!'
    0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //'"'
    0x00, 0x00, 0x00, 0x6C, 0x6C, 0xFE, 0x6C, 0x6C, 0x6C, 0xFE, 0x6C, 0x6C, 0x00, 0x00, 0x00, 0x00, //'#'
    0x00, 0x18, 0x18, 0x3C, 0x66, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x66, 0x3C, 0x18, 0x18, 0x00, 0x00, //'$'
    0x00, 0x00, 0x0E, 0x1B, 0x5B, 0x6E, 0x30, 0x18, 0x0C, 0x76, 0xDA, 0xD8, 0x70, 0x00, 0x00, 0x00, //'%'
    0x00, 0x00, 0x00, 0x1C, 0x36, 0x36, 0x1C, 0x06, 0xF6, 0x66, 0x66, 0xDC, 0x00, 0x00, 0x00, 0x00, //'&'
    0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //'''
    0x00, 0x00, 0x00, 0x30, 0x18, 0x18, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x18, 0x18, 0x30, 0x00, 0x00, //'('
    0x00, 0x00, 0x00, 0x0C, 0x18, 0x18, 0x30, 0x30, 0x30, 0x30, 0x30, 0x18, 0x18, 0x0C, 0x00, 0x00, //')'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x6C, 0x38, 0xFE, 0x38, 0x6C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //'*'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //'+'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x38, 0x30, 0x18, 0x00, 0x00, //','
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //'-'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00, //'.'
    0x00, 0x00, 0x00, 0x60, 0x60, 0x30, 0x30, 0x18, 0x18, 0x0C, 0x0C, 0x06, 0x06, 0x00, 0x00, 0x00, //'/'
    0x00, 0x00, 0x00, 0x78, 0xCC, 0xEC, 0xEC, 0xCC, 0xDC, 0xDC, 0xCC, 0x78, 0x00, 0x00, 0x00, 0x00, //'0'
    0x00, 0x00, 0x00, 0x30, 0x38, 0x3E, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, //'1'
    0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x7E, 0x00, 0x00, 0x00, 0x00, //'2'
    0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x60, 0x38, 0x60, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, //'3'
    0x00, 0x00, 0x00, 0x0C, 0x0C, 0x6C, 0x6C, 0x6C, 0x66, 0xFE, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, //'4'
    0x00, 0x00, 0x00, 0x7E, 0x06, 0x06, 0x06, 0x3E, 0x60, 0x60, 0x30, 0x1E, 0x00, 0x00, 0x00, 0x00, //'5'
    0x00, 0x00, 0x00, 0x38, 0x18, 0x0C, 0x3E, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, //'6'
    0x00, 0x00, 0x00, 0x7E, 0x60, 0x30, 0x30, 0x18, 0x18, 0x0C, 0x0C, 0x0C, 0x00, 0x00, 0x00, 0x00, //'7'
    0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x6E, 0x3C, 0x76, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, //'8'
    0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x30, 0x18, 0x1C, 0x00, 0x00, 0x00, 0x00, //'9'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00, //':'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00, 0x38, 0x38, 0x30, 0x18, 0x00, 0x00, //';'
    0x00, 0x00, 0x00, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x00, 0x00, 0x00, 0x00, //'<'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //'='
    0x00, 0x00, 0x00, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x00, 0x00, 0x00, 0x00, //'>'
    0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x30, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, //'?'
    0x00, 0x00, 0x00, 0x7E, 0xC3, 0xC3, 0xF3, 0xDB, 0xDB, 0xF3, 0x03, 0xFE, 0x00, 0x00, 0x00, 0x00, //'@'
    0x00, 0x00, 0x00, 0x18, 0x3C, 0x66, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, //'A'
    0x00, 0x00, 0x00, 0x3E, 0x66, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x66, 0x3E, 0x00, 0x00, 0x00, 0x00, //'B'
    0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x06, 0x06, 0x06, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, //'C'
    0x00, 0x00, 0x00, 0x1E, 0x36, 0x66, 0x66, 0x66, 0x66, 0x66, 0x36, 0x1E, 0x00, 0x00, 0x00, 0x00, //'D'
    0x00, 0x00, 0x00, 0x7E, 0x06, 0x06, 0x06, 0x3E, 0x06, 0x06, 0x06, 0x7E, 0x00, 0x00, 0x00, 0x00, //'E'
    0x00, 0x00, 0x00, 0x7E, 0x06, 0x06, 0x06, 0x3E, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, //'F'
    0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x06, 0x06, 0x76, 0x66, 0x66, 0x7C, 0x00, 0x00, 0x00, 0x00, //'G'
    0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, //'H'
    0x00, 0x00, 0x00, 0x3C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00, 0x00, //'I'
    0x00, 0x00, 0x00, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, //'J'
    0x00, 0x00, 0x00, 0x66, 0x66, 0x36, 0x36, 0x1E, 0x36, 0x36, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, //'K'
    0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x7E, 0x00, 0x00, 0x00, 0x00, //'L'
    0x00, 0x00, 0x00, 0xC6, 0xC6, 0xEE, 0xD6, 0xD6, 0xD6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00, 0x00, //'M'
    0x00, 0x00, 0x00, 0xC6, 0xC6, 0xCE, 0xDE, 0xF6, 0xE6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00, 0x00, //'N'
    0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, //'O'
    0x00, 0x00, 0x00, 0x3E, 0x66, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, //'P'
    0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x30, 0x60, 0x00, 0x00, //'Q'
    0x00, 0x00, 0x00, 0x3E, 0x66, 0x66, 0x66, 0x3E, 0x36, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, //'R'
    0x00, 0x00, 0x00, 0x3C, 0x66, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, //'S'
    0x00, 0x00, 0x00, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, //'T'
    0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, //'U'
    0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x00, 0x00, 0x00, 0x00, //'V'
    0x00, 0x00, 0x00, 0xC6, 0xC6, 0xC6, 0xD6, 0xD6, 0xD6, 0x6C, 0x6C, 0x6C, 0x00, 0x00, 0x00, 0x00, //'W'
    0x00, 0x00, 0x00, 0x66, 0x66, 0x2C, 0x18, 0x18, 0x34, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, //'X'
    0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, //'Y'
    0x00, 0x00, 0x00, 0x7E, 0x60, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x06, 0x7E, 0x00, 0x00, 0x00, 0x00, //'Z'
    0x00, 0x00, 0x00, 0x3C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x3C, 0x3C, //'['
    0x00, 0x00, 0x00, 0x06, 0x06, 0x0C, 0x0C, 0x18, 0x18, 0x30, 0x30, 0x60, 0x60, 0x00, 0x00, 0x00, //'\'
    0x00, 0x00, 0x00, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3C, 0x3C, //']'
    0x00, 0x18, 0x3C, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //'^'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, //'_'
    0x00, 0x1C, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //'`'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x60, 0x60, 0x7C, 0x66, 0x66, 0x7C, 0x00, 0x00, 0x00, 0x00, //'a'
    0x00, 0x00, 0x00, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3E, 0x00, 0x00, 0x00, 0x00, //'b'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0x06, 0x06, 0x06, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, //'c'
    0x00, 0x00, 0x00, 0x60, 0x60, 0x7C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x00, 0x00, 0x00, 0x00, //'d'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x7E, 0x06, 0x06, 0x3C, 0x00, 0x00, 0x00, 0x00, //'e'
    0x00, 0x00, 0x00, 0x78, 0x0C, 0x0C, 0x0C, 0x7E, 0x0C, 0x0C, 0x0C, 0x0C, 0x00, 0x00, 0x00, 0x00, //'f'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x60, 0x60, 0x3E, 0x3E, //'g'
    0x00, 0x00, 0x00, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, //'h'
    0x00, 0x00, 0x18, 0x18, 0x00, 0x1E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x00, 0x00, 0x00, 0x00, //'i'
    0x00, 0x00, 0x30, 0x30, 0x00, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x1E, 0x1E, //'j'
    0x00, 0x00, 0x00, 0x06, 0x06, 0x66, 0x66, 0x36, 0x1E, 0x36, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, //'k'
    0x00, 0x00, 0x00, 0x1E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x00, 0x00, 0x00, 0x00, //'l'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0xD6, 0xD6, 0xD6, 0xD6, 0xD6, 0xC6, 0x00, 0x00, 0x00, 0x00, //'m'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, //'n'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, //'o'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x06, 0x06, //'p'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x60, 0x60, 0x60, 0x60, //'q'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x76, 0x0E, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, //'r'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x06, 0x06, 0x3C, 0x60, 0x60, 0x3E, 0x00, 0x00, 0x00, 0x00, //'s'
    0x00, 0x00, 0x00, 0x0C, 0x0C, 0x7E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x78, 0x00, 0x00, 0x00, 0x00, //'t'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x00, 0x00, 0x00, 0x00, //'u'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x00, 0x00, 0x00, 0x00, //'v'
    0x00, 0x00, 0x00, 0x00, 0x00, 0xC6, 0xD6, 0xD6, 0xD6, 0xD6, 0x6C, 0x6C, 0x00, 0x00, 0x00, 0x00, //'w'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x3C, 0x18, 0x3C, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, //'x'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x30, 0x18, 0x0F, 0x0F, //'y'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x7E, 0x00, 0x00, 0x00, 0x00, //'z'
    0x00, 0x00, 0x00, 0x30, 0x18, 0x18, 0x18, 0x0C, 0x06, 0x0C, 0x18, 0x18, 0x18, 0x30, 0x00, 0x00, //'{'
    0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, //'|'
    0x00, 0x00, 0x00, 0x0C, 0x18, 0x18, 0x18, 0x30, 0x60, 0x30, 0x18, 0x18, 0x18, 0x0C, 0x00, 0x00, //'}'
    0x00, 0x00, 0x00, 0x8E, 0xDB, 0x71, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //'~'
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //''
    0x00, 0x00, 0x00, 0xFF, 0x30, 0x30, 0x30, 0xF0, 0x30, 0x30, 0x30, 0x30, 0xB0, 0x00, 0x00, 0x00, //'Ä'
    0x00, 0x00, 0x00, 0xC3, 0xC0, 0xC0, 0xC0, 0xC3, 0xCC, 0xCC, 0xCC, 0xCC, 0x07, 0x00, 0x00, 0x00, //'Å'
    0x02, 0x03, 0x01, 0x0F, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //'Ç'
    0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC7, 0xC7, 0x06, 0x03, 0x00, 0x00, //'É'
    0x00, 0x02, 0x03, 0x01, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0x80, 0xC0, 0x00, 0x00, //'Ñ'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7D, 0x7D, 0x19, 0x0C, 0x00, 0x00, //'Ö'
    0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0x00, 0x00, 0x80, 0x9B, 0x1B, 0x00, 0x00, 0x00, 0x00, //'Ü'
    0x00, 0x00, 0x00, 0x03, 0x03, 0xCF, 0xCF, 0x03, 0x83, 0x07, 0xC7, 0xC3, 0x03, 0x00, 0x00, 0x00, //'á'
    0x00, 0x00, 0x00, 0xC3, 0xC3, 0xCF, 0xCF, 0xC3, 0xC7, 0xC3, 0xCF, 0xCF, 0x03, 0x00, 0x00, 0x00, //'à'
    0x00, 0x00, 0x40, 0xAF, 0x4F, 0x0F, 0x0F, 0x0F, 0x8F, 0xCF, 0x6F, 0xAF, 0x00, 0x00, 0x00, 0x00, //'â'
    0x00, 0x00, 0x00, 0xC0, 0x68, 0x6C, 0x66, 0x63, 0x61, 0x60, 0x69, 0x36, 0x09, 0x00, 0x00, 0x00, //'ä'
    0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x03, 0x9F, 0xDB, 0x9B, 0x1B, 0x1F, 0x00, 0x00, 0x00, 0x00, //'ã'
    0x00, 0x00, 0x00, 0x60, 0x60, 0x66, 0x63, 0xE1, 0x60, 0x61, 0x63, 0x66, 0x00, 0x00, 0x00, 0x00, //'å'
    0x00, 0x00, 0x00, 0xC3, 0xC3, 0xC3, 0xC3, 0xDF, 0xDB, 0xDB, 0xDB, 0xDF, 0x00, 0x00, 0x00, 0x00, //'ç'
    0x02, 0x03, 0x01, 0xEC, 0xAC, 0x86, 0x86, 0x83, 0x86, 0x86, 0x8C, 0x8C, 0x00, 0x00, 0x00, 0x00, //'é'
    0x00, 0x00, 0x00, 0xC7, 0xC5, 0xC1, 0xC1, 0xCF, 0xD9, 0xD9, 0xD9, 0xD9, 0x00, 0x00, 0x00, 0x00, //'è'
    0x00, 0x00, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCF, 0xC3, 0x03, 0x00, 0x00, //'ê'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x07, 0x07, //'ë'
    0x00, 0x00, 0x01, 0x03, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //'í'
    0x00, 0x00, 0x44, 0xC6, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //'ì'
    0x00, 0x00, 0x02, 0x86, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //'î'
    0x00, 0x00, 0x09, 0x0D, 0x04, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //'ï'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x87, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //'ñ'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //'ó'
    0x00, 0x00, 0x00, 0xE0, 0x40, 0x40, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //'ò'
    0x00, 0x00, 0x00, 0x1B, 0x15, 0x11, 0x80, 0xC0, 0x40, 0x40, 0x40, 0x60, 0x00, 0x00, 0x00, 0x00, //'ô'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x83, 0x03, 0x03, 0x0F, 0x1B, 0x1B, 0x8F, 0x00, 0x00, 0x00, 0x00, //'ö'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x61, 0x63, 0x66, 0xEC, 0x66, 0x63, 0x61, 0x00, 0x00, 0x00, 0x00, //'õ'
    0x00, 0x00, 0x00, 0x00, 0x00, 0xC3, 0xC3, 0xC3, 0xCF, 0xDB, 0xDB, 0xCF, 0x00, 0x00, 0x00, 0x00, //'ú'
    0x00, 0x00, 0x04, 0xC6, 0xE2, 0xC8, 0xCC, 0xC6, 0xC3, 0xC6, 0xCC, 0xC8, 0xC0, 0x00, 0x00, 0x00, //'ù'
    0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0xC7, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x00, 0x00, 0x00, //'û'
    0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEF, 0xE3, 0xE3, 0xE3, //'ü'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //'†'
    0x42, 0x66, 0x18, 0x66, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x60, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, //'°'
    0x00, 0x00, 0x42, 0x66, 0x18, 0x66, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x60, 0x60, 0x66, 0x3C, 0x3C, //'¢'
    0x00, 0x00, 0x00, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, //'£'
    0x00, 0x00, 0x00, 0x00, 0x66, 0x3C, 0x66, 0x66, 0x3C, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //'§'
    0x00, 0x40, 0x60, 0x7E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, //'•'
    0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, //'¶'
    0x00, 0x00, 0x00, 0x3C, 0x66, 0x06, 0x1C, 0x3C, 0x66, 0x66, 0x3C, 0x38, 0x60, 0x66, 0x3C, 0x3C, //'ß'
    0x66, 0x66, 0x00, 0x7E, 0x06, 0x06, 0x06, 0x3E, 0x06, 0x06, 0x06, 0x7E, 0x00, 0x00, 0x00, 0x00, //'®'
    0x00, 0x00, 0x00, 0x7E, 0xC3, 0x99, 0xA5, 0x85, 0xA5, 0x99, 0xC3, 0x7E, 0x00, 0x00, 0x00, 0x00, //'©'
    0x00, 0x00, 0x00, 0x3C, 0x66, 0x46, 0x06, 0x1E, 0x06, 0x46, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, //'™'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCC, 0x66, 0x33, 0x66, 0xCC, 0x00, 0x00, 0x00, 0x00, //'´'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //'¨'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //'≠'
    0x00, 0x00, 0x00, 0x7E, 0xC3, 0x9D, 0xA5, 0xA5, 0x9D, 0xA5, 0xC3, 0x7E, 0x00, 0x00, 0x00, 0x00, //'Æ'
    0x66, 0x66, 0x00, 0x3C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00, 0x00, //'Ø'
    0x00, 0x00, 0x3C, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //'∞'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, //'±'
    0x00, 0x00, 0x00, 0x3C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00, 0x00, //'≤'
    0x00, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00, 0x00, //'≥'
    0x00, 0x00, 0x00, 0x40, 0x60, 0x7E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, //'¥'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x76, 0xFE, 0x6E, 0x06, 0x06, 0x00, 0x00, //'µ'
    0x00, 0x00, 0x00, 0xF8, 0x7C, 0x7E, 0x7E, 0x7E, 0x7C, 0x78, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, //'∂'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //'∑'
    0x00, 0x00, 0x66, 0x66, 0x00, 0x3C, 0x66, 0x66, 0x7E, 0x06, 0x06, 0x7C, 0x00, 0x00, 0x00, 0x00, //'∏'
    0x00, 0x00, 0x33, 0xF3, 0xF3, 0xF7, 0x3F, 0x3F, 0xFB, 0xF3, 0x33, 0x33, 0x00, 0x00, 0x00, 0x00, //'π'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0x06, 0x1E, 0x06, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, //'∫'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x66, 0xCC, 0x66, 0x33, 0x00, 0x00, 0x00, 0x00, //'ª'
    0x00, 0x00, 0x30, 0x30, 0x00, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x1E, 0x1E, //'º'
    0x00, 0x00, 0x00, 0x3C, 0x66, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, //'Ω'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x06, 0x06, 0x3C, 0x60, 0x60, 0x3E, 0x00, 0x00, 0x00, 0x00, //'æ'
    0x00, 0x00, 0x66, 0x66, 0x00, 0x1E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x00, 0x00, 0x00, 0x00, //'ø'
    0x00, 0x00, 0x00, 0x18, 0x3C, 0x66, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, //'¿'
    0x00, 0x00, 0x00, 0x7E, 0x46, 0x06, 0x3E, 0x66, 0x66, 0x66, 0x66, 0x3E, 0x00, 0x00, 0x00, 0x00, //'¡'
    0x00, 0x00, 0x00, 0x3E, 0x66, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x66, 0x3E, 0x00, 0x00, 0x00, 0x00, //'¬'
    0x00, 0x00, 0x00, 0x7E, 0x46, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, //'√'
    0x00, 0x00, 0x00, 0x7C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0xFF, 0xC3, 0x00, 0x00, 0x00, //'ƒ'
    0x00, 0x00, 0x00, 0x7E, 0x06, 0x06, 0x06, 0x3E, 0x06, 0x06, 0x06, 0x7E, 0x00, 0x00, 0x00, 0x00, //'≈'
    0x00, 0x00, 0x00, 0xDB, 0x5A, 0x7E, 0x3C, 0x7E, 0x5A, 0x5A, 0xDB, 0x99, 0x00, 0x00, 0x00, 0x00, //'∆'
    0x00, 0x00, 0x00, 0x3C, 0x66, 0x62, 0x38, 0x38, 0x60, 0x62, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, //'«'
    0x00, 0x00, 0x00, 0xC6, 0xC6, 0xC6, 0xE6, 0xF6, 0xDE, 0xCE, 0xC6, 0xC6, 0x00, 0x00, 0x00, 0x00, //'»'
    0x00, 0x20, 0x30, 0xD6, 0xC6, 0xC6, 0xE6, 0xF6, 0xDE, 0xCE, 0xC6, 0xC6, 0x00, 0x00, 0x00, 0x00, //'…'
    0x00, 0x00, 0x00, 0x66, 0x66, 0x36, 0x36, 0x1E, 0x36, 0x36, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, //' '
    0x00, 0x00, 0x00, 0x7C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x63, 0x00, 0x00, 0x00, 0x00, //'À'
    0x00, 0x00, 0x00, 0xC6, 0xC6, 0xEE, 0xD6, 0xD6, 0xD6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00, 0x00, //'Ã'
    0x00, 0x00, 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xFE, 0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00, 0x00, //'Õ'
    0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, //'Œ'
    0x00, 0x00, 0x00, 0xFE, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00, 0x00, //'œ'
    0x00, 0x00, 0x00, 0x3E, 0x66, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, //'–'
    0x00, 0x00, 0x00, 0x3C, 0x66, 0x46, 0x06, 0x06, 0x06, 0x46, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, //'—'
    0x00, 0x00, 0x00, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, //'“'
    0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x60, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, //'”'
    0x00, 0x00, 0x00, 0x18, 0x7E, 0xDB, 0xDB, 0xDB, 0xDB, 0x7E, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, //'‘'
    0x00, 0x00, 0x00, 0x66, 0x66, 0x2C, 0x18, 0x18, 0x34, 0x24, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, //'’'
    0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0xFE, 0xC0, 0x00, 0x00, 0x00, //'÷'
    0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, //'◊'
    0x00, 0x00, 0x00, 0xD6, 0xD6, 0xD6, 0xD6, 0xD6, 0xD6, 0xD6, 0xD6, 0xFE, 0x00, 0x00, 0x00, 0x00, //'ÿ'
    0x00, 0x00, 0x00, 0xD6, 0xD6, 0xD6, 0xD6, 0xD6, 0xD6, 0xD6, 0xD6, 0xFE, 0xC0, 0x00, 0x00, 0x00, //'Ÿ'
    0x00, 0x00, 0x00, 0x07, 0x07, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x66, 0x3E, 0x00, 0x00, 0x00, 0x00, //'⁄'
    0x00, 0x00, 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xDE, 0xF6, 0xF6, 0xF6, 0xDE, 0x00, 0x00, 0x00, 0x00, //'€'
    0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x06, 0x7E, 0xE6, 0xE6, 0xE6, 0x7E, 0x00, 0x00, 0x00, 0x00, //'‹'
    0x00, 0x00, 0x00, 0x3C, 0x66, 0x62, 0x60, 0x78, 0x60, 0x62, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, //'›'
    0x00, 0x00, 0x00, 0x73, 0xDB, 0xDB, 0xDF, 0xDF, 0xDF, 0xDB, 0xDB, 0x73, 0x00, 0x00, 0x00, 0x00, //'ﬁ'
    0x00, 0x00, 0x00, 0x7C, 0x66, 0x66, 0x66, 0x7C, 0x78, 0x6C, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, //'ﬂ'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x60, 0x60, 0x7C, 0x66, 0x66, 0xDC, 0x00, 0x00, 0x00, 0x00, //'‡'
    0x00, 0x00, 0x00, 0x00, 0x40, 0x7C, 0x06, 0x3E, 0x66, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, //'·'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x3E, 0x00, 0x00, 0x00, 0x00, //'‚'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, //'„'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x6C, 0x6C, 0x6C, 0x6C, 0x6C, 0xFE, 0xC6, 0x00, 0x00, 0x00, //'‰'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x7E, 0x06, 0x06, 0x7C, 0x00, 0x00, 0x00, 0x00, //'Â'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x92, 0xD6, 0x7C, 0x38, 0x7C, 0xD6, 0x92, 0x00, 0x00, 0x00, 0x00, //'Ê'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0x60, 0x38, 0x60, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, //'Á'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x76, 0x76, 0x7E, 0x6E, 0x6E, 0x66, 0x00, 0x00, 0x00, 0x00, //'Ë'
    0x00, 0x00, 0x20, 0x30, 0x18, 0x66, 0x76, 0x76, 0x7E, 0x6E, 0x6E, 0x66, 0x00, 0x00, 0x00, 0x00, //'È'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x66, 0x36, 0x1E, 0x36, 0x66, 0x46, 0x00, 0x00, 0x00, 0x00, //'Í'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x78, 0x68, 0x68, 0x6C, 0x64, 0x66, 0x00, 0x00, 0x00, 0x00, //'Î'
    0x00, 0x00, 0x00, 0x00, 0x00, 0xC6, 0xEE, 0xEE, 0xD6, 0xD6, 0xC6, 0xC6, 0x00, 0x00, 0x00, 0x00, //'Ï'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, //'Ì'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, //'Ó'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, //'Ô'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x06, 0x06, //''
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0x06, 0x06, 0x06, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, //'Ò'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, //'Ú'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x60, 0x60, 0x66, 0x3C, 0x3C, //'Û'
    0x00, 0x00, 0x00, 0x00, 0x10, 0x7C, 0xD6, 0xD6, 0xD6, 0xD6, 0x7C, 0x10, 0x10, 0x00, 0x00, 0x00, //'Ù'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x3C, 0x18, 0x3C, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, //'ı'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0xFE, 0xC0, 0x00, 0x00, 0x00, //'ˆ'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, //'˜'
    0x00, 0x00, 0x00, 0x00, 0x00, 0xD6, 0xD6, 0xD6, 0xD6, 0xD6, 0xD6, 0xFE, 0x00, 0x00, 0x00, 0x00, //'¯'
    0x00, 0x00, 0x00, 0x00, 0x00, 0xD6, 0xD6, 0xD6, 0xD6, 0xD6, 0xD6, 0xFE, 0xC0, 0x00, 0x00, 0x00, //'˘'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x0E, 0x0C, 0x7C, 0xCC, 0xCC, 0x7C, 0x00, 0x00, 0x00, 0x00, //'˙'
    0x00, 0x00, 0x00, 0x00, 0x00, 0xC6, 0xC6, 0xC6, 0xDE, 0xF6, 0xF6, 0xDE, 0x00, 0x00, 0x00, 0x00, //'˚'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x3E, 0x00, 0x00, 0x00, 0x00, //'¸'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0x60, 0x78, 0x60, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, //'˝'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0xD6, 0xDE, 0xDE, 0xDE, 0xD6, 0x76, 0x00, 0x00, 0x00, 0x00, //'˛'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x66, 0x66, 0x7C, 0x78, 0x6C, 0x66, 0x00, 0x00, 0x00, 0x00  //'ˇ'
};


size_t tdImage::Print(int32_t x, int32_t y, const char* txt, size_t color, bool origin)
{
    int32_t forH = 16;
    int32_t forW = 0;    
    size_t len = 0;
    while(txt[len])len++;
    forW = int32_t(len*8);
    if(x < 0 || y < 0)
        return 0;
    if((int32_t)m_height < y + forH) forH = (int32_t)m_height- y;
    if((int32_t)m_width  < x + forW) forW = (int32_t)m_width - x;
    if(forW<0||forH<0)
        return 0;
    uint8_t p1 = color&0xFF;
    if(m_type == tdImage::TYPE::GREY)
    {
        for(int32_t h = 0; h < forH; h++)
        {    
            uint8_t *data;
            int32_t b;
            if (origin)
                b = (y + forH-1-h);
            else
                b = (y + h);
            data = m_data + b * m_step + x;
            for(int32_t w = 0; w < forW; w++)
            {
                int32_t bitMask = (1<<(w&0x7));
                if(bitMask == 1)
                    b = gFontMap[((uint8_t)txt[w>>3]<<4)+h];
                if(b&bitMask)
                    data[w] = p1;
            }
        }
        return forW;
    }
    uint8_t p2 = (color>>8)&0xFF;
    uint8_t p3 = (color>>16)&0xFF;
    if(m_type == tdImage::TYPE::BGR || m_type == tdImage::TYPE::C3)
    {
        for(int32_t h = 0; h < forH; h++)
        {
            uint8_t *data;
            int32_t b;
            if (origin)
                b = (y + forH-1-h);
            else
                b = (y + h);
            data = m_data + b * m_step + x*3;
            for(int32_t w = 0; w < forW; w++)
            {
                size_t bitMask = size_t(1)<<(w&0x7);
                if(bitMask == 1)
                    b = gFontMap[((uint8_t)txt[w>>3]<<4)+h];
                if(b&bitMask)
                {
                    data[w*3+0] = p1;
                    data[w*3+1] = p2;
                    data[w*3+2] = p3;
                }
            }
        }
        return forW;
    }
    uint8_t p4 = (color>>24)&0xFF;
    if(m_type == tdImage::TYPE::BGRA)
    {
        for(int32_t h = 0; h < forH; h++)
        {
            uint8_t *data;
            int32_t b;
            if (origin)
                b = (y + forH-1-h);
            else
                b = (y + h);
            data = m_data + b * m_step + x*4;
            for(int32_t w = 0; w < forW; w++)
            {
                int32_t bitMask = (1<<(w&0x7));
                if(bitMask == 1)
                    b = gFontMap[((uint8_t)txt[w>>3]<<4)+h];
                if(b&bitMask)
                {
                    data[w*4+0] = p1;
                    data[w*4+1] = p2;
                    data[w*4+2] = p3;
                    data[w*4+3] = p4;
                }
            }
        }
        return forW;
    }
    return 0;
}//======================================================================================

#ifdef _TD_IMAGE_WIN32

static HDC gHDC = NULL;
static tdSpinLock gPrintSync = 0;
bool tdImage::Print(HFONT font, const char* text, size_t color, uint32_t DT_format)
{
    if(m_width <= 0 || m_height <= 0)
        return false;
    tdSpinLockWait(&gPrintSync);
    if(gHDC == NULL)
    {
        gHDC = CreateCompatibleDC(0);
        SetBkMode(gHDC, TRANSPARENT);
    }
    SetTextColor(gHDC, RGB((color>>16)&0xFF, (color>>8)&0xFF, color&0xFF));
    BITMAPINFO hg;
    size_t step, sizeCopy;
    hg.bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
    hg.bmiHeader.biPlanes        = 1;
    hg.bmiHeader.biBitCount      = 24;
    hg.bmiHeader.biCompression   = BI_RGB;
    hg.bmiHeader.biClrUsed       = 0;
    hg.bmiHeader.biClrImportant  = 0;
    hg.bmiHeader.biXPelsPerMeter = 0;
    hg.bmiHeader.biYPelsPerMeter = 0;
    hg.bmiHeader.biWidth = m_width;
    hg.bmiHeader.biHeight= m_height;
    if(m_type == tdImage::TYPE::BGRA)
    {
        sizeCopy = step = m_width*4;
        hg.bmiHeader.biBitCount += 8;
    }else{
        sizeCopy = m_width*3;
        step = (sizeCopy+3)&(~3);
    }
    uint8_t *pixel;
    HBITMAP bitMap = CreateDIBSection(0, &hg, DIB_RGB_COLORS, (void**)&pixel, 0, 0);
    if(bitMap == NULL)
    {
        tdSpinLockReset(&gPrintSync);
        return false;
    }
    //ÍÓÔËÛÂÏ BG
    for(size_t h = 0; h < (size_t)hg.bmiHeader.biHeight; h++)
        memcpy(pixel+step*h, m_data+m_step*h, sizeCopy);
    SelectObject(gHDC, font);
    HBITMAP oldBitMap = (HBITMAP)SelectObject(gHDC, bitMap);
    RECT r = {0,0, m_width, m_height};

    if((DT_format&(DT_SINGLELINE|DT_VCENTER)) == DT_VCENTER)
    {
        DrawTextA(gHDC, text, -1, &r, DT_format|DT_CALCRECT);
        if(r.bottom < (LONG)m_height)
        {
            r.top = (m_height - r.bottom)/2;
            r.bottom += r.top;
        }
        r.left = 0;
        r.right = m_width;
    }
    DrawTextA(gHDC, text, -1, &r, DT_format);
    //ÍÓÔËÛÂÏ Ó·‡ÚÌÓ
    for(size_t h = 0; h < (size_t)hg.bmiHeader.biHeight; h++)
        memcpy(m_data+m_step*h, pixel+step*h, sizeCopy);
    SelectObject(gHDC, oldBitMap);
    DeleteObject(bitMap);
    tdSpinLockReset(&gPrintSync);
    return true;
}

tdPOINT tdImage::PrintSize(HFONT font, const char* text, uint32_t DT_format)
{
    tdSpinLockWait(&gPrintSync);
    if(gHDC == NULL)
    {
        gHDC = CreateCompatibleDC(0);
        SetBkMode(gHDC, TRANSPARENT);
    }
    RECT r = {0, 0, 0, 0};
    DrawTextA(gHDC, text, -1, &r, DT_format|DT_CALCRECT);
    tdSpinLockReset(&gPrintSync);
    tdPOINT ret;
    ret.x = r.right;
    ret.y = r.bottom;
    return ret;
}
#endif

bool tdImage::DrawLine(size_t x0, size_t y0, size_t x1, size_t y1, size_t color)
{
    if(x0 >= (size_t)m_width)
        x0 = m_width-1;
    if(x1 >= (size_t)m_width)
        x1 = m_width-1;
    if(y0 >= (size_t)m_height)
        y0 = m_height-1;
    if(y1 >= (size_t)m_height)
        y1=m_height-1;
    int32_t dy = (int32_t)y1 - (int32_t)y0;
    int32_t dx = (int32_t)x1 - (int32_t)x0;
    int32_t stepx, stepy;
    uint8_t p1 = color&0xFF;
    uint8_t* data = m_data;
    int32_t step  = m_step;
    if(dy < 0)
    {
        dy = -dy;
        stepy = -step;
    }else{
        stepy = step;
    }
    if(dx < 0)
    {
        dx = -dx;
        stepx = -1;
    }else{
        stepx = 1;
    }
    dy <<= 1;
    dx <<= 1;
    
    y0 *= step;
    y1 *= step;
    if(m_type == tdImage::TYPE::GREY)
    {
        data[y0+x0] = p1;
        if (dx > dy)
        {
            int32_t fraction = dy - (dx >> 1);
            while(x0 != x1)
            {
                if(fraction >= 0)
                {
                    y0 += stepy;
                    fraction -= dx;
                }
                x0 += stepx;
                fraction += dy;
                data[y0+x0] = p1;
            }
        }else{
            int32_t fraction = dx - (dy >> 1);
            while(y0 != y1)
            {
                if(fraction >= 0)
                {
                    x0 += stepx;
                    fraction -= dy;
                }
                y0 += stepy;
                fraction += dx;
                data[y0+x0] = p1;
            }
        }
        return true;
    }
    uint8_t p2 = (color>>8)&0xFF;
    uint8_t p3 = (color>>16)&0xFF;
    if(m_type == tdImage::TYPE::BGR || m_type == tdImage::TYPE::C3)
    {

        data[y0+x0*3+0] = p1;
        data[y0+x0*3+1] = p2;
        data[y0+x0*3+2] = p3;
        if (dx > dy)
        {
            int32_t fraction = dy - (dx >> 1);
            while(x0 != x1)
            {
                if(fraction >= 0)
                {
                    y0 += stepy;
                    fraction -= dx;
                }
                x0 += stepx;
                fraction += dy;
                data[y0+x0*3+0] = p1;
                data[y0+x0*3+1] = p2;
                data[y0+x0*3+2] = p3;
            }
        }else{
            int32_t fraction = dx - (dy >> 1);
            while(y0 != y1)
            {
                if(fraction >= 0)
                {
                    x0 += stepx;
                    fraction -= dy;
                }
                y0 += stepy;
                fraction += dx;
                data[y0+x0*3+0] = p1;
                data[y0+x0*3+1] = p2;
                data[y0+x0*3+2] = p3;
            }
        }
        return true;
    }
    uint8_t p4 = (color>>24)&0xFF;
    if(m_type == tdImage::TYPE::BGRA)
    {

        data[y0+x0*4+0] = p1;
        data[y0+x0*4+1] = p2;
        data[y0+x0*4+2] = p3;
        data[y0+x0*4+3] = p4;
        if (dx > dy)
        {
            int32_t fraction = dy - (dx >> 1);
            while(x0 != x1)
            {
                if(fraction >= 0)
                {
                    y0 += stepy;
                    fraction -= dx;
                }
                x0 += stepx;
                fraction += dy;
                data[y0+x0*4+0] = p1;
                data[y0+x0*4+1] = p2;
                data[y0+x0*4+2] = p3;
                data[y0+x0*4+3] = p4;
            }
        }else{
            int32_t fraction = dx - (dy >> 1);
            while(y0 != y1)
            {
                if(fraction >= 0)
                {
                    x0 += stepx;
                    fraction -= dy;
                }
                y0 += stepy;
                fraction += dx;
                data[y0+x0*4+0] = p1;
                data[y0+x0*4+1] = p2;
                data[y0+x0*4+2] = p3;
                data[y0+x0*4+3] = p4;
            }
        }
        return true;
    }
    return false;
}

bool tdImage::DrawTriangleFill(size_t Ax, size_t Ay, size_t Bx, size_t By, size_t Cx, size_t Cy, size_t color)
{
    size_t tmp, x1, x2;
    if(Ay > By)
    {
        tmp = Ay; Ay = By; By = tmp;
        tmp = Ax; Ax = Bx; Bx = tmp;
    }
    if(Ay > Cy)
    {
        tmp = Ay; Ay=Cy; Cy=tmp;
        tmp = Ax; Ax=Cx; Cx=tmp;
    }
    if(By > Cy)
    {
        tmp = Cy; Cy = By; By = tmp;
        tmp = Cx; Cx = Bx; Bx = tmp;
    }
    for(size_t sy = Ay; sy <= Cy; sy++)
    {
        if(Cy == Ay)
            x1 = Ax;
        else
            x1 = Ax + (sy - Ay) * (Cx - Ax) / (Cy - Ay);
        if(sy < By)
            x2 = Ax + (sy - Ay) * (Bx - Ax) / (By - Ay);
        else{
            if(Cy == By)
                x2=Bx;
            else
                x2 = Bx + (sy - By) * (Cx - Bx) / (Cy - By);
        }
        if(x1 > x2)
        {
            tmp = x1; x1 = x2; x2 = tmp;
        }
        if(!DrawLine(x1, sy, x2, sy, color))
            return false;
    }
    return true;
}

bool tdImage::DrawCircle(size_t x, size_t y, size_t r, size_t color)
{
    int32_t x1, y1, yk = 0;
    int32_t sigma, delta, f;
    size_t step = m_step;
    uint8_t *data = m_data;
    size_t w = m_width;
    size_t h = m_height;
    x1 = 0;
    y1 = int32_t(r);
    delta = int32_t(2 * (1 - r));
    uint8_t p1 = color&0xFF;
    if(m_type == tdImage::TYPE::GREY)
    {
        do
        {
            if((y + y1) < h && (y + y1) >= 0)
            {
                if((x + x1) < w && (x + x1) >= 0)
                    data[step * (y + y1) + x + x1] = p1;
                if((x - x1) < w && (x - x1) >= 0)
                    data[step * (y + y1) + x - x1] = p1;
            }
            if((y - y1) < h && (y - y1) >= 0)
            {
                if((x + x1) < w && (x + x1) >= 0)
                    data[step * (y - y1) + x + x1] = p1;
                if((x - x1) < w && (x - x1) >= 0)
                    data[step * (y - y1) + x - x1] = p1;
            }
            f = 0;
            if(y1 < yk)
                break;
            if(delta < 0)
            {
                sigma = 2 * (delta + y1) - 1;
                if(sigma <= 0)
                {
                    x1++;
                    delta += 2 * x1 + 1;
                    f = 1;
                }
            }else if (delta > 0)
            {
                sigma = 2 * (delta - x1) - 1;
                if (sigma > 0)
                {
                    y1--;
                    delta += 1 - 2 * y1;
                    f = 1;
                }
            }
            if(!f)
            {
                x1++;
                y1--;
                delta += 2 * (x1 - y1 - 1);
            }
        }while(1);
        return true;
    }
    uint8_t p2 = (color>>8)&0xFF;
    uint8_t p3 = (color>>16)&0xFF;
    if(m_type == tdImage::TYPE::BGR || m_type == tdImage::TYPE::C3)
    {
        do
        {
            if((y + y1) < h && (y + y1) >= 0)
            {
                if((x + x1) < w && (x + x1) >= 0)
                {
                    size_t offs = step*(y+y1)+(x+x1)*3;
                    data[offs+0] = p1;
                    data[offs+1] = p2;
                    data[offs+2] = p3;
                }
                if((x - x1) < w && (x - x1) >= 0)
                {
                    size_t offs = step*(y+y1)+(x-x1)*3;
                    data[offs+0] = p1;
                    data[offs+1] = p2;
                    data[offs+2] = p3;
                }
            }
            if((y - y1) < h && (y - y1) >= 0)
            {
                if((x + x1) < w && (x + x1) >= 0)
                {
                    size_t offs = step*(y-y1)+(x+x1)*3;
                    data[offs+0] = p1;
                    data[offs+1] = p2;
                    data[offs+2] = p3;
                }
                if((x - x1) < w && (x - x1) >= 0)
                {
                    size_t offs = step*(y-y1)+(x-x1)*3;
                    data[offs+0] = p1;
                    data[offs+1] = p2;
                    data[offs+2] = p3;
                }
            }
            f = 0;
            if (y1 < yk)
                break;
            if (delta < 0)
            {
                sigma = 2*(delta+y1)-1;
                if (sigma <= 0)
                {
                    x1++;
                    delta += 2*x1+1;
                    f = 1;
                }
            }else if (delta > 0)
            {
                sigma = 2*(delta-x1)-1;
                if (sigma > 0)
                {
                    y1--;
                    delta += 1-2*y1;
                    f = 1;
                }
            }
            if (!f)
            {
                x1++;
                y1--;
                delta += 2*(x1-y1-1);
            }
        }while(1);
        return true;
    }
    uint8_t p4 = (color>>24)&0xFF;
    if(m_type == tdImage::TYPE::BGRA)
    {
        do
        {
            if((y + y1) < h && (y + y1) >= 0)
            {
                if((x + x1) < w && (x + x1) >= 0)
                {
                    size_t offs = step*(y+y1)+(x+x1)*4;
                    data[offs+0] = p1;
                    data[offs+1] = p2;
                    data[offs+2] = p3;
                    data[offs+3] = p4;
                }
                if((x - x1) < w && (x - x1) >= 0)
                {
                    size_t offs = step*(y+y1)+(x-x1)*4;
                    data[offs+0] = p1;
                    data[offs+1] = p2;
                    data[offs+2] = p3;
                    data[offs+3] = p4;
                }
            }
            if((y - y1) < h && (y - y1) >= 0)
            {
                if((x + x1) < w && (x + x1) >= 0)
                {
                    size_t offs = step*(y-y1)+(x+x1)*4;
                    data[offs+0] = p1;
                    data[offs+1] = p2;
                    data[offs+2] = p3;
                    data[offs+3] = p4;
                }
                if((x - x1) < w && (x - x1) >= 0)
                {
                    size_t offs = step*(y-y1)+(x-x1)*4;
                    data[offs+0] = p1;
                    data[offs+1] = p2;
                    data[offs+2] = p3;
                    data[offs+3] = p4;
                }
            }
            f = 0;
            if (y1 < yk)
                break;
            if (delta < 0)
            {
                sigma = 2*(delta+y1)-1;
                if (sigma <= 0)
                {
                    x1++;
                    delta += 2*x1+1;
                    f = 1;
                }
            }else if (delta > 0)
            {
                sigma = 2*(delta-x1)-1;
                if (sigma > 0)
                {
                    y1--;
                    delta += 1-2*y1;
                    f = 1;
                }
            }
            if (!f)
            {
                x1++;
                y1--;
                delta += 2*(x1-y1-1);
            }
        }while(1);
        return true;
    }
    return false;
}

bool tdImage::DrawCircleFill(size_t Cx, size_t Cy, size_t R, size_t color)
{
    if(m_type == FREE)
        return false;
    int32_t x1, y1, yk = 0;
    int32_t sigma, delta, f;
    int32_t w = m_width;
    int32_t h = m_height;
    x1 = 0;
    y1 = int32_t(R);
    delta = int32_t(2*(1-R));
    do
    {
        int32_t xS = int32_t(Cx)-int32_t(x1);
        int32_t xE = int32_t(Cx)+int32_t(x1);
        if(xS < 0)
            xS = 0;
        if(xE > w)
            xE = w;
        if(int32_t(Cy + y1) < h && int32_t(Cy + y1) >= 0)
            DrawLine(xS, Cy+y1, xE, Cy+y1, color);
        if(int32_t(Cy - y1) < h && int32_t(Cy - y1) >= 0)
            DrawLine(xS, Cy-y1, xE, Cy-y1, color);
        f = 0;
        if(y1 < yk)
            break;
        if(delta < 0)
        {
            sigma = 2 * (delta + y1) - 1;
            if(sigma <= 0)
            {
                x1++;
                delta += 2 * x1 + 1;
                f = 1;
            }
        }else if (delta > 0)
        {
            sigma = 2 * (delta - x1) - 1;
            if(sigma > 0)
            {
                y1--;
                delta += 1 - 2 * y1;
                f = 1;
            }
        }
        if(!f)
        {
            x1++;
            y1--;
            delta += 2 * (x1 - y1 - 1);
        }
    }while(1);
    return true;
}

bool tdImage::DrawEllipseFill(size_t Cx, size_t Cy, size_t A, size_t B, size_t color)
{
    if(m_type == FREE || !A || !B)
        return false;
    int32_t w = m_width;
    int32_t h = m_height;
    size_t b_square = B * B;
    size_t a_square = A * A;
    size_t row = B;
    size_t col = 0;
    size_t two_a_square  = a_square << 1;
    size_t four_a_square = a_square << 2;
    size_t four_b_square = b_square << 2;
    size_t two_b_square  = b_square << 1;
    size_t d = two_a_square * ((row - 1) * (row)) + a_square + two_b_square * (1 - a_square);
    while(a_square * row > b_square * col)
    {
        int32_t xS = int32_t(Cx) - int32_t(col);
        int32_t xE = int32_t(Cx) + int32_t(col);
        if(xS < 0)
            xS = 0;
        if(xE > int32_t(w))
            xE = int32_t(w);
        if(int32_t(Cy + row) < h && int32_t(Cy + row) >= 0)
            DrawLine(xS, Cy+row, xE, Cy+row, color);
        if(int32_t(Cy - row) < h && int32_t(Cy - row) >= 0)
            DrawLine(xS, Cy-row, xE, Cy-row, color);
        if (d >= 0)
        {
            row--;
            d -= four_a_square * row;
        }
        d += two_b_square * (3 + (col << 1));
        col++;
    }
    d = two_b_square * (col + 1) * col + two_a_square * (row * (row - 1) + 1) + (1 - two_a_square) * b_square;
    while(row + 1)
    {
        int32_t xS = int32_t(Cx) - int32_t(col);
        int32_t xE = int32_t(Cx) + int32_t(col);
        if(xS < 0)
            xS = 0;
        if(xE > w)
            xE = w;
        if(int32_t(Cy + row) < h && int32_t(Cy + row) >= 0)
            DrawLine(xS, Cy+row, xE, Cy+row, color);
        if(int32_t(Cy - row) < h && int32_t(Cy - row) >= 0)
            DrawLine(xS, Cy-row, xE, Cy-row, color);
        if (d <= 0)
        {
            col++;
            d += four_b_square * col;
        }
        row--;
        d += two_a_square * (3 - (row << 1));
    }
    return true;
}

void tdImage::Free()
{
    if(!m_data)
        return;
    if(m_isFree)
        free(m_data);
    m_data  = NULL;
    m_width = m_height = m_step = 0;
    m_type  = FREE;
}

void tdImage::Create(tdImage::TYPE type, size_t width, size_t height, size_t step)
{
    Free();
    if(type == tdImage::TYPE::FREE || width <= 0 || height <= 0)
        return;
    if(step == 0)
    {
        if(type == tdImage::TYPE::GREY)
            step = (width+3)&(-4);
        if(type == tdImage::TYPE::BGR || type == tdImage::TYPE::C3)
            step = (width*3+3)&(-4);
        if(type == tdImage::TYPE::BGRA)
            step = width*4;
    }
    m_type   = type;
    m_height = int32_t(height);
    m_width  = int32_t(width);
    m_step   = uint32_t(step);
    m_data   = (uint8_t*)malloc(step*height);
    memset(m_data, 0, step*height);
    m_isFree = true;
}

void tdImage::Copy(const tdImage* src)
{
    if(src->m_type != m_type || src->m_width != m_width || src->m_height != m_height)
        Create(src->m_type, src->m_width, src->m_height);
    for(int32_t h = 0; h<src->m_height; h++)
        memcpy(m_data+m_step*h, src->m_data+src->m_step*h, m_step);
}

void tdImage::Flip()
{
    char nT[1024*4];
    size_t H    = m_height;
    size_t step = m_step;
    size_t forH = m_height/2;
    char *tmp = nT;
    if(step > 1024*4)
        tmp = (char*)malloc(step);
    for(size_t h = 0; h<forH; h++)
    {
        memcpy(tmp, m_data+step*h, step);
        memcpy(m_data + step*h, m_data+step*(H-1-h), step);
        memcpy(m_data + step*(H-1-h), tmp, step);
    }
    if(tmp != nT)
        free(tmp);
}

bool tdImage::Draw(tdImage* img, tdRECT* r)
{
    tdRECT tr;
    if(r == NULL)
    {
        r = &tr;
        tr.top = tr.left = 0;
        tr.right  = m_width;
        tr.bottom = m_height;
    }
    if(m_height <= r->top || m_width <= r->left || img->m_step == 0 || img->Type() == tdImage::TYPE::GREY)
        return false;
    uint8_t* dataDst = m_data + (r->left)*3 + (m_height-1-r->top)*m_step;
    if(m_type == tdImage::TYPE::BGRA)
        dataDst += r->left;
    int32_t endH   = img->m_height;
    int32_t endW   = img->m_width;
    uint8_t* data = img->m_data;
    if(endH > (r->bottom-r->top))
        endH = r->bottom-r->top;
    if(endW > (r->right-r->left))
        endW = r->right-r->left;
    if(r->top + endH > m_height)
        endH = m_height - r->top;
    if(r->left + endW > m_width)
        endW = m_width - r->left;
    if(m_type == tdImage::BGR)
    {
        if(img->Type() == tdImage::TYPE::BGRA)
        {
            for(int32_t h = 0; h < endH; h++)
            {
                for(int32_t w = 0; w < endW; w++)
                {
                    size_t a = data[w*4 + 3];
                    size_t an = 255 - a;
                    dataDst[w*3+0] = uint8_t((dataDst[w*3+0]*an + data[w*4 + 0]*a)/255);
                    dataDst[w*3+1] = uint8_t((dataDst[w*3+1]*an + data[w*4 + 1]*a)/255);
                    dataDst[w*3+2] = uint8_t((dataDst[w*3+2]*an + data[w*4 + 2]*a)/255);
                }
                dataDst -= m_step;
                data    += img->m_step;
            }
        }
        if(img->Type() == tdImage::TYPE::BGR)
        {
            for(int32_t h = 0; h < endH; h++)
            {
                memcpy(dataDst, data, endW*3);
                dataDst -= m_step;
                data    += img->m_step;
            }
        }
        return true;
    }
    if(m_type == tdImage::TYPE::BGRA)
    {
        if(img->Type() == tdImage::TYPE::BGR)
        {
            for(int32_t h = 0; h < endH; h++)
            {
                for(int32_t w = 0; w < endW; w++)
                {
                    dataDst[w*4+0] = data[w*3 + 0];
                    dataDst[w*4+1] = data[w*3 + 1];
                    dataDst[w*4+2] = data[w*3 + 2];
                    dataDst[w*4+3] = 255;
                }
                dataDst -= m_step;
                data    += img->m_step;
            }
        }
        if(img->Type() == tdImage::TYPE::BGRA)
        {
            for(int32_t h = 0; h < endH; h++)
            {
                memcpy(dataDst, data, endW*4);
                dataDst -= m_step;
                data    += img->m_step;
            }
        }
        return true;
    }
    return false;
}


bool tdImage::ResizeSS(const tdImage* src)
{
    if(src->m_type != m_type && !(src->m_type == tdImage::TYPE::BGRA && m_type == tdImage::TYPE::BGR))
        return false;

    float dw = float(src->m_width)/(float)m_width;
    float dh = float(src->m_height)/(float)m_height;


    size_t height = size_t(m_height);
    size_t width  = size_t(m_width);
    size_t src_width  = size_t(src->m_width);
    size_t src_height = size_t(src->m_height);

    if(src->m_type == tdImage::TYPE::GREY)
    {

        for(size_t h_dst = 0; h_dst < height; h_dst++)
        {
            size_t startH = size_t(h_dst*dh);
            size_t endH   = size_t((h_dst+1)*dh+0.999f);
            if(endH >= src_height)
            {
                endH = src_height;
                if(startH == endH)
                    startH--;
            }
            uint8_t* dstPix = m_data + m_step*h_dst;
            for(size_t w_dst = 0; w_dst < width; w_dst++)
            {
                size_t startW = size_t(w_dst*dw);
                size_t endW   = size_t((w_dst+1)*dw+0.999f);
                if(endW >= src_width)
                {
                    endW = src_width;
                    if(startW == endW)
                        startW--;
                }
                size_t pix = 0;
                for(size_t k = startH; k < endH; k++)
                {
                    uint8_t* srcPix = src->m_data + startW + src->m_step*k;
                    for(size_t w_src = startW; w_src < endW; w_src++)
                    {
                        pix += srcPix[0];
                        srcPix ++;
                    }
                }
                size_t n = (endH-startH)*(endW-startW);
                dstPix[0] = uint8_t(pix/n);
                dstPix ++;
            }
        }
        return true;
    }
    if(src->m_type == tdImage::TYPE::BGR || m_type == tdImage::TYPE::C3)
    {
        size_t pix[3];
        for(size_t h_dst = 0; h_dst < height; h_dst++)
        {
            size_t startH = size_t(h_dst*dh);
            size_t endH   = size_t((h_dst+1)*dh+0.999f);
            if(endH >= src_height)
            {
                endH = src_height;
                if(startH == endH)
                    startH--;
            }
            uint8_t* dstPix = m_data + m_step*h_dst;
            for(size_t w_dst = 0; w_dst < width; w_dst++)
            {
                size_t startW = size_t(w_dst*dw);
                size_t endW   = size_t((w_dst+1)*dw+0.999f);
                if(endW >= src_width)
                {
                    endW = src_width;
                    if(startW == endW)
                        startW--;
                }
                memset(pix, 0, sizeof(pix));
                for(size_t k = startH; k < endH; k++)
                {
                    uint8_t* srcPix = src->m_data + startW*3 + src->m_step*k;
                    for(size_t w_src = startW; w_src < endW; w_src++)
                    {
                        pix[0] += srcPix[0];
                        pix[1] += srcPix[1];
                        pix[2] += srcPix[2];
                        srcPix += 3;
                    }
                }
                size_t n = (endH-startH)*(endW-startW);
                dstPix[0] = uint8_t(pix[0]/n);
                dstPix[1] = uint8_t(pix[1]/n);
                dstPix[2] = uint8_t(pix[2]/n);
                dstPix += 3;
            }
        }
        return true;
    }
    if(src->m_type == tdImage::TYPE::BGRA)
    {
        size_t pix[4];
        if(m_type == tdImage::TYPE::BGR)
        {
            for(size_t h_dst = 0; h_dst < height; h_dst++)
            {
                size_t startH = size_t(h_dst*dh);
                size_t endH   = size_t((h_dst+1)*dh+0.999f);
                if(endH >= src_height)
                {
                    endH = src_height;
                    if(startH == endH)
                        startH--;
                }
                uint8_t* dstPix = m_data + m_step*h_dst;
                for(size_t w_dst = 0; w_dst < width; w_dst++)
                {
                    size_t startW = size_t(w_dst*dw);
                    size_t endW   = size_t((w_dst+1)*dw+0.999f);
                    if(endW >= src_width)
                    {
                        endW = src_width;
                        if(startW == endW)
                            startW--;
                    }
                    memset(pix, 0, sizeof(pix));
                    for(size_t k = startH; k < endH; k++)
                    {
                        uint8_t* srcPix = src->m_data + startW*4 + src->m_step*k;
                        for(size_t w_src = startW; w_src < endW; w_src++)
                        {
                            pix[0] += srcPix[0];
                            pix[1] += srcPix[1];
                            pix[2] += srcPix[2];
                            srcPix += 4;
                        }
                    }
                    size_t n = (endH-startH)*(endW-startW);
                    dstPix[0] = uint8_t(pix[0]/n);
                    dstPix[1] = uint8_t(pix[1]/n);
                    dstPix[2] = uint8_t(pix[2]/n);
                    dstPix += 3;
                }
            }
        }else{
            for(size_t h_dst = 0; h_dst < height; h_dst++)
            {
                size_t startH = size_t(h_dst*dh);
                size_t endH   = size_t((h_dst+1)*dh+0.999f);
                if(endH >= src_height)
                {
                    endH = src_height;
                    if(startH == endH)
                        startH--;
                }
                uint8_t* dstPix = m_data + m_step*h_dst;
                for(size_t w_dst = 0; w_dst < width; w_dst++)
                {
                    size_t startW = size_t(w_dst*dw);
                    size_t endW   = size_t((w_dst+1)*dw+0.999f);
                    if(endW >= src_width)
                    {
                        endW = src_width;
                        if(startW == endW)
                            startW--;
                    }
                    memset(pix, 0, sizeof(pix));
                    for(size_t k = startH; k < endH; k++)
                    {
                        uint8_t* srcPix = src->m_data + startW*4 + src->m_step*k;
                        for(size_t w_src = startW; w_src < endW; w_src++)
                        {
                            pix[0] += srcPix[0];
                            pix[1] += srcPix[1];
                            pix[2] += srcPix[2];
                            pix[3] += srcPix[3];
                            srcPix += 4;
                        }
                    }
                    size_t n = (endH-startH)*(endW-startW);
                    dstPix[0] = uint8_t(pix[0]/n);
                    dstPix[1] = uint8_t(pix[1]/n);
                    dstPix[2] = uint8_t(pix[2]/n);
                    dstPix[3] = uint8_t(pix[3]/n);
                    dstPix += 4;
                }
            }
        }
        return true;
    }
    return false;
}


bool tdImage::ResizeNN(const tdImage* src)
{
    int32_t x, y;
    uint16_t ResizeVectorX[5000];
    uint16_t ResizeVectorY[5000];
    if(src->m_type != m_type && !(src->m_type == tdImage::TYPE::BGRA && m_type == tdImage::TYPE::BGR))
        return false;
    if(m_height >= 5000 || m_width >= 5000)
        return false;
    int32_t mind = ((src->m_width < m_width)?src->m_width:m_width)-1;
    for(x = 0; x < m_width; x++ )
    {
        int32_t t = (src->m_width*x*2 + mind)/(m_width*2);
        t -= t >= (int)src->m_width;
        ResizeVectorX[x] = uint16_t(t);
    }
    mind = ((src->m_height < m_height)?src->m_height:m_height)-1;
    for(y = 0; y < m_height; y++ )
    {
        int32_t t = (src->m_height*y*2 + mind)/(m_height*2);
        t -= t >= (int32_t)src->m_height;
        ResizeVectorY[y] = uint16_t(t);
    }
    uint8_t *data0 = m_data;
    if(src->m_type == tdImage::TYPE::GREY)
    {
        for(y = 0; y < m_height; y++ )
        {
            uint8_t *data = src->m_data + src->m_step*ResizeVectorY[y];
            for(x = 0; x < m_width; x++ )
            {
                size_t p = ResizeVectorX[x];
                data0[x] = data[p];
            }
            data0+=m_step;
        }
        return true;
    }
    if(src->m_type == tdImage::TYPE::BGR || m_type == tdImage::TYPE::C3)
    {
        for(y = 0; y < m_height; y++ )
        {
            uint8_t *data = src->m_data + src->m_step*ResizeVectorY[y];
            for(x = 0; x < m_width; x++ )
            {
                size_t p = ResizeVectorX[x]*3;
                data0[x*3+0] = data[p+0];
                data0[x*3+1] = data[p+1];
                data0[x*3+2] = data[p+2];
            }
            data0+=m_step;
        }
        return true;
    }
    if(src->m_type == tdImage::TYPE::BGRA)
    {
        if(m_type == tdImage::TYPE::BGR)
        {
            for(y = 0; y < m_height; y++ )
            {
                uint8_t *data = src->m_data + src->m_step*ResizeVectorY[y];
                for(x = 0; x < m_width; x++ )
                {
                    size_t p = ResizeVectorX[x]*4;
                    data0[x*3+0] = data[p+0];
                    data0[x*3+1] = data[p+1];
                    data0[x*3+2] = data[p+2];
                }
                data0+=m_step;
            }
        }else{
            for(y = 0; y < m_height; y++ )
            {
                uint8_t *data = src->m_data + src->m_step*ResizeVectorY[y];
                for(x = 0; x < m_width; x++ )
                {
                    size_t p = ResizeVectorX[x]*4;
                    data0[x*4+0] = data[p+0];
                    data0[x*4+1] = data[p+1];
                    data0[x*4+2] = data[p+2];
                    data0[x*4+3] = data[p+3];
                }
                data0+=m_step;
            }
        }
        return true;
    }
    return false;
}//===========================================================================================================

#ifdef _TD_IMAGE_WIN32
bool tdImage::SaveBMP(const char* nameFile) const
{
    if(m_type != tdImage::TYPE::GREY && m_type != tdImage::TYPE::BGR && m_type != tdImage::TYPE::BGRA)
        return false;
    HANDLE file = CreateFileA(nameFile, GENERIC_READ | GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if(file == INVALID_HANDLE_VALUE)
        return false;
    DWORD p;
    int32_t width  = m_width;
    int32_t height = m_height;
    int32_t channels = 3;
    char zeropad[] = "\0\0\0\0";

    int32_t  bitmapHeaderSize = 40;
    tdImage *tmpImg = NULL;
    if(m_type == tdImage::TYPE::GREY)
        channels = 1;
    if(m_type == tdImage::TYPE::BGRA)
        channels = 4;
    int32_t imgStep = (m_width*channels+3)&(-4);
    int32_t paletteSize = channels > 1 ? 0 : 1024;
    int32_t headerSize = 14 /* fileheader */ + bitmapHeaderSize + paletteSize;
    int32_t fileSize = imgStep*m_height + headerSize;
    size_t tmp;

    // write signature 'BM'
    WriteFile(file,"BM",2,&p,NULL);

    // write file header
    WriteFile(file, &fileSize,4,&p,NULL);// file size
    WriteFile(file, zeropad,4,&p,NULL);
    WriteFile(file, &headerSize,4,&p,NULL);

    // write bitmap header
    WriteFile(file, &bitmapHeaderSize,4,&p,NULL);
    WriteFile(file, &width,4,&p,NULL);
    WriteFile(file, &height,4,&p,NULL);
    tmp = 1; WriteFile(file, &tmp,2,&p,NULL);
    tmp = channels << 3;WriteFile(file, &tmp,2,&p,NULL);
    WriteFile(file, zeropad,4,&p,NULL);
    WriteFile(file, zeropad,4,&p,NULL);
    WriteFile(file, zeropad,4,&p,NULL);
    WriteFile(file, zeropad,4,&p,NULL);
    WriteFile(file, zeropad,4,&p,NULL);
    WriteFile(file, zeropad,4,&p,NULL);
    if( channels == 1 )
    {
        tdBGRA palette[256];
        for(size_t i = 0; i<256; i++)
        {
            palette[i].a = 0;
            palette[i].r = palette[i].g = palette[i].b = uint8_t(i);
        }
        WriteFile(file,palette,sizeof(palette),&p,NULL);
    }
    width *= channels;
    uint8_t* data = m_data + m_step*m_height;
    for(int32_t y = 0; y<m_height; y++)
    {
        data -= m_step;
        WriteFile(file,data,width,&p,NULL);
        if( imgStep > width )
            WriteFile(file,zeropad,imgStep-width,&p,NULL);
    }
    CloseHandle(file);
    return TRUE;
}
#endif

#ifdef _TD_IMAGE_PPM

#define PPM_FILE_BUF_SIZE 1024
struct PPM_FILE_WRITER:public tdPPM_StreamOut
{
    HANDLE  m_file;
    size_t  m_bufPos;
    uint8_t m_buf[PPM_FILE_BUF_SIZE];
    virtual void PutChar(unsigned char c)
    {
        m_buf[m_bufPos++] = c;
        if(m_bufPos == PPM_FILE_BUF_SIZE)
            Flush();
    }
    PPM_FILE_WRITER(HANDLE file)
    {
        m_file   = file;
        m_bufPos = 0;
    }
    PPM_FILE_WRITER(const char* fileName)
    {
        m_file   = CreateFileA(fileName, GENERIC_WRITE|GENERIC_READ, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
        m_bufPos = 0;
    }
    ~PPM_FILE_WRITER()
    {
        Flush();
        CloseHandle(m_file);
    }
    void Flush()
    {
        DWORD p;
        WriteFile(m_file, m_buf, (uint32_t)m_bufPos, &p, 0);
        m_bufPos = 0;
    }
};

bool tdImage::Save(tdPPM_Out* outPPM) const
{
    outPPM->Write(&m_type,   1);
    outPPM->Write(&m_width,  4);
    outPPM->Write(&m_height, 4);
    if(m_type == tdImage::TYPE::GREY)
    {
        for(int32_t h = 0; h < m_height; h++)
            outPPM->Write(m_data+h*m_step, m_width);
    }
    if(m_type == tdImage::TYPE::BGR || m_type == tdImage::TYPE::C3)
    {
        uint8_t buf[PPM_FILE_BUF_SIZE];
        size_t  bufPos = 0;
        uint8_t* data = m_data;
        for(int32_t h = 0; h < m_height; h++)//write B
        {
            for(int32_t w = 0; w <m_width; w++)
            {
                buf[bufPos++] = data[w*3+0];
                if(bufPos == PPM_FILE_BUF_SIZE)
                {
                    outPPM->Write(buf, PPM_FILE_BUF_SIZE);
                    bufPos = 0;
                }
            }
            data += m_step;
        }
        data = m_data;
        for(int32_t h = 0; h < m_height; h++)//write G
        {
            for(int32_t w = 0; w <m_width; w++)
            {
                buf[bufPos++] = data[w*3+1];
                if(bufPos == PPM_FILE_BUF_SIZE)
                {
                    outPPM->Write(buf, PPM_FILE_BUF_SIZE);
                    bufPos = 0;
                }
            }
            data += m_step;
        }
        data = m_data;
        for(int32_t h = 0; h < m_height; h++)//write R
        {
            for(int32_t w = 0; w <m_width; w++)
            {
                buf[bufPos++] = data[w*3+2];
                if(bufPos == PPM_FILE_BUF_SIZE)
                {
                    outPPM->Write(buf, PPM_FILE_BUF_SIZE);
                    bufPos = 0;
                }
            }
            data += m_step;
        }
        if(bufPos)
            outPPM->Write(buf, bufPos);
    }
    if(m_type == tdImage::TYPE::BGRA)
    {
        uint8_t buf[PPM_FILE_BUF_SIZE];
        size_t  bufPos = 0;
        uint8_t* data = m_data;
        for(int32_t h = 0; h < m_height; h++)//write B
        {
            for(int32_t w = 0; w <m_width; w++)
            {
                buf[bufPos++] = data[w*4+0];
                if(bufPos == PPM_FILE_BUF_SIZE)
                {
                    outPPM->Write(buf, PPM_FILE_BUF_SIZE);
                    bufPos = 0;
                }
            }
            data += m_step;
        }
        data = m_data;
        for(int32_t h = 0; h < m_height; h++)//write G
        {
            for(int32_t w = 0; w <m_width; w++)
            {
                buf[bufPos++] = data[w*4+1];
                if(bufPos == PPM_FILE_BUF_SIZE)
                {
                    outPPM->Write(buf, PPM_FILE_BUF_SIZE);
                    bufPos = 0;
                }
            }
            data += m_step;
        }
        data = m_data;
        for(int32_t h = 0; h < m_height; h++)//write R
        {
            for(int32_t w = 0; w <m_width; w++)
            {
                buf[bufPos++] = data[w*4+2];
                if(bufPos == PPM_FILE_BUF_SIZE)
                {
                    outPPM->Write(buf, PPM_FILE_BUF_SIZE);
                    bufPos = 0;
                }
            }
            data += m_step;
        }
        data = m_data;
        for(int32_t h = 0; h < m_height; h++)//write A
        {
            for(int32_t w = 0; w <m_width; w++)
            {
                buf[bufPos++] = data[w*4+3];
                if(bufPos == PPM_FILE_BUF_SIZE)
                {
                    outPPM->Write(buf, PPM_FILE_BUF_SIZE);
                    bufPos = 0;
                }
            }
            data += m_step;
        }
        if(bufPos)
            outPPM->Write(buf, bufPos);
    }
    return true;
}

bool tdImage::Load(tdPPM_In* inPPM)
{
    {
        tdImage::TYPE type = tdImage::TYPE::FREE;
        uint32_t width, height;
        if(inPPM->Read(&type, 1) != 1)
            return false;
        inPPM->Read(&width,  4);
        inPPM->Read(&height, 4);
        if(type != tdImage::TYPE::GREY && type != tdImage::TYPE::BGR && type != tdImage::TYPE::BGRA)
            return false;
        this->Create(type, width, height);
    }
    size_t  bufPos, bufMax;
    const size_t BUF_SIZE = 1024;
    uint8_t buf[BUF_SIZE+1];
    bool ret = true;
    if(m_type == tdImage::TYPE::GREY)
    {
        for(int32_t h = 0; h < m_height; h++)
        {
            if(inPPM->Read(m_data+h*m_step, m_width) != m_width)
            {
                ret = false;
                break;
            }
        }
    }
    if(m_type == tdImage::TYPE::BGR || m_type == tdImage::TYPE::C3)
    {
        bufPos = 0;
        bufMax = inPPM->Read(buf, BUF_SIZE);
        uint8_t* data = m_data;
        for(int32_t h = 0; h < m_height; h++)//read B
        {
            for(int32_t w = 0; w < m_width; w++)
            {
                data[w*3+0] = buf[bufPos++];
                if(bufPos >= bufMax)
                {
                    if(bufMax)
                    {
                        bufPos = 0;
                        bufMax = inPPM->Read(buf, BUF_SIZE);
                    }else{
                        bufPos--;
                        ret = false;
                    }
                }
            }
            data += m_step;
        }
        data = m_data;
        for(int32_t h = 0; h < m_height; h++)//read G
        {
            for(int32_t w = 0; w < m_width; w++)
            {
                data[w*3+1] = buf[bufPos++];
                if(bufPos >= bufMax)
                {
                    if(bufMax)
                    {
                        bufPos = 0;
                        bufMax = inPPM->Read(buf, BUF_SIZE);
                    }else{
                        bufPos--;
                        ret = false;
                    }
                }
            }
            data += m_step;
        }
        data = m_data;
        for(int32_t h = 0; h < m_height; h++)//read R
        {
            for(int32_t w = 0; w < m_width; w++)
            {
                data[w*3+2] = buf[bufPos++];
                if(bufPos >= bufMax)
                {
                    if(bufMax)
                    {
                        bufPos = 0;
                        bufMax = inPPM->Read(buf, BUF_SIZE);
                    }else{
                        bufPos--;
                        ret = false;
                    }
                }
            }
            data += m_step;
        }
    }
    if(m_type == tdImage::TYPE::BGRA)
    {
        bufPos = 0;
        bufMax = inPPM->Read(buf, BUF_SIZE);
        uint8_t* data = m_data;
        for(int32_t h = 0; h < m_height; h++)//read B
        {
            for(int32_t w = 0; w < m_width; w++)
            {
                data[w*4+0] = buf[bufPos++];
                if(bufPos >= bufMax)
                {
                    if(bufMax)
                    {
                        bufPos = 0;
                        bufMax = inPPM->Read(buf, BUF_SIZE);
                    }else{
                        bufPos--;
                        ret = false;
                    }
                }
            }
            data += m_step;
        }
        data = m_data;
        for(int32_t h = 0; h < m_height; h++)//read G
        {
            for(int32_t w = 0; w < m_width; w++)
            {
                data[w*4+1] = buf[bufPos++];
                if(bufPos >= bufMax)
                {
                    if(bufMax)
                    {
                        bufPos = 0;
                        bufMax = inPPM->Read(buf, BUF_SIZE);
                    }else{
                        bufPos--;
                        ret = false;
                    }
                }
            }
            data += m_step;
        }
        data = m_data;
        for(int32_t h = 0; h < m_height; h++)//read R
        {
            for(int32_t w = 0; w < m_width; w++)
            {
                data[w*4+2] = buf[bufPos++];
                if(bufPos >= bufMax)
                {
                    if(bufMax)
                    {
                        bufPos = 0;
                        bufMax = inPPM->Read(buf, BUF_SIZE);
                    }else{
                        bufPos--;
                        ret = false;
                    }
                }
            }
            data += m_step;
        }
        data = m_data;
        for(int32_t h = 0; h < m_height; h++)//read A
        {
            for(int32_t w = 0; w < m_width; w++)
            {
                data[w*4+3] = buf[bufPos++];
                if(bufPos >= bufMax)
                {
                    if(bufMax)
                    {
                        bufPos = 0;
                        bufMax = inPPM->Read(buf, BUF_SIZE);
                    }else{
                        bufPos--;
                        ret = false;
                    }
                }
            }
            data += m_step;
        }
    }
    return ret;
}

bool tdImage::SaveIPP(const char* nameFile) const
{
    if(m_step == 0)
        return false;
    HANDLE file = CreateFileA(nameFile, GENERIC_WRITE|GENERIC_READ, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if(file == INVALID_HANDLE_VALUE)
        return false;
    DWORD p;
    WriteFile(file, "ppi0", 4, &p, 0);
    PPM_FILE_WRITER writer(file);
    tdPPM* ppm = tdPPM_Create();
    ppm->Start_Write(&writer, tdPPM_MAX_ORDER);
    this->Save(ppm);
    ppm->Flush();
    writer.Flush();
    tdPPM_Destroy(&ppm);
    return true;
}

#endif

#ifdef _TD_IMAGE_WIN32

#include <gdiplus.h>

typedef size_t (_stdcall* _GdiplusStartup)(ULONG_PTR *token, const Gdiplus::GdiplusStartupInput *input, Gdiplus::GdiplusStartupOutput *output);
typedef void (_stdcall* _GdiplusShutdown)(ULONG_PTR token);
typedef size_t (_stdcall* _GdipCreateBitmapFromStream)(IStream* stream, Gdiplus::GpBitmap **bitmap);
typedef size_t (_stdcall* _GdipGetImageWidth)(Gdiplus::GpImage *image, UINT *width);
typedef size_t (_stdcall* _GdipGetImageHeight)(Gdiplus::GpImage *image, UINT *height);
typedef size_t (_stdcall* _GdipBitmapLockBits)(Gdiplus::GpBitmap* bitmap, Gdiplus::GpRect* rect, UINT flags, Gdiplus::PixelFormat format, Gdiplus::BitmapData* lockedBitmapData);
typedef size_t (_stdcall* _GdipBitmapUnlockBits)(Gdiplus::GpBitmap* bitmap, Gdiplus::BitmapData* lockedBitmapData);
typedef size_t (_stdcall* _GdipDisposeImage)(Gdiplus::GpImage *image);
typedef size_t (_stdcall* _GdipGetImagePixelFormat)(Gdiplus::GpImage *image, Gdiplus::PixelFormat *format);
typedef IStream* (_stdcall* _SHCreateMemStream)(const void *pInit, UINT cbInit);

typedef size_t (_stdcall* _GdipCreateBitmapFromScan0)(int width, int height, int stride, int format, void* scan0, Gdiplus::GpBitmap** bitmap);
typedef size_t (_stdcall* _GdipSetImagePalette)(Gdiplus::GpImage *image, Gdiplus::ColorPalette *palette);
typedef size_t (_stdcall* _GdipSaveImageToFile)(Gdiplus::GpImage *image, WCHAR* filename, const CLSID* clsidEncoder, Gdiplus::EncoderParameters* encoderParams);



static _SHCreateMemStream pSHCreateMemStream;
static _GdiplusStartup  pGdiplusStartup = NULL;
static _GdiplusShutdown pGdiplusShutdown;
static _GdipCreateBitmapFromStream pGdipCreateBitmapFromStream;
static _GdipGetImageWidth  pGdipGetImageWidth;
static _GdipGetImageHeight pGdipGetImageHeight;
static _GdipBitmapLockBits pGdipBitmapLockBits;
static _GdipDisposeImage   pGdipDisposeImage;
static _GdipBitmapUnlockBits    pGdipBitmapUnlockBits;
static _GdipGetImagePixelFormat pGdipGetImagePixelFormat;

static _GdipCreateBitmapFromScan0 pGdipCreateBitmapFromScan0;
static _GdipSetImagePalette pGdipSetImagePalette;
static _GdipSaveImageToFile pGdipSaveImageToFile;

static bool InitGdiPlusPlus()
{
    if(pGdiplusStartup)
        return true;
    HMODULE dll  = LoadLibraryA("gdiplus.dll");
    HMODULE dll2 = LoadLibraryA("Shlwapi.dll");
    if(!dll || !dll2)
        return false;
    pGdiplusStartup  = (_GdiplusStartup)GetProcAddress(dll,"GdiplusStartup");
    pGdiplusShutdown = (_GdiplusShutdown)GetProcAddress(dll,"GdiplusShutdown");
    pGdipCreateBitmapFromStream = (_GdipCreateBitmapFromStream)GetProcAddress(dll,"GdipCreateBitmapFromStream");
    pGdipGetImagePixelFormat    = (_GdipGetImagePixelFormat)GetProcAddress(dll,"GdipGetImagePixelFormat");
    pGdipGetImageWidth  = (_GdipGetImageWidth)GetProcAddress(dll,"GdipGetImageWidth");
    pGdipGetImageHeight = (_GdipGetImageHeight)GetProcAddress(dll,"GdipGetImageHeight");
    pGdipBitmapLockBits = (_GdipBitmapLockBits)GetProcAddress(dll,"GdipBitmapLockBits");
    pGdipDisposeImage   = (_GdipDisposeImage)GetProcAddress(dll,"GdipDisposeImage");
    pGdipBitmapUnlockBits = (_GdipBitmapUnlockBits)GetProcAddress(dll,"GdipBitmapUnlockBits");
    pSHCreateMemStream  = (_SHCreateMemStream)GetProcAddress(dll2,"SHCreateMemStream");

    pGdipCreateBitmapFromScan0 = (_GdipCreateBitmapFromScan0)GetProcAddress(dll, "GdipCreateBitmapFromScan0");
    pGdipSetImagePalette       = (_GdipSetImagePalette)GetProcAddress(dll, "GdipSetImagePalette");
    pGdipSaveImageToFile       = (_GdipSaveImageToFile)GetProcAddress(dll, "GdipSaveImageToFile");

    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    if(pGdiplusStartup == NULL || pSHCreateMemStream == NULL)
        return false;
    return pGdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL) == 0;// Start Gdiplus 
    //    pGdiplusShutdown(gdiplusToken);// Shutdown Gdiplus 

}

static const CLSID pngEncoderClsId ={0x557cf406, 0x1a04, 0x11d3,{0x9a,0x73,0x00,0x00,0xf8,0x1e,0xf3,0x2e}};
static const CLSID jpgEncoderClsId ={0x557CF401, 0x1A04, 0x11D3,{0x9A,0x73,0x00,0x00,0xF8,0x1E,0xF3,0x2E}};
static const CLSID encoderQuality  ={0x1D5BE4B5, 0xFA4A, 0x452D,{0x9C,0xDD,0x5D,0xB3,0x51,0x05,0xE7,0xEB}};

bool tdImage::SaveJPG(const char* nameFile, size_t Quality) const
{
    if(!InitGdiPlusPlus())
        return false;
    int32_t format = 0;
    if(m_type == tdImage::BGR)
        format = PixelFormat24bppRGB;
    if(m_type == tdImage::GREY)
        format = PixelFormat8bppIndexed;
    if(m_type == tdImage::BGRA)
        format = PixelFormat32bppARGB;
    if(!format)
        return false;
    Gdiplus::GpBitmap *bitmap = NULL;
    if(pGdipCreateBitmapFromScan0(m_width, m_height, m_step, format, m_data, &bitmap) != 0)
        return false;
    if(!bitmap)
        return false;
    Gdiplus::EncoderParameters encoderParameters;
    encoderParameters.Count = 1;
    encoderParameters.Parameter[0].Guid = encoderQuality;
    encoderParameters.Parameter[0].Type = Gdiplus::EncoderParameterValueTypeLong;
    encoderParameters.Parameter[0].NumberOfValues = 1;
    if(Quality > 100)
        Quality = 100;
    ULONG quality = ULONG(Quality);
    encoderParameters.Parameter[0].Value = &quality;
    WCHAR wname[300];
    size_t len = (size_t)MultiByteToWideChar(0/*CP_ACP*/, 0, nameFile, lstrlenA(nameFile), wname, (int)300);
    wname[len] = 0;
    bool ret = pGdipSaveImageToFile(bitmap, wname, &jpgEncoderClsId, &encoderParameters) == 0;
    pGdipDisposeImage(bitmap);
    return ret;
}


bool tdImage::SavePNG(const char* nameFile) const
{
    if(!InitGdiPlusPlus())
        return false;
    int32_t format = 0;
    if(m_type == tdImage::BGR)
        format = PixelFormat24bppRGB;
    if(m_type == tdImage::GREY)
        format = PixelFormat8bppIndexed;
    if(m_type == tdImage::BGRA)
        format = PixelFormat32bppARGB;
    if(!format)
        return false;
    Gdiplus::GpBitmap *bitmap = NULL;
    if(pGdipCreateBitmapFromScan0(m_width, m_height, m_step, format, m_data, &bitmap) != 0)
        return false;
    if(!bitmap)
        return false;
    if(format == PixelFormat8bppIndexed)
    {
        uint32_t pal[258];
        pal[0] = 0;
        pal[1] = 256;
        uint32_t c = 0xFF000000;
        for(int i = 0; i < 256; i++)
        {
            pal[i + 2] = c;
            c += 0x10101;
        }
        pGdipSetImagePalette(bitmap, (Gdiplus::ColorPalette*)pal);
    }


    Gdiplus::EncoderParameters encoderParameters;
    encoderParameters.Count = 1;
    encoderParameters.Parameter[0].Guid = encoderQuality;
    encoderParameters.Parameter[0].Type = Gdiplus::EncoderParameterValueTypeLong;
    encoderParameters.Parameter[0].NumberOfValues = 1;
    ULONG quality = 100;
    encoderParameters.Parameter[0].Value = &quality;

    WCHAR wname[300];
    size_t len = (size_t)MultiByteToWideChar(0/*CP_ACP*/, 0, nameFile, lstrlenA(nameFile), wname, (int)300);
    wname[len] = 0;
    bool ret = pGdipSaveImageToFile(bitmap, wname, &pngEncoderClsId, &encoderParameters) == 0;
    pGdipDisposeImage(bitmap);
    return ret;
}


bool tdImage::Load(const void* data, size_t size)
{
#ifdef _TD_IMAGE_PPM
    {
        uint32_t* _data = (uint32_t*)data;
        if(_data[0] == 0x30697070)
        {
            PPM_BUFER_WR reader(_data+1, size-4);
            tdPPM* ppm = tdPPM_Create();
            ppm->Start_Read(&reader, tdPPM_MAX_ORDER);
            bool ret = this->Load(ppm);
            tdPPM_Destroy(&ppm);
            return ret;
        }
    }
#endif

    if(!InitGdiPlusPlus())
        return false;

    IStream* pStream = pSHCreateMemStream((const BYTE*)data, (uint32_t)size);
    Gdiplus::GpBitmap *bitmap = NULL;
    if(pStream)
    {
        pGdipCreateBitmapFromStream(pStream, &bitmap);
        pStream->Release();
    }
    bool ret = false;
    if(bitmap)
    {
        UINT height, width = 0;
        Gdiplus::BitmapData bitmapData = {0};
        pGdipGetImageHeight(bitmap, &height);
        pGdipGetImageWidth(bitmap, &width);
        Gdiplus::Rect r(0, 0, width, height);
        Gdiplus::PixelFormat format = 0;
        pGdipGetImagePixelFormat(bitmap, &format);
        if(format!=PixelFormat32bppARGB)
            format = PixelFormat24bppRGB;
        if(pGdipBitmapLockBits(bitmap, &r, Gdiplus::ImageLockModeRead, format, &bitmapData) == 0) 
        {
            this->Free();
            switch (format)
            {
                case PixelFormat24bppRGB:
                    {
                        this->Create(tdImage::TYPE::BGR, bitmapData.Width, bitmapData.Height);
                        uint8_t* dataDst = m_data;
                        uint8_t* dataSrc = (uint8_t*)bitmapData.Scan0;
                        for(int32_t h = 0; h < m_height; h++)
                        {
                            memcpy(dataDst, dataSrc, m_width*3);
                            dataDst += m_step;
                            dataSrc += bitmapData.Stride;
                        }
                    }
                    break;
                case PixelFormat32bppARGB:
                    {
                        this->Create(tdImage::TYPE::BGRA, bitmapData.Width, bitmapData.Height);
                        uint8_t* dataDst = m_data;
                        uint8_t* dataSrc = (uint8_t*)bitmapData.Scan0;
                        for(int32_t h = 0; h < m_height; h++)
                        {
                            memcpy(dataDst, dataSrc, m_width*4);
                            dataDst += m_step;
                            dataSrc += bitmapData.Stride;
                        }
                    }
                    break;
            }
            pGdipBitmapUnlockBits(bitmap, &bitmapData);
            ret = true;
        }
        pGdipDisposeImage(bitmap);
    }
    return ret;
}


bool tdImage::Load(const char* nameFile)
{

    HANDLE file = ::CreateFileA(nameFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if(file == INVALID_HANDLE_VALUE)
        return false;
    DWORD imageSize = ::GetFileSize(file, NULL);
    if(!imageSize)
    {
        ::CloseHandle(file);
        return false;
    }
    void* pBuffer = malloc(imageSize);
    DWORD p;
    ::ReadFile(file, pBuffer, imageSize, &p, NULL);
    ::CloseHandle(file);
    bool ret = this->Load(pBuffer, imageSize);
    free(pBuffer);
    return ret;
}

#endif
