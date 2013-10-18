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



#include <stdlib.h>
#include <algorithm>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

using std::map;
using std::multimap;
using std::string;
using std::vector;

#ifndef __ARG_PARSER_H
#define __ARG_PARSER_H

#pragma once

namespace argp
{

typedef multimap<int, string, std::greater<int>> szPolyMap;
typedef map<int, string, std::greater<int>> szSingleMap;
typedef map<int, int, std::greater<int>> intSingleMap;

enum eCARGP_FLAG
{
	NO_PARAMS	= 0,
	PARAM_OPT	= 1,
	PARAM_REQ	= 2,
	CMD_ALIAS	= 4
};

enum eCARGP_STATUS
{
	ARG_FOUND	= 0,
	ARG_MISSING	= 1,
	ARG_INVALID	= 2,
	ERROR		= 4
};

class CArgParser
{
private:
	int						m_iErrorCode;
	szPolyMap				m_CommandMap;
	intSingleMap			m_CommandParamCount;
	vector<string>			m_ArgvInput;
	szSingleMap				m_InputCommands;			// Stores the command and value, i.e. the parser's output
	szSingleMap::iterator	m_Pos;
	string					m_szInvalidArgs;

public:
	CArgParser();
	~CArgParser();

	// General functions
	int		GetErrorCode();
	void	ResetErrorCode();

	void	EmptyParser(void);
	int		SetOption(int command, string name, int param_flag);	// SetOption(enum for later processing of commands, "name" typed by user, int flag)
	void	AddDefaultOptions(void);								// Adds "help" (command 0) and "version" (command 1) options
	int		ParseArgs(int argc, char* argv[]);
	
	// For non-looping options checks
	int		CheckForArgument(int option, string &szParam);
	
	//	For looping options checks
	int		GetNextArgument(string &szParam);
	string*	GetInvalidArguments(void);
	int		GetArgumentCount(void);
	void	ResetArglistPosition(void);
};


}	// End namespace

#endif