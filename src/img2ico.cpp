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

CIMG2ICO::CIMG2ICO(const char* path, const char* name, int type)
{
	m_iErrorCode = I2IE_SUCCESS;

	SetDirectoryInputPath(path);
	SetDirectoryOutputPath(path);
	SetOutputFileName(name);
	SetOutputFileType(type);

	m_Params.ImageCount = 0;
	m_Params.ColorTransparent = PackColors(255,255,255,0,32);
}

CIMG2ICO::~CIMG2ICO()
{
}

int		CIMG2ICO::ConvertFiles(void)
{
	ReadInputFiles();
	WriteOutputFile();

	return m_iErrorCode;
}

void	CIMG2ICO::LoadImage(const char* filename)
{
	uBuffer	buffer[14];
	fstream imagefile;

	ZeroBuffer(&buffer[0], 14);
	imagefile.open(filename, ios::in | ios::binary);

	if (imagefile.is_open() == true)
	{
		image img;
		img.pData.clear();

		imagefile.read(&buffer[0].byte[0], 10);
		imagefile.read(&buffer[3].byte[0], 44);

		if (buffer[0].dword == PNG_HEADER_DWORD)
		{
			if (buffer[2].dword == PNG_CHUNK_IHDR)
			{
				// Load image parameters
			}

			if (buffer[2].dword == PNG_CHUNK_PLTE)
			{
				// Load image palette
			}

			// Load entire PNG into m_ImageArray<>->pData

		}
		else if (buffer[0].word[0] == BMP_HEADER_WORD)
		{
			__int32 offset				= buffer[3].dword;
			img.Width					= (buffer[5].dword > IMG2ICO_MAX_DIM) ? IMG2ICO_MAX_DIM : buffer[5].dword;
			img.Height					= (buffer[6].dword > IMG2ICO_MAX_DIM) ? IMG2ICO_MAX_DIM : buffer[6].dword;
			img.NumPlanes				= 1;
			img.BitsPerPixel			= (buffer[7].word[1] > IMG2ICO_MAX_BPP) ? IMG2ICO_MAX_BPP : buffer[7].word[1];
			img.ImgSize					= img.Width * img.Height * (img.BitsPerPixel / 8);
			img.MaskSize				= img.Width * img.Height / 8;
			img.FileSize				= BMP_HEADER_SIZE + img.ImgSize + img.MaskSize;		// header size + image size + andmask size

			if (buffer[8].dword == BMP_BI_RGB)
			{
				imagefile.seekg(offset, imagefile.beg);
				img.pData.reserve(img.ImgSize + img.MaskSize);

				//	Read each byte one at a time
				for (int i = 0; i < img.ImgSize; i++)
				{
					img.pData.push_back(imagefile.get());
				}

				// Build AND mask
				uBuffer pix = {0};
				unsigned __int8	p = 255;
				int j = 0;
					
				for (int i = (img.Width * img.Height); i > 0; i--)
				{
					switch (img.BitsPerPixel)
					{
					case 1:
						p = 0;	// Needs correct implementation
						break;
					case 2:
						p = 0;	// Needs correct implementation
						break;
					case 4:
						p = 0;	// Needs correct implementation
						break;
					case 8:
						p = 0;	// Needs correct implementation
						break;
					case 16:
						p = 0;	// Needs correct implementation
						break;
					case 24:
							pix.byte[1] = img.pData[i];
							pix.byte[2] = img.pData[i+1];
							pix.byte[3] = img.pData[i+2];
							p |= (pix.dword == m_Params.ColorTransparent) ? (1 << j) : 0;
						break;
					case 32:
						// Take data from the Alpha channel, iterate through 8 pixels each pass (monochrome bitmap creation)
							p |= (img.pData[i+3] == 0) ? (1 << j) : 0;
					}

					img.pData.push_back(p);
					p = 0;
					j = ( j >= 8) ? 0 : (j + 1);
				}

				img.pData.shrink_to_fit();
				m_ImageArray.push_back(img);
				m_Params.ImageCount++;
			}
			else
			{
				m_iErrorCode |= I2IE_FILE_COMPRESSION;
			}
		}
		else
		{
			m_iErrorCode |= I2IE_FILE_UNSUPPORTED;		// Input File is not BMP or PNG
		}

		imagefile.close();
	}
	else
	{
		m_iErrorCode |= I2IE_EMPTY_DIRECTORY;
	}
}

