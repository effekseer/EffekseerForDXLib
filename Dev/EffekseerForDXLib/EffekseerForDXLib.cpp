﻿
#include "EffekseerForDXLib.h"
#include "Effekseer.Modules.h"
#include <array>
#include <assert.h>
#include <map>
#include <string>
#include <vector>

static ::Effekseer::SettingRef g_setting = nullptr;

static ::Effekseer::ManagerRef g_manager2d = nullptr;
static ::EffekseerRenderer::RendererRef g_renderer2d = nullptr;
static float g_time2d = 0.0f;

static ::Effekseer::ManagerRef g_manager3d = nullptr;
static ::EffekseerRenderer::RendererRef g_renderer3d = nullptr;
static float g_time3d = 0.0f;

static ::Effekseer::Backend::GraphicsDeviceRef g_graphicsDevice = nullptr;

static ::Effekseer::Server* g_server = nullptr;

static bool g_isDistortionEnabled = false;

static int32_t nextEffectHandle = 0;
static std::map<std::wstring, int32_t> effectFileNameToEffectHandle;
static std::map<int32_t, std::wstring> effectHandleToEffectFileName;
static std::map<int32_t, ::Effekseer::EffectRef> effectHandleToEffect;

static int32_t g_backgroundWidth = 0;
static int32_t g_backgroundHeight = 0;
static IDirect3DSurface9* g_dx9_backgroundSurface = nullptr;
static IDirect3DTexture9* g_dx9_backgroundTexture = nullptr;

static ID3D11Texture2D* g_dx11_backGroundTexture = nullptr;
static ID3D11ShaderResourceView* g_dx11_backGroundTextureSRV = nullptr;
static D3D11_TEXTURE2D_DESC g_dx11_backGroundTextureDesc;

Effekseer::FileInterface* g_effectFile = nullptr;

static EffekseerFileOpenFunc g_openFunc = nullptr;
static EffekseerFileReadSizeFunc g_readSizeFunc = nullptr;

static std::wstring ToWide(const char* pText)
{
	int Len = ::MultiByteToWideChar(CP_ACP, 0, pText, -1, NULL, 0);

	wchar_t* pOut = new wchar_t[Len + 1];
	::MultiByteToWideChar(CP_ACP, 0, pText, -1, pOut, Len);
	std::wstring Out(pOut);
	delete[] pOut;

	return Out;
}

static std::string ToMulti(const wchar_t* pText)
{
	int Len = ::WideCharToMultiByte(932, 0, pText, -1, NULL, 0, NULL, NULL);

	char* pOut = new char[Len + 1];
	::WideCharToMultiByte(932, 0, pText, -1, pOut, Len, NULL, NULL);
	std::string Out(pOut);
	delete[] pOut;

	return Out;
}

static bool AllocateBackgroundBuffer(int32_t sizeX, int32_t sizeY)
{
	if (sizeX == 0 || sizeY == 0)
	{
		return false;
	}

	LPDIRECT3DDEVICE9 dx9_device = (LPDIRECT3DDEVICE9)GetUseDirect3DDevice9();

	if (dx9_device != NULL)
	{
		HRESULT hr;

		hr = dx9_device->CreateTexture(
			sizeX, sizeY, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &g_dx9_backgroundTexture, NULL);
		if (FAILED(hr))
		{
			return false;
		}

		g_dx9_backgroundTexture->GetSurfaceLevel(0, &g_dx9_backgroundSurface);

		return true;
	}

	return false;
}

static void DeallocateBackgroundBuffer()
{
	ES_SAFE_RELEASE(g_dx9_backgroundTexture);
	ES_SAFE_RELEASE(g_dx9_backgroundSurface);
}

class EffekseerFileReader : public Effekseer::FileReader
{
	std::vector<uint8_t> data;
	int32_t position;

public:
	EffekseerFileReader(std::vector<uint8_t>& data)
	{
		this->data = data;
		position = 0;
	}

	virtual ~EffekseerFileReader() {}

	size_t Read(void* buffer, size_t size)
	{
		auto readable = size;
		if (data.size() - position < size)
			readable = data.size() - position;

		memcpy(buffer, &(data[position]), readable);
		position += static_cast<int>(readable);
		return readable;
	}

	void Seek(int position)
	{
		this->position = position;
		if (this->position < 0)
			this->position = 0;
		if (this->position > static_cast<int>(data.size()))
			this->position = static_cast<int>(data.size());
	}

