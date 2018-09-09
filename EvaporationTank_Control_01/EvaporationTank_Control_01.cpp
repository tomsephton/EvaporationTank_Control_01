// EvaporationTank_Control_01.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "EvaporationTank_Control_01.h"

//#include <stdlib.h>
#include <windows.h>
//#include <conio.h>
#include <process.h>
//#include <string>
//#include <xstring>
//#include <sstream>

//#include <Gl\glew.h>              // OpenGL
#include <Gl\Gl.h>              // OpenGL
#include <Gl\Glu.h>             // GLU library
//#include <Gl\Glaux.h>			// AUX library objects
//#include ".\INCLUDE\Gl\Glaux.h"	// AUX library objects

#include "LogFileOutput.h"
#include "EvaporationTankA.h"
#include "EvaporationTankB.h"
#include "DistillateSupplyTank.h"


// Win32 Palette handle
HPALETTE hPalette = NULL;

// GL Identifier for 2D font display lists
GLint nFontList;
int fontHeight = 15;

// GL Keep track of windows changing width and height
GLfloat windowWidth;
GLfloat windowHeight;

int mouseX = 0;
int mouseY = 0;

int logInterval = 1000;
int longLogInterval = 30000;
int shortLogInterval = 1000;
bool flagExit = false;
bool showChartRecorder = true;

// Pointers to objects instantiated from WinMain()
DaqDevice* ptrDaqDevice;
BinaryInput* ptrBinaryInput;
BinaryOutput* ptrBinaryOutput;
AnalogInput* ptrAnalogInput;
AnalogOutput* ptrAnalogOutput;
HANDLE hInputOutputThread; // Tank Logic thread (WIN32)
HANDLE hLogFileOutputThread; // Tank Logic thread (WIN32)
LogFileOutput* ptrLogFileOutput;
HANDLE hEvaporationTankAThread; // Tank Logic thread (WIN32)
EvaporationTankA* ptrEvaporationTankA;
HANDLE hEvaporationTankBThread; // Tank Logic thread (WIN32)
EvaporationTankB* ptrEvaporationTankB;
HANDLE hDistillateSupplyTankThread; // Tank Logic thread (WIN32)
DistillateSupplyTank* ptrDistillateSupplyTank;
ProgramExceptions* ptrProgramExceptions;

/////////////////////////////////////////////////////////////////
// Select the pixel format for a given device context
void SetDCPixelFormat(HDC hDC)
{
	int nPixelFormat;

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),	// Size of this structure
		1,								// Version of this structure	
		PFD_DRAW_TO_WINDOW |			// Draw to Window (not to bitmap)
		PFD_SUPPORT_OPENGL |			// Support OpenGL calls in window
		PFD_DOUBLEBUFFER,				// Double buffered mode
		PFD_TYPE_RGBA,					// RGBA Color mode
		32,								// Want 32 bit color 
		0,0,0,0,0,0,					// Not used to select mode
		0,0,							// Not used to select mode
		0,0,0,0,0,						// Not used to select mode
		24,								// Size of depth buffer
		8,								// Number of bits for the stencilbuffer 
		0,								// Number of Aux buffers in the framebuffer
		PFD_MAIN_PLANE,          		// Specify Main, Overlay, or Underlay Plane
		0,								// Not used 
		0,0,0 };						// Not used 

	// Choose a pixel format that best matches that described in pfd
	nPixelFormat = ChoosePixelFormat(hDC, &pfd);

	// Set the pixel format for the device context
	SetPixelFormat(hDC, nPixelFormat, &pfd);
}

