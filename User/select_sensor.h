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

#ifndef INC_SELECT_SENSOR_H_
#define INC_SELECT_SENSOR_H_

// Sensor selection
//#define ADIS16470
//#define ADIS16475
//#define ADIS16477
//#define ADIS16495
//#define ADIS16500
#define ADIS16505

// Selection of series
#if defined(ADIS16470) | defined(ADIS16500)
#define NONE_BMLZ
#else
//#define _1BMLZ //unconfirmed
#define _2BMLZ //confirmed
//#define _3BMLZ //unconfirmed
#endif

// Source code change by sensor.
#if defined(ADIS16470)
#include "adis16470.h"
#define ADIS1647X
#elif defined(ADIS16475)
#include "adis16475.h"
#define ADIS1647X
#elif defined(ADIS16477)
#include "adis16477.h"
#define ADIS1647X
#define AVAILABLE_32BIT_BURST
#define SUPPORT_BURST_FIRM_REV 0x134
#elif defined(ADIS16500)
#include "adis16500.h"
#define ADIS1647X
#define AVAILABLE_32BIT_BURST
#define SUPPORT_BURST_FIRM_REV 0x000
#elif defined(ADIS16505)
#include "adis16505.h"
#define ADIS1647X
#define AVAILABLE_32BIT_BURST
#define SUPPORT_BURST_FIRM_REV 0x000
#elif defined(ADIS16495)
#include "adis16495.h"
#define ADIS1649X
#define AVAILABLE_32BIT_BURST
#define SUPPORT_BURST_FIRM_REV 0x000
#endif

// Series definition check.
#if !defined(ADIS1647X) && !defined(ADIS1649X)
#error "Define either ADIS1647X or ADIS1649X."
#endif

// BMLZ definition check.
#if !defined(NONE_BMLZ) && !defined(_1BMLZ) && !defined(_2BMLZ) && !defined(_3BMLZ)
#error "Check the BMLZ selection."
#endif

#endif /* INC_SELECT_SENSOR_H_ */