	int GetPosition() { return position; }

	size_t GetLength() { return data.size(); }
};

class EffekseerFile : public Effekseer::FileInterface
{
public:
	EffekseerFile() {}
	virtual ~EffekseerFile() {}

	Effekseer::FileReader* OpenRead(const EFK_CHAR* path)
	{
		auto path_ = ToMulti((wchar_t*)path);

		auto fileHandle = g_openFunc(path_.c_str());

		if (fileHandle == 0)
			return 0;

		auto size = g_readSizeFunc(path_.c_str());
		std::vector<uint8_t> data;
		data.resize(static_cast<size_t>(size));
		FileRead_read(data.data(), static_cast<int>(size), fileHandle);
		FileRead_close(fileHandle);

		return new EffekseerFileReader(data);
	}

	Effekseer::FileWriter* OpenWrite(const EFK_CHAR* path) { return nullptr; }
};

static bool CopyRenderTargetToBackground()
{
	if (g_dx9_backgroundSurface == nullptr)
		return false;

	bool ret = false;

	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9)GetUseDirect3DDevice9();
	if (device == NULL)
		return nullptr;

	HRESULT hr;
	IDirect3DSurface9* tempRender = nullptr;
	IDirect3DSurface9* tempDepth = nullptr;

	hr = device->GetRenderTarget(0, &tempRender);
	if (FAILED(hr))
	{
		goto Exit;
	}

	hr = device->GetDepthStencilSurface(&tempDepth);
	if (FAILED(hr))
	{
		goto Exit;
	}

	device->SetRenderTarget(0, g_dx9_backgroundSurface);
	device->SetDepthStencilSurface(nullptr);

	D3DSURFACE_DESC desc;
	tempRender->GetDesc(&desc);
	const bool isSame = (desc.Width == g_backgroundWidth && desc.Height == g_backgroundHeight);

	device->StretchRect(tempRender, nullptr, g_dx9_backgroundSurface, nullptr, (isSame ? D3DTEXF_POINT : D3DTEXF_LINEAR));

	device->SetRenderTarget(0, tempRender);
	device->SetDepthStencilSurface(tempDepth);

	ret = true;

Exit:;
	ES_SAFE_RELEASE(tempRender);
	ES_SAFE_RELEASE(tempDepth);

	return ret;
}

