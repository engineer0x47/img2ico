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

sImage::sImage()
{
	imgbytes = nullptr;

	for (int i = 0; i < 16; i++)
	{
		hdr.bytes[i] = 0;
	}
}

CIMG2ICO::CIMG2ICO(char* path, int type)
{
	m_szPath.assign(path);
	m_bSequenceData = false;
	m_bUseRawData = false;
	m_sImageArray = nullptr;
	m_iType = type;
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

			if (imageout->hdr.s.Size == _BMP_BI_RGB)
			{
				imageout->hdr.s.Size = (imageout->hdr.s.Height * imageout->hdr.s.Width * imageout->hdr.s.BPP_Vcor / 8);
				imagefile.seekg(imageout->hdr.s.Offset, imagefile.beg);
				imageout->imgbytes = new char[imageout->hdr.s.Size];
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

int	CIMG2ICO::ReadConfigFile(void)
{
	int		retval = 0;
	string	szConfigFilename;
	fstream	c_file;

	if ((m_szPath.length()) != 0)
	{
		szConfigFilename.assign(m_szPath);
		szConfigFilename.append(_SZ_PATHSEPARATOR);
	}
	
	szConfigFilename.append("config");
	c_file.open(szConfigFilename.data(), ios::in);

	if (c_file.is_open())
	{
		// Read config parameters into m_sFileInfo

		c_file.close();
	}
	else
	{
		if (m_iType == T_ANI)
		{
			retval = 40;
		}
	}

	return retval;
}

int		CIMG2ICO::ReadInputFiles()
{
	int retval = 0;

	// Find out which directory we are in

	// Read config file if present
	ReadConfigFile();

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

void	CIMG2ICO::SetOutputFileType(int type)
{
	m_iType = type;
}

int		CIMG2ICO::WriteOutputFile(char* outfile)
{
	int		retval = 0;
	fstream	file;
	string	szOutFilename;
		
	if (m_sFileInfo.NumFrames != 0)
	{
		if ((m_szPath.length()) != 0)
		{
			szOutFilename.assign(m_szPath);
			szOutFilename.append(_SZ_PATHSEPARATOR);
		}
		szOutFilename.append(outfile);

		file.open(szOutFilename.data(), ios::out | ios::binary);

		if (file.is_open())
		{
			// Write file header
			switch(m_iType)
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
				buf[0].word[1] = m_iType;
				buf[1].word[0] = m_sFileInfo.NumFrames;

				file.write(&buf[0].byte[0], 6);

			}

			// Write file body
			switch(m_iType)
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
					m_sImageArray[i].hdr.s.Offset = (i == 0) ? 22 : (m_sImageArray[i-1].hdr.s.Offset + m_sImageArray[i-1].hdr.s.Size);
					file << m_sImageArray[i];
				}

				// Write images
				for (int i = 0; i < m_sFileInfo.NumFrames; i++)
				{
					file.write(m_sImageArray->imgbytes, m_sImageArray->hdr.s.Size);
				}

				uBuffer padding[20];
				for (int i = 0; i <20; i++)
				{
					padding[i].dword = 0;
				}

				file.write(&padding[0].byte[0], 80);
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
	
	image->hdr.s.Offset		= buf[2].dword;
	image->hdr.s.Width		= buf[4].dword;
	image->hdr.s.Height		= buf[5].dword;
	image->hdr.s.BPP_Vcor	= buf[6].word[1];
	image->hdr.s.Size		= buf[7].dword;
	
	return in;
}

std::fstream& operator<<(std::fstream &out, const sImage image)
{
	out.write(&image.hdr.bytes[0], 16 );

	return out;
}