////////////////////////////////////////////////////////////////////////
// If necessary, creates a 3-3-2 palette for the device context listed.
HPALETTE GetOpenGLPalette(HDC hDC)
{
	HPALETTE hRetPal = NULL;	// Handle to palette to be created
	PIXELFORMATDESCRIPTOR pfd;	// Pixel Format Descriptor
	LOGPALETTE *pPal;			// Pointer to memory for logical palette
	int nPixelFormat;			// Pixel format index
	int nColors;				// Number of entries in palette
	int i;						// Counting variable
	BYTE RedRange,GreenRange,BlueRange;
								// Range for each color entry (7,7,and 3)

	// Get the pixel format index and retrieve the pixel format description
	nPixelFormat = GetPixelFormat(hDC);
	DescribePixelFormat(hDC, nPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	// Does this pixel format require a palette?  If not, do not create a
	// palette and just return NULL
	if(!(pfd.dwFlags & PFD_NEED_PALETTE))
		return NULL;

	// Number of entries in palette.  8 bits yeilds 256 entries
	nColors = 1 << pfd.cColorBits;	

	// Allocate space for a logical palette structure plus all the palette entries
	pPal = (LOGPALETTE*)malloc(sizeof(LOGPALETTE) +nColors*sizeof(PALETTEENTRY));

	// Fill in palette header 
	pPal->palVersion = 0x300;		// Windows 3.0
	pPal->palNumEntries = nColors; // table size

	// Build mask of all 1's.  This creates a number represented by having
	// the low order x bits set, where x = pfd.cRedBits, pfd.cGreenBits, and
	// pfd.cBlueBits.  
	RedRange = (1 << pfd.cRedBits) -1;
	GreenRange = (1 << pfd.cGreenBits) - 1;
	BlueRange = (1 << pfd.cBlueBits) -1;

	// Loop through all the palette entries
	for(i = 0; i < nColors; i++)
	{
		// Fill in the 8-bit equivalents for each component
		pPal->palPalEntry[i].peRed = (i >> pfd.cRedShift) & RedRange;
		pPal->palPalEntry[i].peRed = (unsigned char)(
			(double) pPal->palPalEntry[i].peRed * 255.0 / RedRange);

		pPal->palPalEntry[i].peGreen = (i >> pfd.cGreenShift) & GreenRange;
		pPal->palPalEntry[i].peGreen = (unsigned char)(
			(double)pPal->palPalEntry[i].peGreen * 255.0 / GreenRange);

		pPal->palPalEntry[i].peBlue = (i >> pfd.cBlueShift) & BlueRange;
		pPal->palPalEntry[i].peBlue = (unsigned char)(
			(double)pPal->palPalEntry[i].peBlue * 255.0 / BlueRange);

		pPal->palPalEntry[i].peFlags = (unsigned char) NULL;
	}
		
	// Create the palette
	hRetPal = CreatePalette(pPal);

	// Go ahead and select and realize the palette for this device context
	SelectPalette(hDC,hRetPal,FALSE);
	RealizePalette(hDC);

	// Free the memory used for the logical palette structure
	free(pPal);

	// Return the handle to the new palette
	return hRetPal;
}


///////////////////////////////////////////////////
// Setup. Use a Windows font to create the bitmaps
void SetupRC(HDC hDC)
{
	// Setup the Font characteristics
	HFONT hFont;
	LOGFONT logfont;

	logfont.lfHeight = -fontHeight;
	logfont.lfWidth = 0;
	logfont.lfEscapement = 0;
	logfont.lfOrientation = 0;
	logfont.lfWeight = FW_MEDIUM;
	logfont.lfItalic = FALSE;
	logfont.lfUnderline = FALSE;
	logfont.lfStrikeOut = FALSE;
	logfont.lfCharSet = ANSI_CHARSET;
	logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	logfont.lfQuality = DEFAULT_QUALITY;
	logfont.lfPitchAndFamily = DEFAULT_PITCH;
//	strcpy_s(logfont.lfFaceName, strlen(logfont.lfFaceName), "Arial");

	// Create the font and display list
	hFont = CreateFontIndirect(&logfont);
	SelectObject (hDC, hFont); 

	//Create display lists for glyphs 0 through 128
	nFontList = glGenLists(128);
	wglUseFontBitmaps(hDC, 0, 128, nFontList);

	DeleteObject(hFont); // Don't need original font anymore

	// Black Background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
}

/////////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene(PTCHAR pchPromptBuffer, PTCHAR pchInputBuffer, DWORD lineLength, DWORD textHeight)
{
	// Set background clearing color to light gray
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Display exeptions thrown in WinMain
	ptrProgramExceptions->DisplayProgramExceptions(nFontList, 0, 0);
	// Draw distillate tank and controls
	if (ptrDistillateSupplyTank != NULL)
	{
		ptrDistillateSupplyTank->Display2D(nFontList, 990.0, 300.0);
	}
	// Draw evaporation tank A and controls
	if (ptrEvaporationTankA != NULL)
	{
		ptrEvaporationTankA->Display2D(nFontList, 190.0, 300.0);
		ptrEvaporationTankA->DrawPumpSpeedChart(nFontList);
	}
	// Draw evaporation tank B and controls
	if (ptrEvaporationTankB != NULL)
	{
		ptrEvaporationTankB->Display2D(nFontList, 590.0, 300.0);
		ptrEvaporationTankB->DrawPumpSpeedChart(nFontList);
	}

	// TEMPORARY MOUSE AND DRAWING TEST CODE
	if (false)
	{
		// Box modelview
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(float(1.0), float(1.0), 0.0);

		// box
		glBegin(GL_POLYGON);
		glColor3f(0.1f, 0.1f, 0.8f);
		glVertex2f(-70.0f, 70.0f);
		glVertex2f(70.0f, 70.0f);
		glVertex2f(70.0f, -70.0f);
		glVertex2f(-70.0f, -70.0f);
		glEnd();

		glPopMatrix();
	}
}

///////////////////////////////////////////
// Window has changed size
void ChangeSize(GLsizei w, GLsizei h)
{
	GLfloat nRange = 100.0f;
	GLfloat fAspect;

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;

	fAspect = (GLfloat)w/(GLfloat)h;

	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	windowHeight = (GLfloat)h;
	windowWidth = (GLfloat)w;

// Set the clipping volume
	gluOrtho2D(0.0f, windowWidth, 0.0f, windowHeight);

	// Viewing transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

////////////////////////////////////////////////////////////////
// Called by Windows callbacks to update mouse status
void MouseUpdate(UINT nFlags, POINTS points)
{
	int mouseAction;

	if (nFlags & MK_LBUTTON) mouseAction = 1;
	else if (nFlags & MK_RBUTTON) mouseAction = 2;
	else mouseAction = 0;
	
	mouseX = (int)points.x;
	mouseY = (int)windowHeight - points.y;
	
	if (ptrEvaporationTankA != NULL)
	{
		ptrEvaporationTankA->CheckMouseAction(mouseX, mouseY, mouseAction);
		if (ptrEvaporationTankA->GetDistillateMeteringPumpPtr() != NULL)
		{
			ptrEvaporationTankA->GetDistillateMeteringPumpPtr()->CheckMouseAction(mouseX, mouseY, mouseAction);
		}
	}
	if (ptrEvaporationTankB != NULL)
	{
		ptrEvaporationTankB->CheckMouseAction(mouseX, mouseY, mouseAction);
		if (ptrEvaporationTankB->GetDistillateMeteringPumpPtr() != NULL)
		{
			ptrEvaporationTankB->GetDistillateMeteringPumpPtr()->CheckMouseAction(mouseX, mouseY, mouseAction);
		}
	}
}

// Input/Output object thread function 
unsigned _stdcall InputOutputThread(void* ptrArg)
{
	try
	{
		// unpack the thread argument
		unsigned n = reinterpret_cast<unsigned>(ptrArg);

		if (ptrDaqDevice->OpenDeviceConnection())
		{
	//		ptrBinaryOutput->ZeroAllBits(ptrDaqDevice->GetDaqName(), ptrDaqDevice->GetHandle());
			while (!flagExit)
			{
				ptrBinaryInput->ReadInputDevices(ptrDaqDevice->GetDaqName(), ptrDaqDevice->GetHandle());
				ptrBinaryInput->DisplayExceptions(nFontList, 0, 100);
				Sleep(10);
				ptrAnalogInput->ReadInputDevices(ptrDaqDevice->GetDaqName(), ptrDaqDevice->GetHandle());
				ptrAnalogInput->DisplayExceptions(nFontList, 60, 100);
				Sleep(10);
	//			ptrBinaryOutput->UpdateAllRelays(ptrDaqDevice->GetDaqName(), ptrDaqDevice->GetHandle());
	//			Sleep(10);
				ptrAnalogOutput->UpdateAnalogValves(ptrDaqDevice->GetDaqName(), ptrDaqDevice->GetHandle());
				ptrAnalogOutput->DisplayExceptions(nFontList, 120, 100);
				Sleep(10);
//				throw ProgramException(" ptrDaqDevice->OpenDeviceConnection()loop running");
			}
			// Exiting
			// Set ouput values to zero state
			ptrAnalogOutput->SetRequestedValveSetting(0, 0, 4.0);
			ptrAnalogOutput->UpdateAnalogValves(ptrDaqDevice->GetDaqName(), ptrDaqDevice->GetHandle());
			// Zero all outputs
	//		ptrBinaryOutput->ZeroAllBits(ptrDaqDevice->GetDaqName(), ptrDaqDevice->GetHandle());
			Sleep(10);
			ptrDaqDevice->CloseDeviceConnection();
			return 0;
		}
		else
			throw ProgramException("Failure in ptrDaqDevice->OpenDeviceConnection()");
			return -1;
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(1, "Main :: ", pe.GetExceptionDescription(), true);
	}
	return 0;
}

// Log File Output object thread function 
unsigned _stdcall LogFileOutputThread(void* ptrArg)
{
	// unpack the thread argument
	unsigned n = reinterpret_cast<unsigned>(ptrArg);

	// Set pointer to LogFileOutput object in all objects with logic
	if (ptrEvaporationTankA != NULL) ptrEvaporationTankA->SetLogFileOutputPtr(ptrLogFileOutput);
	if (ptrEvaporationTankB != NULL) ptrEvaporationTankB->SetLogFileOutputPtr(ptrLogFileOutput);
	if (ptrDistillateSupplyTank != NULL) ptrDistillateSupplyTank->SetLogFileOutputPtr(ptrLogFileOutput);
	
	Sleep(6000); // Wait for tank and vessel objects to write column labels
	ptrLogFileOutput->WriteColumnLabels();
	int intervalCounter = 0;
	while (!flagExit)
	{
		if (logInterval <= shortLogInterval && intervalCounter < 120) intervalCounter += 1;
		else
		{
			logInterval = longLogInterval;
			intervalCounter = 0;
		}

		ptrLogFileOutput->WriteCurrentData();
		ptrLogFileOutput->SetDataItem(0, "Operator Input Event", "");
		Sleep(logInterval);
	}
	return 0;
} 

// Evaporation Tank A thread function 
unsigned _stdcall EvaporationTankAThread(void* ptrArg)
{
	// unpack the thread argument
	unsigned n = reinterpret_cast<unsigned>(ptrArg);
	ptrEvaporationTankA->InstantiateObjects(ptrBinaryInput, ptrBinaryOutput, ptrAnalogInput, ptrAnalogOutput, "A");
	ptrEvaporationTankA->InitializeObjects();
	ptrEvaporationTankA->SetDistillateSupplyTankPtr(ptrDistillateSupplyTank);
	Sleep(100);
	while (!flagExit)
	{
		ptrEvaporationTankA->TankControlLogic();
		Sleep(100);
	}
	// Stop distillate pump on exit
	ptrEvaporationTankA->SetDistillatePumpSpeed(0.0);
	return 0;
}

// Evaporation Tank B thread function 
unsigned _stdcall EvaporationTankBThread(void* ptrArg)
{
	// unpack the thread argument
	unsigned n = reinterpret_cast<unsigned>(ptrArg);
	ptrEvaporationTankB->InstantiateObjects(ptrBinaryInput, ptrBinaryOutput, ptrAnalogInput, ptrAnalogOutput, "B");
	ptrEvaporationTankB->InitializeObjects();
	ptrEvaporationTankB->SetDistillateSupplyTankPtr(ptrDistillateSupplyTank);
	Sleep(100);
	while (!flagExit)
	{
		ptrEvaporationTankB->TankControlLogic();
		Sleep(100);
	}
	// Stop pump on exit
	ptrEvaporationTankB->SetDistillatePumpSpeed(0.0);
	return 0;
}

// Distillate Supply Tank thread function 
unsigned _stdcall DistillateSupplyTankThread(void* ptrArg)
{
	// unpack the thread argument
	unsigned n = reinterpret_cast<unsigned>(ptrArg);
	ptrDistillateSupplyTank->InstantiateObjects(ptrBinaryInput, ptrBinaryOutput, ptrAnalogInput, ptrAnalogOutput, "C");
	ptrDistillateSupplyTank->InitializeObjects();
	Sleep(100);
	while (!flagExit)
	{
		ptrDistillateSupplyTank->TankControlLogic();
		Sleep(100);
	}
	return 0;
}

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.

	ptrProgramExceptions = new ProgramExceptions();

	// Set up Input/Output objects
	ptrDaqDevice = new DaqDevice();
	ptrBinaryInput = new BinaryInput();
//	ptrBinaryOutput = new BinaryOutput();
	ptrAnalogInput = new AnalogInput();
	ptrAnalogOutput = new AnalogOutput();

	ptrBinaryInput->SetDaqDevicePtr(ptrDaqDevice);
//	ptrBinaryOutput->SetDaqDevicePtr(ptrDaqDevice);
	ptrAnalogInput->SetDaqDevicePtr(ptrDaqDevice);
	ptrAnalogOutput->SetDaqDevicePtr(ptrDaqDevice);
	
	// Start Input/Output thread
	try
	{
		if (ptrDaqDevice != NULL && ptrBinaryInput != NULL && ptrAnalogInput != NULL && ptrAnalogOutput != NULL)
//		if (ptrDaqDevice != NULL && ptrBinaryInput != NULL && ptrBinaryOutput != NULL && ptrAnalogInput != NULL && ptrAnalogOutput != NULL)
		{
			unsigned threadID00;
			hInputOutputThread = (HANDLE)_beginthreadex(NULL, 0, InputOutputThread, NULL, 0, &threadID00);
			if (!hInputOutputThread)
			{
				throw ProgramException("_beginthreadex() failed on Input/Output thread");
			}
		}
		else throw ProgramException("Input/Output objects failed to instantiate");
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(1, "Main :: ", pe.GetExceptionDescription(), true);
	}

	Sleep(1000);// Wait for inputs to be read before starting logic threads

	// Start thread to run Distillate Supply Tank logic
	ptrDistillateSupplyTank = new DistillateSupplyTank();
	try
	{
		if (ptrDistillateSupplyTank != NULL)
		{
			unsigned threadID01;
			hDistillateSupplyTankThread = (HANDLE)_beginthreadex(NULL, 0, DistillateSupplyTankThread, NULL, 0, &threadID01);
			if (!hDistillateSupplyTankThread)
			{
				throw ProgramException("_beginthreadex() failed on DistillateSupplyTankThread thread start\n");
			}
		}
		else throw ProgramException("Null pointer on DistillateSupplyTankThread thread start\n");
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(1, "Main :: ", pe.GetExceptionDescription(), true);
	}

	Sleep(100);// Wait for distillate supply tank logic

	// Start thread to run Evaporation Tank A logic
	ptrEvaporationTankA = new EvaporationTankA();
	try
	{
		if (ptrEvaporationTankA != NULL)
		{
			unsigned threadID02;
			hEvaporationTankAThread = (HANDLE)_beginthreadex(NULL, 0, EvaporationTankAThread, NULL, 0, &threadID02);
			if (!hEvaporationTankAThread)
			{
				throw ProgramException("_beginthreadex() failed on EvaporationTankA thread start\n");
			}
		}
		else throw ProgramException("Null pointer on EvaporationTankA thread start\n");
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(1, "Main :: ", pe.GetExceptionDescription(), true);
	}

	// Start thread to run Evaporation Tank B logic
	ptrEvaporationTankB = new EvaporationTankB();
	try
	{
		if (ptrEvaporationTankB != NULL)
		{
			unsigned threadID03;
			hEvaporationTankBThread = (HANDLE)_beginthreadex(NULL, 0, EvaporationTankBThread, NULL, 0, &threadID03);
			if (!hEvaporationTankBThread)
			{
				throw ProgramException("_beginthreadex() failed on EvaporationTankB thread start\n");
			}
		}
		else throw ProgramException("Null pointer on EvaporationTankB thread start\n");
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(1, "Main :: ", pe.GetExceptionDescription(), true);
	}

	// Start Log File Output thread
	ptrLogFileOutput = new LogFileOutput();
	try
	{
		if (ptrLogFileOutput != NULL)
		{
			unsigned threadID04;
			hLogFileOutputThread = (HANDLE)_beginthreadex(NULL, 0, LogFileOutputThread, NULL, 0, &threadID04);
			if (!hLogFileOutputThread)
			{
				throw ProgramException("_beginthreadex() failed on Log File Output thread start\n");
			}
		}
		else throw ProgramException("Null pointer on LogFileOutput thread start\n");
	}
	catch (ProgramException& pe)
	{
		ptrProgramExceptions->SetProgramException(1, "Main :: ", pe.GetExceptionDescription(), true);
	}
	Sleep(100);

	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_EVAPORATIONTANK_CONTROL_01, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EVAPORATIONTANK_CONTROL_01));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//

