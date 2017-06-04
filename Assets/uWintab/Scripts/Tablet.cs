using System;
using System.Text;
using UnityEngine;

namespace uWintab
{

public class Tablet : MonoBehaviour 
{
    IntPtr defaultUnityWndProc = IntPtr.Zero;

    void OnEnable()
    {
        var hWnd = Win32API.GetActiveWindow();

        defaultUnityWndProc = Win32API.GetWindowLongPtr(hWnd, Win32API.GWLP_WNDPROC);
        Lib.SetUnityWndProc(defaultUnityWndProc);
        Win32API.SetWindowLongPtr(hWnd, Win32API.GWLP_WNDPROC, Lib.GetWndProc());

        Lib.Initialize(hWnd);
    }

    void OnDisable()
    {
        Lib.Finalize();

        var hWnd = Win32API.GetForegroundWindow();
        Win32API.SetWindowLongPtr(hWnd, Win32API.GWLP_WNDPROC, defaultUnityWndProc);
    }

    void Update()
    {
        Lib.Update();

        Debug.Log(Lib.GetX() + " " + Lib.GetY());
        Debug.Log(Lib.GetAzimuth());
    }
}

}