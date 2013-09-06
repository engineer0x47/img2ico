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

#include <iostream>
#include <fstream>

#pragma once

#ifdef WIN32
#define _SZ_PATHSEPARATOR "//"
#else
#define _SZ_PATHSEPARATOR "/\"
#endif

#ifndef __IMG2ICO_H__
#define __IMG2ICO_H__

#define _PNG_HEADER_DWORD	0x89504E47
#define _PNG_CHUNK_IHDR		0x49484452
#define _PNG_CHUNK_PLTE		0x504C5445
#define _BMP_HEADER_WORD	0x4D42
#define _BMP_BI_RGB			0

enum F_TYPE
{
	T_ICO = 1,
	T_CUR = 2,
	T_ANI = 3
};

enum I_TYPE
{
	I_ICON		= 1,
	I_RAW		= 0,
	I_SEQUENCE	= 2
};

union uBuffer
{
	__int8	byte[4];
	__int16	word[2];
	__int32	dword;
};

//ANI: Cursor name, Artist information, default frame rate, sequence info, cursor hotspot, individual frames in ico format, individual frame rates
struct sANI_Header
{
	int	NumFrames;	// Also number of images for ICO / CUR
	int	NumSteps;
	int	Width;
	int	Height;
	int	BitCount;
	int	DisplayRate;
	int	Flags;

	sANI_Header();
};

struct sANI_Chunk
{
	__int8	tag[4];
	uBuffer	size;
	__int8*	data;
};

typedef union IconDirEntry
{
	struct s
	{
		__int8	Width;
		__int8	Height;
		__int8	ColorCount;
		__int8	Reserved;
		__int16	Planes_Hcor;
		__int16	BPP_Vcor;
		__int32	Size;
		__int32 Offset;
	} s;
	char	bytes[16];
} IconDirEntry;

typedef struct IconImage
{
	union header
	{
		struct s
		{
			__int32	biSize;
			__int32	biWidth;
			__int32	biHeight;
			__int16	biPlanes;
			__int16	biBitCount;
			__int32	biCompression;
			__int32	biSizeImage;
			__int32	biXPelsPerMeter;
			__int32	biYPelsPerMeter;
			__int32	biClrUsed;
			__int32	biClrImportant;
		} s;
		char	h_bytes[40];
	} header;

	__int8* colors;
	__int8*	xor;
	__int8*	and;
} IconImage;

struct sImage
{
	IconDirEntry	dir;
	IconImage		img;

	sImage();
};

class CIMG2ICO
{
private:
	std::string		m_szPath;
	std::string		m_szName;
	sANI_Header		m_sANI_Header;
	int				m_iType;
	int				m_iCount;
	bool			m_bSequenceData;
	bool			m_bUseRawData;
	sImage*			m_sImageArray;

	int		LoadImage(const char* filename, struct sImage* imageout);
	int		ReadConfigFile(void);
	int		ReadInputFiles(void);
	int		WriteOutputFile(void);

public:
	CIMG2ICO(const char* path = "", const char* name = "icon.ico", const int type = T_ICO);
	~CIMG2ICO();

	void	SetDirectoryPath(const char* path);
	void	SetOutputFileType(int type);
	void	SetOutputFileName(const char* name);
	int		ConvertFiles(void);
};

#endif