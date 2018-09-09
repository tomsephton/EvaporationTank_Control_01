#pragma once

#include <string>
#include <xstring>
#include <sstream>

class ProgramException
{
private:
	std::string exceptionDescription;
public:
	ProgramException(std::string exceptionText);
	virtual ~ProgramException(void);
	std::string GetExceptionDescription();
};

