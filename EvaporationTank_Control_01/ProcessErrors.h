#pragma once

#include <string>
#include <xstring>
#include <sstream>

#include <Gl\Gl.h>              // OpenGL
#include <Gl\Glu.h>             // GLU library
//#include <Gl\Glaux.h>			// AUX library objects
#include ".\INCLUDE\Gl\Glaux.h"	// AUX library objects

#define MAX_PROCESS_ERRORS	10

class ProcessErrors
{
protected:
	struct processError
	{
		int index;
		std::string component;
		std::string errorText;
		bool status;
	};
	processError processErrorArray[MAX_PROCESS_ERRORS];

	std::string strVesselName;
	std::ostringstream strstrText;

public:
	ProcessErrors();
	virtual ~ProcessErrors();

	bool SetProcessError(int index, std::string component, std::string errorText, bool status);
	std::string GetProcessErrorComponent(int index);
	std::string GetProcessErrorText(int index);
	bool GetProcessErrorStatus(int index);
	int GetNumberProcessErrors(void);
	void DisplayProcessErrors(GLint nFontList);
};

