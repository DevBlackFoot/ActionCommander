#pragma once

#include "SanyoniEngine_typedef.h"

namespace SanyoniEngine
{
	class MathHelper
	{
	public:
		template <typename T>
		static int Sign(T val)
		{
			return (T(0) < val) - (val < T(0));
		}

		template <typename TReturnType, typename TElem1, typename TElem2, typename TElem3>
		static TReturnType Clamp(TElem1 _value, TElem2 _min, TElem3 _max)
		{
			if (_value < _min)
				return _min;

			if (_value > _max)
				return _max;

			return static_cast<TReturnType>(_value);
		}

		template <typename TElem1, typename TElem2, typename TElem3>
		static bool InRange(TElem1 _value, TElem2 _min, TElem3 _max)
		{
			if (_value < _min)
				return false;

			if (_value > _max)
				return false;

			return true;
		}

#ifndef _ALMOST_EQUALS_
#define _ALMOST_EQUALS_

		template <typename T1, typename T2>
		static bool AlmostEquals(const T1& _lhs, const T2& _rhs)
		{
			static_assert(false, "AlmostEquals comparisons are only allowed for primitive floating point types.");
			return false;
		}

#define __DEFINE_ALMOST_EQUALS__(T1, T2) template <> \
										static bool AlmostEquals(const T1& _lhs, const T2& _rhs) { return std::abs(_lhs - _rhs) < 0.001; } \
										template <> \
										static bool AlmostEquals(const volatile T1& _lhs, const T2& _rhs) { return std::abs(_lhs - _rhs) < 0.001; } \
										template <> \
										static bool AlmostEquals(const T1& _lhs, const volatile T2& _rhs) { return std::abs(_lhs - _rhs) < 0.001; } \
										template <> \
										static bool AlmostEquals(const volatile T1& _lhs, const volatile T2& _rhs) { return std::abs(_lhs - _rhs) < 0.001; }

		__DEFINE_ALMOST_EQUALS__(float, float)
		__DEFINE_ALMOST_EQUALS__(float, double)
		__DEFINE_ALMOST_EQUALS__(float, short)
		__DEFINE_ALMOST_EQUALS__(float, int)
		__DEFINE_ALMOST_EQUALS__(float, long)
		__DEFINE_ALMOST_EQUALS__(float, long long)
		__DEFINE_ALMOST_EQUALS__(float, unsigned short)
		__DEFINE_ALMOST_EQUALS__(float, unsigned int)
		__DEFINE_ALMOST_EQUALS__(float, unsigned long)
		__DEFINE_ALMOST_EQUALS__(float, unsigned long long)

		__DEFINE_ALMOST_EQUALS__(double, float)
		__DEFINE_ALMOST_EQUALS__(double, double)
		__DEFINE_ALMOST_EQUALS__(double, short)
		__DEFINE_ALMOST_EQUALS__(double, int)
		__DEFINE_ALMOST_EQUALS__(double, long)
		__DEFINE_ALMOST_EQUALS__(double, long long)
		__DEFINE_ALMOST_EQUALS__(double, unsigned short)
		__DEFINE_ALMOST_EQUALS__(double, unsigned int)
		__DEFINE_ALMOST_EQUALS__(double, unsigned long)
		__DEFINE_ALMOST_EQUALS__(double, unsigned long long)

		//
		__DEFINE_ALMOST_EQUALS__(short, float)
		__DEFINE_ALMOST_EQUALS__(int, float)
		__DEFINE_ALMOST_EQUALS__(long, float)
		__DEFINE_ALMOST_EQUALS__(long long, float)
		__DEFINE_ALMOST_EQUALS__(unsigned short, float)
		__DEFINE_ALMOST_EQUALS__(unsigned int, float)
		__DEFINE_ALMOST_EQUALS__(unsigned long, float)
		__DEFINE_ALMOST_EQUALS__(unsigned long long, float)

		__DEFINE_ALMOST_EQUALS__(short, double)
		__DEFINE_ALMOST_EQUALS__(int, double)
		__DEFINE_ALMOST_EQUALS__(long, double)
		__DEFINE_ALMOST_EQUALS__(long long, double)
		__DEFINE_ALMOST_EQUALS__(unsigned short, double)
		__DEFINE_ALMOST_EQUALS__(unsigned int, double)
		__DEFINE_ALMOST_EQUALS__(unsigned long, double)
		__DEFINE_ALMOST_EQUALS__(unsigned long long, double)

#endif
	};

	class StringHelper
	{
	public:
#ifndef _FORMAT_
#define _FORMAT_

		template <typename ... Args>
		static tstring Format(const tstring& _format, Args ... _args)
		{
			return ToString(_format.c_str(), _args ...);
		}

		template <typename ... Args>
		static tstring Format(const TCHAR* _format, Args ... _args)
		{
			static constexpr int BUFFER_SIZE = 1024;

			TCHAR _buffer[BUFFER_SIZE];
			_sntprintf_s(_buffer, BUFFER_SIZE, _format, _args ...);

			const size_t _length = _tcslen(_buffer);
			return tstring{_buffer, _buffer + _length};
		}

#endif

#ifndef _SPLIT_
#define _SPLIT_

