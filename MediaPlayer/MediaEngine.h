#pragma once
#include <cstdint>
#include <string_view>

#include <Windows.h>
#include <d3d11_4.h>
#include <mfapi.h>
#include <mfmediaengine.h>
#include <winrt/base.h>

#include "MPAPI.h"

class IMFNotify
{
public:
    virtual ~IMFNotify() = default;

    IMFNotify(const IMFNotify&) = delete;
    IMFNotify& operator=(const IMFNotify&) = delete;

    IMFNotify(IMFNotify&&) = delete;
    IMFNotify& operator=(IMFNotify&&) = delete;
public:
    virtual void OnMediaEngineEvent(uint32_t meEvent) = 0;
protected:
    IMFNotify() = default;
};

class MediaEnginePlayer : public IMFNotify
{
public:
    MediaEnginePlayer() = default;
    ~MediaEnginePlayer()
    {
        Shutdown();
        MFShutdown();
    }

    MediaEnginePlayer(const MediaEnginePlayer&) = delete;
    MediaEnginePlayer& operator=(const MediaEnginePlayer&) = delete;

    MediaEnginePlayer(MediaEnginePlayer&&) = default;
    MediaEnginePlayer& operator=(MediaEnginePlayer&&) = default;
public:
    void Initialize();
    void Shutdown();

    void Play();
    void SetMuted(bool muted);

    void SetSource(std::wstring_view sourceUri);

    bool TransferFrame(RECT rcTarget);

    // Callbacks
    void OnMediaEngineEvent(uint32_t meEvent) override;
    VideoSlice GetFrame();
    void FrameFinished();

    // Properties
    void GetNativeVideoSize(uint32_t& cx, uint32_t& cy);
    bool IsPlaying() const { return m_isPlaying; }
    bool IsInfoReady() const { return m_isInfoReady; }
    bool IsFinished() const { return m_isFinished; }
private:
    winrt::com_ptr<ID3D11Texture2D>     m_texture;
    winrt::com_ptr<ID3D11Texture2D>     Show;
    winrt::com_ptr<ID3D11Device3>        m_device;
    winrt::com_ptr<ID3D11DeviceContext1> m_context;
    winrt::com_ptr<IMFMediaEngine>      m_mediaEngine;
    std::unique_ptr<uint32_t[]> buffer;

    MFARGB  m_bkgColor{};
    uint32_t width = 0;
    uint32_t height = 0;


    bool m_isPlaying = false;
    bool m_isInfoReady = false;
    bool m_isFinished = false;

    D3D11_MAPPED_SUBRESOURCE mappedResource;
};

