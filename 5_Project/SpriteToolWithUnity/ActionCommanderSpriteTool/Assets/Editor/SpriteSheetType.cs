using System.Collections;
using System.Collections.Generic;
using System.Linq;
using NUnit.Framework;
using TMPro;
using UnityEditor;
using UnityEngine;
using UnityEngine.Serialization;

[System.Serializable]
public class SpriteSheetInfo
{
    public string sheetName;
    public SpriteInfo[] spriteInfos;

    public static SpriteSheetInfo Parse(string _assetPath)
    {
        Sprite[] _sprites = AssetDatabase.LoadAllAssetsAtPath(_assetPath)
            .OfType<Sprite>().ToArray();

        SpriteSheetInfo _outInfo = new SpriteSheetInfo();
        _outInfo.sheetName = _assetPath;
        _outInfo.spriteInfos = new SpriteInfo[_sprites.Length];

        for (int i = 0; i < _sprites.Length; i++)
        {
            _outInfo.spriteInfos[i] = SpriteInfo.Parse(_sprites[i]);
        }

        return _outInfo;
    }
}


[System.Serializable]
public class SpriteInfo
{
    public string name;
    public IntRect rect;
    public Vector2 pivot;

    public static SpriteInfo Parse(Sprite _sprite)
    {
        Assert.IsTrue(_sprite.pixelsPerUnit == 1.0f, "texture 파일의 pixels per unit을 반드시 1로 설정해주어야 합니다. texture의 pixelsPerUnit 설정이 올바른지, 그리고 Max Size가 충분히 주어졌는지 확인해주세요.");

        int _rectX = (int)_sprite.rect.x;
        int _rectY = (int)_sprite.rect.y;
        int _rectWidth = (int)_sprite.rect.width;
        int _rectHeight = (int)_sprite.rect.height;
        int _spritePivotX = (int)_sprite.pivot.x;
        int _spritePivotY = (int)_sprite.pivot.y;

        SpriteInfo _outInfo = new SpriteInfo();
        _outInfo.name = _sprite.name;
        _outInfo.rect = new IntRect(
            _rectX,
            _rectY,
            _rectX + _rectWidth,
            _rectY + _rectHeight
        );
        _outInfo.pivot = new Vector2(
            (float)_spritePivotX / (float)_rectWidth,
            (float)_spritePivotY / (float)_rectHeight
        );

        return _outInfo;
    }
}


[System.Serializable]
public class IntRect
{
    public int left;
    public int top;
    public int right;
    public int bottom;

    public IntRect()
    {
    }

    public IntRect(int _left, int _top, int _right, int _bottom)
    {
        left = _left;
        top = _top;
        right = _right;
        bottom = _bottom;
    }

    public int GetWidth()
    {
        return right - left;
    }

    public int GetHeight()
    {
        return bottom - top;
    }
}