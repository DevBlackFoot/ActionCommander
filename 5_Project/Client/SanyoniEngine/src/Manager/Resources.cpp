#include "SanyoniEngine_pch.h"
#include "Resources.h"

#include <dwrite.h>
#include <wincodec.h>
#include <rapidjson/document.h>
#include <rapidjson/encodedstream.h>
#include <rapidjson/filereadstream.h>

#include "AnimationClip.h"
#include "Core.h"
#include "FontCollection.h"
#include "Helper.h"
#include "TextFormat.h"

IMPLEMENT_SINGLETON_CLASS(Resources)

rapidjson::Document Resources::LoadJsonFromFile(const tstring& _filePath)
{
	FILE* _fp;
	fopen_s(&_fp, StringHelper::ToString(_filePath).c_str(), "rb");

	char _buffer[50000];
	rapidjson::FileReadStream _frs(_fp, _buffer, sizeof(_buffer));
	rapidjson::EncodedInputStream<rapidjson::UTF8<>, rapidjson::FileReadStream> _eis(_frs);

	rapidjson::Document _document;
	auto _parseError = _document.ParseStream(_eis).GetParseError();

	fclose(_fp);

	return _document;
}

HRESULT Resources::LoadBitmapFromFile(Bitmap** _outBitmap, const tstring _fileUri)
{
	assert(_outBitmap != nullptr);

	if (const auto _iter = m_BitmapFromFileMap.find(_fileUri);
		_iter != m_BitmapFromFileMap.end())
	{
		// 이미 캐싱되어 있는 bitmap 이라면,
		// 더 이상 진행하지 않고 캐시되어 있는 bitmap를 반환합니다.
		*_outBitmap = _iter->second;
		return S_OK;
	}

	Bitmap* _bitmap = nullptr;

	HRESULT _hr = S_OK;

	IWICBitmapDecoder* _decoder = nullptr;
	IWICBitmapFrameDecode* _source = nullptr;
	IWICFormatConverter* _converter = nullptr;

	const std::wstring _fileUriWStr = StringHelper::ToWString(_fileUri);
	const auto _imagingFactory = Core::GetWICImagingFactory();
	const auto _renderTarget = Core::GetRenderTarget();

	_hr = _imagingFactory->CreateDecoderFromFilename(
		_fileUriWStr.c_str(),
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&_decoder
	);

	if (_hr == S_OK)
	{
		// Create the initial frame.
		_hr = _decoder->GetFrame(0, &_source);
	}

	if (_hr == S_OK)
	{
		// Convert the image format to 32bppPBGRA
		// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
		_hr = _imagingFactory->CreateFormatConverter(&_converter);
	}

	if (_hr == S_OK)
	{
		_hr = _converter->Initialize(
			_source,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			nullptr,
			0.,
			WICBitmapPaletteTypeMedianCut
		);
	}

	if (_hr == S_OK)
	{
		// Create a Direct2D bitmap from the WIC bitmap.
		_hr = _renderTarget->CreateBitmapFromWicBitmap(
			_converter,
			nullptr,
			&_bitmap
		);
	}

	if (_hr != S_OK)
	{
		assert(false);
	}


	// 불러온 bitmap을 캐싱합니다.
	m_BitmapFromFileMap.emplace(_fileUri, _bitmap);


	if (_decoder != nullptr)
		_decoder->Release();

	if (_source != nullptr)
		_source->Release();

	if (_converter != nullptr)
		_converter->Release();

	*_outBitmap = _bitmap;
	return _hr;
}

