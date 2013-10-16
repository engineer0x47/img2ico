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

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::fstream;
using std::ios;
using std::multimap;
using std::string;
using std::vector;

#pragma once

#ifndef __UTIL_H__
#define __UTIL_H__


#ifdef NDEBUG
// ANI files are not supported at this time and won't be part of the help message
// shown unless this is a debug build.
#define _IMG2ICO_ANI_UNSUPPORTED
#endif

#ifdef WIN32
#define SZ_PATHSEPARATOR "//"
#else
#define SZ_PATHSEPARATOR "/\"
#endif

#define IMG2ICO_VERSION 	"0.1.1.00219"

#define IMG2ICO_SZ_MAXLEN	64
#define IMG2ICO_PATH_MAXLEN	192
#define IMG2ICO_MAX_DIM		256			// Maximum image size of 256x256
#define IMG2ICO_MAX_BPP		32

#define PNG_HEADER_DWORD	0x89504E47
#define PNG_CHUNK_IHDR		0x49484452
#define PNG_CHUNK_PLTE		0x504C5445
#define BMP_HEADER_WORD		0x4D42
#define BMP_BI_RGB			0
#define BMP_HEADER_SIZE		40

#define data_block vector<__int8>
typedef unsigned __int8		__uint8;
typedef unsigned __int16	__uint16;
typedef unsigned __int32	__uint32;

enum IMG2ICO_ERROR
{
	I2IE_SUCCESS			= 0x00,
	I2IE_FILE_FAILED		= 0x01,
	I2IE_FILE_NOT_FOUND		= 0x02,
	I2IE_FILE_UNSUPPORTED	= 0x04,
	I2IE_FILE_COMPRESSION	= 0x08,
	I2IE_EMPTY_DIRECTORY	= 0x10,
	I2IE_EMPTY_OUTPUT		= 0x20,
	I2IE_NO_CONFIGFILE		= 0x40,
	I2IE_UNKNOWN			= 0x80
};

enum F_TYPE
{
	T_ICO = 1,
	T_CUR = 2,
	T_ANI = 3
};

enum I_TYPE
{
	I_RAW		= 0,
	I_ICON		= 1,
	I_SEQUENCE	= 2
};

union uBuffer
{
	__int8		byte[4];
	__int16		word[2];
	__int32		dword;
};

union uBuffer_u
{
	__uint8		byte[4];
	__uint16	word[2];
	__uint32	dword;
};

enum chunkID
{
	ID_RIFF	= 0,
	ID_RIFX	= 0,
	ID_LIST	= 0,
	ID_ANIH	= 1,
	ID_JUNK	= 2,

};

struct chunk
{
	char	tag[4];
	__int32	size;
	data_block	pData;
};

struct image
{
	__int32	Width;
	__int32	Height;
	__int16	NumPlanes;
	__int16	BitsPerPixel;
	__int32 ImgSize;
	__int32 MaskSize;
	__int32 FileSize;
	data_block	pData;
};

struct sParameters
{
	__int32		FileType;			// ICO, ANI, CUR
	__int32		ImageCount;			// Number of images for ICO / CUR, frames for ANI, 256 max
	__int32		Width;				// Master width
	__int32		Height;				// Master height
	__int32		BitsPerPixel;		// Master BPP
	__uint32	ColorTransparent;	// Master transparent color, used for mask generation
	__int32		Hcoord;				// For CUR / ANI hotspot only
	__int32		Vcoord;				// For CUR / ANI hotspot only
	__int32		DisplayRate;		// ANI Framerate
	__int32		NumSteps;			// Only applies to ANI files if no 'seq ' chunk is present
};

void	szToLcase(char* sz, const int size);
void	ZeroBuffer(uBuffer* buffer, const int numDwords);
void	ZeroBuffer(uBuffer_u* buffer, const int numDwords);

// If it is already RGB332 (8BPP) or RGB565 (16BPP) PackColors is much faster!
__uint32	PackColors(const __uint8 a, const __uint8 r, const __uint8 g, const __uint8 b, const __uint8 bpp);		// Pack colors, range is already correct
__uint32	PackColors8(const __uint8 a, const __uint8 r, const __uint8 g, const __uint8 b, const __uint8 bpp);		// Pack colors and convert to proper bpp

#endif