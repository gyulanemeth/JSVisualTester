#include <windows.h>

#include "interface.hpp"
#include "commands.hpp"
#include "screenshot.hpp"
#include "imagecompare.hpp"

static ScreenShot screenShot;
static ImageCompare imageCompare;

void TRSleep (int milliSeconds)
{
	SleepCommand command (milliSeconds);
	command.Execute ();
}

void TRMoveWindow (int left, int top, int right, int bottom)
{
	RECT rect;
	rect.left = left;
	rect.top = top;
	rect.right = right;
	rect.bottom = bottom;
	MoveWindowCommand command (rect);
	command.Execute ();
}

void TRKeyPress (int keyCode, int specialKeyCode)
{
	KeyPressCommand command (static_cast<byte> (keyCode), static_cast<byte> (specialKeyCode));
	command.Execute ();
}

void TRTextWrite (const wchar_t* text)
{
	TextWriteCommand command (text);
	command.Execute ();
}

void TRMouseClick (int x, int y)
{
	MouseClickCommand command (x, y);
	command.Execute ();
}

void TRMouseDoubleClick (int x, int y)
{
	MouseDoubleClickCommand command (x, y);
	command.Execute ();
}

void TRMouseDragDrop (int x, int y, int newX, int newY, int milliSeconds)
{
	MouseDragDropCommand command (x, y, newX, newY, milliSeconds);
	command.Execute ();
}

void TRCaptureScreen (const wchar_t* fileName)
{
	screenShot.CaptureScreen (fileName);
}

void TRCaptureClient (const wchar_t* fileName)
{
	screenShot.CaptureClient (fileName);
}

void TRCaptureRect (int left, int top, int right, int bottom, const wchar_t* fileName)
{
	RECT rect;
	rect.left = left;
	rect.right = right;
	rect.top = top;
	rect.bottom = bottom;
	screenShot.CaptureRect (rect, fileName);
}

void TRGetForegroundClientRect (RECT* rect)
{
	HWND foregroundWindow = GetForegroundWindow ();
	GetClientRect (foregroundWindow, rect);

	POINT start;
	start.x = rect->left;
	start.y = rect->top;
	ClientToScreen (foregroundWindow, &start);

	rect->left = start.x;
	rect->top = start.y;
	rect->right = rect->left + rect->right;
	rect->bottom = rect->top + rect->bottom;
}

int TRCompareImages (const wchar_t* image1, const wchar_t* image2, const wchar_t* result)
{
	ImageCompare::Parameters parameters;
	parameters.SetImageName1 (image1);
	parameters.SetImageName2 (image2);
	parameters.SetResultName (result);

	int diffCount = 0;
	ImageCompare imageCompare;
	ImageCompare::Result cmpResult = imageCompare.Compare (parameters, &diffCount);

	int retVal = 0;
	switch (cmpResult) {
		case ImageCompare::InternalError:
			retVal = -3;
			break;
		case ImageCompare::InvalidFile:
			retVal = -2;
			break;
		case ImageCompare::NotEqualSize:
			retVal = -1;
			break;
		case ImageCompare::NotEqualContent:
			retVal = diffCount;
			break;
		case ImageCompare::Equal:
			retVal = 0;
			break;
	}

	return retVal;
}
