#include <iostream>
#include <windows.h>
#include <cassert>
#include <string>
#include <memory>
#include "Wintab.h"
#include "Common.h"
#include "Tablet.h"

#define UWT_TEST_APP



namespace 
{
    std::unique_ptr<Tablet> g_tablet;
}


#ifndef UWT_TEST_APP





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