#pragma once
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct VideoSlice
    {
        uint32_t* pData;
        uint32_t RowPitch;
        uint32_t DepthPitch;
    }VideoSlice;

    void* MPInitialize();
    void  MPRelease(void* player);
    void  MPGetVideoSize(void* player, uint32_t* x, uint32_t* y);
    bool  MPIsFinished(void* player);
    bool  MPIsInfoReady(void* player);
    void  MPFrameFinished(void* player);

    VideoSlice MPGetFrame(void* player);
    void MPSetVideoResource(void* player, const wchar_t* name);
    void MPPlay(void* player);

#ifdef __cplusplus
}
#endif