bool PrepareTexture_DX11(uint32_t width, uint32_t height, DXGI_FORMAT format)
{
	ID3D11Device* dx11_device = (ID3D11Device*)GetUseDirect3D11Device();
	if (dx11_device == nullptr)
		return false;

	ES_SAFE_RELEASE(g_dx11_backGroundTextureSRV);
	ES_SAFE_RELEASE(g_dx11_backGroundTexture);

	ZeroMemory(&g_dx11_backGroundTextureDesc, sizeof(g_dx11_backGroundTextureDesc));
	g_dx11_backGroundTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	g_dx11_backGroundTextureDesc.Format = format;
	g_dx11_backGroundTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	g_dx11_backGroundTextureDesc.Width = width;
	g_dx11_backGroundTextureDesc.Height = height;
	g_dx11_backGroundTextureDesc.CPUAccessFlags = 0;
	g_dx11_backGroundTextureDesc.MipLevels = 1;
	g_dx11_backGroundTextureDesc.ArraySize = 1;
	g_dx11_backGroundTextureDesc.SampleDesc.Count = 1;
	g_dx11_backGroundTextureDesc.SampleDesc.Quality = 0;

	HRESULT hr = S_OK;
	hr = dx11_device->CreateTexture2D(&g_dx11_backGroundTextureDesc, nullptr, &g_dx11_backGroundTexture);
	if (FAILED(hr))
	{
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	switch (format)
	{
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		break;
	case DXGI_FORMAT_R16G16B16A16_TYPELESS:
		srvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		break;
	default:
		srvDesc.Format = format;
		break;
	}
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	hr = dx11_device->CreateShaderResourceView(g_dx11_backGroundTexture, &srvDesc, &g_dx11_backGroundTextureSRV);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

static void Effekseer_Distort()
{
	LPDIRECT3DDEVICE9 dx9_device = (LPDIRECT3DDEVICE9)GetUseDirect3DDevice9();
	ID3D11Device* dx11_device = (ID3D11Device*)GetUseDirect3D11Device();
	ID3D11DeviceContext* dx11_device_context = (ID3D11DeviceContext*)GetUseDirect3D11DeviceContext();

	if (dx9_device != nullptr)
	{
		if (GetUseDirect3DDevice9() == NULL)
			return;
		auto renderer2d = (EffekseerRendererDX9::Renderer*)g_renderer2d.Get();
		auto renderer3d = (EffekseerRendererDX9::Renderer*)g_renderer3d.Get();

		if (g_dx9_backgroundTexture == NULL)
		{
			g_renderer2d->SetBackground(NULL);
			g_renderer3d->SetBackground(NULL);
			return;
		}

		CopyRenderTargetToBackground();
		renderer2d->SetBackground(g_dx9_backgroundTexture);
		renderer3d->SetBackground(g_dx9_backgroundTexture);
	}

	if (dx11_device != nullptr)
	{
		auto renderer2d = (EffekseerRendererDX11::Renderer*)g_renderer2d.Get();
		auto renderer3d = (EffekseerRendererDX11::Renderer*)g_renderer3d.Get();

		if (g_dx11_backGroundTexture == nullptr)
		{
			g_renderer2d->SetBackground(NULL);
			g_renderer3d->SetBackground(NULL);
			return;
		}

		HRESULT hr = S_OK;

		ID3D11RenderTargetView* renderTargetView = nullptr;
		ID3D11Texture2D* renderTexture = nullptr;

		dx11_device_context->OMGetRenderTargets(1, &renderTargetView, nullptr);
		renderTargetView->GetResource(reinterpret_cast<ID3D11Resource**>(&renderTexture));

		// レンダーターゲット情報を取得
		D3D11_TEXTURE2D_DESC renderTextureDesc;
		renderTexture->GetDesc(&renderTextureDesc);

		// シザリング範囲を取得
		UINT numScissorRects = 1;
		D3D11_RECT scissorRect;
		dx11_device_context->RSGetScissorRects(&numScissorRects, &scissorRect);

		// 描画範囲を計算
		uint32_t width = g_backgroundWidth;
		uint32_t height = g_backgroundHeight;

		if (width == renderTextureDesc.Width && height == renderTextureDesc.Height)
		{
			// 背景テクスチャへコピー
			dx11_device_context->CopyResource(g_dx11_backGroundTexture, renderTexture);
		}
		else
		{
			// 背景テクスチャへ部分的コピー
			D3D11_BOX srcBox;
			srcBox.left = scissorRect.left;
			srcBox.top = scissorRect.top;
			srcBox.right = scissorRect.right;
			srcBox.bottom = scissorRect.bottom;
			srcBox.front = 0;
			srcBox.back = 1;
			dx11_device_context->CopySubresourceRegion(g_dx11_backGroundTexture, 0, 0, 0, 0, renderTexture, 0, &srcBox);
		}

		// 取得したリソースの参照カウンタを下げる
		ES_SAFE_RELEASE(renderTexture);
		ES_SAFE_RELEASE(renderTargetView);

		auto backgroundTexture = EffekseerRendererDX11::CreateTexture(g_graphicsDevice, g_dx11_backGroundTextureSRV, nullptr, nullptr);

		g_renderer2d->SetBackground(backgroundTexture);
		g_renderer3d->SetBackground(backgroundTexture);
	}
}

int Effekseer_Init(int particleMax, EffekseerFileOpenFunc openFunc, EffekseerFileReadSizeFunc readSizeFunc)
{
	return Effkseer_Init(particleMax, openFunc, readSizeFunc);
}

int Effkseer_Init(int particleMax, EffekseerFileOpenFunc openFunc, EffekseerFileReadSizeFunc readSizeFunc)
{
	LPDIRECT3DDEVICE9 dx9_device = (LPDIRECT3DDEVICE9)GetUseDirect3DDevice9();
	ID3D11Device* dx11_device = (ID3D11Device*)GetUseDirect3D11Device();
	ID3D11DeviceContext* dx11_device_context = (ID3D11DeviceContext*)GetUseDirect3D11DeviceContext();

	if (dx9_device == NULL && dx11_device == NULL)
		return -1;

	g_openFunc = openFunc;
	g_readSizeFunc = readSizeFunc;

	g_effectFile = new EffekseerFile();

	// 設定ファイルを作成する。
	g_setting = Effekseer::Setting::Create();

	// マネージャー(2D)を生成する。
	g_manager2d = ::Effekseer::Manager::Create(particleMax);

	// マネージャー(3D)を生成する。
	g_manager3d = ::Effekseer::Manager::Create(particleMax);

	// レンダラーを生成する。
	if (dx9_device != NULL)
	{
		g_graphicsDevice = ::EffekseerRendererDX9::CreateGraphicsDevice(dx9_device);
		g_renderer2d = ::EffekseerRendererDX9::Renderer::Create(g_graphicsDevice, particleMax);
		g_renderer3d = ::EffekseerRendererDX9::Renderer::Create(g_graphicsDevice, particleMax);
	}
	else
	{
		g_renderer2d = ::EffekseerRendererDX11::Renderer::Create(dx11_device, dx11_device_context, particleMax);
		g_renderer3d = ::EffekseerRendererDX11::Renderer::Create(dx11_device, dx11_device_context, particleMax);
		g_graphicsDevice = ::EffekseerRendererDX11::CreateGraphicsDevice(dx11_device, dx11_device_context);
	}

	// 描画方法を設定する。(2D)
	g_manager2d->SetSpriteRenderer(g_renderer2d->CreateSpriteRenderer());
	g_manager2d->SetRibbonRenderer(g_renderer2d->CreateRibbonRenderer());
	g_manager2d->SetRingRenderer(g_renderer2d->CreateRingRenderer());
	g_manager2d->SetModelRenderer(g_renderer2d->CreateModelRenderer());
	g_manager2d->SetTrackRenderer(g_renderer2d->CreateTrackRenderer());

	// 描画方法を設定する。(3D)
	g_manager3d->SetSpriteRenderer(g_renderer3d->CreateSpriteRenderer());
	g_manager3d->SetRibbonRenderer(g_renderer3d->CreateRibbonRenderer());
	g_manager3d->SetRingRenderer(g_renderer3d->CreateRingRenderer());
	g_manager3d->SetModelRenderer(g_renderer3d->CreateModelRenderer());
	g_manager3d->SetTrackRenderer(g_renderer3d->CreateTrackRenderer());

	// 設定を用意する。
	g_setting->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
	g_setting->SetEffectLoader(Effekseer::Effect::CreateEffectLoader(g_effectFile));

	g_setting->SetTextureLoader(EffekseerRenderer::CreateTextureLoader(g_graphicsDevice, g_effectFile));
	g_setting->SetModelLoader(EffekseerRenderer::CreateModelLoader(g_graphicsDevice, g_effectFile));

	g_setting->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>(g_effectFile));

	// HACK renderer経由でないAPIにいずれ置き換える。
	g_setting->SetMaterialLoader(g_renderer2d->CreateMaterialLoader(g_effectFile));

	// 設定を適用する。
	g_manager2d->SetSetting(g_setting);
	g_manager3d->SetSetting(g_setting);

	return 0;
}

int Effekseer_StartNetwork(int port) { return Effkseer_InitServer(port); }

int Effkseer_InitServer(int port)
{
	if (g_server != NULL)
		return -1;
	g_server = Effekseer::Server::Create();
	if (!g_server->Start(port))
	{
		ES_SAFE_DELETE(g_server);
		return -1;
	}

	return 0;
}

int Effekseer_InitDistortion(float scale)
{
	int sizeX, sizeY, colorBitDepth;

	if (GetScreenState(&sizeX, &sizeY, &colorBitDepth) != 0)
		return -1;

	sizeX = static_cast<int32_t>(sizeX * scale);
	sizeY = static_cast<int32_t>(sizeY * scale);

	LPDIRECT3DDEVICE9 dx9_device = (LPDIRECT3DDEVICE9)GetUseDirect3DDevice9();
	ID3D11Device* dx11_device = (ID3D11Device*)GetUseDirect3D11Device();
	ID3D11DeviceContext* dx11_device_context = (ID3D11DeviceContext*)GetUseDirect3D11DeviceContext();

	if (dx9_device != NULL)
	{
		DeallocateBackgroundBuffer();

		if (AllocateBackgroundBuffer(sizeX, sizeY))
		{
			g_backgroundWidth = sizeX;
			g_backgroundHeight = sizeY;
		}
		else
		{
			return -1;
		}
	}
	else if (dx11_device != NULL)
	{
		g_backgroundWidth = sizeX;
		g_backgroundHeight = sizeY;

		PrepareTexture_DX11(g_backgroundWidth, g_backgroundHeight, DXGI_FORMAT_R8G8B8A8_UNORM);
	}

	g_isDistortionEnabled = true;
	return 0;
}

void Effkseer_End()
{
	// サーバーを停止する
	if (g_server != NULL)
	{
		g_server->Stop();
		ES_SAFE_DELETE(g_server);
	}

	// 読み込まれたエフェクトを削除する。
	effectHandleToEffect.clear();

	// エフェクト管理用インスタンスを破棄する。
	g_manager2d.Reset();

	g_manager3d.Reset();

	// 描画用インスタンスを破棄する。
	g_renderer2d.Reset();

	g_renderer3d.Reset();

	ES_SAFE_DELETE(g_effectFile);

	DeallocateBackgroundBuffer();

	ES_SAFE_RELEASE(g_dx11_backGroundTexture);
	ES_SAFE_RELEASE(g_dx11_backGroundTextureSRV);

	g_graphicsDevice.Reset();

	g_setting.Reset();
}

void Effekseer_SetGraphicsDeviceLostCallbackFunctions()
{
	// デバイスロスト時のコールバックを設定する。(フルスクリーンウインドウ切り替えのために必要)
	SetGraphicsDeviceLostCallbackFunction(Effkseer_DeviceLost, NULL);
	SetGraphicsDeviceRestoreCallbackFunction(Effkseer_DeviceRestore, NULL);
}

void Effekseer_Set2DSetting(int windowWidth, int windowHeight)
{
	if (g_renderer2d == nullptr)
		return;

	// 投影行列を設定を設定する。
	g_renderer2d->SetProjectionMatrix(
		::Effekseer::Matrix44().OrthographicLH(static_cast<float>(windowWidth), static_cast<float>(windowHeight), 1.0f, 400.0f));

	// カメラ行列を設定
	g_renderer2d->SetCameraMatrix(::Effekseer::Matrix44().LookAtLH(::Effekseer::Vector3D(windowWidth / 2.0f, -windowHeight / 2.0f, -200.0f),
																   ::Effekseer::Vector3D(windowWidth / 2.0f, -windowHeight / 2.0f, 200.0f),
																   ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));
}

void Effekseer_Sync3DSetting()
{
	if (g_renderer3d == nullptr)
		return;

	MATRIX proj = GetCameraProjectionMatrix();
	MATRIX view = GetCameraViewMatrix();

	Effekseer::Matrix44 efproj, efview;

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			efproj.Values[j][i] = proj.m[j][i];
			efview.Values[j][i] = view.m[j][i];
		}
	}

	g_renderer3d->SetProjectionMatrix(efproj);
	g_renderer3d->SetCameraMatrix(efview);
}

