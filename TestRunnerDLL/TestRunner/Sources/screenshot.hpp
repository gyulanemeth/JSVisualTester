#ifndef SCREENSHOT_HPP
#define SCREENSHOT_HPP

#include <windows.h>
#include <gdiplus.h>
#include <string>

using namespace Gdiplus;

class ScreenShot
{
public:
	ScreenShot ();
	~ScreenShot ();

	bool		CaptureScreen (const wchar_t* fileName);
	bool		CaptureClient (const wchar_t* fileName);
	bool		CaptureRect (const RECT& rect, const wchar_t* fileName);

private:
	bool		InitGDIPlus ();
	void		ShutGDIPlus ();
	bool		GetClassIdByName (const std::wstring& fileName, CLSID* classId);
	bool		GetEncoderClassId (const wchar_t* format, CLSID* classId);

	bool		CaptureScreen (HBITMAP *bitmapHandle, const RECT& rect);

	bool		SaveBitmap (const HBITMAP bitmapHandle, const wchar_t* fileName);
	bool		SaveBitmapAsPNG (const HBITMAP bitmapHandle, const wchar_t* fileName);

	ULONG_PTR	gdiPlusToken;
};

#endif
