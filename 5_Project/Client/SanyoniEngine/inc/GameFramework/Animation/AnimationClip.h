#pragma once

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/encodedstream.h>

#include "SanyoniEngine_types.h"

namespace SanyoniEngine
{
	struct FloatValueCurve
	{
		struct FloatValueCurveKeyframe
		{
			float time;
			float value;
		};

		tstring typeName;
		tstring propertyName;
		std::vector<FloatValueCurveKeyframe> keyframes;

		static FloatValueCurve Parse(const rapidjson::GenericValue<rapidjson::UTF8<>>& _jsonValue)
		{
			assert(_jsonValue.HasMember("typeName"));
			assert(_jsonValue.HasMember("propertyName"));
			assert(_jsonValue.HasMember("keyframes"));


			FloatValueCurve _newCurve;

			_newCurve.typeName = StringHelper::ToWString(_jsonValue["typeName"].GetString());
			_newCurve.propertyName = StringHelper::ToWString(_jsonValue["propertyName"].GetString());

			const auto _keyframes = _jsonValue["keyframes"].GetArray();
			_newCurve.keyframes = std::vector<FloatValueCurveKeyframe>(_keyframes.Size());

			for (int i = 0; i < _keyframes.Size(); ++i)
			{
				const auto& _keyframe = _keyframes[i];

				assert(_keyframe.HasMember("time"));
				assert(_keyframe.HasMember("value"));


				_newCurve.keyframes[i].time = _keyframe["time"].GetFloat();
				_newCurve.keyframes[i].value = _keyframe["value"].GetFloat();
			}

			return _newCurve;
		}
	};

	struct StringValueCurve
	{
		struct StringValueCurveKeyframe
		{
			float time;
			tstring value;
		};

		tstring typeName;
		tstring propertyName;
		std::vector<StringValueCurveKeyframe> keyframes;

		static StringValueCurve Parse(const rapidjson::GenericValue<rapidjson::UTF8<>>& _jsonValue)
		{
			assert(_jsonValue.HasMember("typeName"));
			assert(_jsonValue.HasMember("propertyName"));
			assert(_jsonValue.HasMember("keyframes"));


			StringValueCurve _newCurve;

			_newCurve.typeName = StringHelper::ToWString(_jsonValue["typeName"].GetString());
			_newCurve.propertyName = StringHelper::ToWString(_jsonValue["propertyName"].GetString());

			const auto _keyframes = _jsonValue["keyframes"].GetArray();
			_newCurve.keyframes = std::vector<StringValueCurveKeyframe>(_keyframes.Size());

			for (int i = 0; i < _keyframes.Size(); ++i)
			{
				const auto& _keyframe = _keyframes[i];

				assert(_keyframe.HasMember("time"));
				assert(_keyframe.HasMember("value"));


				_newCurve.keyframes[i].time = _keyframe["time"].GetFloat();
				_newCurve.keyframes[i].value = StringHelper::ToWString(_keyframe["value"].GetString());
			}

			return _newCurve;
		}
	};

	struct AnimationEventInfo
	{
		float time;
		tstring functionName;
		tstring parameter;

		static AnimationEventInfo Parse(const rapidjson::GenericValue<rapidjson::UTF8<>>& _jsonValue)
		{
			assert(_jsonValue.HasMember("time"));
			assert(_jsonValue.HasMember("functionName"));
			assert(_jsonValue.HasMember("parameter"));


			AnimationEventInfo _newInfo;

			_newInfo.time = _jsonValue["time"].GetFloat();
			_newInfo.functionName = StringHelper::ToWString(_jsonValue["functionName"].GetString());
			_newInfo.parameter = StringHelper::ToWString(_jsonValue["parameter"].GetString());

			return _newInfo;
		}
	};

	struct AnimationClip
	{
		/*
		 * json 파일로부터 읽어들인 속성들입니다.
		 */

		bool bIsLooping;
		float length;

		std::vector<FloatValueCurve> valueCurves;
		std::vector<AnimationEventInfo> eventInfos;

		FloatValueCurve positionXCurve;
		FloatValueCurve positionYCurve;
		FloatValueCurve positionZCurve;
		StringValueCurve spriteCurve;

		static AnimationClip* Parse(tstring _filePath)
		{
			// 파일을 읽고 rapidjson을 준비합니다.
			FILE* _fp;
			fopen_s(&_fp, StringHelper::ToString(_filePath).c_str(), "rb");

			char _buffer[50000];
			rapidjson::FileReadStream _frs(_fp, _buffer, sizeof(_buffer));
			rapidjson::EncodedInputStream<rapidjson::UTF8<>, rapidjson::FileReadStream> _eis(_frs);

			rapidjson::Document _document;
			auto _parseError = _document.ParseStream(_eis).GetParseError();

			fclose(_fp);


			// assert 검사합니다.
			assert(_parseError == rapidjson::ParseErrorCode::kParseErrorNone);
			assert(_document.HasMember("name"));
			assert(_document.HasMember("bIsLooping"));
			assert(_document.HasMember("length"));
			assert(_document.HasMember("valueCurves"));
			assert(_document.HasMember("eventInfos"));
			assert(_document.HasMember("positionXCurve"));
			assert(_document.HasMember("positionYCurve"));
			assert(_document.HasMember("positionZCurve"));
			assert(_document.HasMember("spriteCurve"));


			// 단일 속성 정보를 담습니다.
			FloatValueCurve _positionXCurve = FloatValueCurve::Parse(_document["positionXCurve"]);
			FloatValueCurve _positionYCurve = FloatValueCurve::Parse(_document["positionYCurve"]);
			FloatValueCurve _positionZCurve = FloatValueCurve::Parse(_document["positionZCurve"]);
			StringValueCurve _spriteCurve = StringValueCurve::Parse(_document["spriteCurve"]);


			// 배열 정보를 담습니다.
			const auto _jsonValueCurves = _document["valueCurves"].GetArray();
			const auto _jsonEventInfos = _document["eventInfos"].GetArray();

			std::vector<FloatValueCurve> _valueCurveList(_jsonValueCurves.Size());
			std::vector<AnimationEventInfo> _eventInfoList(_jsonEventInfos.Size());

			for (int i = 0; i < _jsonValueCurves.Size(); ++i)
			{
				FloatValueCurve _newCurve = FloatValueCurve::Parse(_jsonValueCurves[i]);
				_valueCurveList[i] = _newCurve;
			}

			for (int i = 0; i < _jsonEventInfos.Size(); ++i)
			{
				AnimationEventInfo _newEvent = AnimationEventInfo::Parse(_jsonEventInfos[i]);
				_eventInfoList[i] = _newEvent;
			}


			// 반환합니다.
			AnimationClip* _outClip = new AnimationClip();
			_outClip->bIsLooping = _document["bIsLooping"].GetBool();
			_outClip->length = _document["length"].GetFloat();
			_outClip->valueCurves = _valueCurveList;
			_outClip->eventInfos = _eventInfoList;
			_outClip->positionXCurve = _positionXCurve;
			_outClip->positionYCurve = _positionYCurve;
			_outClip->positionZCurve = _positionZCurve;
			_outClip->spriteCurve = _spriteCurve;

			return _outClip;
		}
	};
}
