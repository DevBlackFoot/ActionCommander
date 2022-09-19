using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using NUnit.Framework;
using UnityEditor;
using UnityEngine;

public class SpriteSheetExporter : Editor
{
    [MenuItem("Assets/Parse SpriteSheet")]
    public static void Parse()
    {
        var _texture = Selection.activeObject as Texture2D;
        string _texturePath = AssetDatabase.GetAssetPath(_texture);

        SpriteSheetInfo _sheetInfo = SpriteSheetInfo.Parse(_texturePath);
        string _outSheetJson = JsonUtility.ToJson(_sheetInfo, true);

        Debug.Log(_outSheetJson);

        string _newFilePath =
            Path.GetFileNameWithoutExtension(_sheetInfo.sheetName);

        // 대상 디렉토리를 지정합니다.
        // 디렉토리가 없다면 생성합니다.
        string _directory = $"{Application.dataPath}/../_exported/SpriteSheet";
        if (Directory.Exists(_directory) == false)
            Directory.CreateDirectory(_directory);

        // 대상 경로에 파일을 기록합니다.
        using (StreamWriter _fs = new StreamWriter(
            $"{_directory}/{_newFilePath}.json",
            false,
            Encoding.UTF8,
            _outSheetJson.Length
        ))
        {
            _fs.Write(_outSheetJson);
            _fs.Close();
        }
    }

    // Note that we pass the same path, and also pass "true" to the second argument.
    [MenuItem("Assets/Parse SpriteSheet", true)]
    private static bool testOptionValidation()
    {
        // This returns true when the selected object is a Variable (the menu item will be disabled otherwise).
        return Selection.activeObject is Texture2D;
    }
}