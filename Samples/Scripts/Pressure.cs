using UnityEngine;

namespace uWintab
{

public class Pressure : MonoBehaviour 
{
    [SerializeField]
    Transform pen;

    Tablet tablet_;

    void Start()
    {
        tablet_ = FindObjectOfType<Tablet>();
    }

    void LateUpdate()
    {
        if (tablet_.deviceNum == 0) return;

        transform.localPosition = pen.localPosition;

        var pressure = tablet_.pressure;
        transform.localScale = new Vector3(pressure, 1f, pressure);
    }
}

}