#define DLLEXPORT __declspec(dllexport)

#include <string>

extern "C"
{
	DLLEXPORT void TRSleep (int milliSeconds);
	DLLEXPORT void TRMoveWindow (int left, int top, int right, int bottom);
	DLLEXPORT void TRKeyPress (int keyCode, int scanCode);
	DLLEXPORT void TRTextWrite (const wchar_t* text);
	DLLEXPORT void TRMouseClick (int x, int y);
	DLLEXPORT void TRMouseDoubleClick (int x, int y);
	DLLEXPORT void TRMouseDragDrop (int x, int y, int newX, int newY, int milliSeconds);

	DLLEXPORT void TRCaptureScreen (const wchar_t* fileName);
	DLLEXPORT void TRCaptureClient (const wchar_t* fileName);
	DLLEXPORT void TRCaptureRect (int left, int top, int right, int bottom, const wchar_t* fileName);
	DLLEXPORT void TRGetForegroundClientRect (RECT* rect);

	DLLEXPORT int TRCompareImages (const wchar_t* image1, const wchar_t* image2, const wchar_t* result);
}
