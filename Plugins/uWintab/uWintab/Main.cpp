#include <iostream>
#include <windows.h>
#include <cassert>
#include <string>
#include <memory>
#include <Unity/IUnityInterface.h>
#include "Wintab.h"
#include "Common.h"
#include "Tablet.h"
#include "Debug.h"

// #define UWT_TEST_APP



namespace 
{
    std::unique_ptr<Tablet> g_tablet;
    WNDPROC g_unityWndProc;


    LRESULT CALLBACK UwcWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) 
    {
        switch (msg)
        {
            case WM_ACTIVATE:
            {
                const auto state = GET_WM_ACTIVATE_STATE(wp, lp);
                g_tablet->Enable(state);
                if (state) 
                {
                    g_tablet->Overwrap();
                }
                break;
            }
            case WT_PACKET:
            {
                g_tablet->ReceivePacket(lp, wp);
                return 0;
            }
            case WT_PACKETEXT:
            {
                g_tablet->ReceivePacketExt(lp, wp);
                return 0;
            }
            case WT_PROXIMITY:
            {
                g_tablet->ReceiveProximity(lp, wp);
                break;
            }
        }

        return ::CallWindowProc(g_unityWndProc, hwnd, msg, wp, lp);
    }
}


#ifndef UWT_TEST_APP


extern "C"
{
    UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API UwtInitialize(HWND hWnd)
    {
        Debug::Initialize();
        Wintab::Load();

        g_tablet = std::make_unique<Tablet>();
        g_tablet->Open(hWnd);
    }


    UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API UwtFinalize()
    {
        if (g_tablet)
        {
            g_tablet->Close();
            g_tablet.reset();
        }

        Wintab::Unload();
        Debug::Finalize();
    }


    UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API UwtSetUnityWndProc(WNDPROC wndProc)
    {
        g_unityWndProc = wndProc;
    }


    UNITY_INTERFACE_EXPORT WNDPROC UNITY_INTERFACE_API UwtGetWndProc()
    {
        return (WNDPROC)UwcWndProc;
    }


    UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API UwtUpdate()
    {
        if (!g_tablet) return;
        g_tablet->Update();
    }


    UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API UwtGetDeviceName(char* buf, int len)
    {
        if (!g_tablet) return;
        strcpy_s(buf, len, g_tablet->GetDeviceName().c_str());
    }

    UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API UwtIsPressureSupported()
    {
        if (!g_tablet) return false;
        return g_tablet->IsPressureSupported();
    }


    UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API UwtIsWheelSupported()
    {
        if (!g_tablet) return false;
        return g_tablet->IsWheelSupported();
    }


    UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API UwtIsOrientationSupported()
    {
        if (!g_tablet) return false;
        return g_tablet->IsOrientationSupported();
    }


    UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API UwtIsExpKeysSupported()
    {
        if (!g_tablet) return false;
        return g_tablet->IsExpKeysSupported();
    }


    UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API UwtGetVersion(char* buf, int len)
    {
        if (!g_tablet) return;
        strcpy_s(buf, len, g_tablet->GetVersion().c_str());
    }


    UNITY_INTERFACE_EXPORT int UNITY_INTERFACE_API UwtGetDeviceNum()
    {
        if (!g_tablet) return false;
        return g_tablet->GetDeviceNum();
    }


    UNITY_INTERFACE_EXPORT int UNITY_INTERFACE_API UwtGetExpKeyNum(int tabletId)
    {
        if (!g_tablet) return false;
        return static_cast<int>(g_tablet->GetExpKeyNum(tabletId));
    }


    UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API UwtGetX()
    {
        if (!g_tablet) return 0.f;
        return g_tablet->GetX();
    }


    UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API UwtGetY()
    {
        if (!g_tablet) return 0.f;
        return g_tablet->GetY();
    }


    UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API UwtGetPressure()
    {
        if (!g_tablet) return 0.f;
        return g_tablet->GetPressure();
    }


    UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API UwtGetWheel()
    {
        if (!g_tablet) return 0.f;
        return g_tablet->GetWheel();
    }


    UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API UwtGetAzimuth()
    {
        if (!g_tablet) return 0.f;
        return g_tablet->GetAzimuth();
    }


    UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API UwtGetAltitude()
    {
        if (!g_tablet) return 0.f;
        return g_tablet->GetAltitude();
    }


    UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API UwtGetTwist()
    {
        if (!g_tablet) return 0.f;
        return g_tablet->GetTwist();
    }


    UNITY_INTERFACE_EXPORT int UNITY_INTERFACE_API UwtGetPenId()
    {
        if (!g_tablet) return -1;
        return g_tablet->GetPenId();
    }


    UNITY_INTERFACE_EXPORT Tablet::CursorType UNITY_INTERFACE_API UwtGetCursor()
    {
        if (!g_tablet) return Tablet::CursorType::Invalid;
        return g_tablet->GetCursor();
    }


    UNITY_INTERFACE_EXPORT int UNITY_INTERFACE_API UwtGetTime()
    {
        if (!g_tablet) return -1;
        return g_tablet->GetTime();
    }


    UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API UwtGetProximity()
    {
        if (!g_tablet) return false;
        return g_tablet->GetProximity();
    }


    UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API UwtGetButton(int id)
    {
        if (!g_tablet) return false;
        return g_tablet->GetButton(id);
    }


    UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API UwtGetButtonDown(int id)
    {
        if (!g_tablet) return false;
        return g_tablet->GetButtonDown(id);
    }


    UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API UwtGetButtonUp(int id)
    {
        if (!g_tablet) return false;
        return g_tablet->GetButtonUp(id);
    }


    UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API UwtGetExpKey(int tabletId, int controlId)
    {
        if (!g_tablet) return false;
        return g_tablet->GetExpKey(tabletId, controlId);
    }


    UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API UwtGetExpKeyDown(int tabletId, int controlId)
    {
        if (!g_tablet) return false;
        return g_tablet->GetExpKeyDown(tabletId, controlId);
    }


    UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API UwtGetExpKeyUp(int tabletId, int controlId)
    {
        if (!g_tablet) return false;
        return g_tablet->GetExpKeyUp(tabletId, controlId);
    }
}