int LoadEffekseerEffect(const char* fileName, float magnification)
{
	auto fileName_ = ToWide(fileName);
	return LoadEffekseerEffect(fileName_.c_str(), magnification);
}

int LoadEffekseerEffect(const wchar_t* fileName, float magnification)
{
	if (g_setting == nullptr)
		return -1;

	if (effectFileNameToEffectHandle.count(fileName) > 0)
	{
		return effectFileNameToEffectHandle[fileName];
	}

	auto effect = Effekseer::Effect::Create(g_setting, (const EFK_CHAR*)fileName, magnification);
	if (effect == nullptr)
		return -1;

	int32_t handle = nextEffectHandle;
	nextEffectHandle++;

	effectFileNameToEffectHandle[fileName] = handle;
	effectHandleToEffectFileName[handle] = fileName;
	effectHandleToEffect[handle] = effect;

	if (g_server != nullptr)
	{
		wchar_t _dir[_MAX_DIR];
		wchar_t _drive[_MAX_DRIVE];
		wchar_t _fileName[_MAX_FNAME];
		wchar_t _ext[_MAX_EXT];
		_wsplitpath_s(fileName, _drive, _dir, _fileName, _ext);
		g_server->Register((const EFK_CHAR*)_fileName, effect);
	}

	return handle;
}