HRESULT Resources::LoadBitmapFromResource(Bitmap** _outBitmap, const tstring _resourceName, const tstring _resourceType)
{
	assert(_outBitmap != nullptr);

	if (const auto _iter = m_BitmapFromResourceMap.find(_resourceName);
		_iter != m_BitmapFromResourceMap.end())
	{
		// 이미 캐싱되어 있는 bitmap 이라면,
		// 더 이상 진행하지 않고 캐시되어 있는 bitmap를 반환합니다.
		*_outBitmap = _iter->second;
		return S_OK;
	}

	Bitmap* _bitmap = nullptr;

	HRESULT _hr = S_OK;
	IWICBitmapDecoder* _decoder = nullptr;
	IWICBitmapFrameDecode* _source = nullptr;
	IWICStream* _stream = nullptr;
	IWICFormatConverter* _converter = nullptr;

	HGLOBAL _imageResDataHandle = nullptr;
	DWORD _imageFileSize = 0;

	const std::wstring _resourceNameWStr = StringHelper::ToWString(_resourceName);
	const std::wstring _resourceTypeWStr = StringHelper::ToWString(_resourceType);
	const auto _imagingFactory = Core::GetWICImagingFactory();
	const auto _renderTarget = Core::GetRenderTarget();

	// Locate the resource.
	const HRSRC _imageResHandle =
		FindResourceW(HINST_THISCOMPONENT, _resourceNameWStr.c_str(), _resourceTypeWStr.c_str());

	_hr = _imageResHandle ? S_OK : E_FAIL;
	if (SUCCEEDED(_hr))
	{
		// Load the resource.
		_imageResDataHandle = LoadResource(HINST_THISCOMPONENT, _imageResHandle);

		_hr = _imageResDataHandle ? S_OK : E_FAIL;
	}

	if (SUCCEEDED(_hr))
	{
		// Lock it to get a system memory pointer.
		_bitmap = static_cast<Bitmap*>(LockResource(_imageResDataHandle));

		_hr = _bitmap ? S_OK : E_FAIL;
	}

	if (SUCCEEDED(_hr))
	{
		// Calculate the size.
		_imageFileSize = SizeofResource(HINST_THISCOMPONENT, _imageResHandle);

		_hr = _imageFileSize ? S_OK : E_FAIL;
	}

	if (SUCCEEDED(_hr))
	{
		// Create a WIC stream to map onto the memory.
		_hr = _imagingFactory->CreateStream(&_stream);
	}

	if (SUCCEEDED(_hr))
	{
		// Initialize the stream with the memory pointer and size.
		_hr = _stream->InitializeFromMemory(
			reinterpret_cast<BYTE*>(_bitmap),
			_imageFileSize
		);
	}

	if (SUCCEEDED(_hr))
	{
		// Create a decoder for the stream.
		_hr = _imagingFactory->CreateDecoderFromStream(
			_stream,
			nullptr,
			WICDecodeMetadataCacheOnLoad,
			&_decoder
		);
	}

	if (SUCCEEDED(_hr))
	{
		// Create the initial frame.
		_hr = _decoder->GetFrame(0, &_source);
	}

	if (SUCCEEDED(_hr))
	{
		// Convert the image format to 32bppPBGRA
		// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
		_hr = _imagingFactory->CreateFormatConverter(&_converter);
	}

	if (SUCCEEDED(_hr))
	{
		_hr = _converter->Initialize(
			_source,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			nullptr,
			0.,
			WICBitmapPaletteTypeMedianCut
		);
	}

	if (SUCCEEDED(_hr))
	{
		//create a Direct2D bitmap from the WIC bitmap.
		_hr = _renderTarget->CreateBitmapFromWicBitmap(
			_converter,
			nullptr,
			&_bitmap
		);
	}

	// 불러온 bitmap을 캐싱합니다.
	m_BitmapFromFileMap.emplace(_resourceName, _bitmap);

	_decoder->Release();
	_source->Release();
	_stream->Release();
	_converter->Release();

	*_outBitmap = _bitmap;
	return _hr;
}

