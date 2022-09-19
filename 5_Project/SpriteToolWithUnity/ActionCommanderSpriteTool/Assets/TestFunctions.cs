using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TestFunctions : MonoBehaviour
{
    public void TestFunction()
    {
    }

    public void TestFunctionWithArgs(int _int1)
    {
        Debug.Log($"{_int1}");
    }
}