int DeleteEffekseerEffect(int effectResourceHandle)
{
	if (effectHandleToEffectFileName.count(effectResourceHandle) > 0)
	{
		auto fileName = effectHandleToEffectFileName[effectResourceHandle];
		auto effect = effectHandleToEffect[effectResourceHandle];
		effectFileNameToEffectHandle.erase(fileName);
		effectHandleToEffect.erase(effectResourceHandle);
		effectHandleToEffectFileName.erase(effectResourceHandle);

		if (g_server != nullptr)
		{
			g_server->Unregister(effect);
		}
	}

	return -1;
}

int PlayEffekseer2DEffect(int effectResourceHandle)
{
	if (g_manager2d == nullptr)
		return -1;

	if (effectHandleToEffect.count(effectResourceHandle) > 0)
	{
		auto effect = effectHandleToEffect[effectResourceHandle];
		return g_manager2d->Play(effect, 0, 0, 0);
	}

	return -1;
}

int PlayEffekseer3DEffect(int effectResourceHandle)
{
	if (g_manager3d == nullptr)
		return -1;

	if (effectHandleToEffect.count(effectResourceHandle) > 0)
	{
		auto effect = effectHandleToEffect[effectResourceHandle];
		return g_manager3d->Play(effect, 0, 0, 0);
	}

	return -1;
}

