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


#ifdef WIN32
#include <Windows.h>
#define WIN32_LEAN_AND_MEAN
#endif

using namespace std;

void	PrintHelpInformation(void);
void	PrintVersionInformation(void);


int main(int argc, char* argv[])
{
	int			retval = 0;
	string		path;
	string		out_file;
	int			type = T_ICO;
	CIMG2ICO	converter;

	// Parse commands
	vector<string> parameters;

	for (int i=1; i<argc; i++)
	{
        parameters.push_back(argv[i]);
    }

	cout << endl;

	PrintHelpInformation();

	cout << endl;
	cout << endl;
	cout << endl;

	// Find out which directory we are in

	// Read input parameters
	path.assign("");
	out_file.assign("Icon7");

	converter.SetDirectoryPath(path.data());
	converter.SetOutputFileName(out_file.data());
	converter.SetOutputFileType(type);


	// Convert data
	retval = converter.ConvertFiles();


	#ifdef WIN32
	system("PAUSE");
	#endif

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
		 << "\n    -n, --name NAME                          Output filename"
		 << "\n    -a, --artist A_NAME                      Artists name"
		 << "\n    -f, --framerate RATE                     Default framerate"
	#endif
		 << "\n    -s, --hotspot HORxVERT                   Cursor hotspot"
		 << "\n\n    -i PATH                                  Input path"
		 << "\n    -o PATH                                  Output path\n\n"
		 << "By default this program will put all image files in the working"
		 << "\ndirectory in ICO format in 'Icon.ico'.  The file extension"
		 << "\nwill be automatically added to the output filename.";

	#ifndef _IMG2ICO_ANI_UNSUPPORTED
	cout << "\nFor Animated cursor"
		 << "\nfiles an optional file named 'config' may be placed in the"
		 << "\ninput directory and formatted as below.  The default sequence"
		 << "\nwill be used if a config file is not found.  The file"
		 << "\n\nimg2ico Animated Icon Config"
		 << "\nName \"icon name\""
		 << "\nArtist \"artist name\""
		 << "\nDefaultFrameRate \"30\""
		 << "\nSequenceInformation \"1,2,3,2,1\""
		 << "\nCursorHotspot \"0,0\"\n\n";
	#endif
}

void	PrintVersionInformation(void)
{
	cout << "\nIMG2ICO Verison " << IMG2ICO_VERSION << endl;
}