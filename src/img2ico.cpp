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
	m_sFileInfo.Width = 0;
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
	imagefile.open(filename, ios::in | ios::binary);

	if (imagefile.is_open() == true)
	{
		imagefile.read(&buffer.byte[0], 4);

		if (buffer.dword == _PNG_HEADER_DWORD)
		{
			imagefile.read(&buffer.byte[0], 4);
			imagefile.read(&buffer.byte[0], 4);
			
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
			imagefile >> imageout;

			if (imageout->Size == _BMP_BI_RGB)
			{
				imageout->Size = (imageout->Height * imageout->Width * imageout->BPP_Vcor / 8);
				imagefile.seekg(imageout->Offset, imagefile.beg);
				imageout->imgbytes = new char[imageout->Size];
				imagefile.read(imageout->imgbytes, sizeof(imageout->imgbytes));
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
		m_sFileInfo.NumFrames = 0;
		retval = -1;
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

	// Populate all class variables



	
	//// Start test code
	m_sImageArray = new sImage;
	
	retval = LoadImage("0.bmp", &m_sImageArray[0]);	// temporary code

	//// End test code


	return retval;
}

void	CIMG2ICO::SetDirectoryPath(char* path)
{
	m_szPath.assign(path);
}

int		CIMG2ICO::WriteOutputFile(char* outfile, int type)
{
	int		retval = 0;
	fstream	file;
	string	outfilename;
		
	if (m_sFileInfo.NumFrames != 0)
	{
		if ((m_szPath.length()) != 0)
		{
			outfilename.assign(m_szPath);
			outfilename.append(_SZ_PATHSEPARATOR);
		}
		outfilename.append(outfile);

		file.open(outfilename.data(), ios::out | ios::binary);

		if (file.is_open())
		{
			// Write file header
			switch(type)
			{
			case T_ANI:
				uBuffer aBuf[32];

				file << "ACON" << ((__int32)(32)) << ((__int32)(m_sFileInfo.NumFrames)) << ((__int32)(m_sFileInfo.NumSteps)) << ((__int32)(m_sFileInfo.Width)) << ((__int32)(m_sFileInfo.Height));
				file << ((__int32)(m_sFileInfo.BitCount)) << ((__int32)(1)) << ((__int32)(m_sFileInfo.DisplayRate)) << ((__int32)(m_sFileInfo.Flags)) << "fram";
				retval = 1;
				file.write(&aBuf[0].byte[0], sizeof(aBuf));
				break;
			default:
			case T_CUR:
			case T_ICO:
				uBuffer buf[2];

				buf[0].word[0] = 0;
				buf[0].word[1] = type;
				buf[1].word[0] = m_sFileInfo.NumFrames;

				file.write(&buf[0].byte[0], 6);

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
					m_sImageArray[i].Offset = (i == 0) ? 22 : (m_sImageArray[i-1].Offset + m_sImageArray[i-1].Size);
					file << m_sImageArray[i];
				}
				// Write images
				for (int i = 0; i < m_sFileInfo.NumFrames; i++)
				{
					file.write(m_sImageArray->imgbytes, m_sImageArray->Size);
				}
			}

			file.close();
		}
		else
		{
			retval = 1;
		}
	}

	return retval;
}

std::fstream& operator>>(std::fstream &in, sImage* image)
{
	uBuffer buf[8];

	in.read(&buf[0].byte[2], sizeof(buf)-2);

	image->Offset		= buf[2].dword;
	image->Width		= buf[4].dword;
	image->Height		= buf[5].dword;
	image->BPP_Vcor		= buf[6].word[1];
	image->Size			= buf[7].dword;
	
	return in;
}

std::fstream& operator<<(std::fstream &out, const sImage image)
{
	uBuffer buf[4];

	buf[0].byte[0]	= image.Width;
	buf[0].byte[1]	= image.Height;
	buf[0].byte[2]	= image.Colors;
	buf[0].byte[3]	= image.Reserved;
	buf[1].word[0]	= image.Planes_Hcor;
	buf[1].word[1]	= image.BPP_Vcor;
	buf[2].dword	= image.Size;
	buf[3].dword	= image.Offset;
	
	out.write(&buf[0].byte[0], sizeof(buf));

	return out;
}