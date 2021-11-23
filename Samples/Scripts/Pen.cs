using UnityEngine;

namespace uWintab
{

public class Pen : MonoBehaviour 
{
    [SerializeField]
    Vector2 localScale = new Vector2(1.92f, 1.08f);

    [SerializeField]
    float hoverHeight = 0.1f;

    Tablet tablet_;
    float y_ = 0f;

    void Start()
    {
        tablet_ = FindObjectOfType<Tablet>();
    }

    void Update()
    {
        if (!tablet_.isAvailable) return;

        UpdatePosition();
        UpdateRotation();
    }

    void UpdatePosition()
    {
        var x = (tablet_.x - 0.5f) * localScale.x;
        var z = (tablet_.y - 0.5f) * localScale.y;
        y_ = Mathf.Lerp(y_, (tablet_.proximity ? 0f : hoverHeight), 0.2f);
        transform.localPosition = new Vector3(x, y_, z);
    }

    void UpdateRotation()
    {
        var yaw = tablet_.azimuth * 360f;
        var pitch = (0.5f - tablet_.altitude) * 180f;
        var rot = Quaternion.Euler(pitch, yaw, 0f);
        transform.localRotation = Quaternion.FromToRotation(Vector3.up, rot * Vector3.up);
    }
}

}