int IsEffekseer2DEffectPlaying(int playingEffectHandle)
{
	if (g_manager2d == nullptr)
		return -1;

	return g_manager2d->Exists(playingEffectHandle) ? 0 : -1;
}

int IsEffekseer3DEffectPlaying(int playingEffectHandle)
{
	if (g_manager3d == nullptr)
		return -1;

	return g_manager3d->Exists(playingEffectHandle) ? 0 : -1;
}

int StopEffekseer2DEffect(int playingEffecHandle)
{
	if (g_manager2d == nullptr)
		return -1;

	g_manager2d->StopEffect(playingEffecHandle);

	return 0;
}

int StopEffekseer3DEffect(int playingEffecHandle)
{
	if (g_manager3d == nullptr)
		return -1;

	g_manager3d->StopEffect(playingEffecHandle);

	return 0;
}

int SetPosPlayingEffekseer2DEffect(int playingEffectHandle, float x, float y, float z)
{
	if (g_manager2d == nullptr)
		return -1;

	g_manager2d->SetLocation(playingEffectHandle, ::Effekseer::Vector3D(x, -y, z));

	return 0;
}

int SetPosPlayingEffekseer3DEffect(int playingEffectHandle, float x, float y, float z)
{
	if (g_manager3d == nullptr)
		return -1;

	g_manager3d->SetLocation(playingEffectHandle, ::Effekseer::Vector3D(x, y, z));

	return 0;
}

int SetRotationPlayingEffekseer2DEffect(int playingEffectHandle, float x, float y, float z)
{
	if (g_manager2d == nullptr)
		return -1;

	g_manager2d->SetRotation(playingEffectHandle, -x, -y, -z);

	return 0;
}

int SetRotationPlayingEffekseer3DEffect(int playingEffectHandle, float x, float y, float z)
{
	if (g_manager3d == nullptr)
		return -1;

	g_manager3d->SetRotation(playingEffectHandle, x, y, z);

	return 0;
}

int SetScalePlayingEffekseer2DEffect(int playingEffectHandle, float x, float y, float z)
{
	if (g_manager2d == nullptr)
		return -1;
	g_manager2d->SetScale(playingEffectHandle, x, y, z);
	return 0;
}

int SetScalePlayingEffekseer3DEffect(int playingEffectHandle, float x, float y, float z)
{
	if (g_manager3d == nullptr)
		return -1;
	g_manager3d->SetScale(playingEffectHandle, x, y, z);
	return 0;
}

float GetSpeedPlayingEffekseer2DEffect(int playingEffectHandle)
{
	if (g_manager2d == nullptr)
		return 0;

	g_manager2d->GetSpeed(playingEffectHandle);

	return 0;
}

int SetSpeedPlayingEffekseer2DEffect(int playingEffectHandle, float speed)
{
	if (g_manager2d == nullptr)
		return -1;

	g_manager2d->SetSpeed(playingEffectHandle, speed);

	return 0;
}

float GetSpeedPlayingEffekseer3DEffect(int playingEffectHandle)
{
	if (g_manager3d == nullptr)
		return 0;

	g_manager3d->GetSpeed(playingEffectHandle);

	return 0;
}

int SetSpeedPlayingEffekseer3DEffect(int playingEffectHandle, float speed)
{
	if (g_manager3d == nullptr)
		return -1;

	g_manager3d->SetSpeed(playingEffectHandle, speed);

	return 0;
}

int SetColorPlayingEffekseer2DEffect(int playingEffectHandle, int r, int g, int b, int a)
{
	if (g_manager2d == nullptr)
		return -1;

	g_manager2d->SetAllColor(playingEffectHandle, Effekseer::Color(r, g, b, a));

	return 0;
}

