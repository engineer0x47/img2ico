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



#include "arg_parser.h"

using namespace argp;

CArgParser::CArgParser()
{
}

CArgParser::~CArgParser()
{
}

void	CArgParser::EmptyParser(void)
{
	m_iErrorCode = 0;
	m_CommandMap.clear();
	m_CommandParamCount.clear();
	m_InputCommands.clear();
	m_ArgvInput.clear();
}

int		CArgParser::GetErrorCode()
{
	return m_iErrorCode;
}

void	CArgParser::ResetErrorCode()
{
	m_iErrorCode = 0;
}

int		CArgParser::SetOption(int command, string name, int param_flag)
{
	m_CommandMap.emplace(command, name);

	if (param_flag != CMD_ALIAS)
	{
		m_CommandParamCount.emplace(command, param_flag);
	}

	return m_iErrorCode;
}

void	CArgParser::AddDefaultOptions(void)
{
	SetOption(0, "help", NO_PARAMS);
	SetOption(0, "h", CMD_ALIAS);
	SetOption(0, "?", CMD_ALIAS);
	SetOption(1, "version", NO_PARAMS);
	SetOption(1, "v", CMD_ALIAS);
}
	
int	CArgParser::ParseArgs(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		m_ArgvInput.push_back(argv[i]);
	}

	// Add meat of the program here:













	m_Pos = m_InputCommands.begin();  // initialize the iterator for GetNextArgument()

	if ( !m_szInvalidArgs.empty() )
	{
		m_iErrorCode |= ARG_INVALID;
	}

	return m_iErrorCode & ARG_INVALID;
}

string*	CArgParser::GetInvalidArguments(void)
{
	return &m_szInvalidArgs;
}

int		CArgParser::CheckForArgument(int option, string &szParam)
{
	szSingleMap::iterator t_Pos = m_InputCommands.find(option);
	
	if (t_Pos != m_InputCommands.end())
	{
		szParam.assign(t_Pos->second.data());
		return ARG_FOUND;
	}
	else
	{
		m_iErrorCode |= ARG_MISSING;	// Command not in m_CommandMap
	}

	return ARG_MISSING;
}

int		CArgParser::GetNextArgument(string &szParam)
{
	int retval = 0;
	retval = m_Pos->first;
	szParam.assign(m_Pos->second.data());
	m_Pos++;

	return retval;
}

#pragma warning (push)
#pragma warning (disable: 4267)				// If we have even close to 2^32 commands we are doing something wrong
int		CArgParser::GetArgumentCount(void)
{
	return m_InputCommands.size();
}
#pragma warning (pop)

void	CArgParser::ResetArglistPosition(void)
{
	m_Pos = m_InputCommands.begin();
}