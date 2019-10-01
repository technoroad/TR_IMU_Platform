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

#ifndef INITIAL_PARAMETER_H_
#define INITIAL_PARAMETER_H_

#include <libraries.h>

#define VERSION 0x20191001		// Version
#define gain_p_init 1.0				// Kp gain
#define gain_i_init 0.1				// Ki gain
#define send_cycle_ms_init 10		//Send cycle[ms]

// Standby counter at startup
#if defined(ADIS16470)
#define startup_time_s_init 120;		// 120[s]
#elif defined(ADIS16475)
#define startup_time_s_init 120;		// 120[s]
#elif defined(ADIS16495)
#define startup_time_s_init 40;		// 40[s]
#endif

#define CSUM_SIZE 1

#endif /* INITIAL_PARAMETER_H_ */
