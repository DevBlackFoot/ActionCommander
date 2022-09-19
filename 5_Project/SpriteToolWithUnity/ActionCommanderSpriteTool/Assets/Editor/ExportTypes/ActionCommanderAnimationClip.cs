using System;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

[System.Serializable]
public class ActionCommanderAnimationClip
{
    public string name;
    public bool bIsLooping;
    public float length;

    public FloatValueCurve[] valueCurves;
    public AnimationEventInfo[] eventInfos;

    // curves of specific types
    public FloatValueCurve positionXCurve;
    public FloatValueCurve positionYCurve;
    public FloatValueCurve positionZCurve;
    public StringValueCurve spriteCurve;

    public static ActionCommanderAnimationClip Parse(ParsedAnimationClip _clip)
    {
        Dictionary<string, Func<UnityEngine.Object, string>> _mappers =
            new Dictionary<string, Func<UnityEngine.Object, string>>()
            {
                {
                    typeof(UnityEngine.Sprite).Name, _obj =>
                    {
                        Sprite _sprite = (Sprite)_obj;
                        return $"{AssetDatabase.GetAssetPath(_sprite)}::{_sprite.name}";
                    }
                }
            };

        ActionCommanderAnimationClip _outClip = new ActionCommanderAnimationClip();
        _outClip.name = _clip.name;
        _outClip.bIsLooping = _clip.bIsLooping;
        _outClip.length = _clip.length;
        _outClip.valueCurves = _clip.valueCurves;
        _outClip.eventInfos = _clip.eventInfos;

        // 
        _outClip.positionXCurve = FloatValueCurve.ParseSpecific(
            _clip.valueCurves,
            "UnityEngine.Transform",
            "m_LocalPosition.x"
        );

        _outClip.positionYCurve = FloatValueCurve.ParseSpecific(
            _clip.valueCurves,
            "UnityEngine.Transform",
            "m_LocalPosition.y"
        );

        _outClip.positionZCurve = FloatValueCurve.ParseSpecific(
            _clip.valueCurves,
            "UnityEngine.Transform",
            "m_LocalPosition.z"
        );

        // mapping object curves
        _outClip.spriteCurve = StringValueCurve.ParseSpecificObject(
            _clip.objectCurves,
            "UnityEngine.SpriteRenderer",
            "m_Sprite",
            _mappers[typeof(UnityEngine.Sprite).Name]
        );

        return _outClip;
    }
}