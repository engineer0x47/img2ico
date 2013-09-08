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

std::fstream& operator<<(std::fstream &out, const IconDirEntry icon_dir);
std::fstream& operator<<(std::fstream &out, const IconImage &image);
std::fstream& operator<<(std::fstream &out, const sICO_Header ico_hdr);
std::fstream& operator<<(std::fstream &out, const sANI_Header ani_hdr);
std::fstream& operator<<(std::fstream &out, const sANI_Chunk &ani_chunk);

void	SZtoLCASE(char* sz, const int size);

sANI_Header::sANI_Header()
{
	s.HeaderID = 'ACON';
	s.HeaderSize = 36;
	s.NumFrames = 1;
	s.NumSteps = 1;
	s.Width = 1;
	s.Height = 1;
	s.BitsPerPixel = 8;
	s.NumPlanes = 1;
	s.DisplayRate = 30;
	s.Flags = 0 | I_ICON;
}

sANI_Chunk::sANI_Chunk()
{
	for (int i = 0; i < 8; i++)
	{
		bytes[i] = 0;
	}

	data = nullptr;	
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

	img.header.s.HeaderSize = 40;
	img.header.s.NumPlanes = 1;
}

CIMG2ICO::CIMG2ICO(const char* path, const char* name, int type)
{
	m_iErrorCode = I2IE_SUCCESS;
	m_iTransparentColor = 0x00FE00FE;	// violet color
	m_bSequenceData = false;
	m_bUseRawData = false;
	m_sImageArray = nullptr;
	m_sICO_Header.s.Reserved = 0;
	m_sICO_Header.s.Count = 1;

	SetDirectoryInputPath(path);
	SetDirectoryOutputPath(path);
	SetOutputFileName(name);
	SetOutputFileType(type);
}

CIMG2ICO::~CIMG2ICO()
{
	// Free any allocated memory
	if (m_sImageArray != nullptr)
	{
		for (int i = 0; i < m_sICO_Header.s.Count; i++)
		{
			if (m_sImageArray[i].img.and != nullptr)
			{
				delete m_sImageArray[i].img.and;
			}

			if (m_sImageArray[i].img.xor != nullptr)
			{
				delete m_sImageArray[i].img.xor;
			}

			if (m_sImageArray[i].img.colors != nullptr)
			{
				delete m_sImageArray[i].img.colors;
			}
		}

		delete m_sImageArray;
	}
}

