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

#include "load_img.h"

using namespace std;

int	LoadImage(char* filename, struct sImage* imageout)
{
	int		retval = 0;
	uBuffer	buffer;

	fstream imagefile;

	imagefile.open(filename, ios::in);

	if (imagefile.is_open() == true)
	{
		imagefile >> buffer.dword;

		if (buffer.dword == _PNG_HEADER_DWORD)
		{
			imagefile >> buffer.dword >> buffer.dword;
			
			if (buffer.dword == _PNG_CHUNK_IHDR)
			{
				// Load image parameters
			}

			if (buffer.dword == _PNG_CHUNK_PLTE)
			{
				// Load image palette
			}

			// Load entire PNG into sImage.imagedata

		}
		else if (buffer.word[0] == _BMP_HEADER_WORD)
		{
			__int32 DataOffset = 0;

			imagefile >> buffer.dword >> buffer.dword >> DataOffset >> imageout->Width >> imageout->Height >> buffer.word[0] >> imageout->BPP_Vcor >> buffer.dword;

			if (buffer.dword == _BMP_BI_RGB)
			{
				imageout->Size = (imageout->BPP_Vcor * imageout->Height * imageout->Width);
				//int imgsize = imagefile.tellg();
				//imgsize -= DataOffset;
				imagefile.seekg(DataOffset, imagefile.beg);
				imageout->imagedata = new int[imageout->Size];
			}
			else
			{
				retval = 1;
			}
		}
		else
		{
			retval = 1;		// File is not BMP or PNG
		}

		imagefile.close();
	}
	else
	{
		retval = 1;
	}

	return retval;
}