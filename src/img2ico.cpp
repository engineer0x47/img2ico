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

#include "img2ico.h"

using namespace std;

CIMG2ICO::CIMG2ICO(char* path)
{
	m_szPath.assign(path);
	m_bSequenceData = false;
	m_bUseRawData = false;
	m_sImageArray = nullptr;

	m_sFileInfo.BitCount = 0;
	m_sFileInfo.DisplayRate = 0;
	m_sFileInfo.Height = 0;
	m_sFileInfo.NumFrames = 1;
	m_sFileInfo.NumSteps = 1;
	m_sFileInfo.Flags = 0 | I_ICON;
}

CIMG2ICO::~CIMG2ICO()
{
	// Free any allocated bitmaps
	if (m_sImageArray != nullptr)
	{
	//	delete m_sImageArray->imagedata;
	}
}


int	CIMG2ICO::LoadImage(char* filename, struct sImage* imageout)
{
	int		retval = 0;
	uBuffer	buffer;
	fstream imagefile;

	buffer.dword = 0;
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
		printf("failed to open bmp file");
	}

	return retval;
}

int		CIMG2ICO::ReadInputFiles()
{
	int retval = 0;

	// Find out which directory we are in

	// Read config file if present

	// Find out how many images are in the directory (PNG or BMP only)

	// Allocate array for images

	// Load images into sImages

	//// Start test code
	m_sImageArray = new sImage;
	
	retval = LoadImage("0.bmp", &m_sImageArray[0]);	// temporary code

	//// End test code

	// Populate all class variables


	return retval;
}

void	CIMG2ICO::SetDirectoryPath(char* path)
{
	m_szPath.assign(path);
}

int		CIMG2ICO::WriteFile(char* outfile, int type)
{
	int		retval = 0;
	fstream	file;
	string	outfilename;
		
	if ((m_szPath.length()) != 0)
	{
		outfilename.assign(m_szPath);
		outfilename.append(_SZ_PATHSEPARATOR);
	}
	outfilename.append(outfile);

	file.open(outfilename.data(), std::ios::out);

	if (file.is_open())
	{
		// Write file header
		switch(type)
		{
		case T_ANI:
			file << "ACON" << ((__int32)(32)) << ((__int32)(m_sFileInfo.NumFrames)) << ((__int32)(m_sFileInfo.NumSteps)) << ((__int32)(m_sFileInfo.Width)) << ((__int32)(m_sFileInfo.Height));
			file << ((__int32)(m_sFileInfo.BitCount)) << ((__int32)(1)) << ((__int32)(m_sFileInfo.DisplayRate)) << ((__int32)(m_sFileInfo.Flags)) << "fram";
			retval = 1;
			break;
		default:
		case T_CUR:
		case T_ICO:
			file << ((__int16)(0)) << ((__int16)(type)) << ((__int16)m_sFileInfo.NumFrames) << ((__int16)(m_sFileInfo.NumFrames * 16));
		}

		// Write file body
		switch(type)
		{
		case T_ANI:
			// Write frames
			
			if (m_bSequenceData == true)
			{
				// write sequence data
			}
			retval = 1;
			break;
		default:
		case T_CUR:
		case T_ICO:
			// Build image directory
			for (int i = 0; i < m_sFileInfo.NumFrames; i++)
			{
				file << m_sImageArray[i];
			}
			// Write images
			for (int i = 0; i < m_sFileInfo.NumFrames; i++)
			{
				file << m_sImageArray[i].imagedata;
			}
		}

		file.close();
	}
	else
	{
		retval = 1;
		printf("failed to open icon file");
	}

	return retval;
}

std::fstream& operator<<(std::fstream &out, const sImage image)
{
	out << image.Width << image.Height << image.Colors << image.Reserved << image.Planes_Hcor << image.BPP_Vcor << image.Size;
	
	return out;
}