void	CIMG2ICO::WriteOutputFile(void)
{
	fstream	file;
	string	szOutFilename = "";
	
	if (m_Params.ImageCount > 0)
	{

		szOutFilename.assign(m_szOutPath);
		szOutFilename.append(SZ_PATHSEPARATOR);
		szOutFilename.append(m_szName);

		switch(m_Params.FileType)
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
			uBuffer buffer[10];

			ZeroBuffer(&buffer[0], 10);

			switch (m_Params.FileType)
			{
			case T_ANI:
				//file << m_sANI_Header;
					// Write frames
				//m_iErrorCode |= m_ANI_File.WriteOutputFileANI(m_szOutPath.data(), m_szName.data());
				//if (m_bSequenceData == true)
				//{
					// write sequence data
				//}
				m_iErrorCode += I2IE_FILE_UNSUPPORTED;
			break;
			default:
			case T_CUR:
			case T_ICO:
				buffer[0].word[1] = m_Params.FileType;
				buffer[1].word[0] = m_Params.ImageCount;
				file.write(&buffer[0].byte[0], 6);

				__int32 offset = 6 + (16 * m_Params.ImageCount);

				// Some documentation (daubnet.com) specifies CUR files with a different dirEntry size than ICO

				// Write Directory Entries
				for (int i = 0; i < m_Params.ImageCount; i++)
				{
					buffer[0].byte[0]	= m_ImageArray[i].Width;
					buffer[0].byte[1]	= m_ImageArray[i].Height;
					buffer[0].byte[2]	= ( (m_ImageArray[i].BitsPerPixel == 1) || (m_ImageArray[i].BitsPerPixel == 4) ) ? (1 << m_ImageArray[i].BitsPerPixel) : 0;
					buffer[0].byte[3]	= 0;
					buffer[1].word[0]	= 0;	// For some reason this instance of "ColorPlanes" is zero in MS-generated files?!?
					buffer[1].word[1]	= m_ImageArray[i].BitsPerPixel;
					buffer[2].dword		= m_ImageArray[i].FileSize;
					buffer[3].dword		= offset;

					file.write(&buffer[0].byte[0], 16 );

					if (i > 0)
					{
						offset += m_ImageArray[i-1].FileSize;
					}
				}

				// Write images
				for (int i = 0; i < m_Params.ImageCount; i++)
				{
					buffer[0].dword		= BMP_HEADER_SIZE;
					buffer[1].dword		= m_ImageArray[i].Width;
					buffer[2].dword		= m_ImageArray[i].Height * 2;
					buffer[3].word[0]	= m_ImageArray[i].NumPlanes;
					buffer[3].word[1]	= m_ImageArray[i].BitsPerPixel;
					buffer[4].dword		= BMP_BI_RGB;
					buffer[5].dword		= m_ImageArray[i].ImgSize;
					buffer[6].dword		= 0;
					buffer[7].dword		= 0;
					buffer[8].dword		= 0;
					buffer[9].dword		= 0;

					file.write(&buffer[0].byte[0], 40);									// Write Header
					file.write(&m_ImageArray[i].pData[0], m_ImageArray[i].ImgSize);		// Write Image

					// Write Mask with 4-byte padding (need to implement the padding part)
					
					file.write(&m_ImageArray[i].pData[m_ImageArray[i].ImgSize], m_ImageArray[i].MaskSize);

				/*	for (int j = 0; j < m_ImageArray[i].MaskSize; j++)
					{
						file.write(&m_ImageArray[i].pData[m_ImageArray[i].ImgSize + j], 1);
					}	*/
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
}

void	CIMG2ICO::ReadConfigFile(void)
{
	string		szConfigFilename = "";
	fstream		c_file;

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
				szToLcase(&cParam[0], IMG2ICO_SZ_MAXLEN);
				szParams[i].assign(&cParam[i]);
				szValues[i].assign(&cValue[i]);
				bFoundStart = false;
			}
			else
			{
				c_file.getline(&cParam[i], IMG2ICO_SZ_MAXLEN, ' ');
				c_file.getline(&cValue[i], IMG2ICO_SZ_MAXLEN, '"');
				c_file.getline(&cValue[i], IMG2ICO_SZ_MAXLEN, ',');
				szToLcase(&cParam[0], IMG2ICO_SZ_MAXLEN);
				szParams[i].assign(&cParam[i]);
				szParams[i].append("_h");
				szValues[i].assign(&cValue[i]);
				c_file.get(&cWaste, 1);
				if (cWaste != ',')
				{
					m_iErrorCode |= I2IE_UNKNOWN;
				}
				c_file.getline(&cValue[i+1], IMG2ICO_SZ_MAXLEN, '"');
				szToLcase(&cParam[0], IMG2ICO_SZ_MAXLEN);
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
				szValues[i].shrink_to_fit();

				if (szValues[i].length() > IMG2ICO_SZ_MAXLEN)
				{
					szValues[i].resize(IMG2ICO_SZ_MAXLEN);
				}

				m_szName.assign(szValues[i].data());
			}

			// Artist, update Chunk
			if ( szCommands[1].compare( szParams[i].data() ) )
			{
				SetArtistNameANI(szValues[i].data());
			}

			// Default Framerate
			if ( szCommands[2].compare( szParams[i].data() ) )
			{
				SetDefaultFrameRateANI( (__int32)(_atoi64(szValues[i].data())) );
			}

			// SequenceInformation, update Chunk
			if ( szCommands[3].compare( szParams[i].data() ) )
			{
			}

			// CursorHotspot
			if ( szCommands[4].compare( szParams[i].data() ) )
			{
				SetCursorHotspot( (__int32)(_atoi64(szValues[i].data())), m_Params.Vcoord);
			}
			
			// CursorHotspot
			if ( szCommands[5].compare( szParams[i].data() ) )
			{
				SetCursorHotspot( m_Params.Hcoord, (__int32)(_atoi64(szValues[i].data())) );
			}
		}

		c_file.close();
	}
	else
	{
		m_iErrorCode |= I2IE_NO_CONFIGFILE;
	}
}

