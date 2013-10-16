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
	int									m_iErrorCode;
	sParameters							m_Params;
	string								m_szInPath;
	string								m_szOutPath;
	string								m_szName;
	vector<image>						m_ImageArray;
	vector<image>::iterator				m_itImageArray;
	multimap<chunkID, chunk>			m_ChunkList;
	multimap<chunkID, chunk>::iterator	m_itChunkList;

	void	ReadConfigFile(void);
	void	ReadInputFiles(void);
	void	ResampleImage(image* image);		// This routine is iterated through to generate mipmaps

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
	void	SetTransparentColor(const __int8 r, const __int8 g, const __int8 b);
};

#endif