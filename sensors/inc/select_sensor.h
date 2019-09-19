/*
The MIT License (MIT)

Copyright (c) 2019 Techno Road Inc.

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

// Please select a sensor.
//#define ADIS16470
//#define ADIS16475
#define ADIS16495

// Switching source code read by sensor.
#if defined(ADIS16470)
	#include "adis16470.h"
	#define NONE_BMLZ
#elif defined(ADIS16475)
	#include "adis16475.h"
#elif defined(ADIS16495)
	#include "adis16495.h"
#endif

// Selection of series
//#define _1BMLZ //unconfirmed
#define _2BMLZ //confirmed
//#define _3BMLZ //unconfirmed

//Standard Gravity Acceleration
#define G_ACCL 9.80665

#if defined(ADIS16470)|defined(ADIS16475)
	#define ADIS1647X
#endif

#endif /* INC_SELECT_SENSOR_H_ */
