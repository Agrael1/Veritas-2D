//#include "../VEngine/include/VeritasEngine.h"
#include "MediaEngine.h"

int main()
{
	MediaEnginePlayer mp;
	mp.Initialize();
	mp.SetSource(L"C:/users/aa/desktop/BadApple.mp4");
	mp.Play();

	uint32_t x, y;

	mp.GetNativeVideoSize(x, y);



	while (!mp.IsFinished())
	{
		auto x = mp.GetFrame();
		if (x.RowPitch)
			;
	}
	return 0;
}
