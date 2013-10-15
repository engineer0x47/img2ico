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

#include "img2ico_util.h"

void	szToLcase(char* sz, const int size)
{
	for (int j = 0; j < size; j++)
	{
		sz[j] = tolower(sz[j]);
	}
}

void	ZeroBuffer(uBuffer* buffer, const int numDwords)
{
	for (int i = 0; i < numDwords; i++)
	{
		buffer[i].dword = 0;
	}
}

__uint32	PackColors(const __uint8 a, const __uint8 r, const __uint8 g, const __uint8 b, const __uint8 bpp)
{
	uBuffer_u packed;
	packed.dword = 0;

	switch (bpp)
	{
	case 8:
		packed.byte[3] = ( (r << 5) | (g << 2) | b );
		break;
	case 16:
		packed.word[1] = ( (r << 11) | (g << 5) | b );
		break;
	case 24:
		packed.byte[1] = r;
		packed.byte[2] = g;
		packed.byte[3] = b;
		break;
	case 32:
		packed.byte[0] = a;
		packed.byte[1] = r;
		packed.byte[2] = g;
		packed.byte[3] = b;
	}

	return packed.dword;
}

__uint32	PackColors8(const __uint8 a, const __uint8 r, const __uint8 g, const __uint8 b, const __uint8 bpp)
{
	uBuffer_u packed;
	packed.dword = 0;

	switch (bpp)
	{
	case 8:
		packed.byte[3] = ( ((r*15/255) & 0x07) << 5) | (((g*15/255) & 0x07) << 2) | ((b*3/255) & 0x03);
		break;
	case 16:
		packed.word[1] = ( ((r*31/255) & 0x1F) << 11) | (((g*63/255) & 0x3F) << 5) | ((b*31/255) & 0x1F);
		break;
	case 24:
		packed.byte[1] = r;
		packed.byte[2] = g;
		packed.byte[3] = b;
		break;
	case 32:
		packed.byte[0] = a;
		packed.byte[1] = r;
		packed.byte[2] = g;
		packed.byte[3] = b;
	}

	return packed.dword;
}