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

/* Includes ------------------------------------------------------------------*/
#include "libraries.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void reverse(char *str, int len);
static int intToStr(int x, char str[], int d);
static void lftoa(double n, char *res, int afterpoint);

/* Global variables ----------------------------------------------------------*/

//==============================================================================
// DEC to HEX
//==============================================================================
uint8_t put_x16(uint8_t num) {
  if (num < 10) {
    return ('0' + num);
  } else {
    return ('A' + (num - 10));
  }
}

//===================================================================
// Character concatenation
//===================================================================
char* str_concat(char *str1, const char *str2) {

  // Advance pointer to end of string
  while (*(str1++) != '\0')
    ;

  // Return one pointer to erase '\0'
  str1 -= 1;

  // Add str2
  do {
    *(str1++) = *str2;
  } while (*(str2++) != '\0');

  return str1;
}

//===================================================================
// Determine if string is decimal
//===================================================================
bool IsDecString(char *str) {
  char *endptr;
  strtol(str, &endptr, 10);

  if (*endptr == 0) {
    return true;
  } else {
    return false;
  }
}

//===================================================================
// Determine if string is hexadecimal
//===================================================================
bool IsHexString(char *str) {
  char *endptr;
  strtol(str, &endptr, 16);

  if (*endptr == 0) {
    return true;
  } else {
    return false;
  }
}

//===================================================================
// Convert hexadecimal string to 16-bit value
//===================================================================
uint16_t HexStringToU16(char *str) {
  char *endptr;
  uint16_t val = strtol(str, &endptr, 16);

  if (*endptr == 0) {
    return val;
  } else {
    return false;
  }
}

//===================================================================
// Convert dec string to dec
//===================================================================
uint16_t DecStringToDec(char *str) {
  char *endptr;
  uint16_t val = strtol(str, &endptr, 10);

  if (*endptr == 0) {
    return val;
  } else {
    return false;
  }
}

//===================================================================
// String comparison
//===================================================================
uint8_t str_compare(char *str1, const char *str2) {
  uint8_t res = strcmp(str1, str2);

  if (res == 0)
    return 1;
  else
    return 0;
}

//===================================================================
// Convert integer value to string and return
//===================================================================
char* str_putn(int32_t num, char len) {
  static uint8_t s[11];
  memset(s, 0, 11);

  uint8_t n = 1;
  if (num < 0) {
    s[0] = '-';
    num = -num;
  } else {
    n = 0;
  }
  switch (len) {
    case 9:
      s[n] = (uint8_t) ('0' + ((num % 1000000000) / 100000000));
      n++;
    case 8:
      s[n] = (uint8_t) ('0' + ((num % 100000000) / 10000000));
      n++;
    case 7:
      s[n] = (uint8_t) ('0' + ((num % 10000000) / 1000000));
      n++;
    case 6:
      s[n] = (uint8_t) ('0' + ((num % 1000000) / 100000));
      n++;
    case 5:
      s[n] = (uint8_t) ('0' + ((num % 100000) / 10000));
      n++;
    case 4:
      s[n] = (uint8_t) ('0' + ((num % 10000) / 1000));
      n++;
    case 3:
      s[n] = (uint8_t) ('0' + ((num % 1000) / 100));
      n++;
    case 2:
      s[n] = (uint8_t) ('0' + ((num % 100) / 10));
      n++;
    case 1:
      s[n] = (uint8_t) ('0' + ((num % 10) / 1));
      n++;
  }
  return (char*) s;
}

//==============================================================================
// Convert integer value to hexadecimal string
//==============================================================================
char* str_putx(uint32_t num, uint8_t len) {
  static uint8_t s[12];
  memset(s, 0, 12);
  uint8_t n = 0;
  switch (len) {
    case 8:
      s[n] = put_x16(num / 0x10000000);
      n++;
    case 7:
      s[n] = put_x16((num % 0x10000000) / 0x1000000);
      n++;
    case 6:
      s[n] = put_x16((num % 0x1000000) / 0x100000);
      n++;
    case 5:
      s[n] = put_x16((num % 0x100000) / 0x10000);
      n++;
    case 4:
      s[n] = put_x16((num % 0x10000) / 0x1000);
      n++;
    case 3:
      s[n] = put_x16((num % 0x1000) / 0x100);
      n++;
    case 2:
      s[n] = put_x16((num % 0x100) / 0x10);
      n++;
    case 1:
      s[n] = put_x16((num % 0x10));
      n++;
  }
  return (char*) s;
}