int		CIMG2ICO::LoadImage(const char* filename, struct sImage* image)
{
	uBuffer	buffer;
	fstream imagefile;

	buffer.dword = 0;
	imagefile.open(filename, ios::in | ios::binary);

	if (imagefile.is_open() == true)
	{
		imagefile.read(&buffer.byte[0], 4);

		if (buffer.dword == PNG_HEADER_DWORD)
		{
			imagefile.read(&buffer.byte[0], 4);
			imagefile.read(&buffer.byte[0], 4);
			
			if (buffer.dword == PNG_CHUNK_IHDR)
			{
				// Load image parameters
			}

			if (buffer.dword == PNG_CHUNK_PLTE)
			{
				// Load image palette
			}

			// Load entire PNG into sImage.imagedata

		}
		else if (buffer.word[0] == BMP_HEADER_WORD)
		{
			imagefile >> image;

			if (image->img.header.s.CompressionType == BMP_BI_RGB)
			{
				imagefile.seekg(image->dir.s.Offset, imagefile.beg);
				image->img.xor = new __int8[image->img.XorSize];
				imagefile.read(image->img.xor, image->img.XorSize);
				image->img.and = new __int8[image->img.AndmaskSize];

				for (int i = 0; i < image->img.AndmaskSize; i++)
				{
					image->img.and[i] = 0xFF;
				}

				// Build AND mask
				if ( (image->img.xor != nullptr) && (image->img.and != nullptr) )
				{
					uBuffer buf;
					__int8	p = 0;
					buf.dword = 0;
					
					for (int i = 0; i < (image->dir.s.Height * image->dir.s.Width / 8); i++)
					{
						switch (image->dir.s.BPP_Vcor)
						{
						case 1:
						case 2:
							buf.byte[3] = 1;
							break;
						case 4:
							if ( ((image->dir.s.Height * image->dir.s.Width) & 0x01) == 1)
							{
								buf.byte[0] = image->img.xor[i/2+1];
							}
							else
							{
								buf.byte[0] = image->img.xor[i/2];
							}							
							buf.byte[1] = buf.byte[2] = buf.byte[3]= 0;
							buf.byte[3] = (buf.dword == m_iTransparentColor) ? 1 : 0;
							break;
						case 8:
							buf.byte[0] = image->img.xor[i];
							buf.byte[3] = 0;
							buf.byte[3] = (buf.dword == m_iTransparentColor) ? 1 : 0;
							break;
						case 16:
							buf.byte[0] = image->img.xor[i*2];
							buf.byte[1] = image->img.xor[(i*2)+1];
							buf.byte[3] = 0;
							buf.byte[3] = (buf.dword == m_iTransparentColor) ? 1 : 0;
							break;
						case 24:
							buf.byte[0] = image->img.xor[i*4];
							buf.byte[1] = image->img.xor[(i*4)+1];
							buf.byte[2] = image->img.xor[(i*4)+2];
							buf.byte[3] = 0;
							buf.byte[3] = (buf.dword == m_iTransparentColor) ? 1 : 0;
							break;
						case 32:
							buf.byte[3] = image->img.xor[(i*4)+3];
						}
						
						image->img.and[i] &= (buf.byte[3] == 0) ? (0x00 ^ (1 << p)) : 0xFF;

						p = (p < 7) ? p++ : 0;
					}
				}
			}
			else
			{
				m_iErrorCode += I2IE_FILE_COMPRESSION;
			}
		}
		else
		{
			m_iErrorCode += I2IE_FILE_UNSUPPORTED;		// Input File is not BMP or PNG
		}

		imagefile.close();
	}
	else
	{
		m_sANI_Header.s.NumFrames = 0;
		m_iErrorCode += I2IE_EMPTY_DIRECTORY;
	}

	return m_iErrorCode;
}

int		CIMG2ICO::WriteOutputFile(void)
{
	fstream	file;
	string	szOutFilename = "";
	
	if ( (m_sICO_Header.s.Count != 0) && (m_sImageArray != nullptr) )
	{

		szOutFilename.assign(m_szOutPath);
		szOutFilename.append(SZ_PATHSEPARATOR);
		szOutFilename.append(m_szName);

		switch(m_sICO_Header.s.Type)
		{	
		case T_ANI:
			szOutFilename.append(".ani");
			break;
		case T_CUR:
			szOutFilename.append(".cur");
			break;
		case T_ICO:
		default:
			szOutFilename.append(".ico");
		}

		file.open(szOutFilename.data(), ios::out | ios::binary);

		if (file.is_open())
		{
			switch(m_sICO_Header.s.Type)
			{
			case T_ANI:
				file << m_sANI_Header;

				// Write frames
			
				if (m_bSequenceData == true)
				{
					// write sequence data
				}
				m_iErrorCode += I2IE_FILE_UNSUPPORTED;
			break;
			default:
			case T_CUR:
			case T_ICO:
				file << m_sICO_Header;

				// Build image directory
				for (int i = 0; i < m_sICO_Header.s.Count; i++)
				{
					m_sImageArray[i].dir.s.Offset = (i == 0) ? (6 + (16 * m_sICO_Header.s.Count) ) : (m_sImageArray[i-1].dir.s.Offset + m_sImageArray[i-1].dir.s.Size);
					file << m_sImageArray[i].dir;
				}

				// Write images
				for (int i = 0; i < m_sICO_Header.s.Count; i++)
				{
					file << m_sImageArray[i].img;
				}
			}

			file.close();
		}
		else
		{
			m_iErrorCode += I2IE_FILE_FAILED;
		}
	}
	else
	{
		m_iErrorCode += I2IE_EMPTY_OUTPUT;
	}

	return m_iErrorCode;
}


