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

#include <Windows.h>
#include <queue>
#include <string>
#define WIN32_LEAN_AND_MEAN

#pragma once

#ifndef __IMG2ICO_DLL_H__
#define __IMG2ICO_DLL_H__

struct sICO_Message
{
	__int8	Command;
	__int8	numbers[4];
	std::string message;
};

struct sICO_MessageQueue
{
	std::queue<sICO_Message> msg_queue;
	
};

sICO_MessageQueue	g_ICO_MessageQueue;

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call,	LPVOID lpReserved);

#define DLL_EXPORT __declspec( dllexport )

void	DLL_EXPORT LoadImageFile(const char* filename);
void	DLL_EXPORT WriteOutputFile(void);

int		DLL_EXPORT ConvertFiles(void);
int		DLL_EXPORT GetErrorCodes(void);
void	DLL_EXPORT ResetErrorCodes(void);

// Animated icons and cursors
void	DLL_EXPORT SetCursorHotspot(const int h = 0, const int v = 0);

// Animated icons only
void	DLL_EXPORT SetArtistName(const char* artist);
void	DLL_EXPORT SetDefaultFrameRate(const int rate);

// Works for all file types
void	DLL_EXPORT SetDirectoryInputPath(const char* in_path);
void	DLL_EXPORT SetDirectoryOutputPath(const char* out_path);
void	DLL_EXPORT SetOutputFileType(const int type);
void	DLL_EXPORT SetOutputFileName(const char* name);
void	DLL_EXPORT SetTransparentColor(const int r, const int g, const int b, const int image_index);


#endif