void	CIMG2ICO::ReadInputFiles(void)
{
	vector<string>	filelist;

	// Read config file if present and ANI file type
	if (m_Params.FileType == T_ANI)
	{
		ReadConfigFile();
	}
	
	// Find out how many images are in the directory (PNG or BMP only) and add them to the list



	LoadImage("0.bmp");

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
		switch (m_Params.FileType)
		{
		case T_ANI:
			m_szName.assign("ani_cursor");
			break;
		case T_CUR:
			m_szName.assign("cursor");
			break;
		case T_ICO:
			default:
			m_szName.assign("icon");
		}
	}
	else
	{
		m_szName.assign(filename);
	}	
}

void	CIMG2ICO::SetTransparentColor(const __int8 r, const __int8 g, const __int8 b)
{
	m_Params.ColorTransparent = PackColors8(r, g, b, 0, m_Params.BitsPerPixel);
}

void	CIMG2ICO::SetOutputFileType(const int type)
{
	m_Params.FileType = ( (type > 0) && (type <= 3) ) ? type : T_ICO;
}

void	CIMG2ICO::SetArtistNameANI(const char* artist)
{
}

void	CIMG2ICO::SetDefaultFrameRateANI(const int rate)	
{
	m_Params.DisplayRate = (rate < 120) ? rate : 120;
}

void	CIMG2ICO::SetCursorHotspot(const int h, const int v)	
{
	m_Params.Hcoord = ( (h <= (IMG2ICO_MAX_DIM - 1)) && (h <= (m_Params.Height - 1)) ) ? h : 0;
	m_Params.Vcoord = ( (v <= (IMG2ICO_MAX_DIM - 1)) && (h <= (m_Params.Width - 1)) )? h : 0;
}