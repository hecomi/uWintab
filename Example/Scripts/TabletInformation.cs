using UnityEngine;
using UnityEngine.UI;

public class TabletInformation : MonoBehaviour
{
    [SerializeField]
    uWintab.Tablet uwtTablet;

    [SerializeField]
    Text uiText;

    void Update()
    {
        if (!uwtTablet || !uiText) return;

        string str = "";
        str += "name: " + uwtTablet.deviceName + "\n";
        str += "version: " + uwtTablet.version + "\n";
        str += "isPressureSupported: " + uwtTablet.isPressureSupported + "\n";
        str += "isWheelSupported: " + uwtTablet.isWheelSupported + "\n";
        str += "isOrientationSupported: " + uwtTablet.isOrientationSupported + "\n";
        str += "isExpKeysSupported: " + uwtTablet.isExpKeysSupported + "\n";
        str += "deviceNum: " + uwtTablet.deviceNum + "\n";
        str += "expKeyNum: " + uwtTablet.expKeyNum + "\n";
        str += "x: " + uwtTablet.x + "\n";
        str += "y: " + uwtTablet.y + "\n";
        str += "pressure: " + uwtTablet.pressure + "\n";
        str += "wheel: " + uwtTablet.wheel + "\n";
        str += "azimuth: " + uwtTablet.azimuth + "\n";
        str += "altitude: " + uwtTablet.altitude + "\n";
        str += "twist: " + uwtTablet.twist + "\n";
        str += "penId: " + uwtTablet.penId + "\n";
        str += "cursor: " + uwtTablet.cursor + "\n";
        str += "time: " + uwtTablet.time + "\n";
        str += "proximity: " + uwtTablet.proximity + "\n";
        str += "button 1: " + uwtTablet.GetButton(1) + "\n";
        str += "button 2: " + uwtTablet.GetButton(2) + "\n";
        for (int i = 0; i < uwtTablet.expKeyNum; ++i)
        {
            str += "expkey " + i + ": " + uwtTablet.GetExpKey(i) + "\n";
        }

        uiText.text = str;
    }
}