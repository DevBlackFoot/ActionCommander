using System;
using System.Linq;
using UnityEditor;
using UnityEngine;
using UnityEngine.Assertions;

[System.Serializable]
public class ParsedAnimationClip
{
    public string name;
    public bool bIsLooping;
    public float length;

    public FloatValueCurve[] valueCurves;
    public ObjectCurve[] objectCurves;
    public AnimationEventInfo[] eventInfos;

    public static ParsedAnimationClip Parse(AnimationClip _clip)
    {
        var _valueCurveBindings = UnityEditor.AnimationUtility.GetCurveBindings(_clip);
        var _objectRefCurveBindings = AnimationUtility.GetObjectReferenceCurveBindings(_clip);

        var _valueCurves = _valueCurveBindings.Select(_binding => FloatValueCurve.Parse(_clip, _binding));
        var _objectCurves = _objectRefCurveBindings.Select(_binding => ObjectCurve.Parse(_clip, _binding));
        var _eventInfos = _clip.events.Select(_event =>
        {
            AnimationEventInfo _outInfo = new AnimationEventInfo();
            _outInfo.time = _event.time;
            _outInfo.functionName = _event.functionName;
            _outInfo.parameter = _event.stringParameter;
            return _outInfo;
        });

        ParsedAnimationClip _outParsed = new ParsedAnimationClip();
        _outParsed.name = _clip.name;
        _outParsed.bIsLooping = _clip.isLooping;
        _outParsed.length = _clip.length;

        _outParsed.valueCurves = _valueCurves.ToArray();
        _outParsed.objectCurves = _objectCurves.ToArray();
        _outParsed.eventInfos = _eventInfos.ToArray();

        return _outParsed;
    }
}

[System.Serializable]
public class AnimationEventInfo
{
    public float time;
    public string functionName;
    public string parameter;
}

[System.Serializable]
public class FloatValueCurve
{
    [System.Serializable]
    public struct ValueCurveKeyframe
    {
        public float time;
        public float value;
    }

    public string typeName;
    public string propertyName;
    public ValueCurveKeyframe[] keyframes;

    public static FloatValueCurve Parse(AnimationClip _clip, EditorCurveBinding _binding)
    {
        var _curve = AnimationUtility.GetEditorCurve(_clip, _binding);

        FloatValueCurve _outCurve = new FloatValueCurve();
        _outCurve.typeName = _binding.type.ToString();
        _outCurve.propertyName = _binding.propertyName;

        _outCurve.keyframes = new ValueCurveKeyframe[_curve.keys.Length];
        for (int i = 0; i < _curve.keys.Length; ++i)
        {
            var _keyframe = _curve.keys[i];

            _outCurve.keyframes[i].time = _keyframe.time;
            _outCurve.keyframes[i].value = _keyframe.value;
        }

        return _outCurve;
    }

    public static FloatValueCurve ParseSpecific(
        FloatValueCurve[] _curves,
        string _targetTypeName,
        string _targetPropertyName)
    {
        try
        {
            var _targetCurve = _curves.First(_curve =>
                _curve.typeName.Equals(_targetTypeName) && _curve.propertyName.Equals(_targetPropertyName)
            );

            // Assert.IsTrue(_targetCurve.keyframes.Length > 0);

            return _targetCurve;
        }
        catch (InvalidOperationException _e)
        {
            // linq first 조건에 만족하는 요소가 없을 때 실행됩니다.
            return null;
        }
        catch (Exception _e)
        {
            return null;
        }
    }
}

[System.Serializable]
public class StringValueCurve
{
    [System.Serializable]
    public struct StringValueCurveKeyframe
    {
        public float time;
        public string value;
    }

    public string typeName;
    public string propertyName;
    public StringValueCurveKeyframe[] keyframes;

    public static StringValueCurve Parse(
        AnimationClip _clip,
        EditorCurveBinding _binding,
        System.Func<ObjectReferenceKeyframe, StringValueCurveKeyframe> _pred)
    {
        var _keys = AnimationUtility.GetObjectReferenceCurve(_clip, _binding);

        StringValueCurve _outCurve = new StringValueCurve();
        _outCurve.typeName = _binding.type.ToString();
        _outCurve.propertyName = _binding.propertyName;

        _outCurve.keyframes = new StringValueCurveKeyframe[_keys.Length];
        for (int i = 0; i < _keys.Length; ++i)
        {
            _outCurve.keyframes[i] = _pred(_keys[i]);
        }

        return _outCurve;
    }

    public static StringValueCurve ParseSpecificObject(
        ObjectCurve[] _objectCurves,
        string _targetTypeName,
        string _targetPropertyName,
        Func<UnityEngine.Object, string> _mapper)
    {
        try
        {
            // Assert.IsTrue(_objectCurves.Length > 0);

            var _targetObjCurve = _objectCurves.First(_objCurve =>
                _objCurve.typeName.Equals(_targetTypeName) && _objCurve.propertyName.Equals(_targetPropertyName)
            );

            Assert.IsTrue(_targetObjCurve.keyframes.Length > 0);


            StringValueCurve _outCurve = new StringValueCurve();
            _outCurve.typeName = _targetObjCurve.typeName;
            _outCurve.propertyName = _targetObjCurve.propertyName;

            _outCurve.keyframes = new StringValueCurveKeyframe[_targetObjCurve.keyframes.Length];
            for (int i = 0; i < _targetObjCurve.keyframes.Length; i++)
            {
                _outCurve.keyframes[i].time = _targetObjCurve.keyframes[i].time;
                _outCurve.keyframes[i].value = _mapper(_targetObjCurve.keyframes[i].obj);
            }

            return _outCurve;
        }
        catch (InvalidOperationException _e)
        {
            // linq first 조건에 만족하는 요소가 없을 때 실행됩니다.
            return null;
        }
        catch (Exception _e)
        {
            return null;
        }
    }
}


[System.Serializable]
public class ObjectCurve
{
    [System.Serializable]
    public struct ObjectCurveKeyframe
    {
        public float time;
        public UnityEngine.Object obj;
    }

    public string typeName;
    public string propertyName;
    public ObjectCurveKeyframe[] keyframes;

    public static ObjectCurve Parse(AnimationClip _clip, EditorCurveBinding _binding)
    {
        var _keys = AnimationUtility.GetObjectReferenceCurve(_clip, _binding);

        ObjectCurve _outCurve = new ObjectCurve();
        _outCurve.typeName = _binding.type.ToString();
        _outCurve.propertyName = _binding.propertyName;

        _outCurve.keyframes = new ObjectCurveKeyframe[_keys.Length];
        for (int i = 0; i < _keys.Length; ++i)
        {
            var _keyframe = _keys[i];

            _outCurve.keyframes[i].time = _keyframe.time;
            _outCurve.keyframes[i].obj = _keyframe.value;
        }

        return _outCurve;
    }
}