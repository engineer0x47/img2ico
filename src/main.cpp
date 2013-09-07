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

#ifdef WIN32
#include <Windows.h>
#define WIN32_LEAN_AND_MEAN
#endif

using namespace std;

int main(int argc, char* argv[])
{
	int			retval = 0;
	string		path;
	string		out_file;
	int			type = T_ICO;
	CIMG2ICO	converter;















	// Find out which directory we are in

	// Read input parameters
	path.assign("");
	out_file.assign("Icon7.ico");

	converter.SetDirectoryPath(path.data());
	converter.SetOutputFileName(out_file.data());
	converter.SetOutputFileType(type);


	// Convert data
	retval = converter.ConvertFiles();

	return retval;
}