/*
Copyright (c) 2013 Aaron Stone

Licensed under The MIT License (MIT):

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

#include <fstream>

#pragma once

#ifndef __LOAD_IMG_H__
#define __LOAD_IMG_H__

#define _PNG_HEADER_DWORD	0x89504E47
#define _PNG_CHUNK_IHDR		0x49484452
#define _PNG_CHUNK_PLTE		0x504C5445
#define _BMP_HEADER_WORD	0x4D42
#define _BMP_BI_RGB			0

struct sImage
{
	__int8	Width;
	__int8	Height;
	__int8	Colors;
	__int8	Reserved;
	__int16	Planes_Hcor;
	__int16	BPP_Vcor;
	__int32	Size;
	__int32 offset;

	int*	imagedata;
};

union uBuffer
{
	__int8	byte[4];
	__int16	word[2];
	__int32	dword;
};

int	LoadImage(char* filename, struct sImage* imageout);


#endif