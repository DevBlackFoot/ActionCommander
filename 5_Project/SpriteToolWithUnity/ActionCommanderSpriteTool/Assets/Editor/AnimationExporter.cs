using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using UnityEditor;
using UnityEditor.Experimental;
using UnityEditor.VersionControl;
using UnityEngine;
using FileMode = System.IO.FileMode;

public class AnimationExporter : Editor
{
    public static string GetAssetFilePathByGUID(string _guid)
    {
        AssetDatabase.GUIDToAssetPath(_guid);
        return string.Empty;
    }

    [MenuItem("Assets/Parse")]
    public static void Parse()
    {
        var _clip = Selection.activeObject as AnimationClip;
        ParsedAnimationClip _parsed = ParsedAnimationClip.Parse(_clip);
        ActionCommanderAnimationClip _outClip = ActionCommanderAnimationClip.Parse(_parsed);
        string _outClipJson = JsonUtility.ToJson(_outClip, true);

        Debug.Log(_outClipJson);

        // 대상 디렉토리를 지정합니다.
        // 디렉토리가 없다면 생성합니다.
        string _directory = $"{Application.dataPath}/../_exported/Animation";
        if (Directory.Exists(_directory) == false)
            Directory.CreateDirectory(_directory);

        // 대상 경로에 파일을 기록합니다.
        using (StreamWriter _fs = new StreamWriter(
            $"{_directory}/{_outClip.name}.json",
            false,
            Encoding.UTF8,
            _outClipJson.Length
        ))
        {
            _fs.Write(_outClipJson);
            _fs.Close();
        }
    }

    // Note that we pass the same path, and also pass "true" to the second argument.
    [MenuItem("Assets/Parse", true)]
    private static bool testOptionValidation()
    {
        // This returns true when the selected object is a Variable (the menu item will be disabled otherwise).
        return Selection.activeObject is AnimationClip;
    }
}