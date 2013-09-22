#ifndef IMAGECOMPARE_HPP
#define IMAGECOMPARE_HPP

#include <windows.h>
#include <gdiplus.h>
#include <string>

using namespace Gdiplus;

class ImageCompare
{
public:
	class Parameters
	{
	public:
		Parameters ();
		~Parameters ();

		void					SetImageName1 (const std::wstring& imageName);
		void					SetImageName2 (const std::wstring& imageName);
		void					SetResultName (const std::wstring& imageName);

		const std::wstring&		GetImageName1 () const;
		const std::wstring&		GetImageName2 () const;
		const std::wstring&		GetResultName () const;

	private:
		std::wstring imageName1;
		std::wstring imageName2;
		std::wstring resultName;
	};

	enum Result
	{
		InternalError,
		InvalidFile,
		NotEqualSize,
		NotEqualContent,
		Equal
	};

	ImageCompare ();
	~ImageCompare ();

	Result		Compare (const Parameters& parameters, int* diffCount);

private:
	int			CalculateDiff (Bitmap& bitmap1, Bitmap& bitmap2, Rect& rect);
	bool		CreateDiffFile (Bitmap& bitmap1, Bitmap& bitmap2, Rect& rect, const std::wstring& fileName);

	bool		InitGDIPlus ();
	void		ShutGDIPlus ();
	bool		GetClassIdByName (const std::wstring& fileName, CLSID* classId);
	bool		GetEncoderClassId (const wchar_t* format, CLSID* classId);

	ULONG_PTR	gdiPlusToken;
};

#endif
