#include <windows.h>

#include "commands.hpp"

static void CalculateSurfaceCoords (int xCoord, int yCoord, int& newXCoord, int& newYCoord)
{
	int screenWidth = GetSystemMetrics (SM_CXSCREEN);
	int screenHeight = GetSystemMetrics (SM_CYSCREEN);
	newXCoord = xCoord * 65535 / screenWidth;
	newYCoord = yCoord * 65535 / screenHeight;
}

static void PressSpecialKeys (byte shiftState, DWORD flags)
{
	if (shiftState & 1) {
		keybd_event (VK_SHIFT, 0, flags, 0);
	}
	if (shiftState & 2) {
		keybd_event (VK_CONTROL, 0, flags, 0);
	}
	if (shiftState & 4) {
		keybd_event (VK_MENU, 0, flags, 0);
	}
}

static void PressKeyWithSpecialKeys (byte keyCode, byte shiftState)
{
	PressSpecialKeys (shiftState, 0);
	keybd_event (keyCode, 0, 0 , 0);
	keybd_event (keyCode, 0, KEYEVENTF_KEYUP, 0);
	PressSpecialKeys (shiftState, KEYEVENTF_KEYUP);
}

static void PressCharacterKey (char character)
{
	short keyScan = VkKeyScan (character);
	byte keyCode = LOBYTE (keyScan);
	byte shiftState = HIBYTE (keyScan);
	PressKeyWithSpecialKeys (keyCode, shiftState);
}

Command::Command ()
{

}

Command::~Command ()
{

}

SleepCommand::SleepCommand (int milliSeconds) :
	Command (),
	milliSeconds (milliSeconds)
{

}

SleepCommand::~SleepCommand ()
{

}

void SleepCommand::Execute ()
{
	Sleep (milliSeconds);
}

MoveWindowCommand::MoveWindowCommand (const RECT& rect) :
	Command (),
	rect (rect)
{

}

MoveWindowCommand::~MoveWindowCommand ()
{

}

void MoveWindowCommand::Execute ()
{
	HWND hwnd = GetForegroundWindow ();
	int x = rect.left;
	int y = rect.top;
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	ShowWindow (hwnd, SW_RESTORE);
	MoveWindow (hwnd, x, y, width, height, true);
}

KeyPressCommand::KeyPressCommand (byte keyCode, byte specialKeyCode) :
	Command (),
	keyCode (keyCode),
	specialKeyCode (specialKeyCode)
{
	
}

KeyPressCommand::~KeyPressCommand ()
{

}

void KeyPressCommand::Execute ()
{
	if (specialKeyCode != 0) {
		keybd_event (specialKeyCode, 0, 0 , 0);
	}
	keybd_event (keyCode, 0, 0 , 0);
	keybd_event (keyCode, 0, KEYEVENTF_KEYUP, 0);
	if (specialKeyCode != 0) {
		keybd_event (specialKeyCode, 0, KEYEVENTF_KEYUP, 0);
	}
}

TextWriteCommand::TextWriteCommand (const std::wstring& text) :
	Command (),
	text (text)
{
	
}

TextWriteCommand::~TextWriteCommand ()
{

}

void TextWriteCommand::Execute ()
{
	for (unsigned int i = 0; i < text.length (); i++) {
		char character = static_cast<char> (text[i]);
		PressCharacterKey (character);
	}
}

MouseCommand::MouseCommand (int xCoord, int yCoord) :
	Command (),
	xCoord (xCoord),
	yCoord (yCoord)
{

}

MouseCommand::~MouseCommand ()
{

}

MouseClickCommand::MouseClickCommand (int xCoord, int yCoord) :
	MouseCommand (xCoord, yCoord)	
{

}

MouseClickCommand::~MouseClickCommand ()
{

}

void MouseClickCommand::Execute ()
{
	int surfaceX = 0;
	int surfaceY = 0;
	CalculateSurfaceCoords (xCoord, yCoord, surfaceX, surfaceY);
	mouse_event (MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, surfaceX, surfaceY, 0, 0);
	mouse_event (MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, surfaceX, surfaceY, 0, 0);
}

MouseDoubleClickCommand::MouseDoubleClickCommand (int xCoord, int yCoord) :
	MouseCommand (xCoord, yCoord)	
{

}

MouseDoubleClickCommand::~MouseDoubleClickCommand ()
{

}

void MouseDoubleClickCommand::Execute ()
{
	int surfaceX = 0;
	int surfaceY = 0;
	CalculateSurfaceCoords (xCoord, yCoord, surfaceX, surfaceY);
	mouse_event (MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, surfaceX, surfaceY, 0, 0);
	mouse_event (MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, surfaceX, surfaceY, 0, 0);
	mouse_event (MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, surfaceX, surfaceY, 0, 0);
}

MouseDragDropCommand::MouseDragDropCommand (int xCoord, int yCoord, int newXCoord, int newYCoord, int milliSeconds) :
	MouseCommand (xCoord, yCoord),
	newXCoord (newXCoord),
	newYCoord (newYCoord),
	milliSeconds (milliSeconds)
{

}

MouseDragDropCommand::~MouseDragDropCommand ()
{

}

void MouseDragDropCommand::Execute ()
{
	int surfaceX = 0;
	int surfaceY = 0;
	CalculateSurfaceCoords (xCoord, yCoord, surfaceX, surfaceY);

	int newSurfaceX = 0;
	int newSurfaceY = 0;
	CalculateSurfaceCoords (newXCoord, newYCoord, newSurfaceX, newSurfaceY);

	mouse_event (MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, surfaceX, surfaceY, 0, 0);

	mouse_event (MOUSEEVENTF_LEFTDOWN, surfaceX, surfaceY, 0, 0);
	Sleep (milliSeconds);

	mouse_event (MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, newSurfaceX, newSurfaceY, 0, 0);

	Sleep (milliSeconds);
	mouse_event (MOUSEEVENTF_LEFTUP, newSurfaceX, newSurfaceY, 0, 0);
}
