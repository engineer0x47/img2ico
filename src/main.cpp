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

#ifdef WIN32
	#include <direct.h>
	#define GetWorkingDir _getcwd
#else
	#include <unistd.h>
	#define GetWorkingDir getcwd
#endif

#include "img2ico.h"
#include "..\res\resource.h"

using std::cin;
using std::cout;
using std::string;
using std::vector;

void	PrintHelpInformation(void);
void	PrintVersionInformation(void);
void	ParseCommandLineOptions(CIMG2ICO* img2ico, int argc, char* argv[]);

int		main(int argc, char* argv[])
{
	bool		bPipe = true;		// Set false if the user intends to enable "wait for key" message
	CIMG2ICO	converter;

	ParseCommandLineOptions(&converter, argc, argv);
	converter.ConvertFiles();
	
	if (!bPipe)
	{
		cout << "Press [ENTER] to exit program...";
		cin.get();
	}

	return converter.GetErrorCodes();
}





void	PrintHelpInformation(void)
{
	PrintVersionInformation();

	cout << "\nUsage: img2ico [OPTIONS]\n"
		 << "\nOptions:\n"
		 << "\n    -h, -?, --help                       Help"
		 << "\n    -v, --version                        Version information"
		 << "\n    -t, --type TYPE                      File type"
		 << "\n                        TYPE = 'ico'         Icon file"
		 << "\n                        TYPE = 'cur'         Cursor file"
	#ifndef _IMG2ICO_ANI_UNSUPPORTED
		 << "\n                        TYPE = 'ani'         Animated Cursor file"
	#endif
		 << "\n    -m, --mipmap                         Generate full mipmaps of icon file"
		 << "\n    -n, --name NAME                      Output filename"
		 << "\n    -p, --pipe                           Disable wait on key at program end"
	#ifndef _IMG2ICO_ANI_UNSUPPORTED
		 << "\n    -a, --artist A_NAME                  Artists name"
		 << "\n    -f, --framerate RATE                 Default framerate"
	#endif
		 << "\n    -l, --hotspot HORxVERT               Cursor hotspot"
		 << "\n\n    -i PATH                              Input path"
		 << "\n    -o PATH                              Output path\n\n"
		 << "By default this program will put all image files in the working"
		 << "\ndirectory in ICO format in 'Icon.ico', and both the input and"
		 << "\noutput paths are the same.  The file extension will be"
		 << "\nautomatically added to the output filename.  Maximum path"
		 << "\nlength is 192 chars, and maximum name length is 64 chars."
		 << "\nImage dimensions cannot exceed 256x256, maximum bits per pixel"
		 << "\nis 32 (4 bytes).\n";
	#ifndef _IMG2ICO_ANI_UNSUPPORTED
	cout << "\nFor Animated cursor files an optional file named 'config' may"
		 << "\nbe placed in the input directory and formatted as below."
		 << "\nThe default sequence will be used if a config file is not found."
		 << "\nThe maximum string length is 64 characters."
		 << "\n\nimg2ico Animated Icon Config"
		 << "\nName \"cursor name\""
		 << "\nArtist \"artist name\""
		 << "\nDefaultFrameRate \"30\""
		 << "\nSequenceInformation \"1,2,3,2,1\""
		 << "\nCursorHotspot \"0,0\"\n";
	#endif
	cout << "\n";
}

void	PrintVersionInformation(void)
{
	cout << "\nIMG2ICO Verison " << IMG2ICO_VERSION << endl;
}

void	ParseCommandLineOptions(CIMG2ICO* img2ico, int argc, char* argv[])
{
	char			szWorkingDir[FILENAME_MAX];
	bool			bPrintHelp = false;
	bool			bPrintVersion = false;
	vector<string>	params;

	for (int i=1; i<argc; i++)
	{
		params.push_back(argv[i]);
	}

	// Find out which directory we are in
	if (!GetWorkingDir(&szWorkingDir[0], FILENAME_MAX))
	{
		cout << "\n IMG2ICO Failed to get current working directory, using '.\\' !\n";
	}

	// parse command line arguments









	/*
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
				SZ_to_LCASE(&cParam[0], IMG2ICO_SZ_MAXLEN);
				szParams[i].assign(&cParam[i]);
				szValues[i].assign(&cValue[i]);
				bFoundStart = false;
			}
			else
			{
				c_file.getline(&cParam[i], IMG2ICO_SZ_MAXLEN, ' ');
				c_file.getline(&cValue[i], IMG2ICO_SZ_MAXLEN, '"');
				c_file.getline(&cValue[i], IMG2ICO_SZ_MAXLEN, ',');
				SZ_to_LCASE(&cParam[0], IMG2ICO_SZ_MAXLEN);
				szParams[i].assign(&cParam[i]);
				szParams[i].append("_h");
				szValues[i].assign(&cValue[i]);
				c_file.get(&cWaste, 1);
				if (cWaste != ',')
				{
					printf("FUCKUP WITH FILE PARSING!");
				}
				c_file.getline(&cValue[i+1], IMG2ICO_SZ_MAXLEN, '"');
				SZ_to_LCASE(&cParam[0], IMG2ICO_SZ_MAXLEN);
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






	*/

	
/*	for (int i=1; i<argc; i++)
	{
		if ((argv[i] == "-h") || (argv[i] == "--help") )
		{
			bPrintHelp = true;
		}

		if ((argv[i] == "-v") || (argv[i] == "--version") )
		{
			bPrintVersion = true;
		}
	}

	if (bPrintHelp == false)
	{
		if (bPrintVersion == false)
		{
			for (int i=1; i<argc; i++)
			{
				if (argv[i+1] != NULL)
				{
					if ( (argv[i] == "-t") || (argv[i] == "--type") || (argv[i] == "-?") )
					{
						if (argv[i+1] == "ico")
						{
							type = T_ICO;
							i++;
						}

						if (argv[i+1] == "cur")
						{
							type = T_CUR;
							i++;
						}

						if (argv[i+1] == "ani")
						{
							type = T_ANI;
							i++;
						}

						img2ico->SetOutputFileType(type);
					}

					if ( (argv[i] == "-n") || (argv[i] == "--name") )
					{
						img2ico->SetOutputFileName(argv[i+1]);
						i++;
					}

					if (argv[i] == "-i")
					{
						img2ico->SetDirectoryInputPath(argv[i+1]);
						i++;
					}

					if (argv[i] == "-o")
					{
						img2ico->SetDirectoryOutputPath(argv[i+1]);
						i++;
					}

					if ( ( (argv[i] == "-l") || (argv[i] == "--hotspot") ) && (type != T_ICO) )
					{
						// Turn text into number, figure out how to turn ###x### into h and v coords
						img2ico->SetCursorHotspot(h, v);
					}

					if ( ( (argv[i] == "-f") || (argv[i] == "--framerate") ) && (type == T_ANI) )
					{
						// Turn text into number
						img2ico->SetDefaultFrameRateANI(f);
						i++;
					}

					if ( ( (argv[i] == "-a") || (argv[i] == "--artist") ) && (type == T_ANI) )
					{
						img2ico->SetArtistNameANI(argv[i+1]);
						i++;
					}
				}
			}
		}
		else
		{
			PrintVersionInformation();
		}
	}
	else
	{
		PrintHelpInformation();
	} */
}