int		CIMG2ICO::ReadConfigFile(void)
{
	string	szConfigFilename = "";
	fstream	c_file;

	szConfigFilename.assign(m_szInPath);
	szConfigFilename.append(SZ_PATHSEPARATOR);
	szConfigFilename.append("config");
	c_file.open(szConfigFilename.data(), ios::in);

	if (c_file.is_open())
	{
		char	cWaste;
		char	cParam[IMG2ICO_SZ_MAXLEN];
		char	cValue[IMG2ICO_SZ_MAXLEN];
		bool	bFoundStart = false;
		string	szCommands[6];
		string	szParams[6];
		string	szValues[6];

		szCommands[0].assign("name");
		szCommands[1].assign("artist");
		szCommands[2].assign("defaultframerate");
		szCommands[3].assign("sequenceinformation");
		szCommands[4].assign("cursorhotspot_h");
		szCommands[5].assign("cursorhotspot_v");

		for (int i = 0; i < IMG2ICO_SZ_MAXLEN; i++)
		{
			cParam[i] = 0;
			cValue[i] = 0;
		}

		// Read config parameters into m_sANI_Header
		while (bFoundStart == false)
		{
			c_file.get(&cWaste, 1);
			bFoundStart = (cWaste == '\n') ? true : false;
		}

		for (int i = 0; i < 5; i++)
		{				
			while (bFoundStart == false)
			{
				c_file.get(&cWaste, 1);
				bFoundStart = (cWaste == '\n') ? true : false;
			} 

			if (i <= 4)
			{
				c_file.getline(&cParam[i], IMG2ICO_SZ_MAXLEN, ' ');
				c_file.getline(&cValue[i], IMG2ICO_SZ_MAXLEN, '"');
				c_file.getline(&cValue[i], IMG2ICO_SZ_MAXLEN, '"');
				SZtoLCASE(&cParam[0], IMG2ICO_SZ_MAXLEN);
				szParams[i].assign(&cParam[i]);
				szValues[i].assign(&cValue[i]);
				bFoundStart = false;
			}
			else
			{
				c_file.getline(&cParam[i], IMG2ICO_SZ_MAXLEN, ' ');
				c_file.getline(&cValue[i], IMG2ICO_SZ_MAXLEN, '"');
				c_file.getline(&cValue[i], IMG2ICO_SZ_MAXLEN, ',');
				SZtoLCASE(&cParam[0], IMG2ICO_SZ_MAXLEN);
				szParams[i].assign(&cParam[i]);
				szParams[i].append("_h");
				szValues[i].assign(&cValue[i]);
				c_file.get(&cWaste, 1);
				if (cWaste != ',')
				{
					m_iErrorCode += I2IE_UNKNOWN;
				}
				c_file.getline(&cValue[i+1], IMG2ICO_SZ_MAXLEN, '"');
				SZtoLCASE(&cParam[0], IMG2ICO_SZ_MAXLEN);
				szParams[i+1].assign(&cParam[i+1]);
				szParams[i].append("_v");
				szValues[i+1].assign(&cValue[i+1]);
			}
		}

		// Parse Config file
		for (int i = 0; i < 6; i++)
		{
			// Name
			if ( szCommands[0].compare( szParams[i].data() ) )
			{
			}

			// Artist
			if ( szCommands[1].compare( szParams[i].data() ) )
			{
			}

			// Default Framerate
			if ( szCommands[2].compare( szParams[i].data() ) )
			{
				m_sANI_Header.s.DisplayRate = (__int32)(_atoi64(szValues[i].data()));
			}

			// SequenceInformation
			if ( szCommands[3].compare( szParams[i].data() ) )
			{
			}

			// CursorHotspot
			if ( szCommands[4].compare( szParams[i].data() ) )
			{
				m_sANI_Info.s.NumPlanes_Hcor = (__int32)(_atoi64(szValues[i].data()));
			}
			
			// CursorHotspot
			if ( szCommands[5].compare( szParams[i].data() ) )
			{
				m_sANI_Info.s.BPP_Vcor = (__int32)(_atoi64(szValues[i].data()));
			}
		}

		c_file.close();
	}
	else
	{
		m_iErrorCode += I2IE_NO_CONFIGFILE;
	}

	return m_iErrorCode;
}