// Application name for Window top bar 
//static LPCTSTR lpszAppName = "Evaporation Tank Control (version 1.00)";

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EVAPORATIONTANK_CONTROL_01));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	// No need for background brush for OpenGL window
//	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.hbrBackground	= NULL;	
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_EVAPORATIONTANK_CONTROL_01);
	wcex.lpszClassName	= szWindowClass;
//	wcex.lpszClassName	= lpszAppName;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass,
		szTitle,
		// OpenGL requires WS_CLIPCHILDREN and WS_CLIPSIBLINGS
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		// Window position and size
		0, 0,
		1200, 700,
		NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
#define BUFSIZE 65535 
#define SHIFTED 0x8000

// Callback function for messages received by main window
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	static HGLRC hRC = NULL;		// Permanent Rendering context
	static HDC hDC = NULL;			// Private GDI Device context

	TEXTMETRIC tm;             // structure for text metrics 
    static DWORD dwCharX;      // average width of characters 
    static DWORD dwCharY;      // height of characters 
    static DWORD dwClientX;    // width of client area 
    static DWORD dwClientY;    // height of client area 
    static DWORD dwLineLen;    // line length 
    static DWORD dwLines;      // text lines in client area 
    static int nCaretPosX = 0; // horizontal position of caret 
    static int nCaretPosY = 0; // vertical position of caret 
    static int nCharWidth = 0; // width of a character 
    static int cch = 0;        // characters in buffer 
    static int nCurChar = 0;   // index of current character 
    static PTCHAR pchInputBuf; // input buffer 
