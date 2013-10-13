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

#pragma once

#ifndef __IMG2ICO_H__
#define __IMG2ICO_H__

class CIMG2ICO
{
private:
	int				m_iErrorCode;
	sParameters		m_Params;
	string			m_szInPath;
	string			m_szOutPath;
	string			m_szName;

	vector<data_block>			m_ImageArray;
	multimap<chunkID, chunk>	m_ChunkList;

	void	ReadConfigFile(void);
	void	ReadInputFiles(void);

public:
	CIMG2ICO(const char* path = ".", const char* name = "icon", const int type = T_ICO);
	~CIMG2ICO();
		
	void	LoadImage(const char* filename);
	void	WriteOutputFile(void);
	int		ConvertFiles(void);
	int		GetErrorCodes(void);
	void	ResetErrorCodes(void);

	// Animated icons and cursors
	void	SetCursorHotspot(const int h = 0, const int v = 0);

	// Animated icons only
	void	SetArtistNameANI(const char* artist);
	void	SetDefaultFrameRateANI(const int rate);

	// Works for all file types
	void	SetDirectoryInputPath(const char* in_path);
	void	SetDirectoryOutputPath(const char* out_path);
	void	SetOutputFileType(const int type);
	void	SetOutputFileName(const char* name);
	void	SetTransparentColor(const int r, const int g, const int b, const int image_index);
};

#endif


/*
union sICO_Header
{
	struct s
	{
		__int16	Reserved;
		__int16	Type;
		__int16	Count;
	} s;
	char	bytes[6];
};

union IconDirEntry
{
	struct s
	{
		__int8	Width;
		__int8	Height;
		__int8	ColorCount;
		__int8	Reserved;
		__int16	NumPlanes_Hcor;
		__int16	BPP_Vcor;
		__int32	Size;
		__int32 Offset;
	} s;
	char	bytes[16];
};


struct IconImage
{
	union header
	{
		struct s
		{
			__int32	HeaderSize;
			__int32	Width;
			__int32	Height;
			__int16	NumPlanes;
			__int16	BitsPerPixel;
			__int32	CompressionType;
			__int32	ImageSize;
			__int32	XPelsPerMeter;
			__int32	YPelsPerMeter;
			__int32	ClrUsed;
			__int32	ClrImportant;
		} s;
		char	h_bytes[40];
	} header;

	__int8*	xor;
	__int8*	and;

	int		AndmaskSize;
	int		XorSize;

	__int32	TransparentColor;
};

struct sImage
{
	IconDirEntry	dir;
	IconImage		img;

	sImage();
};



union sANI_Header
{
	struct s
	{
		__int32	HeaderID;
		__int32	HeaderSize;
		__int32	NumFrames;	// Also number of images for ICO / CUR
		__int32	NumSteps;
		__int32	Width;
		__int32	Height;
		__int32	BitsPerPixel;
		__int32 NumPlanes;
		__int32	DisplayRate;
		__int32	Flags;
	} s;
	char	bytes[40];

	sANI_Header();
};

union sANI_Chunk
{
	struct s
	{
		__int8	tag[4];
		__int32	size;
	} s;
	char	bytes[8];

	__int8*	data;

	sANI_Chunk();
};
*/