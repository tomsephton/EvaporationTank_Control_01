#include "StdAfx.h"
#include "ProgramException.h"


ProgramException::ProgramException(std::string exceptionText)
{
	exceptionDescription = exceptionText;
}

ProgramException::~ProgramException(void)
{
	exceptionDescription = "";
}

std::string ProgramException::GetExceptionDescription()
{
	return exceptionDescription;
}