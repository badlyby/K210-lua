/*
The MIT License (MIT)

Copyright (c) 2020 badlyby

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef __LOADBMP_H__
#define __LOADBMP_H__

#include<stdint.h>
#include<stdlib.h>
#include<stdio.h>
#include "ff.h"

static inline uint16_t RGB565(int r, int g, int b)
{
	r <<= 8;
	r &= 0xF800;
	g <<= 3;
	g &= 0x7E0;
	b >>= 3;
	b &= 0x1F;
	return r | g | b; 
}
int read_bmp_size(const char *name, uint32_t *width, uint32_t *height);
int loadbmp(const char *name, uint16_t *imgbuf);

#endif //__LOADBMP_H__