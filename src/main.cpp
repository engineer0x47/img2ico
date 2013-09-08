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

#include <vector>
#include <iostream>
#include "img2ico.h"
#include "..\res\resource.h"

using namespace std;

void	PrintHelpInformation(void);
void	PrintVersionInformation(void);
void	ParseCommandLineOptions(CIMG2ICO* img2ico, int argc, char* argv[]);

int main(int argc, char* argv[])
{
	int			retval = 0;
	CIMG2ICO	converter;

	ParseCommandLineOptions(&converter, argc, argv);
	retval = converter.ConvertFiles();
	
	cout << "Press [ENTER] to exit program...";
	cin.get();

	return retval;
}





void	PrintHelpInformation(void)
{
	PrintVersionInformation();

	cout << "\nUsage: img2ico [OPTIONS] -i [PATH] -o [PATH]\n"
		 << "\nOptions:\n"
		 << "\n    -h, -?, --help                           Help"
		 << "\n    -v, --version                            Version information"
		 << "\n    -t, --type TYPE                          File type"
		 << "\n                        TYPE = 'ico'             Icon file"
		 << "\n                        TYPE = 'cur'             Cursor file"
	#ifndef _IMG2ICO_ANI_UNSUPPORTED
		 << "\n                        TYPE = 'ani'             Animated Cursor file"
	#endif
		 << "\n    -n, --name NAME                          Output filename"
	#ifndef _IMG2ICO_ANI_UNSUPPORTED
		 << "\n    -a, --artist A_NAME                      Artists name"
		 << "\n    -f, --framerate RATE                     Default framerate"
	#endif
		 << "\n    -l, --hotspot HORxVERT                   Cursor hotspot"
		 << "\n\n    -i PATH                                  Input path"
		 << "\n    -o PATH                                  Output path\n\n"
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
	int				type = T_ICO;
	int				h = 0, v = 0, f = 30;
	bool			bPrintHelp = false, bPrintVersion = false;
	vector<string>	params;

	for (int i=1; i<argc; i++)
	{
		params.push_back(argv[i]);
	}

	// Find out which directory we are in


	// parse command line arguments

	
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