		/**
		 * 원문: https://5kyc1ad.tistory.com/289
		 */
		static std::vector<tstring> Split(const tstring& _str, const tstring& _token)
		{
			// Check parameters
			if (_token.length() == 0 || _str.find(_token) == tstring::npos)
				return std::vector<tstring>({_str});


			// return var
			std::vector<tstring> _retList;


			int _findOffset = 0;
			int _splitOffset = 0;
			while ((_splitOffset = _str.find(_token, _findOffset)) != tstring::npos)
			{
				_retList.push_back(_str.substr(_findOffset, _splitOffset - _findOffset));
				_findOffset = _splitOffset + _token.length();
			}
			_retList.push_back(_str.substr(_findOffset, _str.length() - _findOffset));

			return _retList;
		}

#endif

#ifndef _CONVERT_CHAR_WCHAR_TCHAR_
#define _CONVERT_CHAR_WCHAR_TCHAR_

		static std::wstring StringToWString(const std::string& _string)
		{
			return StringToWString(_string.c_str());
		}

		static std::wstring StringToWString(const char* _string)
		{
			const int _stringLength = static_cast<int>(std::strlen(_string));
			const int _bufferLength = _stringLength + 1;
			wchar_t* _buffer = new wchar_t[_bufferLength];

			// MultiByteToWideChar 함수가 문자열의 끝에 자동으로 null 문자 ('\0')을 넣어주지 않습니다.
			// 따라서 문자열을 변환을 마친 후 그 뒤에다 수동으로 null문자를 넣어주어야 합니다.
			int _end = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, _string, _stringLength,
			                               _buffer, _bufferLength);
			_buffer[_end] = '\0';

			auto _retVal = std::wstring{_buffer};

			delete[] _buffer;
			return _retVal;
		}

		static std::string WStringToString(const std::wstring& _string)
		{
			return WStringToString(_string.c_str());
		}

		static std::string WStringToString(const wchar_t* _string)
		{
			const int _stringLength = static_cast<int>(std::wcslen(_string));
			const int _bufferLength = _stringLength * 2 + 1;
			char* _buffer = new char[_bufferLength];

			// WideCharToMultiByte 함수가 문자열의 끝에 자동으로 null 문자 ('\0')을 넣어주지 않습니다.
			// 따라서 문자열을 변환을 마친 후 그 뒤에다 수동으로 null문자를 넣어주어야 합니다.
			int _end = WideCharToMultiByte(CP_UTF8, 0, _string, _stringLength,
			                               _buffer, _bufferLength,
			                               nullptr, nullptr);
			_buffer[_end] = '\0';

			auto _retVal = std::string{_buffer};

			delete[] _buffer;
			return _retVal;
		}

		static std::wstring ToWString(const std::string& _string)
		{
			return StringToWString(_string);
		}

		static std::wstring ToWString(const std::wstring& _string)
		{
			return _string;
		}

		static std::wstring ToWString(const char* _string)
		{
			return StringToWString(_string);
		}

		static std::wstring ToWString(const wchar_t* _string)
		{
			return std::wstring(_string);
		}

		static std::string ToString(const std::wstring& _string)
		{
			return WStringToString(_string);
		}

		static std::string ToString(const std::string& _string)
		{
			return _string;
		}

		static std::string ToString(const wchar_t* _string)
		{
			return WStringToString(_string);
		}

		static std::string ToString(const char* _string)
		{
			return std::string{_string};
		}

		static tstring ToTString(const std::string& _string)
		{
#if defined (_UNICODE)
			return StringToWString(_string);
#else
			return _string;
#endif
		}

		static tstring ToTString(const std::wstring& _string)
		{
#if defined (_UNICODE)
			return _string;
#else
			return WStringToString(_string);
#endif
		}

		static tstring ToTString(const char* _string)
		{
#if defined( _UNICODE )
			return StringToWString(_string);
#else
			return std::string{ _string };
#endif
		}

		static tstring ToTString(const wchar_t* _string)
		{
#if defined( _UNICODE )
			return std::wstring{_string};
#else
			return WStringToString(_string);
#endif
		}

#endif
	};

	class DebugHelper
	{
	public:
#ifndef _PRINT_DEBUG_STRING_
#define _PRINT_DEBUG_STRING_

		template <typename ... Args>
		static void PrintDebugString(const tstring& _format, Args ... _args)
		{
			const tstring _formatted = StringHelper::Format(&_format, _args ...);
			OutputDebugString(_formatted.c_str());
		}

		template <typename ... Args>
		static void PrintDebugString(const TCHAR* _format, Args ... _args)
		{
			const tstring _formatted = StringHelper::Format(_format, _args ...);
			OutputDebugString(_formatted.c_str());
		}

#endif
	};
};
