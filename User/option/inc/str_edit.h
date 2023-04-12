/*
 The MIT License (MIT)

 Copyright (c) 2023 Techno Road Inc.

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */

#ifndef STR_EDIT_H_
#define STR_EDIT_H_

// USBCDC バイナリ10進数→ASCII16進数変換
extern uint8_t put_x16(uint8_t num);

char* str_concat(char *str1, const char *str2);

bool IsDecString(char *str);
bool IsHexString(char *str);
uint16_t HexStringToU16(char *str);
uint16_t DecStringToDec(char *str);

char* str_putn(int32_t num, char len);
char* str_putn2(int val);

char* str_putx(uint32_t num, uint8_t len);
char* str_putf(float val, int bp, int ap);
char* str_putlf(double val, int ap);

bool IsDoubleString(char *str);
double DoubleStringToDouble(char *str);

uint32_t StringSplit(char *str, char **plist, char *delim, uint32_t max);
char ToUpper(char c);
#endif /* STR_EDIT_H_ */
