#pragma once

#include <string>
#include <xstring>
#include <sstream>

#include <Gl\Gl.h>              // OpenGL
#include <Gl\Glu.h>             // GLU library
//#include <Gl\Glaux.h>			// AUX library objects
#include ".\INCLUDE\Gl\Glaux.h"	// AUX library objects

#define MAX_PROGRAM_EXCEPTIONS	12

class ProgramExceptions
{
protected:
	struct programException
	{
		int index;
		std::string function;
		std::string exceptionDescription;
		bool status;
	};
	programException programExceptionArray[MAX_PROGRAM_EXCEPTIONS];

	std::string strFunctionName;
	std::ostringstream strstrText;

public:
	ProgramExceptions();
	virtual ~ProgramExceptions();

	bool SetProgramException(int index, std::string function, std::string exceptionDescription, bool status);
	std::string GetProgramExceptionFunction(int index);
	std::string GetProgramExceptionDescription(int index);
	bool GetProgramExceptionStatus(int index);
	int GetNumberProgramExceptions(void);
	void DisplayProgramExceptions(GLint nFontList, int xpos, int ypos);
	void ClearProgramException(int index);
	std::string GetAllProgramExceptions(void);
};

