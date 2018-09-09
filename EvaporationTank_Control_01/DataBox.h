// DataBox.h: interface for the DataBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATABOX_H__6412CAEB_EC91_4AED_BA45_F06A09DA3ED8__INCLUDED_)
#define AFX_DATABOX_H__6412CAEB_EC91_4AED_BA45_F06A09DA3ED8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <xstring>
#include <sstream>

#include <Gl\Gl.h>              // OpenGL
#include <Gl\Glu.h>             // GLU library
#include <Gl\Glaux.h>			// AUX library objects

#define MAX_VALUE_LINES	3

class DataBox  
{
protected:
	std::string strSensorName;
	std::string strSensorValue[MAX_VALUE_LINES];
	std::string strSensorUnits[MAX_VALUE_LINES];
	std::ostringstream strstrDisplayValue;

public:
	DataBox();
	virtual ~DataBox();

	void SetSensorDisplayName(std::string name);
	void DrawDataBox2D(GLint nFontList, std::string values[MAX_VALUE_LINES], std::string units[MAX_VALUE_LINES], int xPos, int yPos, int xOrigin, int yOrigin);
};

#endif // !defined(AFX_DATABOX_H__6412CAEB_EC91_4AED_BA45_F06A09DA3ED8__INCLUDED_)
