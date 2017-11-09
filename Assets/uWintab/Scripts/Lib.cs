using System.Text;
using System.Runtime.InteropServices;

#pragma warning disable 114, 465

namespace uWintab
{

public enum CursorType
{
    Invalid = -1,
    Cursor = 0,
    Pen = 1,
    TailSwitch = 2,
}

public enum DebugMode
{
    None = 0,
    File = 1,
    UnityLog = 2,
}

public static class Lib
{
    public const string name = "uWintab";

    [DllImport(name, EntryPoint = "UwtIsAvailable")]
    public static extern bool IsAvailable();
    [DllImport(name, EntryPoint = "UwtInitialize")]
    public static extern void Initialize();
    [DllImport(name, EntryPoint = "UwtFinalize")]
    public static extern void Finalize();
    [DllImport(name, EntryPoint = "UwtGetDeviceName")]
    private static extern void GetDeviceName(StringBuilder buf, int len);
    [DllImport(name, EntryPoint = "UwtIsPressureSupported")]
    public static extern bool IsPressureSupported();
    [DllImport(name, EntryPoint = "UwtIsWheelSupported")]
    public static extern bool IsWheelSupported();
    [DllImport(name, EntryPoint = "UwtIsOrientationSupported")]
    public static extern bool IsOrientationSupported();
    [DllImport(name, EntryPoint = "UwtIsExpKeysSupported")]
    public static extern bool IsExpKeysSupported();
    [DllImport(name, EntryPoint = "UwtGetVersion")]
    private static extern void GetVersion(StringBuilder buf, int len);
    [DllImport(name, EntryPoint = "UwtGetDeviceNum")]
    public static extern int GetDeviceNum();
    [DllImport(name, EntryPoint = "UwtGetExpKeyNum")]
    public static extern int GetExpKeyNum(int tabletId);
    [DllImport(name, EntryPoint = "UwtGetX")]
    public static extern float GetX();
    [DllImport(name, EntryPoint = "UwtGetY")]
    public static extern float GetY();
    [DllImport(name, EntryPoint = "UwtGetPressure")]
    public static extern float GetPressure();
    [DllImport(name, EntryPoint = "UwtGetWheel")]
    public static extern float GetWheel();
    [DllImport(name, EntryPoint = "UwtGetAzimuth")]
    public static extern float GetAzimuth();
    [DllImport(name, EntryPoint = "UwtGetAltitude")]
    public static extern float GetAltitude();
    [DllImport(name, EntryPoint = "UwtGetTwist")]
    public static extern float GetTwist();
    [DllImport(name, EntryPoint = "UwtGetPenId")]
    public static extern int GetPenId();
    [DllImport(name, EntryPoint = "UwtGetCursor")]
    public static extern CursorType GetCursor();
    [DllImport(name, EntryPoint = "UwtGetTime")]
    public static extern int GetTime();
    [DllImport(name, EntryPoint = "UwtGetProximity")]
    public static extern bool GetProximity();
    [DllImport(name, EntryPoint = "UwtGetButton")]
    public static extern bool GetButton(int id);
    [DllImport(name, EntryPoint = "UwtGetButtonDown")]
    public static extern bool GetButtonDown(int id);
    [DllImport(name, EntryPoint = "UwtGetButtonUp")]
    public static extern bool GetButtonUp(int id);
    [DllImport(name, EntryPoint = "UwtGetExpKey")]
    public static extern bool GetExpKey(int tabletId, int controlId);
    [DllImport(name, EntryPoint = "UwtGetExpKeyDown")]
    public static extern bool GetExpKeyDown(int tabletId, int controlId);
    [DllImport(name, EntryPoint = "UwtGetExpKeyUp")]
    public static extern bool GetExpKeyUp(int tabletId, int controlId);

    public static string GetDeviceName()
    {
        var buf = new StringBuilder(64);
        GetDeviceName(buf, buf.Capacity);
        return buf.ToString();
    }

    public static string GetVersion()
    {
        var buf = new StringBuilder(16);
        GetVersion(buf, buf.Capacity);
        return buf.ToString();
    }
}

}