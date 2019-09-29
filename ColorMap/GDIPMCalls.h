#pragma once 

#ifdef __cplusplus
extern "C"
{
#endif 

	typedef struct _BitmapData
	{
		unsigned Width;
		unsigned Height;
		int Stride;
		int PixelFormat;
		void* Scan0;
		UINT_PTR Reserved;
	}BitmapData;

	DECLARE_HANDLE(HGDIPM);

	HGDIPM GDIPMStart();
	void GDIPMStop(HGDIPM handle);

	BitmapData GetImageFromFileA(const char* filename);
	BitmapData GetImageFromFileW(const wchar_t* filename);

#ifdef __cplusplus
}
#endif 