SpriteSheet* Resources::LoadSpriteSheetFromFile(const tstring& _fileUri)
{
	if (const auto _iter = m_SpriteSheetMap.find(_fileUri);
		_iter != m_SpriteSheetMap.end())
	{
		// 이미 캐싱되어 있는 json 파일이라면,
		// 더 이상 진행하지 않고 캐시되어 있는 sheet를 반환합니다.
		return _iter->second;
	}


	SpriteSheet* _outSpriteSheet = nullptr;
	HRESULT _hr = SpriteSheet::LoadFromFile(&_outSpriteSheet, _fileUri);


	// 불러온 sprite sheet를 캐싱합니다.
	m_SpriteSheetMap.emplace(_fileUri, _outSpriteSheet);

	return _outSpriteSheet;
}

SpriteSheet* Resources::LoadSpriteSheetFromJsonFile(const tstring& _jsonFileUri)
{
	if (const auto _iter = m_JsonSpriteSheetMap.find(_jsonFileUri);
		_iter != m_JsonSpriteSheetMap.end())
	{
		// 이미 캐싱되어 있는 json 파일이라면,
		// 더 이상 진행하지 않고 캐시되어 있는 sheet를 반환합니다.
		return LoadSpriteSheetFromFile(_iter->second);
	}


	SpriteSheet* _outSpriteSheet = nullptr;
	HRESULT _hr = SpriteSheet::LoadFromJsonFile(&_outSpriteSheet, _jsonFileUri);


	// 불러온 sprite sheet를 캐싱합니다.
	m_SpriteSheetMap.emplace(_outSpriteSheet->GetBitmapFileName(), _outSpriteSheet);
	m_JsonSpriteSheetMap.emplace(_jsonFileUri, _outSpriteSheet->GetBitmapFileName());

	return _outSpriteSheet;
}

AnimationClip* Resources::LoadAnimationClipFromJsonFile(const tstring& _jsonFileUri)
{
	if (const auto _iter = m_AnimationClipMap.find(_jsonFileUri);
		_iter != m_AnimationClipMap.end())
	{
		// 이미 캐싱되어 있는 json 파일이라면,
		// 더 이상 진행하지 않고 캐시되어 있는 clip을 반환합니다.
		return _iter->second;
	}

	AnimationClip* _outClip = nullptr;
	_outClip = AnimationClip::Parse(_jsonFileUri);


	// 불러온 clip을 캐싱합니다.
	m_AnimationClipMap.emplace(_jsonFileUri, _outClip);

	return _outClip;
}

FontCollection* Resources::LoadFontCollectionFromFile(const tstring _fileUri)
{
	if (const auto _iter = m_FontCollectionMap.find(_fileUri);
		_iter != m_FontCollectionMap.end())
	{
		// 이미 캐싱되어 있는 font collection 파일이라면,
		// 더 이상 진행하지 않고 캐시되어 있는 font collection을 반환합니다.
		return _iter->second;
	}

	FontCollection* _outFontCollection;
	FontCollection::Create(Core::GetDWriteFactory(), _fileUri, &_outFontCollection);

	// 불러온 font collection을 캐싱합니다.
	m_FontCollectionMap.emplace(_fileUri, _outFontCollection);


	return _outFontCollection;
}

TextFormat* Resources::CreateTextFormat(FontCollection* _fontCollection,
                                        float _fontSize, EFontWeight _fontWeight, EFontStyle _fontStyle,
                                        EFontStretch _fontStretch,
                                        tstring _localeName)
{
	TextFormat* _outTextFormat = nullptr;
	TextFormat::Create(&_outTextFormat,
	                   Core::GetDWriteFactory(), _fontCollection,
	                   _fontSize, _fontWeight, _fontStyle, _fontStretch,
	                   _localeName
	);

	return _outTextFormat;
}

ISprite* Resources::GetSpriteByName(const tstring& _name)
{
	auto _splitted = StringHelper::Split(_name, TEXT("::"));

	auto _sheet = LoadSpriteSheetFromFile(_splitted[0]);

	if (_sheet == nullptr)
		return nullptr;

	if (_splitted.size() == 1)
		return _sheet;

	auto _sprite = _sheet->GetSpriteByName(_splitted[1]);
	return _sprite;
}