#else


namespace 
{
    HINSTANCE g_instance = nullptr;
}


LRESULT CALLBACK WndProc(
    HWND hwnd, 
    UINT msg, 
    WPARAM wp, 
    LPARAM lp) 
{
    switch (msg)
    {
        case WM_CREATE:
        {
            g_tablet = std::make_unique<Tablet>();
            g_tablet->Open(hwnd);
            break;
        }
        case WM_DESTROY:
        {
            g_tablet->Close();
            g_tablet.reset();
            ::PostQuitMessage(0);
            return 0;
        }
        case WM_ACTIVATE:
        {
            const auto state = GET_WM_ACTIVATE_STATE(wp, lp);
            g_tablet->Enable(state);
            if (state) 
            {
                g_tablet->Overwrap();
            }
            break;
        }
        case WM_PAINT:
        {
            std::string text;
            text += "version: " + g_tablet->GetVersion() + "\n";
            text += "x: " + std::to_string(g_tablet->GetX()) + "\n";
            text += "y: " + std::to_string(g_tablet->GetY()) + "\n";
            text += "azimuth: " + std::to_string(g_tablet->GetAzimuth()) + "\n";
            text += "altitude: " + std::to_string(g_tablet->GetAltitude()) + "\n";
            text += "twist: " + std::to_string(g_tablet->GetTwist()) + "\n";
            text += "pressure: " + std::to_string(g_tablet->GetPressure()) + "\n";
            text += "proximity: " + std::to_string(g_tablet->GetProximity()) + "\n";
            text += "time: " + std::to_string(g_tablet->GetTimestamp()) + "\n";
            text += "pen id: " + std::to_string(g_tablet->GetPenId()) + "\n";
            text += "cursor: " + std::to_string((UINT)g_tablet->GetCursor()) + "\n";
            text += "pen button 1: " + std::to_string(g_tablet->GetButton(1)) + "\n";
            text += "pen button 2: " + std::to_string(g_tablet->GetButton(2)) + "\n";
            text += "func key 0: " + std::to_string(g_tablet->GetExpKey(0)) + "\n";
            text += "func key 1: " + std::to_string(g_tablet->GetExpKey(1)) + "\n";
            text += "func key 2: " + std::to_string(g_tablet->GetExpKey(2)) + "\n";
            text += "func key 3: " + std::to_string(g_tablet->GetExpKey(3)) + "\n";
            text += "func key 4: " + std::to_string(g_tablet->GetExpKey(4)) + "\n";
            text += "func key 5: " + std::to_string(g_tablet->GetExpKey(5)) + "\n";

            PAINTSTRUCT ps;
            auto hdc = ::BeginPaint(hwnd, &ps);
            {
                RECT rect;
                ::GetClientRect(hwnd, &rect);
                ::DrawText(hdc, text.c_str(), static_cast<int>(text.size()), &rect, DT_WORDBREAK);
            }
            ::EndPaint(hwnd, &ps);

            break;
        }
        case WT_PACKET:
        {
            g_tablet->ReceivePacket(lp, wp);
            return 0;
        }
        case WT_PACKETEXT:
        {
            g_tablet->ReceivePacketExt(lp, wp);
            return 0;
        }
        case WT_PROXIMITY:
        {
            g_tablet->ReceiveProximity(lp, wp);
            break;
        }
    }

	return ::DefWindowProc(hwnd, msg, wp, lp);
}


BOOL InitApp(HINSTANCE hInstance)
{
	WNDCLASS wc;
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE + 1);
    wc.lpszMenuName =  "uWintabTest";
    wc.lpszClassName = kLibraryName;

    return RegisterClass(&wc);
}


int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    PSTR lpCmdLine,
    int nCmdShow) 
{
    g_instance = hInstance;

    if (!InitApp(hInstance)) return FALSE;

    if (!Wintab::Load()) 
    {
        Wintab::Unload();
        return FALSE;
    }

	if (!Tablet::IsAvailable()) 
    {
		MessageBox(NULL, "WinTab Services Not Available.", kLibraryName, MB_OK | MB_ICONHAND);
		return FALSE;
	}

	auto hwnd = ::CreateWindow(
        TEXT(kLibraryName),
        TEXT("Kitty on your lap"),
        WS_OVERLAPPEDWINDOW,
        100,
        100,
        640,
        480,
        NULL,
        NULL,
        hInstance,
        NULL);

	if (hwnd == NULL) return FALSE;

    ::ShowWindow(hwnd, nCmdShow);

    MSG msg;
    while (::GetMessage(&msg, NULL, 0, 0))
    {
        ::TranslateMessage(&msg);
		::DispatchMessage(&msg);

        if (g_tablet)
        {
            g_tablet->Update();
        }

        ::InvalidateRect(hwnd, NULL, TRUE);
        ::UpdateWindow(hwnd);
	}

    Wintab::Unload();

	return static_cast<int>(msg.wParam);
}


#endif