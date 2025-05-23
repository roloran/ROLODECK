/*******************************************************************************
 * Size: 16 px
 * Bpp: 1
 * Opts: --bpp 1 --size 16 --font C:/Users/i31ewoho/Documents/Spaces/iTransfer/ROLORAN-TDeck-Neuhaus/assets/fonts/HackNerdFont-Regular.ttf -o C:/Users/i31ewoho/Documents/Spaces/iTransfer/ROLORAN-TDeck-Neuhaus/assets/fonts\ui_font_Hack16.c --format lvgl -r 0x20-0xff --no-compress --no-prefilter
 ******************************************************************************/

#include "ui.h"

#ifndef UI_FONT_HACK16
#define UI_FONT_HACK16 1
#endif

#if UI_FONT_HACK16

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xff, 0xfd, 0xf,

    /* U+0022 "\"" */
    0xde, 0xf7, 0xb0,

    /* U+0023 "#" */
    0x9, 0x82, 0x60, 0x90, 0x24, 0x7f, 0xc4, 0xc1,
    0x20, 0x48, 0xff, 0x89, 0x82, 0x41, 0x90,

    /* U+0024 "$" */
    0x10, 0x21, 0xf6, 0x8d, 0x1a, 0x1e, 0xe, 0x16,
    0x2e, 0x5f, 0xe1, 0x2, 0x0,

    /* U+0025 "%" */
    0x70, 0x44, 0x22, 0x11, 0x7, 0x38, 0xe3, 0x9c,
    0x11, 0x8, 0x84, 0x41, 0xc0,

    /* U+0026 "&" */
    0x1c, 0x10, 0x8, 0x4, 0x3, 0x3, 0x81, 0x27,
    0x9b, 0xc5, 0xe3, 0x99, 0xc7, 0xb0,

    /* U+0027 "'" */
    0xff,

    /* U+0028 "(" */
    0x32, 0x66, 0xcc, 0xcc, 0xcc, 0x46, 0x63, 0x10,

    /* U+0029 ")" */
    0xc4, 0x66, 0x33, 0x33, 0x33, 0x26, 0x6c, 0x80,

    /* U+002A "*" */
    0x10, 0x20, 0x47, 0xf3, 0x85, 0x11, 0x22,

    /* U+002B "+" */
    0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18, 0x18,

    /* U+002C "," */
    0x6c, 0x9c,

    /* U+002D "-" */
    0xfc,

    /* U+002E "." */
    0xf0,

    /* U+002F "/" */
    0x2, 0xc, 0x10, 0x60, 0x83, 0x4, 0x18, 0x20,
    0xc1, 0x6, 0xc, 0x0,

    /* U+0030 "0" */
    0x3c, 0x66, 0x42, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb,
    0xc3, 0x42, 0x66, 0x3c,

    /* U+0031 "1" */
    0x31, 0xcf, 0x2c, 0x30, 0xc3, 0xc, 0x30, 0xc3,
    0x3f,

    /* U+0032 "2" */
    0xf9, 0x18, 0x18, 0x30, 0x61, 0x87, 0xc, 0x30,
    0xc3, 0x7, 0xf0,

    /* U+0033 "3" */
    0x7c, 0x7, 0x3, 0x3, 0x6, 0x1c, 0x6, 0x3,
    0x3, 0x3, 0x86, 0xfc,

    /* U+0034 "4" */
    0xe, 0xe, 0x16, 0x16, 0x26, 0x66, 0x46, 0xc6,
    0xff, 0x6, 0x6, 0x6,

    /* U+0035 "5" */
    0x7e, 0x60, 0x60, 0x60, 0x7c, 0x6, 0x3, 0x3,
    0x3, 0x3, 0x86, 0xfc,

    /* U+0036 "6" */
    0x3e, 0x60, 0x60, 0xc0, 0xdc, 0xe6, 0xc3, 0xc3,
    0xc3, 0xc3, 0x66, 0x3c,

    /* U+0037 "7" */
    0xfe, 0xc, 0x10, 0x60, 0xc1, 0x6, 0xc, 0x10,
    0x60, 0xc1, 0x0,

    /* U+0038 "8" */
    0x3c, 0xe7, 0xc3, 0xc3, 0x66, 0x3c, 0x66, 0xc3,
    0xc3, 0xc3, 0xe6, 0x3c,

    /* U+0039 "9" */
    0x3c, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0x67, 0x3b,
    0x3, 0x6, 0x6, 0x7c,

    /* U+003A ":" */
    0xf0, 0x3c,

    /* U+003B ";" */
    0xdb, 0x0, 0x3, 0x65, 0xe0,

    /* U+003C "<" */
    0x1, 0x83, 0x8f, 0x1c, 0xe, 0x1, 0xe0, 0x1e,
    0x3,

    /* U+003D "=" */
    0xff, 0x0, 0x0, 0xff,

    /* U+003E ">" */
    0x80, 0x78, 0x7, 0x0, 0x70, 0x78, 0xe3, 0xc1,
    0x0,

    /* U+003F "?" */
    0x7d, 0xc, 0x18, 0x30, 0xc3, 0xc, 0x18, 0x0,
    0x0, 0xc1, 0x80,

    /* U+0040 "@" */
    0x1e, 0x10, 0x90, 0x39, 0xf9, 0x9c, 0x86, 0x43,
    0x33, 0x8f, 0xa0, 0x10, 0x6, 0x1, 0xf0,

    /* U+0041 "A" */
    0xc, 0xe, 0x7, 0x82, 0xc1, 0x21, 0x90, 0xcc,
    0x46, 0x7f, 0x30, 0xd0, 0x78, 0x10,

    /* U+0042 "B" */
    0xfc, 0xc6, 0xc6, 0xc6, 0xc6, 0xf8, 0xc6, 0xc3,
    0xc3, 0xc3, 0xc7, 0xfc,

    /* U+0043 "C" */
    0x1f, 0x61, 0x60, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
    0xc0, 0x60, 0x61, 0x1f,

    /* U+0044 "D" */
    0xf8, 0xc6, 0xc6, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
    0xc3, 0xc6, 0xc6, 0xf8,

    /* U+0045 "E" */
    0xff, 0x83, 0x6, 0xc, 0x1f, 0xf0, 0x60, 0xc1,
    0x83, 0x7, 0xf0,

    /* U+0046 "F" */
    0xff, 0x83, 0x6, 0xc, 0x1f, 0xf0, 0x60, 0xc1,
    0x83, 0x6, 0x0,

    /* U+0047 "G" */
    0x1e, 0x62, 0x60, 0xc0, 0xc0, 0xc0, 0xc7, 0xc3,
    0xc3, 0x63, 0x63, 0x1e,

    /* U+0048 "H" */
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3,
    0xc3, 0xc3, 0xc3, 0xc3,

    /* U+0049 "I" */
    0xfc, 0xc3, 0xc, 0x30, 0xc3, 0xc, 0x30, 0xc3,
    0x3f,

    /* U+004A "J" */
    0x3e, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x83, 0x6,
    0xe, 0x3f, 0xe0,

    /* U+004B "K" */
    0xc3, 0x61, 0x31, 0x19, 0x8d, 0x87, 0xc3, 0xa1,
    0x98, 0xc6, 0x63, 0x30, 0xd8, 0x30,

    /* U+004C "L" */
    0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
    0xc0, 0xc0, 0xc0, 0xff,

    /* U+004D "M" */
    0xe3, 0xf1, 0xf8, 0xfa, 0xbd, 0x5e, 0xaf, 0x27,
    0x93, 0xc1, 0xe0, 0xf0, 0x78, 0x30,

    /* U+004E "N" */
    0xe3, 0xe3, 0xe3, 0xf3, 0xd3, 0xd3, 0xcb, 0xcb,
    0xcf, 0xc7, 0xc7, 0xc7,

    /* U+004F "O" */
    0x3c, 0x66, 0xc2, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
    0xc3, 0xc3, 0x66, 0x3c,

    /* U+0050 "P" */
    0xfc, 0xc6, 0xc3, 0xc3, 0xc3, 0xc6, 0xfc, 0xc0,
    0xc0, 0xc0, 0xc0, 0xc0,

    /* U+0051 "Q" */
    0x3c, 0x33, 0x30, 0x98, 0x6c, 0x36, 0x1b, 0xd,
    0x86, 0xc3, 0x21, 0x19, 0x87, 0x80, 0x40, 0x18,
    0x6,

    /* U+0052 "R" */
    0xf8, 0xcc, 0xc6, 0xc6, 0xc6, 0xcc, 0xf8, 0xc4,
    0xc4, 0xc6, 0xc3, 0xc3,

    /* U+0053 "S" */
    0x3e, 0x62, 0xc0, 0xc0, 0xe0, 0x7c, 0x3e, 0x7,
    0x3, 0x3, 0x86, 0xfc,

    /* U+0054 "T" */
    0xff, 0x8c, 0x6, 0x3, 0x1, 0x80, 0xc0, 0x60,
    0x30, 0x18, 0xc, 0x6, 0x3, 0x0,

    /* U+0055 "U" */
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
    0xc3, 0xc3, 0x66, 0x3c,

    /* U+0056 "V" */
    0x41, 0xa0, 0xd8, 0x4c, 0x22, 0x31, 0x18, 0xc8,
    0x24, 0x16, 0xf, 0x7, 0x1, 0x80,

    /* U+0057 "W" */
    0xc0, 0xf0, 0x2c, 0x9, 0x32, 0x4c, 0x97, 0x65,
    0x59, 0x5e, 0x73, 0x1c, 0xc3, 0x30, 0x8c,

    /* U+0058 "X" */
    0x61, 0x90, 0x8c, 0xc2, 0x41, 0xc0, 0x60, 0x30,
    0x3c, 0x32, 0x11, 0x98, 0x78, 0x10,

    /* U+0059 "Y" */
    0x40, 0x98, 0x62, 0x10, 0xcc, 0x1e, 0x7, 0x80,
    0xc0, 0x30, 0xc, 0x3, 0x0, 0xc0, 0x30,

    /* U+005A "Z" */
    0xff, 0x3, 0x6, 0x4, 0xc, 0x18, 0x10, 0x30,
    0x20, 0x60, 0xc0, 0xff,

    /* U+005B "[" */
    0xfe, 0x31, 0x8c, 0x63, 0x18, 0xc6, 0x31, 0x8c,
    0x63, 0xe0,

    /* U+005C "\\" */
    0xc0, 0x40, 0x60, 0x20, 0x30, 0x10, 0x18, 0x8,
    0xc, 0xc, 0x4, 0x6, 0x2,

    /* U+005D "]" */
    0xf8, 0xc6, 0x31, 0x8c, 0x63, 0x18, 0xc6, 0x31,
    0x8f, 0xe0,

    /* U+005E "^" */
    0x18, 0x38, 0x2c, 0x46, 0x83,

    /* U+005F "_" */
    0xff,

    /* U+0060 "`" */
    0x46, 0x30,

    /* U+0061 "a" */
    0x7c, 0x3, 0x3, 0x7f, 0xe3, 0xc3, 0xc3, 0xc7,
    0x7b,

    /* U+0062 "b" */
    0xc1, 0x83, 0x7, 0xee, 0xd8, 0xf1, 0xe3, 0xc7,
    0x8f, 0xb6, 0xe0,

    /* U+0063 "c" */
    0x3e, 0xc7, 0x6, 0xc, 0x18, 0x30, 0x31, 0x3e,

    /* U+0064 "d" */
    0x6, 0xc, 0x1b, 0xb6, 0xf8, 0xf1, 0xe3, 0xc7,
    0x8d, 0xbb, 0xb0,

    /* U+0065 "e" */
    0x3c, 0x66, 0xc3, 0xc3, 0xff, 0xc0, 0xc0, 0x61,
    0x3e,

    /* U+0066 "f" */
    0x1e, 0x60, 0xc7, 0xf3, 0x6, 0xc, 0x18, 0x30,
    0x60, 0xc1, 0x80,

    /* U+0067 "g" */
    0x76, 0xdf, 0x1e, 0x3c, 0x78, 0xf1, 0xb7, 0x76,
    0xd, 0x33, 0xe0,

    /* U+0068 "h" */
    0xc1, 0x83, 0x6, 0xee, 0x78, 0xf1, 0xe3, 0xc7,
    0x8f, 0x1e, 0x30,

    /* U+0069 "i" */
    0x30, 0x60, 0x7, 0x83, 0x6, 0xc, 0x18, 0x30,
    0x60, 0xc0, 0xf0,

    /* U+006A "j" */
    0x18, 0xc0, 0xf1, 0x8c, 0x63, 0x18, 0xc6, 0x31,
    0x8f, 0xc0,

    /* U+006B "k" */
    0xc0, 0xc0, 0xc0, 0xc6, 0xcc, 0xd8, 0xf0, 0xf8,
    0xcc, 0xc4, 0xc6, 0xc3,

    /* U+006C "l" */
    0xf0, 0x60, 0xc1, 0x83, 0x6, 0xc, 0x18, 0x30,
    0x60, 0xc0, 0xf0,

    /* U+006D "m" */
    0xf6, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb,
    0xdb,

    /* U+006E "n" */
    0xdd, 0xcf, 0x1e, 0x3c, 0x78, 0xf1, 0xe3, 0xc6,

    /* U+006F "o" */
    0x3c, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x66,
    0x3c,

    /* U+0070 "p" */
    0xdd, 0xdb, 0x1e, 0x3c, 0x78, 0xf1, 0xf6, 0xfd,
    0x83, 0x6, 0x0,

    /* U+0071 "q" */
    0x76, 0xdf, 0x1e, 0x3c, 0x78, 0xf1, 0xb7, 0x76,
    0xc, 0x18, 0x30,

    /* U+0072 "r" */
    0xdf, 0x8c, 0x30, 0xc3, 0xc, 0x30, 0xc0,

    /* U+0073 "s" */
    0x7d, 0x83, 0x7, 0x7, 0xc1, 0xc1, 0xc3, 0xfc,

    /* U+0074 "t" */
    0x10, 0x60, 0xc7, 0xf3, 0x6, 0xc, 0x18, 0x30,
    0x60, 0xc0, 0xf0,

    /* U+0075 "u" */
    0xc7, 0x8f, 0x1e, 0x3c, 0x78, 0xf1, 0xe7, 0x76,

    /* U+0076 "v" */
    0x83, 0xc2, 0x46, 0x46, 0x64, 0x2c, 0x2c, 0x38,
    0x18,

    /* U+0077 "w" */
    0xc0, 0xf0, 0x24, 0x9, 0x36, 0x4d, 0x9d, 0x47,
    0x50, 0xcc, 0x33, 0x0,

    /* U+0078 "x" */
    0xc2, 0x66, 0x2c, 0x38, 0x18, 0x38, 0x64, 0x46,
    0xc3,

    /* U+0079 "y" */
    0x41, 0x21, 0x98, 0xc4, 0x43, 0x60, 0xa0, 0x50,
    0x38, 0x8, 0xc, 0x4, 0xe, 0x0,

    /* U+007A "z" */
    0xfc, 0x30, 0x84, 0x30, 0x84, 0x30, 0xfc,

    /* U+007B "{" */
    0xe, 0x30, 0x60, 0xc1, 0x83, 0x6, 0x70, 0x18,
    0x30, 0x60, 0xc1, 0x83, 0x3, 0x80,

    /* U+007C "|" */
    0xff, 0xff, 0xff, 0xff,

    /* U+007D "}" */
    0xf0, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1c, 0x7,
    0x1c, 0x18, 0x18, 0x18, 0x18, 0x18, 0xf0,

    /* U+007E "~" */
    0x71, 0x91, 0x89, 0x8e,

    /* U+00A0 " " */
    0x0,

    /* U+00A1 "¡" */
    0xf0, 0x7f, 0xff,

    /* U+00A2 "¢" */
    0x8, 0x10, 0x21, 0xf6, 0x99, 0x32, 0x64, 0xc9,
    0x91, 0xa1, 0xf0, 0x81, 0x0,

    /* U+00A3 "£" */
    0x1e, 0x70, 0xc1, 0x83, 0x6, 0x3f, 0x18, 0x30,
    0x60, 0xc7, 0xf0,

    /* U+00A4 "¤" */
    0x0, 0xf9, 0x12, 0x24, 0x4f, 0x80, 0x0,

    /* U+00A5 "¥" */
    0x40, 0x98, 0x62, 0x10, 0xcc, 0x12, 0x1f, 0xe0,
    0xc1, 0xfe, 0xc, 0x3, 0x0, 0xc0, 0x30,

    /* U+00A6 "¦" */
    0xff, 0xf0, 0xff, 0xf0,

    /* U+00A7 "§" */
    0x7d, 0x8b, 0x3, 0x7, 0x99, 0xb1, 0xf3, 0x3c,
    0x30, 0x34, 0x6f, 0x80,

    /* U+00A8 "¨" */
    0xd8,

    /* U+00A9 "©" */
    0x1e, 0x18, 0x65, 0xea, 0xc1, 0xa0, 0x68, 0x1b,
    0x5, 0x7a, 0x61, 0x87, 0x80,

    /* U+00AA "ª" */
    0xf0, 0x5f, 0x19, 0xb4, 0x1f,

    /* U+00AB "«" */
    0x12, 0x4f, 0x36, 0xc6, 0xc6, 0xc4, 0x0,

    /* U+00AC "¬" */
    0xff, 0x80, 0xc0, 0x60, 0x30,

    /* U+00AD "­" */
    0xf0,

    /* U+00AE "®" */
    0x1e, 0x18, 0x65, 0xea, 0x49, 0x92, 0x67, 0x19,
    0x65, 0x4a, 0x61, 0x87, 0x80,

    /* U+00AF "¯" */
    0xf8,

    /* U+00B0 "°" */
    0x74, 0x63, 0x17, 0x0,

    /* U+00B1 "±" */
    0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x0, 0x0,
    0xff,

    /* U+00B2 "²" */
    0xe1, 0x12, 0x4f,

    /* U+00B3 "³" */
    0xf1, 0x61, 0x1e,

    /* U+00B4 "´" */
    0x2a, 0x0,

    /* U+00B5 "µ" */
    0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xce,
    0xfb, 0xc0, 0xc0, 0xc0,

    /* U+00B6 "¶" */
    0x3f, 0x79, 0x79, 0xf9, 0x79, 0x79, 0x19, 0x9,
    0x9, 0x9, 0x9, 0x9, 0x9, 0x9,

    /* U+00B7 "·" */
    0xf0,

    /* U+00B8 "¸" */
    0x27, 0x80,

    /* U+00B9 "¹" */
    0xe1, 0x8, 0x42, 0x7c,

    /* U+00BA "º" */
    0x7b, 0x38, 0x61, 0xcd, 0xe0, 0x3f,

    /* U+00BB "»" */
    0x91, 0x91, 0x99, 0xb6, 0xdb, 0x4, 0x0,

    /* U+00BC "¼" */
    0x30, 0xf0, 0x30, 0x30, 0x30, 0x30, 0xf8, 0xf,
    0xf0, 0x2, 0x6, 0xa, 0xa, 0xf, 0x2, 0x2,

    /* U+00BD "½" */
    0xe0, 0x20, 0x20, 0x20, 0x20, 0xf8, 0x7, 0x78,
    0x8e, 0x1, 0x1, 0x2, 0x4, 0x8, 0xf,

    /* U+00BE "¾" */
    0x70, 0x8, 0x8, 0x30, 0x8, 0x8, 0x70, 0xf,
    0xf0, 0x6, 0x6, 0xa, 0x12, 0x1f, 0x2, 0x2,

    /* U+00BF "¿" */
    0x18, 0x30, 0x0, 0xc1, 0x83, 0xc, 0x30, 0x61,
    0x81, 0x89, 0xf0,

    /* U+00C0 "À" */
    0x10, 0x4, 0x1, 0x1, 0x81, 0xc0, 0xf0, 0x58,
    0x24, 0x32, 0x19, 0x88, 0xcf, 0xe6, 0x1a, 0xf,
    0x2,

    /* U+00C1 "Á" */
    0x6, 0x2, 0x2, 0x1, 0x81, 0xc0, 0xf0, 0x58,
    0x24, 0x32, 0x19, 0x88, 0xcf, 0xe6, 0x1a, 0xf,
    0x2,

    /* U+00C2 "Â" */
    0xc, 0xa, 0xc, 0x81, 0x81, 0xc0, 0xf0, 0x58,
    0x24, 0x32, 0x19, 0x88, 0xcf, 0xe6, 0x1a, 0xf,
    0x2,

    /* U+00C3 "Ã" */
    0x3a, 0x17, 0x7, 0x3, 0x81, 0x41, 0xa0, 0xd8,
    0x4c, 0x22, 0x31, 0x9f, 0xc8, 0x2c, 0x1e, 0xc,

    /* U+00C4 "Ä" */
    0x36, 0x0, 0x2, 0x3, 0x81, 0x40, 0xa0, 0xd8,
    0x44, 0x22, 0x31, 0x9f, 0xc8, 0x2c, 0x1e, 0xc,

    /* U+00C5 "Å" */
    0x18, 0x24, 0x24, 0x3c, 0x18, 0x38, 0x3c, 0x24,
    0x24, 0x66, 0x66, 0x7e, 0xc3, 0xc3, 0x81,

    /* U+00C6 "Æ" */
    0x1f, 0x8b, 0x5, 0x86, 0xc2, 0x61, 0x3d, 0x98,
    0xcc, 0x7e, 0x23, 0x31, 0x98, 0xf0,

    /* U+00C7 "Ç" */
    0x1f, 0x61, 0x60, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
    0xc0, 0x60, 0x61, 0x3f, 0x4, 0x4, 0x1c,

    /* U+00C8 "È" */
    0x20, 0x20, 0x27, 0xfc, 0x18, 0x30, 0x60, 0xff,
    0x83, 0x6, 0xc, 0x18, 0x3f, 0x80,

    /* U+00C9 "É" */
    0xc, 0x30, 0x47, 0xfc, 0x18, 0x30, 0x60, 0xff,
    0x83, 0x6, 0xc, 0x18, 0x3f, 0x80,

    /* U+00CA "Ê" */
    0x18, 0x71, 0x97, 0xfc, 0x18, 0x30, 0x60, 0xff,
    0x83, 0x6, 0xc, 0x18, 0x3f, 0x80,

    /* U+00CB "Ë" */
    0x6c, 0x3, 0xfe, 0xc, 0x18, 0x30, 0x7f, 0xc1,
    0x83, 0x6, 0xc, 0x1f, 0xc0,

    /* U+00CC "Ì" */
    0x40, 0x81, 0x3f, 0x30, 0xc3, 0xc, 0x30, 0xc3,
    0xc, 0x30, 0xcf, 0xc0,

    /* U+00CD "Í" */
    0x8, 0x42, 0x3f, 0x30, 0xc3, 0xc, 0x30, 0xc3,
    0xc, 0x30, 0xcf, 0xc0,

    /* U+00CE "Î" */
    0x31, 0xe4, 0xbf, 0x30, 0xc3, 0xc, 0x30, 0xc3,
    0xc, 0x30, 0xcf, 0xc0,

    /* U+00CF "Ï" */
    0x6c, 0xf, 0xcc, 0x30, 0xc3, 0xc, 0x30, 0xc3,
    0xc, 0x33, 0xf0,

    /* U+00D0 "Ð" */
    0x7c, 0x31, 0x98, 0xcc, 0x36, 0x1f, 0xcd, 0x86,
    0xc3, 0x61, 0xb1, 0x98, 0xcf, 0x80,

    /* U+00D1 "Ñ" */
    0x34, 0x2c, 0x0, 0xe3, 0xe3, 0xe3, 0xf3, 0xd3,
    0xd3, 0xcb, 0xcb, 0xcf, 0xc7, 0xc7, 0xc7,

    /* U+00D2 "Ò" */
    0x10, 0x10, 0x8, 0x3c, 0x66, 0xc2, 0xc3, 0xc3,
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c,

    /* U+00D3 "Ó" */
    0xc, 0x8, 0x10, 0x3c, 0x66, 0xc2, 0xc3, 0xc3,
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c,

    /* U+00D4 "Ô" */
    0x18, 0x14, 0x24, 0x3c, 0x66, 0xc2, 0xc3, 0xc3,
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c,

    /* U+00D5 "Õ" */
    0x34, 0x2c, 0x0, 0x3c, 0x66, 0xc2, 0xc3, 0xc3,
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c,

    /* U+00D6 "Ö" */
    0x6c, 0x0, 0x3c, 0x66, 0xc2, 0xc3, 0xc3, 0xc3,
    0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c,

    /* U+00D7 "×" */
    0x3, 0x8d, 0xb1, 0xc3, 0x8d, 0xb1, 0x81,

    /* U+00D8 "Ø" */
    0x1e, 0x8c, 0xe6, 0x19, 0x8e, 0x65, 0x99, 0x66,
    0x99, 0xa6, 0x71, 0x98, 0x67, 0x33, 0x78,

    /* U+00D9 "Ù" */
    0x10, 0x10, 0x8, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c,

    /* U+00DA "Ú" */
    0x8, 0x8, 0x10, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c,

    /* U+00DB "Û" */
    0x18, 0x3c, 0x24, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c,

    /* U+00DC "Ü" */
    0x6c, 0x0, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
    0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c,

    /* U+00DD "Ý" */
    0x2, 0x1, 0x0, 0x81, 0x2, 0x61, 0x88, 0x43,
    0x30, 0x78, 0x1e, 0x3, 0x0, 0xc0, 0x30, 0xc,
    0x3, 0x0, 0xc0,

    /* U+00DE "Þ" */
    0xc0, 0xc0, 0xc0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7,
    0xfe, 0xc0, 0xc0, 0xc0,

    /* U+00DF "ß" */
    0x7c, 0xc6, 0xc6, 0xdc, 0xd0, 0xd0, 0xd8, 0xce,
    0xc3, 0xc3, 0xc3, 0xfe,

    /* U+00E0 "à" */
    0x20, 0x30, 0x18, 0x8, 0x0, 0x7c, 0x3, 0x3,
    0x7f, 0xe3, 0xc3, 0xc3, 0xc7, 0x7b,

    /* U+00E1 "á" */
    0x6, 0xc, 0x8, 0x10, 0x0, 0x7c, 0x3, 0x3,
    0x7f, 0xe3, 0xc3, 0xc3, 0xc7, 0x7b,

    /* U+00E2 "â" */
    0x8, 0x18, 0x34, 0x26, 0x0, 0x7c, 0x3, 0x3,
    0x7f, 0xe3, 0xc3, 0xc3, 0xc7, 0x7b,

    /* U+00E3 "ã" */
    0x32, 0x2c, 0x0, 0x0, 0x7c, 0x3, 0x3, 0x7f,
    0xe3, 0xc3, 0xc3, 0xc7, 0x7b,

    /* U+00E4 "ä" */
    0x2c, 0x0, 0x0, 0x7c, 0x3, 0x3, 0x7f, 0xe3,
    0xc3, 0xc3, 0xc7, 0x7b,

    /* U+00E5 "å" */
    0x18, 0x24, 0x24, 0x18, 0x0, 0x7c, 0x3, 0x3,
    0x7f, 0xe3, 0xc3, 0xc3, 0xc7, 0x7b,

    /* U+00E6 "æ" */
    0x77, 0x4, 0xc2, 0x61, 0x37, 0xfe, 0x43, 0x21,
    0x90, 0x77, 0x80,

    /* U+00E7 "ç" */
    0x3e, 0xc7, 0x6, 0xc, 0x18, 0x30, 0x31, 0x3e,
    0x8, 0x10, 0xe0,

    /* U+00E8 "è" */
    0x20, 0x30, 0x10, 0x8, 0x0, 0x3c, 0x66, 0xc3,
    0xc3, 0xff, 0xc0, 0xc0, 0x61, 0x3e,

    /* U+00E9 "é" */
    0x4, 0xc, 0x8, 0x10, 0x0, 0x3c, 0x66, 0xc3,
    0xc3, 0xff, 0xc0, 0xc0, 0x61, 0x3e,

    /* U+00EA "ê" */
    0x8, 0x18, 0x24, 0x24, 0x0, 0x3c, 0x66, 0xc3,
    0xc3, 0xff, 0xc0, 0xc0, 0x61, 0x3e,

    /* U+00EB "ë" */
    0x2c, 0x0, 0x0, 0x3c, 0x66, 0xc3, 0xc3, 0xff,
    0xc0, 0xc0, 0x61, 0x3e,

    /* U+00EC "ì" */
    0xc0, 0xc0, 0xc0, 0xf, 0x6, 0xc, 0x18, 0x30,
    0x60, 0xc1, 0x81, 0xe0,

    /* U+00ED "í" */
    0xc, 0x30, 0xc0, 0xf, 0x6, 0xc, 0x18, 0x30,
    0x60, 0xc1, 0x81, 0xe0,

    /* U+00EE "î" */
    0x30, 0xa3, 0x20, 0xf, 0x6, 0xc, 0x18, 0x30,
    0x60, 0xc1, 0x81, 0xe0,

    /* U+00EF "ï" */
    0xd8, 0x0, 0x38, 0x20, 0x82, 0x8, 0x20, 0x82,
    0x7,

    /* U+00F0 "ð" */
    0x30, 0x3c, 0xc, 0x3e, 0x66, 0xc3, 0xc3, 0xc3,
    0xc3, 0xc3, 0x66, 0x3c,

    /* U+00F1 "ñ" */
    0x64, 0xb0, 0x6, 0xee, 0x78, 0xf1, 0xe3, 0xc7,
    0x8f, 0x1e, 0x30,

    /* U+00F2 "ò" */
    0x60, 0x30, 0x10, 0x8, 0x0, 0x3c, 0x66, 0xc3,
    0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c,

    /* U+00F3 "ó" */
    0x6, 0xc, 0x8, 0x10, 0x0, 0x3c, 0x66, 0xc3,
    0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c,

    /* U+00F4 "ô" */
    0x18, 0x18, 0x24, 0x24, 0x0, 0x3c, 0x66, 0xc3,
    0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c,

    /* U+00F5 "õ" */
    0x34, 0x2c, 0x0, 0x3c, 0x66, 0xc3, 0xc3, 0xc3,
    0xc3, 0xc3, 0x66, 0x3c,

    /* U+00F6 "ö" */
    0x6c, 0x0, 0x0, 0x3c, 0x66, 0xc3, 0xc3, 0xc3,
    0xc3, 0xc3, 0x66, 0x3c,

    /* U+00F7 "÷" */
    0x18, 0x18, 0x0, 0xff, 0x0, 0x0, 0x18, 0x18,

    /* U+00F8 "ø" */
    0x0, 0x7, 0xa3, 0x11, 0x8e, 0x65, 0x99, 0x66,
    0x99, 0xc6, 0x33, 0x17, 0x80, 0x0,

    /* U+00F9 "ù" */
    0x60, 0x40, 0x40, 0xc, 0x78, 0xf1, 0xe3, 0xc7,
    0x8f, 0x1e, 0x77, 0x60,

    /* U+00FA "ú" */
    0xc, 0x30, 0x40, 0xc, 0x78, 0xf1, 0xe3, 0xc7,
    0x8f, 0x1e, 0x77, 0x60,

    /* U+00FB "û" */
    0x10, 0x51, 0xa0, 0xc, 0x78, 0xf1, 0xe3, 0xc7,
    0x8f, 0x1e, 0x77, 0x60,

    /* U+00FC "ü" */
    0x6c, 0x0, 0x6, 0x3c, 0x78, 0xf1, 0xe3, 0xc7,
    0x8f, 0x3b, 0xb0,

    /* U+00FD "ý" */
    0x6, 0x2, 0x2, 0x2, 0x0, 0x2, 0x9, 0xc,
    0xc6, 0x22, 0x1b, 0x5, 0x2, 0x81, 0xc0, 0x40,
    0x60, 0x20, 0x70, 0x0,

    /* U+00FE "þ" */
    0xc1, 0x83, 0x6, 0xee, 0xd8, 0xf1, 0xe3, 0xc7,
    0x8f, 0xb6, 0xec, 0x18, 0x30, 0x0,

    /* U+00FF "ÿ" */
    0x36, 0x0, 0x0, 0x8, 0x34, 0x13, 0x18, 0x88,
    0x64, 0x16, 0xa, 0x3, 0x1, 0x0, 0x80, 0xc1,
    0xc0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 154, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 154, .box_w = 2, .box_h = 12, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 4, .adv_w = 154, .box_w = 5, .box_h = 4, .ofs_x = 3, .ofs_y = 8},
    {.bitmap_index = 7, .adv_w = 154, .box_w = 10, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 22, .adv_w = 154, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 35, .adv_w = 154, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 48, .adv_w = 154, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 62, .adv_w = 154, .box_w = 2, .box_h = 4, .ofs_x = 4, .ofs_y = 8},
    {.bitmap_index = 63, .adv_w = 154, .box_w = 4, .box_h = 15, .ofs_x = 2, .ofs_y = -3},
    {.bitmap_index = 71, .adv_w = 154, .box_w = 4, .box_h = 15, .ofs_x = 4, .ofs_y = -3},
    {.bitmap_index = 79, .adv_w = 154, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 86, .adv_w = 154, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 94, .adv_w = 154, .box_w = 3, .box_h = 5, .ofs_x = 3, .ofs_y = -3},
    {.bitmap_index = 96, .adv_w = 154, .box_w = 6, .box_h = 1, .ofs_x = 2, .ofs_y = 4},
    {.bitmap_index = 97, .adv_w = 154, .box_w = 2, .box_h = 2, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 98, .adv_w = 154, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 110, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 122, .adv_w = 154, .box_w = 6, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 131, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 142, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 154, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 166, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 178, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 190, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 201, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 213, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 225, .adv_w = 154, .box_w = 2, .box_h = 7, .ofs_x = 4, .ofs_y = 2},
    {.bitmap_index = 227, .adv_w = 154, .box_w = 3, .box_h = 13, .ofs_x = 3, .ofs_y = -4},
    {.bitmap_index = 232, .adv_w = 154, .box_w = 9, .box_h = 8, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 241, .adv_w = 154, .box_w = 8, .box_h = 4, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 245, .adv_w = 154, .box_w = 9, .box_h = 8, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 254, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 265, .adv_w = 154, .box_w = 9, .box_h = 13, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 280, .adv_w = 154, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 294, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 306, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 318, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 330, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 341, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 352, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 364, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 376, .adv_w = 154, .box_w = 6, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 385, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 396, .adv_w = 154, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 410, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 422, .adv_w = 154, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 436, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 448, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 460, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 472, .adv_w = 154, .box_w = 9, .box_h = 15, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 489, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 501, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 513, .adv_w = 154, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 527, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 539, .adv_w = 154, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 553, .adv_w = 154, .box_w = 10, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 568, .adv_w = 154, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 582, .adv_w = 154, .box_w = 10, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 597, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 609, .adv_w = 154, .box_w = 5, .box_h = 15, .ofs_x = 3, .ofs_y = -2},
    {.bitmap_index = 619, .adv_w = 154, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 632, .adv_w = 154, .box_w = 5, .box_h = 15, .ofs_x = 3, .ofs_y = -2},
    {.bitmap_index = 642, .adv_w = 154, .box_w = 8, .box_h = 5, .ofs_x = 1, .ofs_y = 7},
    {.bitmap_index = 647, .adv_w = 154, .box_w = 8, .box_h = 1, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 648, .adv_w = 154, .box_w = 4, .box_h = 3, .ofs_x = 2, .ofs_y = 10},
    {.bitmap_index = 650, .adv_w = 154, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 659, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 670, .adv_w = 154, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 678, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 689, .adv_w = 154, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 698, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 709, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 720, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 731, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 742, .adv_w = 154, .box_w = 5, .box_h = 15, .ofs_x = 2, .ofs_y = -3},
    {.bitmap_index = 752, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 764, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 775, .adv_w = 154, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 784, .adv_w = 154, .box_w = 7, .box_h = 9, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 792, .adv_w = 154, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 801, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 812, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 823, .adv_w = 154, .box_w = 6, .box_h = 9, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 830, .adv_w = 154, .box_w = 7, .box_h = 9, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 838, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 849, .adv_w = 154, .box_w = 7, .box_h = 9, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 857, .adv_w = 154, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 866, .adv_w = 154, .box_w = 10, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 878, .adv_w = 154, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 887, .adv_w = 154, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 901, .adv_w = 154, .box_w = 6, .box_h = 9, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 908, .adv_w = 154, .box_w = 7, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 922, .adv_w = 154, .box_w = 2, .box_h = 16, .ofs_x = 4, .ofs_y = -4},
    {.bitmap_index = 926, .adv_w = 154, .box_w = 8, .box_h = 15, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 941, .adv_w = 154, .box_w = 8, .box_h = 4, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 945, .adv_w = 154, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 946, .adv_w = 154, .box_w = 2, .box_h = 12, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 949, .adv_w = 154, .box_w = 7, .box_h = 14, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 962, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 973, .adv_w = 154, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 980, .adv_w = 154, .box_w = 10, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 995, .adv_w = 154, .box_w = 2, .box_h = 14, .ofs_x = 4, .ofs_y = -3},
    {.bitmap_index = 999, .adv_w = 154, .box_w = 7, .box_h = 13, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 1011, .adv_w = 154, .box_w = 5, .box_h = 1, .ofs_x = 2, .ofs_y = 11},
    {.bitmap_index = 1012, .adv_w = 154, .box_w = 10, .box_h = 10, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 1025, .adv_w = 154, .box_w = 5, .box_h = 8, .ofs_x = 2, .ofs_y = 4},
    {.bitmap_index = 1030, .adv_w = 154, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 1037, .adv_w = 154, .box_w = 9, .box_h = 4, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 1042, .adv_w = 154, .box_w = 4, .box_h = 1, .ofs_x = 3, .ofs_y = 4},
    {.bitmap_index = 1043, .adv_w = 154, .box_w = 10, .box_h = 10, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 1056, .adv_w = 154, .box_w = 5, .box_h = 1, .ofs_x = 2, .ofs_y = 11},
    {.bitmap_index = 1057, .adv_w = 154, .box_w = 5, .box_h = 5, .ofs_x = 2, .ofs_y = 7},
    {.bitmap_index = 1061, .adv_w = 154, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1070, .adv_w = 154, .box_w = 4, .box_h = 6, .ofs_x = 3, .ofs_y = 9},
    {.bitmap_index = 1073, .adv_w = 154, .box_w = 4, .box_h = 6, .ofs_x = 3, .ofs_y = 9},
    {.bitmap_index = 1076, .adv_w = 154, .box_w = 3, .box_h = 3, .ofs_x = 4, .ofs_y = 10},
    {.bitmap_index = 1078, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 1090, .adv_w = 154, .box_w = 8, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1104, .adv_w = 154, .box_w = 2, .box_h = 2, .ofs_x = 4, .ofs_y = 5},
    {.bitmap_index = 1105, .adv_w = 154, .box_w = 3, .box_h = 3, .ofs_x = 3, .ofs_y = -3},
    {.bitmap_index = 1107, .adv_w = 154, .box_w = 5, .box_h = 6, .ofs_x = 2, .ofs_y = 9},
    {.bitmap_index = 1111, .adv_w = 154, .box_w = 6, .box_h = 8, .ofs_x = 2, .ofs_y = 4},
    {.bitmap_index = 1117, .adv_w = 154, .box_w = 7, .box_h = 7, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 1124, .adv_w = 154, .box_w = 8, .box_h = 16, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 1140, .adv_w = 154, .box_w = 8, .box_h = 15, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1155, .adv_w = 154, .box_w = 8, .box_h = 16, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 1171, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1182, .adv_w = 154, .box_w = 9, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1199, .adv_w = 154, .box_w = 9, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1216, .adv_w = 154, .box_w = 9, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1233, .adv_w = 154, .box_w = 9, .box_h = 14, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1249, .adv_w = 154, .box_w = 9, .box_h = 14, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1265, .adv_w = 154, .box_w = 8, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1280, .adv_w = 154, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1294, .adv_w = 154, .box_w = 8, .box_h = 15, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 1309, .adv_w = 154, .box_w = 7, .box_h = 15, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1323, .adv_w = 154, .box_w = 7, .box_h = 15, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1337, .adv_w = 154, .box_w = 7, .box_h = 15, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1351, .adv_w = 154, .box_w = 7, .box_h = 14, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1364, .adv_w = 154, .box_w = 6, .box_h = 15, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1376, .adv_w = 154, .box_w = 6, .box_h = 15, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1388, .adv_w = 154, .box_w = 6, .box_h = 15, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1400, .adv_w = 154, .box_w = 6, .box_h = 14, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1411, .adv_w = 154, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1425, .adv_w = 154, .box_w = 8, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1440, .adv_w = 154, .box_w = 8, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1455, .adv_w = 154, .box_w = 8, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1470, .adv_w = 154, .box_w = 8, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1485, .adv_w = 154, .box_w = 8, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1500, .adv_w = 154, .box_w = 8, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1514, .adv_w = 154, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 1521, .adv_w = 154, .box_w = 10, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1536, .adv_w = 154, .box_w = 8, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1551, .adv_w = 154, .box_w = 8, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1566, .adv_w = 154, .box_w = 8, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1581, .adv_w = 154, .box_w = 8, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1595, .adv_w = 154, .box_w = 10, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1614, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1626, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1638, .adv_w = 154, .box_w = 8, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1652, .adv_w = 154, .box_w = 8, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1666, .adv_w = 154, .box_w = 8, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1680, .adv_w = 154, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1693, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1705, .adv_w = 154, .box_w = 8, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1719, .adv_w = 154, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1730, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 1741, .adv_w = 154, .box_w = 8, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1755, .adv_w = 154, .box_w = 8, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1769, .adv_w = 154, .box_w = 8, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1783, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1795, .adv_w = 154, .box_w = 7, .box_h = 13, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1807, .adv_w = 154, .box_w = 7, .box_h = 13, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1819, .adv_w = 154, .box_w = 7, .box_h = 13, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1831, .adv_w = 154, .box_w = 6, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1840, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1852, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1863, .adv_w = 154, .box_w = 8, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1877, .adv_w = 154, .box_w = 8, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1891, .adv_w = 154, .box_w = 8, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1905, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1917, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1929, .adv_w = 154, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 1937, .adv_w = 154, .box_w = 10, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1951, .adv_w = 154, .box_w = 7, .box_h = 13, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1963, .adv_w = 154, .box_w = 7, .box_h = 13, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1975, .adv_w = 154, .box_w = 7, .box_h = 13, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1987, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1998, .adv_w = 154, .box_w = 9, .box_h = 17, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 2018, .adv_w = 154, .box_w = 7, .box_h = 15, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 2032, .adv_w = 154, .box_w = 9, .box_h = 15, .ofs_x = 0, .ofs_y = -3}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 95, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 160, .range_length = 96, .glyph_id_start = 96,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 2,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t ui_font_Hack16 = {
#else
lv_font_t ui_font_Hack16 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 19,          /*The maximum line height required by the font*/
    .base_line = 4,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -2,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if UI_FONT_HACK16*/