int SetColorPlayingEffekseer3DEffect(int playingEffectHandle, int r, int g, int b, int a)
{
	if (g_manager3d == nullptr)
		return -1;

	g_manager3d->SetAllColor(playingEffectHandle, Effekseer::Color(r, g, b, a));

	return 0;
}

float GetDynamicInput2DEffect(int playingEffectHandle, int32_t index)
{
	if (g_manager2d == nullptr)
		return 0.0f;

	return g_manager2d->GetDynamicInput(playingEffectHandle, index);
}

void SetDynamicInput2DEffect(int playingEffectHandle, int32_t index, float value)
{
	if (g_manager2d == nullptr)
		return;

	g_manager2d->SetDynamicInput(playingEffectHandle, index, value);
}

float GetDynamicInput3DEffect(int playingEffectHandle, int32_t index)
{
	if (g_manager3d == nullptr)
		return 0.0f;

	return g_manager3d->GetDynamicInput(playingEffectHandle, index);
}

void SetDynamicInput3DEffect(int playingEffectHandle, int32_t index, float value)
{
	if (g_manager3d == nullptr)
		return;

	g_manager3d->SetDynamicInput(playingEffectHandle, index, value);
}

int UpdateEffekseer2D()
{
	if (g_server != nullptr)
	{
		std::array<Effekseer::ManagerRef, 2> managers;
		managers[0] = g_manager2d;
		managers[1] = g_manager3d;

		g_server->Update(managers.data(), static_cast<int>(managers.size()));
	}

	if (g_manager2d == nullptr)
		return -1;
	g_manager2d->Update();

	g_renderer2d->SetTime(g_time2d);
	g_time2d += 1.0f / 60.0f;
	return 0;
}

int DrawEffekseer2D()
{
	if (g_manager2d == nullptr)
		return -1;

	// 頂点バッファに溜まった頂点データを吐き出す。
	RenderVertex();

	// 歪みの処理を行う
	if (g_isDistortionEnabled)
	{
		Effekseer_Distort();
	}

	// エフェクトの描画開始処理を行う。
	g_renderer2d->BeginRendering();

	// エフェクトの描画を行う。
	g_manager2d->DrawBack();

	if (g_isDistortionEnabled)
	{
		g_renderer2d->EndRendering();

		Effekseer_Distort();

		g_renderer2d->BeginRendering();
	}

	g_manager2d->DrawFront();

	// エフェクトの描画終了処理を行う。
	g_renderer2d->EndRendering();

	// DXライブラリの設定を戻す。
	RefreshDxLibDirect3DSetting();

	return 0;
}

int DrawEffekseer2D_Begin()
{
	if (g_manager2d == nullptr)
		return -1;

	// 頂点バッファに溜まった頂点データを吐き出す。
	RenderVertex();

	// 歪みの処理を行う
	if (g_isDistortionEnabled)
	{
		Effekseer_Distort();
	}

	// エフェクトの描画開始処理を行う。
	g_renderer2d->BeginRendering();

	return 0;
}

int DrawEffekseer2D_Draw(int playingEffectHandle)
{
	if (g_manager2d == nullptr)
		return -1;
	g_manager2d->DrawHandle(playingEffectHandle);

	return 0;
}

int DrawEffekseer2D_End()
{
	if (g_manager2d == nullptr)
		return -1;

	// エフェクトの描画終了処理を行う。
	g_renderer2d->EndRendering();

	// DXライブラリの設定を戻す。
	RefreshDxLibDirect3DSetting();

	return 0;
}

int UpdateEffekseer3D()
{
	if (g_server != nullptr)
	{
		std::array<Effekseer::ManagerRef, 2> managers;
		managers[0] = g_manager2d;
		managers[1] = g_manager3d;

		g_server->Update(managers.data(), static_cast<int>(managers.size()));
	}

	if (g_manager3d == nullptr)
		return -1;
	g_manager3d->Update();

	g_renderer3d->SetTime(g_time3d);
	g_time3d += 1.0f / 60.0f;

	return 0;
}

