#include <iostream>
#include <windows.h>
#include <string>
#include <memory>
#include <thread>
#include <Unity/IUnityInterface.h>
#include "Wintab.h"
#include "Common.h"
#include "Tablet.h"
#include "Debug.h"



std::unique_ptr<Tablet> g_tablet;
HINSTANCE g_hInst = nullptr;
HWND g_hWnd = nullptr;
std::thread g_thread;


int WINAPI DllMain(HINSTANCE hInst, DWORD fdReason, PVOID pvReserved)
{
    switch (fdReason)
    {
        case DLL_PROCESS_ATTACH:
        {
            g_hInst = hInst;
            break;
        }
        case DLL_PROCESS_DETACH:
        {
            g_hInst = nullptr;
            break;
        }
        default:
        {
            break;
        }
    }
    return TRUE;
}


LRESULT CALLBACK UwtWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
        case WM_CREATE:
        {
            g_tablet = std::make_unique<Tablet>();
            g_tablet->Open(hWnd);
            break;
        }
        case WM_CLOSE:
        {
            ::DestroyWindow(hWnd);
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

    return ::DefWindowProc(hWnd, msg, wp, lp);
}


void UwtThreadFunc()
{
    WNDCLASS wc;
    wc.style = 0;
    wc.lpfnWndProc = UwtWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = g_hInst;
    wc.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)0;
    wc.lpszMenuName = "uWintab";
    wc.lpszClassName = kLibraryName;

    if (::RegisterClass(&wc))
    {
        g_hWnd = ::CreateWindowEx(
            WS_EX_TOOLWINDOW,
            TEXT(kLibraryName),
            TEXT(kLibraryName),
            WS_POPUP & ~WS_VISIBLE,
            0,
            0,
            0,
            0,
            HWND_DESKTOP,
            NULL,
            g_hInst,
            NULL);

        if (g_hWnd)
        {
            ::ShowWindow(g_hWnd, SW_SHOWNOACTIVATE);
            ::SetWindowPos(g_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
        }
    }

    MSG msg;
    while (::GetMessage(&msg, NULL, 0, 0))
    {
        ::DispatchMessage(&msg);

        if (g_tablet)
        {
            g_tablet->Enable(TRUE);
            g_tablet->Update();
        }

        ::InvalidateRect(g_hWnd, NULL, TRUE);
        ::UpdateWindow(g_hWnd);
    }
    g_hWnd = nullptr;

    ::UnregisterClass(TEXT(kLibraryName), g_hInst);
}


void UwtCreateWindow()
{
    g_thread = std::thread([]
    {
        UwtThreadFunc();
    });
}


void UwtDestroyWindow()
{
    ::SendMessage(g_hWnd, WM_CLOSE, 0, 0);
    if (g_thread.joinable())
    {
        g_thread.join();
    }
}


extern "C"
{
    UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API UwtInitialize()
    {
        if (g_tablet) return;

        Debug::Initialize();
        Wintab::Load();
        UwtCreateWindow();
    }


    UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API UwtFinalize()
    {
        if (!g_tablet) return;

        UwtDestroyWindow();
        Wintab::Unload();
        Debug::Finalize();
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