using UnityEngine;

namespace uWintab
{

public class UwcTablet : MonoBehaviour 
{
    void OnEnable()
    {
        Lib.Initialize();
    }

    void OnDisable()
    {
        Lib.Finalize();
    }

    public string deviceName
    {
        get { return Lib.GetDeviceName(); }
    }

    public string version
    {
        get { return Lib.GetVersion(); }
    }

    public bool isPressureSupported
    {
        get { return Lib.IsPressureSupported(); }
    }

    public bool isWheelSupported
    {
        get { return Lib.IsWheelSupported(); }
    }

    public bool isOrientationSupported
    {
        get { return Lib.IsOrientationSupported(); }
    }

    public bool isExpKeysSupported
    {
        get { return Lib.IsExpKeysSupported(); }
    }

    public int deviceNum
    {
        get { return Lib.GetDeviceNum(); }
    }

    public int expKeyNum
    {
        get { return Lib.GetExpKeyNum(0); }
    }

    public float x
    {
        get { return Lib.GetX(); }
    }

    public float y
    {
        get { return Lib.GetY(); }
    }

    public float pressure
    {
        get { return Lib.GetPressure(); }
    }

    public float wheel
    {
        get { return Lib.GetWheel(); }
    }

    public float azimuth
    {
        get { return Lib.GetAzimuth(); }
    }

    public float altitude
    {
        get { return Lib.GetAltitude(); }
    }

    public float twist
    {
        get { return Lib.GetTwist(); }
    }

    public int penId
    {
        get { return Lib.GetPenId(); }
    }

    public CursorType cursor
    {
        get { return Lib.GetCursor(); }
    }

    public int time
    {
        get { return Lib.GetTime(); }
    }

    public bool proximity
    {
        get { return Lib.GetProximity(); }
    }

    public int GetExpKeyNum(int tabletId)
    {
        return Lib.GetExpKeyNum(tabletId);
    }

    public bool GetButton(int id)
    {
        return Lib.GetButton(id);
    }

    public bool GetButtonDown(int id)
    {
        return Lib.GetButtonDown(id);
    }

    public bool GetButtonUp(int id)
    {
        return Lib.GetButtonUp(id);
    }

    public bool GetExpKey(int tabletId, int id)
    {
        return Lib.GetExpKey(tabletId, id);
    }

    public bool GetExpKey(int id)
    {
        return Lib.GetExpKey(0, id);
    }

    public bool GetExpKeyDown(int tabletId, int id)
    {
        return Lib.GetExpKeyDown(tabletId, id);
    }

    public bool GetExpKeyDown(int id)
    {
        return Lib.GetExpKeyDown(0, id);
    }

    public bool GetExpKeyUp(int tabletId, int id)
    {
        return Lib.GetExpKeyUp(tabletId, id);
    }

    public bool GetExpKeyUp(int id)
    {
        return Lib.GetExpKeyUp(0, id);
    }
}

}