int		CIMG2ICO::ReadInputFiles(void)
{
	vector<string>	filelist;

	// Read config file if present and ANI file type
	if (m_sICO_Header.s.Type == T_ANI)
	{
		ReadConfigFile();
	}
	
	// Find out how many images are in the directory (PNG or BMP only) and add them to the list












	// update image counter
	m_sANI_Header.s.NumFrames = m_sICO_Header.s.Count = 1;

	if (m_sICO_Header.s.Count != 0)
	{
		// Allocate array for images
		m_sImageArray = new sImage[m_sICO_Header.s.Count];

		// Load images into sImages
		for (int i = 0; i < m_sICO_Header.s.Count; i++)
		{
			LoadImage("0.bmp", &m_sImageArray[i]);
		}
	}
	else
	{
		m_iErrorCode += I2IE_EMPTY_DIRECTORY;
	}

	return m_iErrorCode;
}

int		CIMG2ICO::GetErrorCodes(void)
{
	return m_iErrorCode;
}

void	CIMG2ICO::ResetErrorCodes(void)
{
	m_iErrorCode = I2IE_SUCCESS;
}

void	CIMG2ICO::SetDirectoryInputPath(const char* in_path)
{
	if (m_szInPath.length() == 0)
	{
		m_szInPath.assign(".");
	}
	else
	{
		m_szInPath.assign(in_path);
	}
}

void	CIMG2ICO::SetDirectoryOutputPath(const char* out_path)
{
	if (m_szOutPath.length() == 0)
	{
		m_szOutPath.assign(".");
	}
	else
	{
		m_szOutPath.assign(out_path);
	}
}

void	CIMG2ICO::SetOutputFileName(const char* filename)
{
	if ((m_szName.length()) == 0)
	{
		m_szName.assign("icon");
	}
	else
	{
		m_szName.assign(filename);
	}	
}

void	CIMG2ICO::SetTransparentColor(const int r, const int g, const int b)
{
	int rn = (r > 255) ? 255 : r;
	int gn = (r > 255) ? 255 : r;
	int bn = (r > 255) ? 255 : r;

	if (m_sImageArray != nullptr)
	{
		switch (m_sImageArray[0].dir.s.BPP_Vcor)
		{
		case 8:
			rn = (rn*15/255) & 0x07;
			gn = (gn*15/255) & 0x07;
			bn = (bn*3/255) & 0x03;
			m_iTransparentColor = (rn << 5) | (gn << 2) | bn;
			break;
		case 16:
			rn = (rn*31/255) & 0x07;
			gn = (gn*63/255) & 0x07;
			bn = (bn*31/255) & 0x03;
			m_iTransparentColor = ((rn % 31) << 11) | ((gn % 63) << 5) | (bn % 31);
			break;
		case 24:
			m_iTransparentColor = (rn << 16) | (gn << 8) | bn;
		}
	}	
}

void	CIMG2ICO::SetOutputFileType(const int type)
{
	m_sICO_Header.s.Type = ( (type > 0) && (type <= 3) ) ? type : T_ICO;
}