//    int i, j;                  // loop counters 
    int cCR = 0;               // count of carriage returns 
    int nCRIndex = 0;          // index of last carriage return 
//    int nVirtKey;              // virtual-key code 
//    TCHAR szBuf[128];          // temporary buffer 
//    TCHAR ch;                  // current character 
//    SIZE sz;                   // string dimensions 
//    COLORREF crPrevText;       // previous text color 
//    COLORREF crPrevBk;         // previous background color

	static PTCHAR pchPromptBuf; // user prompt buffer 

	switch (message)
	{
		// Window creation, setup for OpenGL
        case WM_CREATE:
	
			// Store the device context
			hDC = GetDC(hWnd);		

			// Select the pixel format
			SetDCPixelFormat(hDC);		

			// Create the rendering context and make it current
			hRC = wglCreateContext(hDC);
			wglMakeCurrent(hDC, hRC);

			// Create the palette
			hPalette = GetOpenGLPalette(hDC);

			// Create a timer that fires 30 times a second
			SetTimer(hWnd,33,1,NULL);
 
            // Get the metrics of the current font. 
 
 //           hDC = GetDC(hWnd); 
            GetTextMetrics(hDC, &tm); 
//            ReleaseDC(hWnd, hDC); 
 
            // Save the average character width and height. 
            dwCharX = tm.tmAveCharWidth; 
            dwCharY = tm.tmHeight; 
 
            // Allocate a buffer to store keyboard input. 
            pchInputBuf = (LPTSTR) GlobalAlloc(GPTR, BUFSIZE * sizeof(TCHAR));
			pchPromptBuf = (LPTSTR) GlobalAlloc(GPTR, BUFSIZE * sizeof(TCHAR));

			// Load initial user prompt into buffer
//			if (ptrProcessOperatorCommand != NULL) strcpy_s(pchPromptBuf, BUFSIZE, ptrProcessOperatorCommand->GetPromptPtr());
			SetupRC(hDC);
			ReleaseDC(hWnd, hDC);
            break;

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	// Timer, waits 1/100th of a second, then invalidates the window so it will be redrawn.
	case WM_TIMER:

		Sleep(10); // Dalays redraw by 1/100th of a second
		InvalidateRect(hWnd,NULL,FALSE); // Forces redraw of window
		break;

// Windows mouse message handlers
	case WM_LBUTTONDOWN:
		MouseUpdate(wParam, MAKEPOINTS(lParam));
		break;

	case WM_LBUTTONUP:
		MouseUpdate(wParam, MAKEPOINTS(lParam));
		break;

	case WM_LBUTTONDBLCLK:
		MouseUpdate(wParam, MAKEPOINTS(lParam));
		break;

	case WM_MBUTTONDOWN:
		MouseUpdate(wParam, MAKEPOINTS(lParam));
		break;

	case WM_MBUTTONUP:
		MouseUpdate(wParam, MAKEPOINTS(lParam));
		break;

	case WM_MBUTTONDBLCLK:
		MouseUpdate(wParam, MAKEPOINTS(lParam));
		break;

	case WM_RBUTTONDOWN:
		MouseUpdate(wParam, MAKEPOINTS(lParam));
		break;

	case WM_RBUTTONUP:
		MouseUpdate(wParam, MAKEPOINTS(lParam));
		break;

	case WM_RBUTTONDBLCLK:
		MouseUpdate(wParam, MAKEPOINTS(lParam));
		break;

	case WM_MOUSEMOVE:
		MouseUpdate(wParam, MAKEPOINTS(lParam));
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		
		// Call OpenGL drawing code
		RenderScene(pchPromptBuf, pchInputBuf, dwLineLen, dwCharY);

		// Call function to swap the buffers
		SwapBuffers(hDC);

		// Validate the newly painted client area
		ValidateRect(hWnd,NULL);
		
		EndPaint(hWnd, &ps);
		break;

		// Windows is telling the application that it may modify
		// the system palette.  This message in essance asks the 
		// application for a new palette.
		case WM_QUERYNEWPALETTE:
			// If the palette was created.
			if(hPalette)
			{
				int nRet;

				// Selects the palette into the current device context
				SelectPalette(hDC, hPalette, FALSE);

				// Map entries from the currently selected palette to
				// the system palette.  The return value is the number 
				// of palette entries modified.
				nRet = RealizePalette(hDC);

				// Repaint, forces remap of palette in current window
				InvalidateRect(hWnd,NULL,FALSE);

				return nRet;
			}
			break;

		case WM_SIZE:

			// Call our function which modifies the clipping
			// volume and viewport
			ChangeSize(LOWORD(lParam), HIWORD(lParam));
			 
            // Save the new width and height of the client area. 
            dwClientX = LOWORD(lParam); 
            dwClientY = HIWORD(lParam); 
 
            // Calculate the maximum width of a line and the 
            // maximum number of lines in the client area. 
            dwLineLen = dwClientX - dwCharX; 
            dwLines = dwClientY / dwCharY; 
            break; 

	case WM_DESTROY:
		flagExit = true;
		Sleep(1000);
		if (hInputOutputThread != NULL) CloseHandle(reinterpret_cast<HANDLE>(hInputOutputThread)); // finish with InputOutput thread (WIN32)
		if (hLogFileOutputThread != NULL) CloseHandle(reinterpret_cast<HANDLE>(hLogFileOutputThread)); // finish with LogFileOutputthread (WIN32)
		if (hEvaporationTankAThread != NULL) CloseHandle(reinterpret_cast<HANDLE>(hEvaporationTankAThread)); // finish with EvaporationTankA thread (WIN32)
		if (hEvaporationTankBThread != NULL) CloseHandle(reinterpret_cast<HANDLE>(hEvaporationTankBThread)); // finish with EvaporationTankB thread (WIN32)
		if (hDistillateSupplyTankThread != NULL) CloseHandle(reinterpret_cast<HANDLE>(hDistillateSupplyTankThread)); // finish with DistillateSupplyTank thread (WIN32)
		glDeleteLists(nFontList, 128);

		// Deselect the current rendering context and delete it
		wglMakeCurrent(hDC,NULL);
		wglDeleteContext(hRC);

		// Delete the palette
		if(hPalette != NULL)
			DeleteObject(hPalette);

		// Tell the application to terminate after the window is gone.
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


