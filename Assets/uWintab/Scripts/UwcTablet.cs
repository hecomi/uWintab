using System;
using UnityEngine;

namespace uWintab
{

public class UwcTablet : MonoBehaviour 
{
    void Update()
    {
        Debug.Log(Lib.GetAzimuth());
    }

    void OnEnable()
    {
        Lib.Initialize();
    }

    void OnDisable()
    {
        Lib.Finalize();
    }
}

}