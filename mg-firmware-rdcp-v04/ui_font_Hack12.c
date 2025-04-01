/*******************************************************************************
 * Size: 12 px
 * Bpp: 1
 * Opts: --bpp 1 --size 12 --font C:/Users/i31ewoho/Documents/Spaces/iTransfer/ROLORAN-TDeck-Neuhaus/assets/fonts/HackNerdFont-Regular.ttf -o C:/Users/i31ewoho/Documents/Spaces/iTransfer/ROLORAN-TDeck-Neuhaus/assets/fonts\ui_font_Hack12.c --format lvgl -r 0x20-0xff --no-compress --no-prefilter
 ******************************************************************************/

#include "ui.h"

#ifndef UI_FONT_HACK12
#define UI_FONT_HACK12 1
#endif

#if UI_FONT_HACK12

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xfd, 0x80,

    /* U+0022 "\"" */
    0xb6, 0x80,

    /* U+0023 "#" */
    0x14, 0x28, 0x53, 0xf2, 0x9f, 0xd2, 0x24, 0x50,

    /* U+0024 "$" */
    0x23, 0xa9, 0x4e, 0x38, 0xa5, 0xaf, 0x88,

    /* U+0025 "%" */
    0x61, 0x22, 0x43, 0x31, 0x9d, 0x84, 0x89, 0xc,

    /* U+0026 "&" */
    0x38, 0x81, 0x3, 0x9, 0x33, 0x62, 0xe2, 0x7a,

    /* U+0027 "'" */
    0xe0,

    /* U+0028 "(" */
    0x4a, 0x49, 0x24, 0x89, 0x0,

    /* U+0029 ")" */
    0x48, 0x92, 0x49, 0x29, 0x0,

    /* U+002A "*" */
    0x21, 0x3e, 0x45, 0x44,

    /* U+002B "+" */
    0x10, 0x4f, 0xc4, 0x10, 0x40,

    /* U+002C "," */
    0xf6, 0x0,

    /* U+002D "-" */
    0xf8,

    /* U+002E "." */
    0xf0,

    /* U+002F "/" */
    0x8, 0x44, 0x22, 0x11, 0x8, 0xc4, 0x20,

    /* U+0030 "0" */
    0x76, 0xeb, 0x5a, 0xd6, 0x3b, 0x70,

    /* U+0031 "1" */
    0x23, 0x28, 0x42, 0x10, 0x84, 0xf8,

    /* U+0032 "2" */
    0xf4, 0x42, 0x11, 0x11, 0x98, 0xf8,

    /* U+0033 "3" */
    0x78, 0x10, 0x4e, 0xc, 0x10, 0x63, 0xf8,

    /* U+0034 "4" */
    0x8, 0x30, 0xa1, 0x44, 0x89, 0x3f, 0x84, 0x8,

    /* U+0035 "5" */
    0x79, 0x4, 0x1e, 0xc, 0x10, 0x63, 0xf8,

    /* U+0036 "6" */
    0x39, 0x8, 0x2e, 0xce, 0x18, 0x53, 0x78,

    /* U+0037 "7" */
    0xfc, 0x30, 0x82, 0x10, 0x43, 0x8, 0x60,

    /* U+0038 "8" */
    0x74, 0x63, 0x17, 0x46, 0x31, 0x70,

    /* U+0039 "9" */
    0x7b, 0x28, 0x61, 0xcd, 0xd0, 0x42, 0x70,

    /* U+003A ":" */
    0xf0, 0xf0,

    /* U+003B ";" */
    0xf0, 0xf7, 0x80,

    /* U+003C "<" */
    0x4, 0x6e, 0x30, 0x38, 0x30,

    /* U+003D "=" */
    0xfc, 0x0, 0x3f,

    /* U+003E ">" */
    0x81, 0x81, 0xc3, 0x73, 0x0,

    /* U+003F "?" */
    0xf0, 0x84, 0x64, 0x20, 0x8, 0x40,

    /* U+0040 "@" */
    0x3c, 0xcd, 0xc, 0xfa, 0x34, 0x68, 0xcf, 0x40,
    0xc0, 0x70,

    /* U+0041 "A" */
    0x10, 0x70, 0xa1, 0x42, 0xcc, 0x9f, 0x23, 0xc2,

    /* U+0042 "B" */
    0xf2, 0x28, 0xa2, 0xf2, 0x38, 0x61, 0xf8,

    /* U+0043 "C" */
    0x3d, 0x18, 0x20, 0x82, 0x8, 0x11, 0x3c,

    /* U+0044 "D" */
    0xf2, 0x28, 0x61, 0x86, 0x18, 0x62, 0xf0,

    /* U+0045 "E" */
    0xfc, 0x21, 0xf, 0xc2, 0x10, 0xf8,

    /* U+0046 "F" */
    0xfc, 0x21, 0xf, 0xc2, 0x10, 0x80,

    /* U+0047 "G" */
    0x39, 0x18, 0x20, 0x8e, 0x18, 0x51, 0x38,

    /* U+0048 "H" */
    0x86, 0x18, 0x61, 0xfe, 0x18, 0x61, 0x84,

    /* U+0049 "I" */
    0xf9, 0x8, 0x42, 0x10, 0x84, 0xf8,

    /* U+004A "J" */
    0x38, 0x42, 0x10, 0x84, 0x31, 0xf0,

    /* U+004B "K" */
    0x8e, 0x6b, 0x38, 0xe2, 0x49, 0xa2, 0x8c,

    /* U+004C "L" */
    0x84, 0x21, 0x8, 0x42, 0x10, 0xf8,

    /* U+004D "M" */
    0xcf, 0x3c, 0xed, 0xb6, 0xd8, 0x61, 0x84,

    /* U+004E "N" */
    0xc7, 0x1e, 0x69, 0xb6, 0x59, 0xe3, 0x8c,

    /* U+004F "O" */
    0x7b, 0x28, 0x61, 0x86, 0x18, 0x72, 0x78,

    /* U+0050 "P" */
    0xf4, 0x63, 0x1f, 0x42, 0x10, 0x80,

    /* U+0051 "Q" */
    0x79, 0x92, 0x14, 0x28, 0x50, 0xa1, 0x26, 0x78,
    0x10, 0x10,

    /* U+0052 "R" */
    0xf2, 0x28, 0xa2, 0xf2, 0x48, 0xa2, 0x84,

    /* U+0053 "S" */
    0x7c, 0x61, 0x87, 0x4, 0x31, 0xf0,

    /* U+0054 "T" */
    0xfe, 0x20, 0x40, 0x81, 0x2, 0x4, 0x8, 0x10,

    /* U+0055 "U" */
    0x8c, 0x63, 0x18, 0xc6, 0x31, 0x70,

    /* U+0056 "V" */
    0xc2, 0x89, 0x12, 0x22, 0xc5, 0xa, 0x14, 0x10,

    /* U+0057 "W" */
    0x83, 0x6, 0x4a, 0xd6, 0xad, 0x9b, 0x36, 0x64,

    /* U+0058 "X" */
    0x46, 0xc8, 0xa0, 0xc1, 0x5, 0xb, 0x22, 0xc2,

    /* U+0059 "Y" */
    0xc6, 0x88, 0xa1, 0x41, 0x2, 0x4, 0x8, 0x10,

    /* U+005A "Z" */
    0xfc, 0x30, 0x84, 0x30, 0x84, 0x30, 0xfc,

    /* U+005B "[" */
    0xf8, 0x88, 0x88, 0x88, 0x88, 0x8f,

    /* U+005C "\\" */
    0x86, 0x10, 0x82, 0x10, 0x42, 0x18, 0x42,

    /* U+005D "]" */
    0xf1, 0x11, 0x11, 0x11, 0x11, 0x1f,

    /* U+005E "^" */
    0x10, 0x70, 0xb2, 0x20,

    /* U+005F "_" */
    0xfc,

    /* U+0060 "`" */
    0xa4,

    /* U+0061 "a" */
    0xf0, 0x5f, 0x18, 0xcd, 0xa0,

    /* U+0062 "b" */
    0x84, 0x21, 0xe8, 0xc6, 0x31, 0x8f, 0x80,

    /* U+0063 "c" */
    0x7e, 0x21, 0x8, 0x61, 0xe0,

    /* U+0064 "d" */
    0x8, 0x42, 0xfc, 0xc6, 0x31, 0x8b, 0xc0,

    /* U+0065 "e" */
    0x7b, 0x38, 0x7f, 0x83, 0x7, 0xc0,

    /* U+0066 "f" */
    0x19, 0x9, 0xf2, 0x10, 0x84, 0x21, 0x0,

    /* U+0067 "g" */
    0x7e, 0x63, 0x18, 0xed, 0xe1, 0xb, 0x80,

    /* U+0068 "h" */
    0x84, 0x21, 0x6c, 0xc6, 0x31, 0x8c, 0x40,

    /* U+0069 "i" */
    0x40, 0xc, 0x44, 0x44, 0x43,

    /* U+006A "j" */
    0x20, 0x72, 0x49, 0x24, 0x9c,

    /* U+006B "k" */
    0x82, 0x8, 0x26, 0xb3, 0x8e, 0x24, 0x9a, 0x20,

    /* U+006C "l" */
    0xe1, 0x8, 0x42, 0x10, 0x84, 0x20, 0xc0,

    /* U+006D "m" */
    0xfd, 0x6b, 0x5a, 0xd6, 0xa0,

    /* U+006E "n" */
    0xb6, 0x63, 0x18, 0xc6, 0x20,

    /* U+006F "o" */
    0x7b, 0x38, 0x61, 0x87, 0x37, 0x80,

    /* U+0070 "p" */
    0xf6, 0x63, 0x18, 0xef, 0xd0, 0x84, 0x0,

    /* U+0071 "q" */
    0x7e, 0xe3, 0x18, 0xed, 0xe1, 0x8, 0x40,

    /* U+0072 "r" */
    0xbe, 0x61, 0x8, 0x42, 0x0,

    /* U+0073 "s" */
    0x7c, 0x20, 0xe0, 0x87, 0xc0,

    /* U+0074 "t" */
    0x21, 0x3e, 0x42, 0x10, 0x84, 0x38,

    /* U+0075 "u" */
    0x8c, 0x63, 0x18, 0xcd, 0xa0,

    /* U+0076 "v" */
    0x8a, 0x2c, 0x94, 0x51, 0x42, 0x0,

    /* U+0077 "w" */
    0x83, 0x5, 0x4a, 0xa6, 0xcd, 0x99, 0x0,

    /* U+0078 "x" */
    0x44, 0x58, 0xe0, 0x82, 0x85, 0x91, 0x0,

    /* U+0079 "y" */
    0x8e, 0x24, 0x96, 0x50, 0xc2, 0x8, 0x23, 0x0,

    /* U+007A "z" */
    0xf8, 0xc4, 0x44, 0x63, 0xe0,

    /* U+007B "{" */
    0x39, 0x8, 0x42, 0x13, 0x4, 0x21, 0x8, 0x70,

    /* U+007C "|" */
    0xff, 0xf8,

    /* U+007D "}" */
    0xe1, 0x8, 0x42, 0x10, 0x64, 0x21, 0x9, 0xc0,

    /* U+007E "~" */
    0x66, 0x99, 0x80,

    /* U+00A0 " " */
    0x0,

    /* U+00A1 "¡" */
    0xf2, 0xff, 0xc0,

    /* U+00A2 "¢" */
    0x10, 0x47, 0xf4, 0x92, 0x49, 0x15, 0x7c, 0x41,
    0x0,

    /* U+00A3 "£" */
    0x3d, 0x4, 0x10, 0xf9, 0x4, 0x10, 0xfc,

    /* U+00A4 "¤" */
    0x1, 0xe4, 0x92, 0x78, 0x0,

    /* U+00A5 "¥" */
    0x44, 0x88, 0xa3, 0x61, 0xf, 0x84, 0x8, 0x10,

    /* U+00A6 "¦" */
    0xf1, 0xe0,

    /* U+00A7 "§" */
    0x79, 0x4, 0x1c, 0x59, 0x26, 0x84, 0x8, 0x27,
    0x0,

    /* U+00A8 "¨" */
    0xa0,

    /* U+00A9 "©" */
    0x38, 0x8a, 0xee, 0x1c, 0x38, 0x5f, 0x1c,

    /* U+00AA "ª" */
    0xe1, 0x79, 0x97, 0xf0,

    /* U+00AB "«" */
    0x2d, 0x6b, 0x12, 0x24,

    /* U+00AC "¬" */
    0xfc, 0x10, 0x40,

    /* U+00AD "­" */
    0xe0,

    /* U+00AE "®" */
    0x38, 0x8a, 0xed, 0x5b, 0xb5, 0x51, 0x1c,

    /* U+00AF "¯" */
    0xe0,

    /* U+00B0 "°" */
    0x69, 0x96,

    /* U+00B1 "±" */
    0x10, 0x4f, 0xc4, 0x10, 0xf, 0xc0,

    /* U+00B2 "²" */
    0xe5, 0x70,

    /* U+00B3 "³" */
    0xe8, 0xf0,

    /* U+00B4 "´" */
    0x6a, 0x0,

    /* U+00B5 "µ" */
    0x8a, 0x28, 0xa2, 0x8a, 0x2f, 0xe0, 0x82, 0x0,

    /* U+00B6 "¶" */
    0x3d, 0xdf, 0x5d, 0x74, 0x51, 0x45, 0x14, 0x51,
    0x40,

    /* U+00B7 "·" */
    0xf0,

    /* U+00B8 "¸" */
    0x47, 0x80,

    /* U+00B9 "¹" */
    0xc9, 0x70,

    /* U+00BA "º" */
    0x69, 0x99, 0x6f,

    /* U+00BB "»" */
    0xb1, 0x66, 0xf4, 0xa0,

    /* U+00BC "¼" */
    0x20, 0xc0, 0x81, 0x2, 0xe, 0x7, 0x72, 0xc,
    0x28, 0x78, 0x20,

    /* U+00BD "½" */
    0x60, 0x40, 0x81, 0x7, 0x1, 0xdc, 0x46, 0x6,
    0x8, 0x20, 0xe0,

    /* U+00BE "¾" */
    0x70, 0x21, 0x80, 0x81, 0x1c, 0x4f, 0x62, 0x1c,
    0x28, 0xf8, 0x20,

    /* U+00BF "¿" */
    0x22, 0x2, 0x26, 0xc8, 0x87,

    /* U+00C0 "À" */
    0x20, 0x20, 0x40, 0x83, 0x85, 0xa, 0x16, 0x64,
    0xf9, 0x1e, 0x10,

    /* U+00C1 "Á" */
    0x8, 0x20, 0x40, 0x83, 0x85, 0xa, 0x16, 0x64,
    0xf9, 0x1e, 0x10,

    /* U+00C2 "Â" */
    0x10, 0x50, 0x80, 0x83, 0x85, 0xa, 0x16, 0x64,
    0xf9, 0x1e, 0x10,

    /* U+00C3 "Ã" */
    0x38, 0x0, 0x41, 0xc2, 0x85, 0xb, 0x32, 0x7c,
    0x8f, 0x8,

    /* U+00C4 "Ä" */
    0x28, 0x0, 0x41, 0x42, 0x85, 0xa, 0x36, 0x7c,
    0x8b, 0x18,

    /* U+00C5 "Å" */
    0x38, 0x50, 0xa0, 0x82, 0x85, 0xa, 0x36, 0x6c,
    0xf9, 0x16, 0x30,

    /* U+00C6 "Æ" */
    0x1e, 0x50, 0xa1, 0x46, 0xe9, 0x1e, 0x24, 0x8e,

    /* U+00C7 "Ç" */
    0x3d, 0x18, 0x20, 0x82, 0x8, 0x11, 0x7c, 0x41,
    0xc,

    /* U+00C8 "È" */
    0x41, 0x9, 0xf8, 0x42, 0x1f, 0x84, 0x21, 0xf0,

    /* U+00C9 "É" */
    0x11, 0x1, 0xf8, 0x42, 0x1f, 0x84, 0x21, 0xf0,

    /* U+00CA "Ê" */
    0x22, 0x85, 0xf8, 0x42, 0x1f, 0x84, 0x21, 0xf0,

    /* U+00CB "Ë" */
    0x50, 0x3f, 0x8, 0x43, 0xf0, 0x84, 0x3e,

    /* U+00CC "Ì" */
    0x41, 0x9, 0xf2, 0x10, 0x84, 0x21, 0x9, 0xf0,

    /* U+00CD "Í" */
    0x11, 0x9, 0xf2, 0x10, 0x84, 0x21, 0x9, 0xf0,

    /* U+00CE "Î" */
    0x22, 0x95, 0xf2, 0x10, 0x84, 0x21, 0x9, 0xf0,

    /* U+00CF "Ï" */
    0x50, 0x3e, 0x42, 0x10, 0x84, 0x21, 0x3e,

    /* U+00D0 "Ð" */
    0x78, 0x89, 0xa, 0x1f, 0x28, 0x50, 0xa2, 0x78,

    /* U+00D1 "Ñ" */
    0x78, 0xc, 0x71, 0xe6, 0x9b, 0x65, 0x9e, 0x38,
    0xc0,

    /* U+00D2 "Ò" */
    0x60, 0xc0, 0x1e, 0xca, 0x18, 0x61, 0x86, 0x1c,
    0x9e,

    /* U+00D3 "Ó" */
    0x8, 0x40, 0x1e, 0xca, 0x18, 0x61, 0x86, 0x1c,
    0x9e,

    /* U+00D4 "Ô" */
    0x31, 0xa0, 0x1e, 0xca, 0x18, 0x61, 0x86, 0x1c,
    0x9e,

    /* U+00D5 "Õ" */
    0x69, 0x60, 0x1e, 0xca, 0x18, 0x61, 0x86, 0x1c,
    0x9e,

    /* U+00D6 "Ö" */
    0x48, 0x7, 0xb2, 0x86, 0x18, 0x61, 0x87, 0x27,
    0x80,

    /* U+00D7 "×" */
    0x8e, 0xdc, 0x65, 0xc4,

    /* U+00D8 "Ø" */
    0x3e, 0x4d, 0x1a, 0x55, 0x2e, 0x58, 0xb2, 0xbc,

    /* U+00D9 "Ù" */
    0x41, 0x1, 0x18, 0xc6, 0x31, 0x8c, 0x62, 0xe0,

    /* U+00DA "Ú" */
    0x11, 0x1, 0x18, 0xc6, 0x31, 0x8c, 0x62, 0xe0,

    /* U+00DB "Û" */
    0x22, 0x81, 0x18, 0xc6, 0x31, 0x8c, 0x62, 0xe0,

    /* U+00DC "Ü" */
    0x50, 0x23, 0x18, 0xc6, 0x31, 0x8c, 0x5c,

    /* U+00DD "Ý" */
    0x8, 0x20, 0x46, 0x34, 0x45, 0xa, 0x8, 0x10,
    0x20, 0x40, 0x80,

    /* U+00DE "Þ" */
    0x82, 0xf, 0xa1, 0x86, 0x1f, 0xa0, 0x80,

    /* U+00DF "ß" */
    0x72, 0x28, 0xac, 0xa2, 0xc8, 0xa1, 0x86, 0xe0,

    /* U+00E0 "à" */
    0x42, 0x8, 0xf, 0x5, 0xf1, 0x8c, 0xde,

    /* U+00E1 "á" */
    0x10, 0x88, 0xf, 0x5, 0xf1, 0x8c, 0xde,

    /* U+00E2 "â" */
    0x21, 0x94, 0xf, 0x5, 0xf1, 0x8c, 0xde,

    /* U+00E3 "ã" */
    0x73, 0x81, 0xe0, 0xbe, 0x31, 0x9b, 0xc0,

    /* U+00E4 "ä" */
    0x50, 0x1, 0xe0, 0xbe, 0x31, 0x9b, 0xc0,

    /* U+00E5 "å" */
    0x7a, 0x4e, 0xf, 0x5, 0xf1, 0x8c, 0xdc,

    /* U+00E6 "æ" */
    0xec, 0x24, 0x4b, 0xf9, 0x12, 0x3b, 0x80,

    /* U+00E7 "ç" */
    0x7e, 0x21, 0x8, 0x61, 0xe2, 0x11, 0x80,

    /* U+00E8 "è" */
    0x40, 0x82, 0x0, 0x7b, 0x38, 0x7f, 0x83, 0x17,
    0x80,

    /* U+00E9 "é" */
    0x8, 0x42, 0x0, 0x7b, 0x38, 0x7f, 0x83, 0x17,
    0x80,

    /* U+00EA "ê" */
    0x20, 0xc4, 0x80, 0x7b, 0x38, 0x7f, 0x83, 0x17,
    0x80,

    /* U+00EB "ë" */
    0x48, 0x0, 0x1e, 0xce, 0x1f, 0xe0, 0xc5, 0xe0,

    /* U+00EC "ì" */
    0x41, 0x0, 0xc2, 0x10, 0x84, 0x20, 0xc0,

    /* U+00ED "í" */
    0x24, 0xc, 0x44, 0x44, 0x43,

    /* U+00EE "î" */
    0x4a, 0xc, 0x44, 0x44, 0x43,

    /* U+00EF "ï" */
    0xa0, 0xc, 0x44, 0x44, 0x43,

    /* U+00F0 "ð" */
    0x61, 0xe1, 0x9e, 0xce, 0x18, 0x61, 0xcd, 0xe0,

    /* U+00F1 "ñ" */
    0x75, 0x81, 0x6c, 0xc6, 0x31, 0x8c, 0x40,

    /* U+00F2 "ò" */
    0x40, 0x83, 0x0, 0x7b, 0x38, 0x61, 0x87, 0x37,
    0x80,

    /* U+00F3 "ó" */
    0x8, 0x43, 0x0, 0x7b, 0x38, 0x61, 0x87, 0x37,
    0x80,

    /* U+00F4 "ô" */
    0x30, 0xc4, 0x80, 0x7b, 0x38, 0x61, 0x87, 0x37,
    0x80,

    /* U+00F5 "õ" */
    0x69, 0x60, 0x1e, 0xce, 0x18, 0x61, 0xcd, 0xe0,

    /* U+00F6 "ö" */
    0x50, 0x0, 0x1e, 0xce, 0x18, 0x61, 0xcd, 0xe0,

    /* U+00F7 "÷" */
    0x10, 0xf, 0xc0, 0x10,

    /* U+00F8 "ø" */
    0x3e, 0xcd, 0x3a, 0xd7, 0x2c, 0xdf, 0x0,

    /* U+00F9 "ù" */
    0x41, 0x1, 0x18, 0xc6, 0x31, 0x9b, 0x40,

    /* U+00FA "ú" */
    0x11, 0x1, 0x18, 0xc6, 0x31, 0x9b, 0x40,

    /* U+00FB "û" */
    0x22, 0x81, 0x18, 0xc6, 0x31, 0x9b, 0x40,

    /* U+00FC "ü" */
    0x50, 0x1, 0x18, 0xc6, 0x31, 0x9b, 0x40,

    /* U+00FD "ý" */
    0x18, 0x42, 0x23, 0x89, 0x25, 0x94, 0x30, 0x82,
    0x8, 0xc0,

    /* U+00FE "þ" */
    0x84, 0x21, 0xed, 0xc6, 0x31, 0xdf, 0xa1, 0x8,
    0x0,

    /* U+00FF "ÿ" */
    0x50, 0x23, 0x14, 0xa9, 0x46, 0x21, 0x9, 0x80
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 116, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 116, .box_w = 1, .box_h = 9, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 3, .adv_w = 116, .box_w = 3, .box_h = 3, .ofs_x = 2, .ofs_y = 6},
    {.bitmap_index = 5, .adv_w = 116, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 13, .adv_w = 116, .box_w = 5, .box_h = 11, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 20, .adv_w = 116, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 28, .adv_w = 116, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 36, .adv_w = 116, .box_w = 1, .box_h = 3, .ofs_x = 3, .ofs_y = 6},
    {.bitmap_index = 37, .adv_w = 116, .box_w = 3, .box_h = 11, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 42, .adv_w = 116, .box_w = 3, .box_h = 11, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 47, .adv_w = 116, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 51, .adv_w = 116, .box_w = 6, .box_h = 6, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 56, .adv_w = 116, .box_w = 2, .box_h = 5, .ofs_x = 3, .ofs_y = -3},
    {.bitmap_index = 58, .adv_w = 116, .box_w = 5, .box_h = 1, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 59, .adv_w = 116, .box_w = 2, .box_h = 2, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 60, .adv_w = 116, .box_w = 5, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 67, .adv_w = 116, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 73, .adv_w = 116, .box_w = 5, .box_h = 9, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 79, .adv_w = 116, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 85, .adv_w = 116, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 92, .adv_w = 116, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 100, .adv_w = 116, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 107, .adv_w = 116, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 114, .adv_w = 116, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 121, .adv_w = 116, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 127, .adv_w = 116, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 134, .adv_w = 116, .box_w = 2, .box_h = 6, .ofs_x = 3, .ofs_y = 1},
    {.bitmap_index = 136, .adv_w = 116, .box_w = 2, .box_h = 9, .ofs_x = 3, .ofs_y = -3},
    {.bitmap_index = 139, .adv_w = 116, .box_w = 6, .box_h = 6, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 144, .adv_w = 116, .box_w = 6, .box_h = 4, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 147, .adv_w = 116, .box_w = 6, .box_h = 6, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 152, .adv_w = 116, .box_w = 5, .box_h = 9, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 158, .adv_w = 116, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 168, .adv_w = 116, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 176, .adv_w = 116, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 183, .adv_w = 116, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 190, .adv_w = 116, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 197, .adv_w = 116, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 203, .adv_w = 116, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 209, .adv_w = 116, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 216, .adv_w = 116, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 223, .adv_w = 116, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 229, .adv_w = 116, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 235, .adv_w = 116, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 242, .adv_w = 116, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 248, .adv_w = 116, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 255, .adv_w = 116, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 262, .adv_w = 116, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 269, .adv_w = 116, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 275, .adv_w = 116, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 285, .adv_w = 116, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 292, .adv_w = 116, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 298, .adv_w = 116, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 306, .adv_w = 116, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 312, .adv_w = 116, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 320, .adv_w = 116, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 328, .adv_w = 116, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 336, .adv_w = 116, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 344, .adv_w = 116, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 351, .adv_w = 116, .box_w = 4, .box_h = 12, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 357, .adv_w = 116, .box_w = 5, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 364, .adv_w = 116, .box_w = 4, .box_h = 12, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 370, .adv_w = 116, .box_w = 7, .box_h = 4, .ofs_x = 0, .ofs_y = 6},
    {.bitmap_index = 374, .adv_w = 116, .box_w = 6, .box_h = 1, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 375, .adv_w = 116, .box_w = 2, .box_h = 3, .ofs_x = 2, .ofs_y = 8},
    {.bitmap_index = 376, .adv_w = 116, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 381, .adv_w = 116, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 388, .adv_w = 116, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 393, .adv_w = 116, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 400, .adv_w = 116, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 406, .adv_w = 116, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 413, .adv_w = 116, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 420, .adv_w = 116, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 427, .adv_w = 116, .box_w = 4, .box_h = 10, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 432, .adv_w = 116, .box_w = 3, .box_h = 13, .ofs_x = 2, .ofs_y = -3},
    {.bitmap_index = 437, .adv_w = 116, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 445, .adv_w = 116, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 452, .adv_w = 116, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 457, .adv_w = 116, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 462, .adv_w = 116, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 468, .adv_w = 116, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 475, .adv_w = 116, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 482, .adv_w = 116, .box_w = 5, .box_h = 7, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 487, .adv_w = 116, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 492, .adv_w = 116, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 498, .adv_w = 116, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 503, .adv_w = 116, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 509, .adv_w = 116, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 516, .adv_w = 116, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 523, .adv_w = 116, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 531, .adv_w = 116, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 536, .adv_w = 116, .box_w = 5, .box_h = 12, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 544, .adv_w = 116, .box_w = 1, .box_h = 13, .ofs_x = 3, .ofs_y = -3},
    {.bitmap_index = 546, .adv_w = 116, .box_w = 5, .box_h = 12, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 554, .adv_w = 116, .box_w = 6, .box_h = 3, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 557, .adv_w = 116, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 558, .adv_w = 116, .box_w = 2, .box_h = 9, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 561, .adv_w = 116, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 570, .adv_w = 116, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 577, .adv_w = 116, .box_w = 6, .box_h = 6, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 582, .adv_w = 116, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 590, .adv_w = 116, .box_w = 1, .box_h = 11, .ofs_x = 3, .ofs_y = -2},
    {.bitmap_index = 592, .adv_w = 116, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 601, .adv_w = 116, .box_w = 3, .box_h = 1, .ofs_x = 2, .ofs_y = 8},
    {.bitmap_index = 602, .adv_w = 116, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 609, .adv_w = 116, .box_w = 4, .box_h = 7, .ofs_x = 2, .ofs_y = 3},
    {.bitmap_index = 613, .adv_w = 116, .box_w = 6, .box_h = 5, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 617, .adv_w = 116, .box_w = 6, .box_h = 3, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 620, .adv_w = 116, .box_w = 3, .box_h = 1, .ofs_x = 2, .ofs_y = 3},
    {.bitmap_index = 621, .adv_w = 116, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 628, .adv_w = 116, .box_w = 3, .box_h = 1, .ofs_x = 2, .ofs_y = 8},
    {.bitmap_index = 629, .adv_w = 116, .box_w = 4, .box_h = 4, .ofs_x = 2, .ofs_y = 5},
    {.bitmap_index = 631, .adv_w = 116, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 637, .adv_w = 116, .box_w = 3, .box_h = 4, .ofs_x = 2, .ofs_y = 7},
    {.bitmap_index = 639, .adv_w = 116, .box_w = 3, .box_h = 4, .ofs_x = 2, .ofs_y = 7},
    {.bitmap_index = 641, .adv_w = 116, .box_w = 3, .box_h = 3, .ofs_x = 3, .ofs_y = 8},
    {.bitmap_index = 643, .adv_w = 116, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 651, .adv_w = 116, .box_w = 6, .box_h = 11, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 660, .adv_w = 116, .box_w = 2, .box_h = 2, .ofs_x = 3, .ofs_y = 4},
    {.bitmap_index = 661, .adv_w = 116, .box_w = 3, .box_h = 3, .ofs_x = 2, .ofs_y = -3},
    {.bitmap_index = 663, .adv_w = 116, .box_w = 3, .box_h = 4, .ofs_x = 2, .ofs_y = 7},
    {.bitmap_index = 665, .adv_w = 116, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 668, .adv_w = 116, .box_w = 6, .box_h = 5, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 672, .adv_w = 116, .box_w = 7, .box_h = 12, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 683, .adv_w = 116, .box_w = 7, .box_h = 12, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 694, .adv_w = 116, .box_w = 7, .box_h = 12, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 705, .adv_w = 116, .box_w = 4, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 710, .adv_w = 116, .box_w = 7, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 721, .adv_w = 116, .box_w = 7, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 732, .adv_w = 116, .box_w = 7, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 743, .adv_w = 116, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 753, .adv_w = 116, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 763, .adv_w = 116, .box_w = 7, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 774, .adv_w = 116, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 782, .adv_w = 116, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 791, .adv_w = 116, .box_w = 5, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 799, .adv_w = 116, .box_w = 5, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 807, .adv_w = 116, .box_w = 5, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 815, .adv_w = 116, .box_w = 5, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 822, .adv_w = 116, .box_w = 5, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 830, .adv_w = 116, .box_w = 5, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 838, .adv_w = 116, .box_w = 5, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 846, .adv_w = 116, .box_w = 5, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 853, .adv_w = 116, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 861, .adv_w = 116, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 870, .adv_w = 116, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 879, .adv_w = 116, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 888, .adv_w = 116, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 897, .adv_w = 116, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 906, .adv_w = 116, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 915, .adv_w = 116, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 919, .adv_w = 116, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 927, .adv_w = 116, .box_w = 5, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 935, .adv_w = 116, .box_w = 5, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 943, .adv_w = 116, .box_w = 5, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 951, .adv_w = 116, .box_w = 5, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 958, .adv_w = 116, .box_w = 7, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 969, .adv_w = 116, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 976, .adv_w = 116, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 984, .adv_w = 116, .box_w = 5, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 991, .adv_w = 116, .box_w = 5, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 998, .adv_w = 116, .box_w = 5, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1005, .adv_w = 116, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1012, .adv_w = 116, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1019, .adv_w = 116, .box_w = 5, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1026, .adv_w = 116, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1033, .adv_w = 116, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 1040, .adv_w = 116, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1049, .adv_w = 116, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1058, .adv_w = 116, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1067, .adv_w = 116, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1075, .adv_w = 116, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1082, .adv_w = 116, .box_w = 4, .box_h = 10, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1087, .adv_w = 116, .box_w = 4, .box_h = 10, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1092, .adv_w = 116, .box_w = 4, .box_h = 10, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1097, .adv_w = 116, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1105, .adv_w = 116, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1112, .adv_w = 116, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1121, .adv_w = 116, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1130, .adv_w = 116, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1139, .adv_w = 116, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1147, .adv_w = 116, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1155, .adv_w = 116, .box_w = 6, .box_h = 5, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 1159, .adv_w = 116, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1166, .adv_w = 116, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1173, .adv_w = 116, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1180, .adv_w = 116, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1187, .adv_w = 116, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1194, .adv_w = 116, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 1204, .adv_w = 116, .box_w = 5, .box_h = 13, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 1213, .adv_w = 116, .box_w = 5, .box_h = 12, .ofs_x = 1, .ofs_y = -3}
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
const lv_font_t ui_font_Hack12 = {
#else
lv_font_t ui_font_Hack12 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 15,          /*The maximum line height required by the font*/
    .base_line = 3,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if UI_FONT_HACK12*/