void	CIMG2ICO::SetArtistNameANI(const char* artist)
{
	if (m_sICO_Header.s.Type == T_ANI)
	{
	}
}

void	CIMG2ICO::SetDefaultFrameRateANI(const int rate)	
{
	m_sANI_Header.s.DisplayRate = rate;
}

void	CIMG2ICO::SetCursorHotspot(const int h, const int v)	
{
	if ( (m_sICO_Header.s.Type == T_ANI) || (m_sICO_Header.s.Type == T_CUR) )
	{
		m_sImageArray->dir.s.NumPlanes_Hcor = (h <= IMG2ICO_MAX_DIM) ? h : 0;
		m_sImageArray->dir.s.BPP_Vcor		= (v <= IMG2ICO_MAX_DIM) ? v : 0;
	}
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
	
	image->dir.s.Offset					= buf[2].dword;
	image->img.header.s.Width			= buf[4].dword;
	image->img.header.s.Height			= buf[5].dword;
	image->img.header.s.BitsPerPixel	= buf[6].word[1];
	image->img.header.s.ImageSize		= (buf[4].dword * buf[5].dword * buf[6].word[1] / 8);

	image->img.header.s.Height			= (image->img.header.s.Height <= IMG2ICO_MAX_DIM) ? image->img.header.s.Height : IMG2ICO_MAX_DIM;
	image->img.header.s.Width			= (image->img.header.s.Width <= IMG2ICO_MAX_DIM) ? image->img.header.s.Width : IMG2ICO_MAX_DIM;
	image->img.header.s.BitsPerPixel	= (image->img.header.s.BitsPerPixel <= IMG2ICO_MAX_BPP) ? image->img.header.s.BitsPerPixel : IMG2ICO_MAX_BPP;
	
	image->dir.s.Width					= image->img.header.s.Width;
	image->dir.s.Height					= image->img.header.s.Height;
	image->dir.s.BPP_Vcor				= image->img.header.s.BitsPerPixel;

	image->img.XorSize					= image->dir.s.Width * image->dir.s.Height * image->img.header.s.BitsPerPixel / 8;
	image->img.AndmaskSize				= image->img.XorSize / image->img.header.s.BitsPerPixel;
	image->img.header.s.Height		   *= 2;

										// header size + image size + andmask size
	image->dir.s.Size					= image->img.header.s.HeaderSize + image->img.header.s.ImageSize + image->img.AndmaskSize;
	
	return in;
}

std::fstream& operator<<(std::fstream &out, const IconDirEntry icon_dir)
{
	out.write(&icon_dir.bytes[0], 16 );

	return out;
}

std::fstream& operator<<(std::fstream &out, const IconImage &image)
{
	out.write(&image.header.h_bytes[0], 40 );

	if (image.colors != nullptr)
	{
		out.write(&image.colors[0], image.header.s.ImageSize );
	}

	if (image.xor != nullptr)
	{
		out.write(&image.xor[0], (image.XorSize) );
	}

	if (image.and != nullptr)
	{
		out.write(&image.and[0], (image.AndmaskSize) );
	}

	return out;
}

std::fstream& operator<<(std::fstream &out, const sICO_Header ico_hdr)
{
	out.write(&ico_hdr.bytes[0], 6 );

	return out;
}

std::fstream& operator<<(std::fstream &out, const sANI_Header ani_hdr)
{
	out.write(&ani_hdr.bytes[0], 40);

	return out;
}

std::fstream& operator<<(std::fstream &out, const sANI_Chunk &ani_chunk)
{
	// Ignore chunk if it has no data
	if (ani_chunk.data != nullptr)
	{
		out.write(&ani_chunk.bytes[0], 8);
		out.write(&ani_chunk.data[0], ani_chunk.s.size);
	}

	return out;
}

void	SZtoLCASE(char* sz, const int size)
{
	for (int j = 0; j < size; j++)
	{
		sz[j] = tolower(sz[j]);
	}
}