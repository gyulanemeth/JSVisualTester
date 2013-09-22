#include "imagecompare.hpp"

#include <gdiplus.h>

static bool EqualColors (const Color& color1, const Color& color2)
{
	return color1.GetR () == color2.GetR () &&
		   color1.GetG () == color2.GetG () &&
		   color1.GetB () == color2.GetB () &&
		   color1.GetA () == color2.GetA ();
}

ImageCompare::Parameters::Parameters ()
{

}

ImageCompare::Parameters::~Parameters ()
{

}

void ImageCompare::Parameters::SetImageName1 (const std::wstring& imageName)
{
	imageName1 = imageName;
}

void ImageCompare::Parameters::SetImageName2 (const std::wstring& imageName)
{
	imageName2 = imageName;
}

void ImageCompare::Parameters::SetResultName (const std::wstring& imageName)
{
	resultName = imageName;
}

const std::wstring& ImageCompare::Parameters::GetImageName1 () const
{
	return imageName1;
}

const std::wstring& ImageCompare::Parameters::GetImageName2 () const
{
	return imageName2;
}

const std::wstring& ImageCompare::Parameters::GetResultName () const
{
	return resultName;
}

ImageCompare::ImageCompare ()
{
	InitGDIPlus ();
}

ImageCompare::~ImageCompare ()
{
	ShutGDIPlus ();
}

ImageCompare::Result ImageCompare::Compare (const Parameters& parameters, int* diffCount)
{
	if (diffCount != NULL) {
		*diffCount = 0;
	}

	const std::wstring& imageName1 = parameters.GetImageName1 ();
	Bitmap bitmap1 (imageName1.c_str ());
	if (bitmap1.GetLastStatus () != Ok) {
		return InvalidFile;
	}

	const std::wstring& imageName2 = parameters.GetImageName2 ();
	Bitmap bitmap2 (imageName2.c_str ());
	if (bitmap2.GetLastStatus () != Ok) {
		return InvalidFile;
	}

	UINT width1 = bitmap1.GetWidth ();
	UINT width2 = bitmap2.GetWidth ();
	if (width1 != width2) {
		return NotEqualSize;
	}

	UINT height1 = bitmap1.GetHeight ();
	UINT height2 = bitmap2.GetHeight ();
	if (height1 != height2) {
		return NotEqualSize;
	}

	Rect rect (0, 0, width1, height1);
	int diffPixels = CalculateDiff (bitmap1, bitmap2, rect);

	const std::wstring& resultName = parameters.GetResultName ();
	bool needResultFile = !resultName.empty ();
	if (needResultFile) {
		if (!CreateDiffFile (bitmap1, bitmap2, rect, resultName)) {
			return InternalError;
		}
	}

	if (diffCount != NULL) {
		*diffCount = diffPixels;
	}

	return (diffPixels == 0 ? Equal : NotEqualContent);
}

int ImageCompare::CalculateDiff (Bitmap& bitmap1, Bitmap& bitmap2, Rect& rect)
{
	int diffPixels = 0;

	BitmapData bitmapData1;
	BitmapData bitmapData2;

	bitmap1.LockBits (&rect, ImageLockModeRead, PixelFormat32bppARGB, &bitmapData1);
	bitmap2.LockBits (&rect, ImageLockModeRead, PixelFormat32bppARGB, &bitmapData2);

	int *rawBitmap1 = (int*) bitmapData1.Scan0;
	int *rawBitmap2 = (int*) bitmapData2.Scan0;
	for (int i = rect.GetLeft (); i < rect.GetRight (); i++) {
		for (int j = rect.GetTop (); j < rect.GetBottom (); j++) {
			int color1 = rawBitmap1[j * bitmapData1.Stride / 4 + i];
			int color2 = rawBitmap2[j * bitmapData2.Stride / 4 + i];
			if (color1 != color2) {
				diffPixels++;
			}
		}
	}

	bitmap1.UnlockBits (&bitmapData1);
	bitmap2.UnlockBits (&bitmapData2);

	return diffPixels;
}

bool ImageCompare::CreateDiffFile (Bitmap& bitmap1, Bitmap& bitmap2, Rect& rect, const std::wstring& fileName)
{
	Bitmap result (rect.GetRight () - rect.GetLeft (), rect.GetBottom () - rect.GetTop ());

	BitmapData bitmapData1;
	BitmapData bitmapData2;
	BitmapData resultData;

	bitmap1.LockBits (&rect, ImageLockModeRead, PixelFormat32bppARGB, &bitmapData1);
	bitmap2.LockBits (&rect, ImageLockModeRead, PixelFormat32bppARGB, &bitmapData2);
	result.LockBits (&rect, ImageLockModeWrite, PixelFormat32bppARGB, &resultData);

	int *rawBitmap1 = (int*) bitmapData1.Scan0;
	int *rawBitmap2 = (int*) bitmapData2.Scan0;
	int *rawResult = (int*) resultData.Scan0;

	for (int i = rect.GetLeft (); i < rect.GetRight (); i++) {
		for (int j = rect.GetTop (); j < rect.GetBottom (); j++) {
			int color1 = rawBitmap1[j * bitmapData1.Stride / 4 + i];
			int color2 = rawBitmap2[j * bitmapData2.Stride / 4 + i];
			if (color1 != color2) {
				rawResult[j * bitmapData1.Stride / 4 + i] = 0xff000000;
			} else {
				rawResult[j * bitmapData1.Stride / 4 + i] = 0xffffffff;
			}
		}
	}

	bitmap1.UnlockBits (&bitmapData1);
	bitmap2.UnlockBits (&bitmapData2);
	result.UnlockBits (&resultData);

	CLSID classID;
	if (!GetClassIdByName (fileName, &classID)) {
		return false;
	}

	Status status= result.Save (fileName.c_str (), &classID, NULL);
	if (status != Ok) {
		return false;
	}

	return true;
}

bool ImageCompare::InitGDIPlus ()
{
	GdiplusStartupInput		input;
	GdiplusStartupOutput	output;
	Status					status;

	status = GdiplusStartup (&gdiPlusToken, &input, &output);
	if (status != Ok) {
		return false;
	}

	return true;
}

void ImageCompare::ShutGDIPlus ()
{
	GdiplusShutdown (gdiPlusToken);
}

bool ImageCompare::GetClassIdByName (const std::wstring& fileName, CLSID* classId)
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

bool ImageCompare::GetEncoderClassId (const wchar_t* format, CLSID* classId)
{
	UINT				count;
	UINT				size;
	ImageCodecInfo*		imageCodecInfo;

	count = 0;
	size = 0;
	imageCodecInfo = NULL;

	GetImageEncodersSize (&count, &size);
	if(size == 0) {
		return false;
	}

	imageCodecInfo = (ImageCodecInfo*) (malloc(size));
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