int DrawEffekseer3D()
{
	if (g_manager3d == nullptr)
		return -1;

	// 頂点バッファに溜まった頂点データを吐き出す。
	RenderVertex();

	// 歪みの処理を行う
	if (g_isDistortionEnabled)
	{
		Effekseer_Distort();
	}

	// エフェクトの描画開始処理を行う。
	g_renderer3d->BeginRendering();

	// エフェクトの描画を行う。
	g_manager3d->DrawBack();

	if (g_isDistortionEnabled)
	{
		g_renderer3d->EndRendering();

		Effekseer_Distort();

		g_renderer3d->BeginRendering();
	}

	g_manager3d->DrawFront();

	// エフェクトの描画終了処理を行う。
	g_renderer3d->EndRendering();

	// DXライブラリの設定を戻す。
	RefreshDxLibDirect3DSetting();

	return 0;
}

int DrawEffekseer3D_Begin()
{
	if (g_manager3d == nullptr)
		return -1;

	// 頂点バッファに溜まった頂点データを吐き出す。
	RenderVertex();

	// 歪みの処理を行う
	if (g_isDistortionEnabled)
	{
		Effekseer_Distort();
	}

	// エフェクトの描画開始処理を行う。
	g_renderer3d->BeginRendering();

	return 0;
}

int DrawEffekseer3D_Draw(int playingEffectHandle)
{
	if (g_manager3d == nullptr)
		return -1;
	g_manager3d->DrawHandle(playingEffectHandle);

	return 0;
}

int DrawEffekseer3D_End()
{
	if (g_manager3d == nullptr)
		return -1;

	// エフェクトの描画終了処理を行う。
	g_renderer3d->EndRendering();

	// DXライブラリの設定を戻す。
	RefreshDxLibDirect3DSetting();

	return 0;
}

::Effekseer::ManagerRef GetEffekseer2DManager() { return g_manager2d; }

::EffekseerRenderer::RendererRef GetEffekseer2DRenderer() { return g_renderer2d; }

::Effekseer::ManagerRef GetEffekseer3DManager() { return g_manager3d; }

::EffekseerRenderer::RendererRef GetEffekseer3DRenderer() { return g_renderer3d; }

::Effekseer::EffectRef GetEffekseerEffect(int effectHandle)
{
	if (effectHandleToEffect.count(effectHandle) > 0)
	{
		return effectHandleToEffect[effectHandle];
	}

	return NULL;
}

void Effkseer_DeviceLost(void* data)
{
	if (GetUseDirect3DDevice9() == NULL)
		return;

	g_renderer2d->SetBackground(nullptr);
	g_renderer3d->SetBackground(nullptr);

	DeallocateBackgroundBuffer();

	// デバイスロストが発生した時に呼ぶ。
	g_renderer2d->OnLostDevice();
	g_renderer3d->OnLostDevice();

	// 読み込んだエフェクトのリソースは全て破棄する。
	for (auto e : effectHandleToEffect)
	{
		auto effect = e.second;
		effect->UnloadResources();
	}

	// DXライブラリは内部でデバイス自体を消去しているのでNULLを設定する。
	auto renderer2d = (EffekseerRendererDX9::Renderer*)g_renderer2d.Get();
	auto renderer3d = (EffekseerRendererDX9::Renderer*)g_renderer3d.Get();
	renderer2d->ChangeDevice(NULL);
	renderer3d->ChangeDevice(NULL);
}

void Effkseer_DeviceRestore(void* data)
{
	if (GetUseDirect3DDevice9() == NULL)
		return;

	// DXライブラリは回復時に内部でデバイスを再生成するので新しく設定する。
	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9)GetUseDirect3DDevice9();

	auto renderer2d = (EffekseerRendererDX9::Renderer*)g_renderer2d.Get();
	auto renderer3d = (EffekseerRendererDX9::Renderer*)g_renderer3d.Get();
	renderer2d->ChangeDevice(device);
	renderer3d->ChangeDevice(device);

	// エフェクトのリソースを再読み込みする。
	for (auto e : effectHandleToEffect)
	{
		auto effect = e.second;
		effect->ReloadResources();
	}

	// デバイスが復帰するときに呼ぶ
	g_renderer2d->OnResetDevice();
	g_renderer3d->OnResetDevice();

	AllocateBackgroundBuffer(g_backgroundWidth, g_backgroundHeight);
	if (g_dx9_backgroundTexture == nullptr)
	{
		g_backgroundWidth = 0;
		g_backgroundHeight = 0;
		g_isDistortionEnabled = false;
	}
	else
	{
		renderer2d->SetBackground(g_dx9_backgroundTexture);
		renderer3d->SetBackground(g_dx9_backgroundTexture);
	}
}