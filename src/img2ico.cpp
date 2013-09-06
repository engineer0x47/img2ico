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

std::fstream& operator>>(std::fstream &in, sImage* image);
std::fstream& operator<<(std::fstream &out, IconDirEntry icon_dir);
std::fstream& operator<<(std::fstream &out, IconImage image);

sANI_Header::sANI_Header()
{
	NumFrames = 1;	// Also number of images for ICO / CUR
	NumSteps = 1;
	Width = 1;
	Height = 1;
	BitCount = 8;
	DisplayRate = 30;
	Flags = 0 | I_ICON;
}

sImage::sImage()
{
	img.and = nullptr;
	img.xor = nullptr;
	img.colors = nullptr;

	for (int i = 0; i < 40; i++)
	{
		img.header.h_bytes[i] = 0;

		if (i < 16)
		{
			dir.bytes[i] = 0;
		}
	}
}

CIMG2ICO::CIMG2ICO(const char* path, const char* name, int type)
{
	m_szPath.assign(path);
	m_szName.assign(name);
	m_bSequenceData = false;
	m_bUseRawData = false;
	m_sImageArray = nullptr;
	m_iType = type;
	m_iCount = 1;
}

CIMG2ICO::~CIMG2ICO()
{
	// Free any allocated bitmaps
	if (m_sImageArray == nullptr)
	{
	//	delete m_sImageArray->imagedata;
	}
}


int	CIMG2ICO::LoadImage(const char* filename, struct sImage* image)
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
			imagefile >> image;

			if (image->img.header.s.biCompression == _BMP_BI_RGB)
			{
				imagefile.seekg(image->dir.s.Offset, imagefile.beg);
				image->img.colors = new __int8[image->dir.s.Size];
				imagefile.read(image->img.colors, image->dir.s.Size);
				image->img.and = new __int8[image->dir.s.Size / (image->img.header.s.biBitCount)];

				// Build AND mask
				for (int i = 0; i < (image->dir.s.Size / (image->img.header.s.biBitCount) ); i++)
				{
					image->img.and[i]  = 0x00;

					image->img.and[i] |= (image->img.colors[i]       == 0) ? 0x01 : 0;
					image->img.and[i] |= (image->img.colors[i+(1*3)] == 0) ? 0x02 : 0;
					image->img.and[i] |= (image->img.colors[i+(2*3)] == 0) ? 0x04 : 0;
					image->img.and[i] |= (image->img.colors[i+(3*3)] == 0) ? 0x08 : 0;
					image->img.and[i] |= (image->img.colors[i+(4*3)] == 0) ? 0x10 : 0;
					image->img.and[i] |= (image->img.colors[i+(5*3)] == 0) ? 0x20 : 0;
					image->img.and[i] |= (image->img.colors[i+(6*3)] == 0) ? 0x40 : 0;
					image->img.and[i] |= (image->img.colors[i+(7*3)] == 0) ? 0x80 : 0;
				}
			}
			else
			{
				retval = 1;
			}
		}
		else
		{
			retval = 1;		// Input File is not BMP or PNG
		}

		imagefile.close();
	}
	else
	{
		m_sANI_Header.NumFrames = 0;
		retval = -1;
	}

	return retval;
}

int	CIMG2ICO::ReadConfigFile(void)
{
	int		retval = 0;
	string	szConfigFilename = "";
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
		// Read config parameters into m_sANI_Header

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

int		CIMG2ICO::ReadInputFiles(void)
{
	int retval = 0;

	// Find out which directory we are in

	// Read config file if present
	ReadConfigFile();

	// Find out how many images are in the directory (PNG or BMP only)

		// update m_iCount

	// Allocate array for images

	// Load images into sImages

	// Populate all class variables



	
	//// Start test code
	m_sImageArray = new sImage;

	retval = LoadImage("0.bmp", &m_sImageArray[0]);	// temporary code

	//// End test code
	
	return retval;
}

void	CIMG2ICO::SetDirectoryPath(const char* path)
{
	m_szPath.assign(path);
}

void	CIMG2ICO::SetOutputFileType(int type)
{
	m_iType = type;
}

int		CIMG2ICO::WriteOutputFile(void)
{
	int		retval = 0;
	fstream	file;
	string	szOutFilename = "";
		
	if (m_sANI_Header.NumFrames != 0)
	{
		if ((m_szPath.length()) != 0)
		{
			szOutFilename.assign(m_szPath);
			szOutFilename.append(_SZ_PATHSEPARATOR);
		}
		szOutFilename.append(m_szName);

		file.open(szOutFilename.data(), ios::out | ios::binary);

		if (file.is_open())
		{
			// Write file header
			switch(m_iType)
			{
			case T_ANI:
				uBuffer aBuf[32];

				file << "ACON" << ((__int32)(32)) << ((__int32)(m_sANI_Header.NumFrames)) << ((__int32)(m_sANI_Header.NumSteps)) << ((__int32)(m_sANI_Header.Width)) << ((__int32)(m_sANI_Header.Height));
				file << ((__int32)(m_sANI_Header.BitCount)) << ((__int32)(1)) << ((__int32)(m_sANI_Header.DisplayRate)) << ((__int32)(m_sANI_Header.Flags)) << "fram";
				retval = 1;
				file.write(&aBuf[0].byte[0], sizeof(aBuf));
				break;
			default:
			case T_CUR:
			case T_ICO:
				uBuffer buf[2];

				buf[0].word[0] = 0;
				buf[0].word[1] = m_iType;
				buf[1].word[0] = m_iCount;

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
				for (int i = 0; i < m_iCount; i++)
				{
					m_sImageArray[i].dir.s.Offset = (i == 0) ? 22 : (m_sImageArray[i-1].dir.s.Offset + m_sImageArray[i-1].dir.s.Size);
					file << m_sImageArray[i].dir;
				}

				// Write images
				for (int i = 0; i < m_iCount; i++)
				{
					file << m_sImageArray[i].img;
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

int		CIMG2ICO::ConvertFiles(void)
{
	int retval = 0;

	retval = ReadInputFiles();
	retval += WriteOutputFile();

	return retval;
}

std::fstream& operator>>(std::fstream &in, sImage* image)
{
	uBuffer buf[8];

	in.read(&buf[0].byte[2], sizeof(buf)-2);
	
	image->dir.s.Offset			= buf[2].dword;
	image->dir.s.Width			= buf[4].dword;
	image->dir.s.Height			= buf[5].dword;
	image->dir.s.BPP_Vcor		= buf[6].word[1];
	image->dir.s.Size			= (buf[4].dword * buf[5].dword * buf[6].word[1] / 8);

	image->img.header.s.biWidth		= (__int32)(image->dir.s.Width);
	image->img.header.s.biHeight		= (__int32)(image->dir.s.Height);
	image->img.header.s.biBitCount	= (__int32)(image->dir.s.BPP_Vcor);
	image->img.header.s.biSizeImage	= (__int32)(image->dir.s.Size);
	
	return in;
}

std::fstream& operator<<(std::fstream &out, IconDirEntry icon_dir)
{
	out.write(&icon_dir.bytes[0], 16 );

	return out;
}

std::fstream& operator<<(std::fstream &out, IconImage image)
{
	out.write(&image.header.h_bytes[0], 40 );
	out.write(&image.colors[0], image.header.s.biSizeImage );

	if (image.xor != nullptr)
	{
		out.write(&image.xor[0], (image.header.s.biSizeImage / image.header.s.biBitCount) );
	}

	if (image.and != nullptr)
	{
		out.write(&image.and[0], (image.header.s.biSizeImage / image.header.s.biBitCount) );
	}

	return out;
}