//==============================================================================
// convert int to string
//==============================================================================
char* str_putn2(int val) {
  static char str[32];
  memset(str, 0, 32);

  sprintf(str, "%d", val);
  return str;
}

//==============================================================================
// convert float to string
//==============================================================================
char* str_putf(float val, int bp, int ap) {
  static char str[32];
  memset(str, 0, 32);

  static char format[10];
  sprintf(format, "%%+0%d.%df", bp + ap + 2, ap);

  sprintf(str, format, val);
  return str;
}

char* str_putlf(double val, int ap) {
  static char str[32];
  memset(str, 0, sizeof(str));

  if (val < 0) {
    str[0] = '-';
    lftoa(-val, &str[1], ap);
  } else {
    str[0] = '+';
    lftoa(val, &str[1], ap);
  }
  return str;
}

//==============================================================================
// Determine if string is decimal.
//==============================================================================
bool IsDoubleString(char *str) {
  char *endptr;
  strtod(str, &endptr);

  if (*endptr == 0) {
    return true;
  } else {
    return false;
  }
}

//==============================================================================
// Convert a string to double.
//==============================================================================
double DoubleStringToDouble(char *str) {
  char *endptr;
  double val = strtod(str, &endptr);

  if (*endptr == 0) {
    return val;
  } else {
    return false;
  }
}

// reverses a string 'str' of length 'len'
static void reverse(char *str, int len) {
  int i = 0, j = len - 1, temp;
  while (i < j) {
    temp = str[i];
    str[i] = str[j];
    str[j] = temp;
    i++;
    j--;
  }
}

// Converts a given integer x to string str[].  d is the number
// of digits required in output. If d is more than the number
// of digits in x, then 0s are added at the beginning.
static int intToStr(int x, char str[], int d) {
  int i = 0;
  if (x == 0) {
    str[i++] = '0';
    return i;
  }
  while (x) {
    str[i++] = (x % 10) + '0';
    x = x / 10;
  }

  // If number of digits required is more, then
  // add 0s at the beginning
  while (i < d)
    str[i++] = '0';

  reverse(str, i);
  str[i] = '\0';
  return i;
}

// Converts a given integer x to string str[].  d is the number
// of digits required in output. If d is more than the number
// of digits in x, then 0s are added at the beginning.
static int intToStr2(int x, char str[], int d) {
  int i = 0;
//	if(x == 0){
//		str[i++] = '0';
//		return i;
//	}
  while (x) {
    str[i++] = (x % 10) + '0';
    x = x / 10;
  }

  // If number of digits required is more, then
  // add 0s at the beginning
  while (i < d)
    str[i++] = '0';

  reverse(str, i);
  str[i] = '\0';
  return i;
}

long my_pow(long a, long n) {
  long ret = 1;
  for (; n > 0; n >>= 1, a = a * a) {
    if (n % 2 == 1)
      ret = ret * a;
  }
  return ret;
}

static void lftoa(double n, char *res, int afterpoint) {
  // Extract integer part
  int ipart = (int) n;

  // Extract doubleing part
  double lfpart = n - (double) ipart;

  // convert integer part to string
  int i = intToStr(ipart, res, 0);

  double x = (double) my_pow(10, afterpoint);

  // check for display option after point
  if (afterpoint != 0) {
    res[i] = '.';  // add dot

    // Get the value of fraction part upto given no.
    // of points after dot. The third parameter is needed
    // to handle cases like 233.007
    lfpart = lfpart * x;

    intToStr2((int) lfpart, res + i + 1, afterpoint);
  }

}

uint32_t StringSplit(char *str, char **plist, char *delim, uint32_t max) {
  uint32_t len = 0;
  char *word_ptr = strtok(str, delim);
  while (word_ptr != NULL) {
    plist[len++] = word_ptr;
    if (len >= max) {
      break;
    }
    word_ptr = strtok(NULL, delim);
  }

  return len;
}

char ToUpper(char c){
  if ((c > 0x60) && (c < 0x7B)) {
    c -=0x20;
  }
  return c;
}
