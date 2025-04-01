/*******************************************************************************
 * Size: 14 px
 * Bpp: 1
 * Opts: --bpp 1 --size 14 --font C:/Users/i31ewoho/Documents/Spaces/iTransfer/ROLORAN-TDeck-Neuhaus/assets/fonts/HackNerdFont-Regular.ttf -o C:/Users/i31ewoho/Documents/Spaces/iTransfer/ROLORAN-TDeck-Neuhaus/assets/fonts\ui_font_Hack14.c --format lvgl -r 0x20-0xff --no-compress --no-prefilter
 ******************************************************************************/

#include "ui.h"

#ifndef UI_FONT_HACK14
#define UI_FONT_HACK14 1
#endif

#if UI_FONT_HACK14

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xff, 0xfc, 0xf0,

    /* U+0022 "\"" */
    0x99, 0x99,

    /* U+0023 "#" */
    0x9, 0xd, 0x4, 0x8f, 0xf1, 0x21, 0xa3, 0xfc,
    0x48, 0x2c, 0x34, 0x0,

    /* U+0024 "$" */
    0x10, 0xed, 0x24, 0x91, 0xc1, 0xc5, 0x16, 0x5f,
    0x84, 0x10,

    /* U+0025 "%" */
    0x60, 0x90, 0x90, 0x63, 0xc, 0x30, 0xce, 0x11,
    0x11, 0xe,

    /* U+0026 "&" */
    0x3c, 0x40, 0x40, 0x60, 0x70, 0x99, 0x89, 0x87,
    0xc6, 0x7f,

    /* U+0027 "'" */
    0xf0,

    /* U+0028 "(" */
    0x69, 0x49, 0x24, 0x91, 0x22,

    /* U+0029 ")" */
    0xc9, 0x12, 0x49, 0x2d, 0x28,

    /* U+002A "*" */
    0x10, 0x23, 0x59, 0xc3, 0x85, 0x11, 0x0,

    /* U+002B "+" */
    0x10, 0x20, 0x47, 0xf1, 0x2, 0x4, 0x0,

    /* U+002C "," */
    0x6c, 0xbc,

    /* U+002D "-" */
    0xfc,

    /* U+002E "." */
    0xf0,

    /* U+002F "/" */
    0x4, 0x30, 0x86, 0x10, 0xc2, 0x8, 0x41, 0x8,
    0x20,

    /* U+0030 "0" */
    0x38, 0x8a, 0x4c, 0x99, 0x32, 0x64, 0xc1, 0x44,
    0x70,

    /* U+0031 "1" */
    0x21, 0x8a, 0x28, 0x20, 0x82, 0x8, 0x23, 0xf0,

    /* U+0032 "2" */
    0xfa, 0x30, 0x41, 0xc, 0x63, 0x18, 0xc3, 0xf0,

    /* U+0033 "3" */
    0xf8, 0x10, 0x43, 0x70, 0x30, 0x41, 0xf, 0xe0,

    /* U+0034 "4" */
    0xc, 0xc, 0x14, 0x24, 0x64, 0x44, 0xff, 0x4,
    0x4, 0x4,

    /* U+0035 "5" */
    0x7c, 0x81, 0x3, 0xc0, 0x40, 0x40, 0x81, 0x85,
    0xf0,

    /* U+0036 "6" */
    0x39, 0x8, 0x2e, 0xce, 0x18, 0x61, 0x4d, 0xe0,

    /* U+0037 "7" */
    0xfc, 0x30, 0x82, 0x18, 0x41, 0xc, 0x21, 0x80,

    /* U+0038 "8" */
    0x7a, 0x18, 0x61, 0x7b, 0x38, 0x61, 0xcd, 0xe0,

    /* U+0039 "9" */
    0x7b, 0x28, 0x61, 0xcd, 0xd0, 0x41, 0x9, 0xc0,

    /* U+003A ":" */
    0xf0, 0x3c,

    /* U+003B ";" */
    0xf0, 0x3d, 0x60,

    /* U+003C "<" */
    0x2, 0x3d, 0xc4, 0x7, 0x3, 0x80, 0x80,

    /* U+003D "=" */
    0xfe, 0x0, 0x7, 0xf0,

    /* U+003E ">" */
    0x81, 0xe0, 0x70, 0x11, 0xce, 0x20, 0x0,

    /* U+003F "?" */
    0x7a, 0x10, 0x42, 0x10, 0x82, 0x0, 0x30, 0xc0,

    /* U+0040 "@" */
    0x1e, 0x63, 0x41, 0x8f, 0x91, 0x91, 0x91, 0x91,
    0x8f, 0x40, 0x20, 0x1e,

    /* U+0041 "A" */
    0x18, 0x1c, 0x14, 0x34, 0x24, 0x22, 0x7e, 0x42,
    0x43, 0xc1,

    /* U+0042 "B" */
    0xfa, 0x18, 0x61, 0xfa, 0x38, 0x61, 0x8f, 0xe0,

    /* U+0043 "C" */
    0x3e, 0x86, 0x4, 0x8, 0x10, 0x20, 0x40, 0x42,
    0x7c,

    /* U+0044 "D" */
    0xf2, 0x28, 0x61, 0x86, 0x18, 0x61, 0x8b, 0xc0,

    /* U+0045 "E" */
    0xfe, 0x8, 0x20, 0xfa, 0x8, 0x20, 0x83, 0xf0,

    /* U+0046 "F" */
    0xfe, 0x8, 0x20, 0xfa, 0x8, 0x20, 0x82, 0x0,

    /* U+0047 "G" */
    0x3c, 0x86, 0x4, 0x8, 0x11, 0xe0, 0xc1, 0x42,
    0x78,

    /* U+0048 "H" */
    0x86, 0x18, 0x61, 0xfe, 0x18, 0x61, 0x86, 0x10,

    /* U+0049 "I" */
    0xfc, 0x82, 0x8, 0x20, 0x82, 0x8, 0x23, 0xf0,

    /* U+004A "J" */
    0x3c, 0x10, 0x41, 0x4, 0x10, 0x41, 0x85, 0xe0,

    /* U+004B "K" */
    0x85, 0x12, 0x65, 0x8e, 0x1e, 0x26, 0x44, 0x8d,
    0xc,

    /* U+004C "L" */
    0x82, 0x8, 0x20, 0x82, 0x8, 0x20, 0x83, 0xf0,

    /* U+004D "M" */
    0xc7, 0x8f, 0xbd, 0x5a, 0xb2, 0x60, 0xc1, 0x83,
    0x4,

    /* U+004E "N" */
    0xc7, 0x1e, 0x69, 0xa6, 0x59, 0x67, 0x8e, 0x30,

    /* U+004F "O" */
    0x38, 0x8a, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x44,
    0x70,

    /* U+0050 "P" */
    0xfa, 0x38, 0x61, 0x8f, 0xe8, 0x20, 0x82, 0x0,

    /* U+0051 "Q" */
    0x38, 0x44, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82,
    0x44, 0x3c, 0xc, 0x2,

    /* U+0052 "R" */
    0xf9, 0x1a, 0x14, 0x28, 0xde, 0x22, 0x46, 0x85,
    0xc,

    /* U+0053 "S" */
    0x7f, 0x18, 0x20, 0x70, 0x70, 0x41, 0x8f, 0xe0,

    /* U+0054 "T" */
    0xff, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10,

    /* U+0055 "U" */
    0x86, 0x18, 0x61, 0x86, 0x18, 0x61, 0x85, 0xe0,

    /* U+0056 "V" */
    0x43, 0x43, 0x42, 0x62, 0x26, 0x24, 0x34, 0x14,
    0x1c, 0x18,

    /* U+0057 "W" */
    0x81, 0xc1, 0xd9, 0x59, 0x59, 0x57, 0x56, 0x66,
    0x66, 0x66,

    /* U+0058 "X" */
    0x43, 0x22, 0x34, 0x1c, 0x18, 0x1c, 0x34, 0x26,
    0x62, 0xc1,

    /* U+0059 "Y" */
    0x82, 0x89, 0xb1, 0x43, 0x82, 0x4, 0x8, 0x10,
    0x20,

    /* U+005A "Z" */
    0xfe, 0xc, 0x30, 0x41, 0x86, 0x8, 0x20, 0xc1,
    0xfc,

    /* U+005B "[" */
    0xf8, 0x88, 0x88, 0x88, 0x88, 0x88, 0xf0,

    /* U+005C "\\" */
    0x80, 0x81, 0x1, 0x2, 0x6, 0x4, 0xc, 0x8,
    0x18, 0x10, 0x30,

    /* U+005D "]" */
    0xf1, 0x11, 0x11, 0x11, 0x11, 0x11, 0xf0,

    /* U+005E "^" */
    0x18, 0x34, 0x26, 0x43,

    /* U+005F "_" */
    0xfe,

    /* U+0060 "`" */
    0x88, 0x80,

    /* U+0061 "a" */
    0xfa, 0x10, 0x5f, 0xc6, 0x18, 0xdd,

    /* U+0062 "b" */
    0x82, 0x8, 0x3e, 0xca, 0x18, 0x61, 0x87, 0x2b,
    0x80,

    /* U+0063 "c" */
    0x3d, 0x18, 0x20, 0x82, 0x4, 0xf,

    /* U+0064 "d" */
    0x4, 0x10, 0x5d, 0x4e, 0x18, 0x61, 0x85, 0x37,
    0x40,

    /* U+0065 "e" */
    0x3c, 0x8e, 0xf, 0xf8, 0x10, 0x10, 0x9f,

    /* U+0066 "f" */
    0x1c, 0x82, 0x3f, 0x20, 0x82, 0x8, 0x20, 0x82,
    0x0,

    /* U+0067 "g" */
    0x75, 0x38, 0x61, 0x86, 0x14, 0xdd, 0x4, 0x37,
    0x80,

    /* U+0068 "h" */
    0x82, 0x8, 0x2e, 0xc6, 0x18, 0x61, 0x86, 0x18,
    0x40,

    /* U+0069 "i" */
    0x20, 0x0, 0x38, 0x20, 0x82, 0x8, 0x20, 0x81,
    0xc0,

    /* U+006A "j" */
    0x10, 0x7, 0x11, 0x11, 0x11, 0x11, 0x1e,

    /* U+006B "k" */
    0x82, 0x8, 0x22, 0x92, 0x8e, 0x2c, 0x9a, 0x28,
    0xc0,

    /* U+006C "l" */
    0xe0, 0x82, 0x8, 0x20, 0x82, 0x8, 0x20, 0x81,
    0xc0,

    /* U+006D "m" */
    0xfd, 0x26, 0x4c, 0x99, 0x32, 0x64, 0xc9,

    /* U+006E "n" */
    0xbb, 0x18, 0x61, 0x86, 0x18, 0x61,

    /* U+006F "o" */
    0x7b, 0x28, 0x61, 0x86, 0x1c, 0xde,

    /* U+0070 "p" */
    0xbb, 0x28, 0x61, 0x86, 0x1c, 0xbe, 0x82, 0x8,
    0x0,

    /* U+0071 "q" */
    0x75, 0x38, 0x61, 0x86, 0x14, 0xdd, 0x4, 0x10,
    0x40,

    /* U+0072 "r" */
    0xbf, 0x18, 0x20, 0x82, 0x8, 0x20,

    /* U+0073 "s" */
    0x7c, 0x61, 0xc3, 0x84, 0x3e,

    /* U+0074 "t" */
    0x0, 0x82, 0x3f, 0x20, 0x82, 0x8, 0x20, 0x81,
    0xc0,

    /* U+0075 "u" */
    0x86, 0x18, 0x61, 0x86, 0x18, 0xdd,

    /* U+0076 "v" */
    0x87, 0x9, 0x12, 0x64, 0x85, 0xe, 0x18,

    /* U+0077 "w" */
    0x81, 0xc1, 0x49, 0x5b, 0x5a, 0x56, 0x66, 0x26,

    /* U+0078 "x" */
    0xc4, 0x98, 0xe1, 0x83, 0xd, 0x13, 0x43,

    /* U+0079 "y" */
    0x42, 0x42, 0x66, 0x24, 0x24, 0x1c, 0x18, 0x18,
    0x10, 0x10, 0x60,

    /* U+007A "z" */
    0xfc, 0x31, 0x84, 0x21, 0xc, 0x3f,

    /* U+007B "{" */
    0xe, 0x20, 0x40, 0x81, 0x2, 0x38, 0x8, 0x10,
    0x20, 0x40, 0x80, 0xe0,

    /* U+007C "|" */
    0xff, 0xfe,

    /* U+007D "}" */
    0xc0, 0x82, 0x8, 0x20, 0xc1, 0xcc, 0x20, 0x82,
    0x8, 0xc0,

    /* U+007E "~" */
    0x63, 0x26, 0x30,

    /* U+00A0 " " */
    0x0,

    /* U+00A1 "¡" */
    0xf2, 0xff, 0xf0,

    /* U+00A2 "¢" */
    0x10, 0x43, 0xd4, 0x92, 0x49, 0x24, 0x50, 0xf1,
    0x4,

    /* U+00A3 "£" */
    0x1e, 0x60, 0x81, 0x2, 0x1f, 0x8, 0x10, 0x21,
    0xfc,

    /* U+00A4 "¤" */
    0x0, 0xf9, 0x12, 0x24, 0x4f, 0x80, 0x0,

    /* U+00A5 "¥" */
    0xc6, 0x88, 0xa7, 0x71, 0x1f, 0xc4, 0x8, 0x10,
    0x20,

    /* U+00A6 "¦" */
    0xf9, 0xf0,

    /* U+00A7 "§" */
    0x39, 0x4, 0x18, 0xda, 0x3c, 0x5f, 0x18, 0x20,
    0x9c,

    /* U+00A8 "¨" */
    0x90,

    /* U+00A9 "©" */
    0x3e, 0x31, 0xb7, 0x74, 0x1a, 0xd, 0x7, 0x76,
    0xc6, 0x3e, 0x0,

    /* U+00AA "ª" */
    0xf0, 0x5f, 0x18, 0xbf, 0xe0,

    /* U+00AB "«" */
    0x26, 0x9a, 0x66, 0xc6, 0x44, 0x40,

    /* U+00AC "¬" */
    0xfe, 0x4, 0x8,

    /* U+00AD "­" */
    0xf0,

    /* U+00AE "®" */
    0x3e, 0x31, 0xbf, 0x74, 0x9b, 0xcd, 0x67, 0x96,
    0xc6, 0x3e, 0x0,

    /* U+00AF "¯" */
    0xf0,

    /* U+00B0 "°" */
    0x69, 0x96,

    /* U+00B1 "±" */
    0x10, 0x23, 0xf8, 0x81, 0x2, 0x0, 0x7f,

    /* U+00B2 "²" */
    0xf1, 0x24, 0xf0,

    /* U+00B3 "³" */
    0xf6, 0x11, 0xe0,

    /* U+00B4 "´" */
    0x36, 0x40,

    /* U+00B5 "µ" */
    0x85, 0xa, 0x14, 0x28, 0x50, 0xb3, 0x7f, 0x81,
    0x2, 0x0,

    /* U+00B6 "¶" */
    0x7f, 0xdf, 0x7d, 0xf5, 0xd1, 0x45, 0x14, 0x51,
    0x45,

    /* U+00B7 "·" */
    0xf0,

    /* U+00B8 "¸" */
    0x27, 0x80,

    /* U+00B9 "¹" */
    0xe2, 0x22, 0x70,

    /* U+00BA "º" */
    0x74, 0x63, 0x17, 0x3, 0xe0,

    /* U+00BB "»" */
    0x90, 0x90, 0x93, 0x6d, 0x92, 0x0,

    /* U+00BC "¼" */
    0xe0, 0x20, 0x20, 0x20, 0x20, 0x78, 0xe, 0xf0,
    0x6, 0xa, 0x12, 0x1f, 0x2, 0x2,

    /* U+00BD "½" */
    0x20, 0xc0, 0x81, 0x2, 0xe, 0x3, 0xb8, 0x9e,
    0x4, 0x8, 0x20, 0x83, 0xc0,

    /* U+00BE "¾" */
    0xf0, 0x21, 0x80, 0x81, 0x1c, 0x4f, 0x60, 0xc,
    0x28, 0x51, 0xf0, 0x40, 0x80,

    /* U+00BF "¿" */
    0x31, 0x80, 0x42, 0x11, 0x10, 0x84, 0x5e,

    /* U+00C0 "À" */
    0x10, 0x8, 0x0, 0x18, 0x1c, 0x14, 0x34, 0x24,
    0x22, 0x7e, 0x42, 0x43, 0xc1,

    /* U+00C1 "Á" */
    0xc, 0x8, 0x0, 0x18, 0x1c, 0x14, 0x34, 0x24,
    0x22, 0x7e, 0x42, 0x43, 0xc1,

    /* U+00C2 "Â" */
    0x18, 0x34, 0x0, 0x18, 0x1c, 0x14, 0x34, 0x24,
    0x22, 0x7e, 0x42, 0x43, 0xc1,

    /* U+00C3 "Ã" */
    0x34, 0x2c, 0x0, 0x18, 0x1c, 0x14, 0x34, 0x24,
    0x22, 0x7e, 0x42, 0x43, 0xc1,

    /* U+00C4 "Ä" */
    0x24, 0x0, 0x0, 0x18, 0x3c, 0x3c, 0x24, 0x24,
    0x66, 0x7e, 0x42, 0xc3, 0xc3,

    /* U+00C5 "Å" */
    0x18, 0x24, 0x24, 0x18, 0x38, 0x3c, 0x24, 0x24,
    0x66, 0x7e, 0x42, 0xc3, 0x81,

    /* U+00C6 "Æ" */
    0x3f, 0x28, 0x28, 0x68, 0x4e, 0x48, 0x78, 0x88,
    0x88, 0x8f,

    /* U+00C7 "Ç" */
    0x3e, 0x86, 0x4, 0x8, 0x10, 0x20, 0x40, 0x42,
    0x7c, 0x20, 0x43, 0x80,

    /* U+00C8 "È" */
    0x60, 0x80, 0x3f, 0x82, 0x8, 0x3e, 0x82, 0x8,
    0x20, 0xfc,

    /* U+00C9 "É" */
    0x10, 0x80, 0x3f, 0x82, 0x8, 0x3e, 0x82, 0x8,
    0x20, 0xfc,

    /* U+00CA "Ê" */
    0x31, 0x40, 0x3f, 0x82, 0x8, 0x3e, 0x82, 0x8,
    0x20, 0xfc,

    /* U+00CB "Ë" */
    0x48, 0x0, 0x3f, 0x82, 0x8, 0x3f, 0x82, 0x8,
    0x20, 0xfc,

    /* U+00CC "Ì" */
    0x40, 0x80, 0x3f, 0x20, 0x82, 0x8, 0x20, 0x82,
    0x8, 0xfc,

    /* U+00CD "Í" */
    0x10, 0x80, 0x3f, 0x20, 0x82, 0x8, 0x20, 0x82,
    0x8, 0xfc,

    /* U+00CE "Î" */
    0x21, 0x40, 0x3f, 0x20, 0x82, 0x8, 0x20, 0x82,
    0x8, 0xfc,

    /* U+00CF "Ï" */
    0x48, 0x0, 0x3f, 0x20, 0x82, 0x8, 0x20, 0x82,
    0x8, 0xfc,

    /* U+00D0 "Ð" */
    0x78, 0x89, 0xa, 0x1f, 0x28, 0x50, 0xa1, 0x44,
    0xf0,

    /* U+00D1 "Ñ" */
    0x69, 0x60, 0x31, 0xc7, 0x9a, 0x69, 0x96, 0x59,
    0xe3, 0x8c,

    /* U+00D2 "Ò" */
    0x30, 0x30, 0x1, 0xc4, 0x50, 0x60, 0xc1, 0x83,
    0x6, 0xa, 0x23, 0x80,

    /* U+00D3 "Ó" */
    0x8, 0x20, 0x1, 0xc4, 0x50, 0x60, 0xc1, 0x83,
    0x6, 0xa, 0x23, 0x80,

    /* U+00D4 "Ô" */
    0x38, 0x88, 0x1, 0xc4, 0x50, 0x60, 0xc1, 0x83,
    0x6, 0xa, 0x23, 0x80,

    /* U+00D5 "Õ" */
    0x7c, 0x0, 0x1, 0xc4, 0x50, 0x60, 0xc1, 0x83,
    0x6, 0xa, 0x23, 0x80,

    /* U+00D6 "Ö" */
    0x28, 0x0, 0x1, 0xc6, 0x58, 0x70, 0xe1, 0xc3,
    0x87, 0xb, 0x23, 0x80,

    /* U+00D7 "×" */
    0x7, 0x37, 0x8c, 0x7b, 0x30, 0x40,

    /* U+00D8 "Ø" */
    0x1f, 0x13, 0x91, 0xc9, 0xa4, 0xd2, 0xc9, 0xc4,
    0xc2, 0x62, 0x5e, 0x0,

    /* U+00D9 "Ù" */
    0x20, 0x40, 0x21, 0x86, 0x18, 0x61, 0x86, 0x18,
    0x61, 0x78,

    /* U+00DA "Ú" */
    0x10, 0x80, 0x21, 0x86, 0x18, 0x61, 0x86, 0x18,
    0x61, 0x78,

    /* U+00DB "Û" */
    0x31, 0x20, 0x21, 0x86, 0x18, 0x61, 0x86, 0x18,
    0x61, 0x78,

    /* U+00DC "Ü" */
    0x48, 0x0, 0x21, 0x86, 0x18, 0x61, 0x86, 0x18,
    0x61, 0x78,

    /* U+00DD "Ý" */
    0x8, 0x20, 0x4, 0x14, 0x4d, 0x8a, 0x1c, 0x10,
    0x20, 0x40, 0x81, 0x0,

    /* U+00DE "Þ" */
    0x82, 0xf, 0xa1, 0x86, 0x1f, 0xa0, 0x82, 0x0,

    /* U+00DF "ß" */
    0x7a, 0x18, 0x66, 0xa2, 0x89, 0xa3, 0x86, 0x1b,
    0x80,

    /* U+00E0 "à" */
    0x60, 0x81, 0x0, 0xfa, 0x10, 0x5f, 0xc6, 0x18,
    0xdd,

    /* U+00E1 "á" */
    0x8, 0x42, 0x0, 0xfa, 0x10, 0x5f, 0xc6, 0x18,
    0xdd,

    /* U+00E2 "â" */
    0x30, 0xa4, 0x80, 0xfa, 0x10, 0x5f, 0xc6, 0x18,
    0xdd,

    /* U+00E3 "ã" */
    0x69, 0x65, 0x80, 0xfa, 0x10, 0x5f, 0xc6, 0x18,
    0xdd,

    /* U+00E4 "ä" */
    0x48, 0x0, 0x3e, 0x84, 0x17, 0xf1, 0x86, 0x37,
    0x40,

    /* U+00E5 "å" */
    0x31, 0x24, 0x8c, 0x3, 0xe8, 0x41, 0x7f, 0x18,
    0x63, 0x74,

    /* U+00E6 "æ" */
    0xf6, 0x9, 0x9, 0x7f, 0x88, 0x88, 0x88, 0x77,

    /* U+00E7 "ç" */
    0x3d, 0x18, 0x20, 0x82, 0x4, 0xf, 0x8, 0x23,
    0x80,

    /* U+00E8 "è" */
    0x60, 0x60, 0x60, 0x3, 0xc8, 0xe0, 0xff, 0x81,
    0x1, 0x9, 0xf0,

    /* U+00E9 "é" */
    0xc, 0x30, 0xc0, 0x3, 0xc8, 0xe0, 0xff, 0x81,
    0x1, 0x9, 0xf0,

    /* U+00EA "ê" */
    0x18, 0x51, 0x10, 0x3, 0xc8, 0xe0, 0xff, 0x81,
    0x1, 0x9, 0xf0,

    /* U+00EB "ë" */
    0x28, 0x0, 0x1, 0xe4, 0x78, 0x7f, 0xc0, 0x80,
    0x84, 0xf8,

    /* U+00EC "ì" */
    0x81, 0x2, 0x0, 0xe0, 0x82, 0x8, 0x20, 0x82,
    0x7,

    /* U+00ED "í" */
    0x8, 0x42, 0x0, 0xe0, 0x82, 0x8, 0x20, 0x82,
    0x7,

    /* U+00EE "î" */
    0x21, 0x45, 0x0, 0xe0, 0x82, 0x8, 0x20, 0x82,
    0x7,

    /* U+00EF "ï" */
    0x90, 0x0, 0x38, 0x20, 0x82, 0x8, 0x20, 0x81,
    0xc0,

    /* U+00F0 "ð" */
    0x61, 0xe1, 0x1e, 0xce, 0x18, 0x61, 0x87, 0x27,
    0x80,

    /* U+00F1 "ñ" */
    0x6a, 0x60, 0x2e, 0xc6, 0x18, 0x61, 0x86, 0x18,
    0x40,

    /* U+00F2 "ò" */
    0x40, 0x81, 0x0, 0x7b, 0x38, 0x61, 0x86, 0x1c,
    0xde,

    /* U+00F3 "ó" */
    0x8, 0x42, 0x0, 0x7b, 0x38, 0x61, 0x86, 0x1c,
    0xde,

    /* U+00F4 "ô" */
    0x31, 0x64, 0x80, 0x7b, 0x38, 0x61, 0x86, 0x1c,
    0xde,

    /* U+00F5 "õ" */
    0x69, 0x60, 0x1e, 0xce, 0x18, 0x61, 0x87, 0x37,
    0x80,

    /* U+00F6 "ö" */
    0x48, 0x0, 0x1e, 0xce, 0x18, 0x61, 0x87, 0x37,
    0x80,

    /* U+00F7 "÷" */
    0x10, 0x3, 0xf8, 0x0, 0x2, 0x0,

    /* U+00F8 "ø" */
    0x3e, 0x66, 0x46, 0x4a, 0x52, 0x62, 0x66, 0x7c,

    /* U+00F9 "ù" */
    0xc1, 0x83, 0x0, 0x86, 0x18, 0x61, 0x86, 0x18,
    0xdd,

    /* U+00FA "ú" */
    0x8, 0x63, 0x0, 0x86, 0x18, 0x61, 0x86, 0x18,
    0xdd,

    /* U+00FB "û" */
    0x30, 0xc4, 0x80, 0x86, 0x18, 0x61, 0x86, 0x18,
    0xdd,

    /* U+00FC "ü" */
    0x48, 0x0, 0x21, 0x86, 0x18, 0x61, 0x86, 0x37,
    0x40,

    /* U+00FD "ý" */
    0x4, 0x8, 0x10, 0x0, 0x42, 0x42, 0x66, 0x24,
    0x24, 0x1c, 0x18, 0x18, 0x10, 0x10, 0x60,

    /* U+00FE "þ" */
    0x82, 0x8, 0x2e, 0xca, 0x18, 0x61, 0x87, 0x2b,
    0xa0, 0x82, 0x0,

    /* U+00FF "ÿ" */
    0x24, 0x0, 0x42, 0x42, 0x66, 0x24, 0x34, 0x3c,
    0x18, 0x18, 0x10, 0x10, 0x60
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 135, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 135, .box_w = 2, .box_h = 10, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 4, .adv_w = 135, .box_w = 4, .box_h = 4, .ofs_x = 2, .ofs_y = 6},
    {.bitmap_index = 6, .adv_w = 135, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 18, .adv_w = 135, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 28, .adv_w = 135, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 38, .adv_w = 135, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 48, .adv_w = 135, .box_w = 1, .box_h = 4, .ofs_x = 4, .ofs_y = 6},
    {.bitmap_index = 49, .adv_w = 135, .box_w = 3, .box_h = 13, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 54, .adv_w = 135, .box_w = 3, .box_h = 13, .ofs_x = 3, .ofs_y = -2},
    {.bitmap_index = 59, .adv_w = 135, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 66, .adv_w = 135, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 73, .adv_w = 135, .box_w = 3, .box_h = 5, .ofs_x = 2, .ofs_y = -3},
    {.bitmap_index = 75, .adv_w = 135, .box_w = 6, .box_h = 1, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 76, .adv_w = 135, .box_w = 2, .box_h = 2, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 77, .adv_w = 135, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 86, .adv_w = 135, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 95, .adv_w = 135, .box_w = 6, .box_h = 10, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 103, .adv_w = 135, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 111, .adv_w = 135, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 119, .adv_w = 135, .box_w = 8, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 129, .adv_w = 135, .box_w = 7, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 138, .adv_w = 135, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 146, .adv_w = 135, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 154, .adv_w = 135, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 162, .adv_w = 135, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 170, .adv_w = 135, .box_w = 2, .box_h = 7, .ofs_x = 3, .ofs_y = 1},
    {.bitmap_index = 172, .adv_w = 135, .box_w = 2, .box_h = 10, .ofs_x = 3, .ofs_y = -3},
    {.bitmap_index = 175, .adv_w = 135, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 182, .adv_w = 135, .box_w = 7, .box_h = 4, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 186, .adv_w = 135, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 193, .adv_w = 135, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 201, .adv_w = 135, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 213, .adv_w = 135, .box_w = 8, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 223, .adv_w = 135, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 231, .adv_w = 135, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 240, .adv_w = 135, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 248, .adv_w = 135, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 256, .adv_w = 135, .box_w = 6, .box_h = 10, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 264, .adv_w = 135, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 273, .adv_w = 135, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 281, .adv_w = 135, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 289, .adv_w = 135, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 297, .adv_w = 135, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 306, .adv_w = 135, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 314, .adv_w = 135, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 323, .adv_w = 135, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 331, .adv_w = 135, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 340, .adv_w = 135, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 348, .adv_w = 135, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 360, .adv_w = 135, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 369, .adv_w = 135, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 377, .adv_w = 135, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 387, .adv_w = 135, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 395, .adv_w = 135, .box_w = 8, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 405, .adv_w = 135, .box_w = 8, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 415, .adv_w = 135, .box_w = 8, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 425, .adv_w = 135, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 434, .adv_w = 135, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 443, .adv_w = 135, .box_w = 4, .box_h = 13, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 450, .adv_w = 135, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 461, .adv_w = 135, .box_w = 4, .box_h = 13, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 468, .adv_w = 135, .box_w = 8, .box_h = 4, .ofs_x = 0, .ofs_y = 7},
    {.bitmap_index = 472, .adv_w = 135, .box_w = 7, .box_h = 1, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 473, .adv_w = 135, .box_w = 3, .box_h = 3, .ofs_x = 2, .ofs_y = 9},
    {.bitmap_index = 475, .adv_w = 135, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 481, .adv_w = 135, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 490, .adv_w = 135, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 496, .adv_w = 135, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 505, .adv_w = 135, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 512, .adv_w = 135, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 521, .adv_w = 135, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 530, .adv_w = 135, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 539, .adv_w = 135, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 548, .adv_w = 135, .box_w = 4, .box_h = 14, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 555, .adv_w = 135, .box_w = 6, .box_h = 11, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 564, .adv_w = 135, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 573, .adv_w = 135, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 580, .adv_w = 135, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 586, .adv_w = 135, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 592, .adv_w = 135, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 601, .adv_w = 135, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 610, .adv_w = 135, .box_w = 6, .box_h = 8, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 616, .adv_w = 135, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 621, .adv_w = 135, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 630, .adv_w = 135, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 636, .adv_w = 135, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 643, .adv_w = 135, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 651, .adv_w = 135, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 658, .adv_w = 135, .box_w = 8, .box_h = 11, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 669, .adv_w = 135, .box_w = 6, .box_h = 8, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 675, .adv_w = 135, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 687, .adv_w = 135, .box_w = 1, .box_h = 15, .ofs_x = 4, .ofs_y = -4},
    {.bitmap_index = 689, .adv_w = 135, .box_w = 6, .box_h = 13, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 699, .adv_w = 135, .box_w = 7, .box_h = 3, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 702, .adv_w = 135, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 703, .adv_w = 135, .box_w = 2, .box_h = 10, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 706, .adv_w = 135, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 715, .adv_w = 135, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 724, .adv_w = 135, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 731, .adv_w = 135, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 740, .adv_w = 135, .box_w = 1, .box_h = 12, .ofs_x = 4, .ofs_y = -2},
    {.bitmap_index = 742, .adv_w = 135, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 751, .adv_w = 135, .box_w = 4, .box_h = 1, .ofs_x = 2, .ofs_y = 10},
    {.bitmap_index = 752, .adv_w = 135, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 763, .adv_w = 135, .box_w = 5, .box_h = 7, .ofs_x = 2, .ofs_y = 4},
    {.bitmap_index = 768, .adv_w = 135, .box_w = 7, .box_h = 6, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 774, .adv_w = 135, .box_w = 7, .box_h = 3, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 777, .adv_w = 135, .box_w = 4, .box_h = 1, .ofs_x = 2, .ofs_y = 3},
    {.bitmap_index = 778, .adv_w = 135, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 789, .adv_w = 135, .box_w = 4, .box_h = 1, .ofs_x = 2, .ofs_y = 10},
    {.bitmap_index = 790, .adv_w = 135, .box_w = 4, .box_h = 4, .ofs_x = 2, .ofs_y = 6},
    {.bitmap_index = 792, .adv_w = 135, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 799, .adv_w = 135, .box_w = 4, .box_h = 5, .ofs_x = 2, .ofs_y = 8},
    {.bitmap_index = 802, .adv_w = 135, .box_w = 4, .box_h = 5, .ofs_x = 2, .ofs_y = 8},
    {.bitmap_index = 805, .adv_w = 135, .box_w = 4, .box_h = 3, .ofs_x = 3, .ofs_y = 9},
    {.bitmap_index = 807, .adv_w = 135, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 817, .adv_w = 135, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 826, .adv_w = 135, .box_w = 2, .box_h = 2, .ofs_x = 3, .ofs_y = 4},
    {.bitmap_index = 827, .adv_w = 135, .box_w = 3, .box_h = 3, .ofs_x = 3, .ofs_y = -3},
    {.bitmap_index = 829, .adv_w = 135, .box_w = 4, .box_h = 5, .ofs_x = 2, .ofs_y = 8},
    {.bitmap_index = 832, .adv_w = 135, .box_w = 5, .box_h = 7, .ofs_x = 2, .ofs_y = 3},
    {.bitmap_index = 837, .adv_w = 135, .box_w = 7, .box_h = 6, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 843, .adv_w = 135, .box_w = 8, .box_h = 14, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 857, .adv_w = 135, .box_w = 7, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 870, .adv_w = 135, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 883, .adv_w = 135, .box_w = 5, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 890, .adv_w = 135, .box_w = 8, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 903, .adv_w = 135, .box_w = 8, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 916, .adv_w = 135, .box_w = 8, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 929, .adv_w = 135, .box_w = 8, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 942, .adv_w = 135, .box_w = 8, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 955, .adv_w = 135, .box_w = 8, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 968, .adv_w = 135, .box_w = 8, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 978, .adv_w = 135, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 990, .adv_w = 135, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1000, .adv_w = 135, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1010, .adv_w = 135, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1020, .adv_w = 135, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1030, .adv_w = 135, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1040, .adv_w = 135, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1050, .adv_w = 135, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1060, .adv_w = 135, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1070, .adv_w = 135, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1079, .adv_w = 135, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1089, .adv_w = 135, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1101, .adv_w = 135, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1113, .adv_w = 135, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1125, .adv_w = 135, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1137, .adv_w = 135, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1149, .adv_w = 135, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 1155, .adv_w = 135, .box_w = 9, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1167, .adv_w = 135, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1177, .adv_w = 135, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1187, .adv_w = 135, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1197, .adv_w = 135, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1207, .adv_w = 135, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1219, .adv_w = 135, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1227, .adv_w = 135, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1236, .adv_w = 135, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1245, .adv_w = 135, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1254, .adv_w = 135, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1263, .adv_w = 135, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1272, .adv_w = 135, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1281, .adv_w = 135, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1291, .adv_w = 135, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1299, .adv_w = 135, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 1308, .adv_w = 135, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1319, .adv_w = 135, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1330, .adv_w = 135, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1341, .adv_w = 135, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1351, .adv_w = 135, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1360, .adv_w = 135, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1369, .adv_w = 135, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1378, .adv_w = 135, .box_w = 6, .box_h = 11, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1387, .adv_w = 135, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1396, .adv_w = 135, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1405, .adv_w = 135, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1414, .adv_w = 135, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1423, .adv_w = 135, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1432, .adv_w = 135, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1441, .adv_w = 135, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1450, .adv_w = 135, .box_w = 7, .box_h = 6, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 1456, .adv_w = 135, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1464, .adv_w = 135, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1473, .adv_w = 135, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1482, .adv_w = 135, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1491, .adv_w = 135, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1500, .adv_w = 135, .box_w = 8, .box_h = 15, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 1515, .adv_w = 135, .box_w = 6, .box_h = 14, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 1526, .adv_w = 135, .box_w = 8, .box_h = 13, .ofs_x = 0, .ofs_y = -3}
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
const lv_font_t ui_font_Hack14 = {
#else
lv_font_t ui_font_Hack14 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 17,          /*The maximum line height required by the font*/
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



#endif /*#if UI_FONT_HACK14*/

