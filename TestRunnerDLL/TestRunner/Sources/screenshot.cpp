#include "screenshot.hpp"

ScreenShot::ScreenShot ()
{
	InitGDIPlus ();
}

ScreenShot::~ScreenShot ()
{
	ShutGDIPlus ();
}

bool ScreenShot::CaptureScreen (const wchar_t* fileName)
{
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = GetSystemMetrics (SM_CXSCREEN);
	rect.bottom = GetSystemMetrics (SM_CYSCREEN);

	HBITMAP bitmapHandle;
	if (!CaptureScreen (&bitmapHandle, rect)) {
		return false;
	}

	if (!SaveBitmap (bitmapHandle, fileName)) {
		return false;
	}

	return true;
}

bool ScreenShot::CaptureClient (const wchar_t* fileName)
{
	RECT rect;
	HWND foregroundWindow = GetForegroundWindow ();
	GetClientRect (foregroundWindow, &rect);

	POINT start;
	start.x = rect.left;
	start.y = rect.top;
	ClientToScreen (foregroundWindow, &start);

	rect.left = start.x;
	rect.top = start.y;
	rect.right = rect.left + rect.right;
	rect.bottom = rect.top + rect.bottom;

	HBITMAP bitmapHandle;
	if (!CaptureScreen (&bitmapHandle, rect)) {
		return false;
	}

	if (!SaveBitmap (bitmapHandle, fileName)) {
		return false;
	}

	return true;
}

bool ScreenShot::CaptureRect (const RECT& rect, const wchar_t* fileName)
{
	HBITMAP bitmapHandle;
	if (!CaptureScreen (&bitmapHandle, rect)) {
		return false;
	}

	if (!SaveBitmap (bitmapHandle, fileName)) {
		return false;
	}

	return true;
}

bool ScreenShot::InitGDIPlus ()
{
	GdiplusStartupInput input;
	GdiplusStartupOutput output;
	Status status = GdiplusStartup (&gdiPlusToken, &input, &output);
	if (status != Ok) {
		return false;
	}

	return true;
}

void ScreenShot::ShutGDIPlus ()
{
	GdiplusShutdown (gdiPlusToken);
}

bool ScreenShot::GetClassIdByName (const std::wstring& fileName, CLSID* classId)
{
	int length = fileName.size ();
	int lastPoint = fileName.find_last_of (L'.');
	std::wstring extension;
	if (lastPoint != -1 && lastPoint < length - 1) {
		extension = fileName.substr (lastPoint + 1, length - lastPoint);
	}

	bool hasClassId = false;
	if (extension == L"bmp") {
		return GetEncoderClassId (L"image/bmp", classId);
	} else if (extension == L"jpg" || extension == L"jpeg") {
		return GetEncoderClassId (L"image/jpeg", classId);
	} else {
		return GetEncoderClassId (L"image/png", classId);
	}

	return false;
}

bool ScreenShot::GetEncoderClassId (const wchar_t* format, CLSID* classId)
{
	UINT count = 0;
	UINT size = 0;
	GetImageEncodersSize (&count, &size);
	if(count == 0 || size == 0) {
		return false;
	}

	ImageCodecInfo* imageCodecInfo = (ImageCodecInfo*) (malloc(size));
	if(imageCodecInfo == NULL) {
		return false;
	}

	GetImageEncoders (count, size, imageCodecInfo);

	for (UINT i = 0; i < count; i++) {
		if (wcscmp (imageCodecInfo[i].MimeType, format) == 0) {
			*classId = imageCodecInfo[i].Clsid;
			free (imageCodecInfo);
			return true;
		}
	}

	free (imageCodecInfo);
	return false;
}

bool ScreenShot::CaptureScreen (HBITMAP *bitmapHandle, const RECT& rect)
{
	if (bitmapHandle == NULL) {
		return false;
	}

	HWND screenHandle = GetDesktopWindow ();
	if (!screenHandle) {
		return false;
	}

	HDC screenContext = GetDC (screenHandle);
	if (screenContext == NULL) {
		return false;
	}

	HDC memoryContext = CreateCompatibleDC (screenContext);
	if (memoryContext == NULL) {
		return false;
	}

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	if (width <= 0 || height <= 0) {
		return false;
	}
	
	*bitmapHandle = CreateCompatibleBitmap (screenContext, width, height);
	if (*bitmapHandle == NULL) {
		return false;
	}
	
	HGDIOBJ oldObject = SelectObject (memoryContext, *bitmapHandle);
	BitBlt (memoryContext, 0, 0, width, height, screenContext, rect.left, rect.top, SRCCOPY);
	SelectObject (memoryContext, oldObject);

    ReleaseDC (screenHandle, screenContext);
	DeleteDC (memoryContext);

	return true;
}

bool ScreenShot::SaveBitmap (const HBITMAP bitmapHandle, const wchar_t* fileName)
{
	CLSID classID;
	if (!GetClassIdByName (fileName, &classID)) {
		return false;
	}

	Bitmap bitmap (bitmapHandle, NULL);
	Status status = bitmap.Save (fileName, &classID, NULL);
	if (status != Ok) {
		return false;
	}

	return true;
}
