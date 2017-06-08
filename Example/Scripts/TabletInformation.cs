using UnityEngine;
using UnityEngine.UI;

namespace uWintab
{

public class TabletInformation : MonoBehaviour
{
    Tablet tablet_;
    Text uiText_;

    void Start()
    {
        tablet_ = FindObjectOfType<Tablet>();
        uiText_ = GetComponent<Text>();
    }

    void Update()
    {
        if (!tablet_ || !uiText_) return;

        string str = "";
        str += "name: " + tablet_.deviceName + "\n";
        str += "version: " + tablet_.version + "\n";
        str += "isPressureSupported: " + tablet_.isPressureSupported + "\n";
        str += "isWheelSupported: " + tablet_.isWheelSupported + "\n";
        str += "isOrientationSupported: " + tablet_.isOrientationSupported + "\n";
        str += "isExpKeysSupported: " + tablet_.isExpKeysSupported + "\n";
        str += "deviceNum: " + tablet_.deviceNum + "\n";
        str += "expKeyNum: " + tablet_.expKeyNum + "\n";
        str += "x: " + tablet_.x + "\n";
        str += "y: " + tablet_.y + "\n";
        str += "pressure: " + tablet_.pressure + "\n";
        str += "wheel: " + tablet_.wheel + "\n";
        str += "azimuth: " + tablet_.azimuth + "\n";
        str += "altitude: " + tablet_.altitude + "\n";
        str += "twist: " + tablet_.twist + "\n";
        str += "penId: " + tablet_.penId + "\n";
        str += "cursor: " + tablet_.cursor + "\n";
        str += "time: " + tablet_.time + "\n";
        str += "proximity: " + tablet_.proximity + "\n";
        str += "button 1: " + tablet_.GetButton(1) + "\n";
        str += "button 2: " + tablet_.GetButton(2) + "\n";
        for (int i = 0; i < tablet_.expKeyNum; ++i)
        {
            str += "expkey " + i + ": " + tablet_.GetExpKey(i) + "\n";
        }

        uiText_.text = str;
    }
}

}