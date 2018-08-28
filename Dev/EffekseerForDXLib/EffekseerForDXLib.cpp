
#include "EffekseerForDXLib.h"

#include <map>
#include <string>
#include <vector>

static ::Effekseer::Manager*				g_manager2d = NULL;
static ::EffekseerRenderer::Renderer*		g_renderer2d = NULL;

static ::Effekseer::Manager*				g_manager3d = NULL;
static ::EffekseerRenderer::Renderer*		g_renderer3d = NULL;

static ::Effekseer::Server*					g_server = NULL;

static bool									g_isDistortionEnabled = false;

static int32_t nextEffectHandle = 0;
static std::map<std::wstring, int32_t>			effectFileNameToEffectHandle;
static std::map<int32_t, std::wstring>			effectHandleToEffectFileName;
static std::map<int32_t, ::Effekseer::Effect*>	effectHandleToEffect;

static int32_t								g_backgroundWidth = 0;
static int32_t								g_backgroundHeight = 0;
static IDirect3DSurface9*					g_dx9_backgroundSurface = nullptr;
static IDirect3DTexture9*					g_dx9_backgroundTexture = nullptr;

static ID3D11Texture2D* g_dx11_backGroundTexture = nullptr;
static ID3D11ShaderResourceView* g_dx11_backGroundTextureSRV = nullptr;
static D3D11_TEXTURE2D_DESC g_dx11_backGroundTextureDesc;

Effekseer::FileInterface*					g_effectFile = nullptr;

static EffekseerFileOpenFunc					g_openFunc = nullptr;
static EffekseerFileReadSizeFunc				g_readSizeFunc = nullptr;

static std::wstring ToWide(const char* pText)
{
	int Len = ::MultiByteToWideChar(CP_ACP, 0, pText, -1, NULL, 0);

	wchar_t* pOut = new wchar_t[Len + 1];
	::MultiByteToWideChar(CP_ACP, 0, pText, -1, pOut, Len);
	std::wstring Out(pOut);
	delete [] pOut;

	return Out;
}

static std::string ToMulti(const wchar_t* pText)
{
	int Len = ::WideCharToMultiByte(932, 0,  pText, -1, NULL, 0, NULL, NULL);

	char* pOut = new char[Len + 1];
	::WideCharToMultiByte(932, 0, pText, -1, pOut, Len, NULL, NULL);
	std::string Out(pOut);
	delete [] pOut;

	return Out;
}

class EffekseerFileReader :
	public Effekseer::FileReader
{
	std::vector<uint8_t>	data;
	int32_t					position;
public:
	EffekseerFileReader(std::vector<uint8_t>& data)
	{
		this->data = data;
		position = 0;
	}

	virtual ~EffekseerFileReader() {}

	size_t Read( void* buffer, size_t size )
	{
		int32_t readable = size;
		if(data.size() - position < size) readable = data.size() - position;
	
		memcpy(buffer, &(data[position]), readable);
		position += readable;
		return readable;
	}

	void Seek(int position)
	{
		this->position = position;
		if(this->position < 0) this->position = 0;
		if(this->position > data.size()) this->position = data.size();
	}

	int GetPosition()
	{
		return position;
	}

	size_t GetLength()
	{
		return data.size();
	}
};

class EffekseerFile :
	public Effekseer::FileInterface
{
public:
	EffekseerFile() {}
	virtual ~EffekseerFile() {}

	Effekseer::FileReader* OpenRead( const EFK_CHAR* path ) 
	{
		auto path_ = ToMulti((wchar_t*)path);
		
		auto fileHandle = g_openFunc(path_.c_str());

		if(fileHandle == 0) return 0;
		
		auto size = g_readSizeFunc(path_.c_str());
		std::vector<uint8_t> data;
		data.resize(size);
		FileRead_read(data.data(), size, fileHandle);
		FileRead_close(fileHandle);

		return new EffekseerFileReader(data);
	}

	Effekseer::FileWriter* OpenWrite( const EFK_CHAR* path )
	{
		return nullptr;
	}
};


class CachedModelLoader
	: public ::Effekseer::ModelLoader
{
private:
	struct CachedModel
	{
		void*						DataPtr;
		int32_t						Count;

		CachedModel()
		{
			DataPtr = NULL;
			Count = 1;
		}
	};

	::Effekseer::ModelLoader*		modelLoader;
	std::map<std::basic_string<EFK_CHAR>, CachedModel>	cache;
	std::map<void*, std::basic_string<EFK_CHAR>>	data2key;

public:

	CachedModelLoader(::Effekseer::ModelLoader* modelLoader)
	{
		this->modelLoader = modelLoader;
	}

	virtual ~CachedModelLoader()
	{
		ES_SAFE_DELETE(modelLoader);
	}


	virtual void* Load(const EFK_CHAR* path) override
	{
		auto key = std::basic_string<EFK_CHAR>(path);

		auto it = cache.find(key);

		if (it != cache.end())
		{
			it->second.Count++;
			return it->second.DataPtr;
		}

		CachedModel v;
		v.DataPtr = modelLoader->Load(path);

		if (v.DataPtr != nullptr)
		{
			cache[key] = v;
			data2key[v.DataPtr] = key;
		}

		return v.DataPtr;
	}

	virtual void Unload(void* data) override
	{
		if (data == nullptr) return;
		auto key = data2key[data];

		auto it = cache.find(key);

		if (it != cache.end())
		{
			it->second.Count--;
			if (it->second.Count == 0)
			{
				modelLoader->Unload(it->second.DataPtr);
				data2key.erase(data);
				cache.erase(key);
			}
		}
	}
};

class CachedTextureLoader
	: public ::Effekseer::TextureLoader
{
private:
	struct CachedTexture
	{
		::Effekseer::TextureData*	DataPtr;
		int32_t						Count;

		CachedTexture()
		{
			DataPtr = NULL;
			Count = 1;
		}
	};

	::Effekseer::TextureLoader*		textureLoader;
	std::map<std::basic_string<EFK_CHAR>, CachedTexture>	cache;
	std::map<::Effekseer::TextureData*, std::basic_string<EFK_CHAR>>	data2key;

public:

	CachedTextureLoader(::Effekseer::TextureLoader* textureLoader)
	{
		this->textureLoader = textureLoader;
	}

	virtual ~CachedTextureLoader()
	{
		ES_SAFE_DELETE(textureLoader);
	}


	virtual ::Effekseer::TextureData* Load(const EFK_CHAR* path, ::Effekseer::TextureType textureType) override
	{ 
		auto key = std::basic_string<EFK_CHAR>(path);

		auto it = cache.find(key);

		if (it != cache.end())
		{
			it->second.Count++;
			return it->second.DataPtr;
		}

		CachedTexture v;
		v.DataPtr = textureLoader->Load(path, textureType);
		
		if (v.DataPtr != nullptr)
		{
			cache[key] = v;
			data2key[v.DataPtr] = key;
		}

		return v.DataPtr;
	}

	virtual void Unload(::Effekseer::TextureData* data) override
	{
		if (data == nullptr) return;
		auto key = data2key[data];

		auto it = cache.find(key);
	
		if (it != cache.end())
		{
			it->second.Count--;
			if (it->second.Count == 0)
			{
				textureLoader->Unload(it->second.DataPtr);
				data2key.erase(data);
				cache.erase(key);
			}
		}
	}
};

static bool CopyRenderTargetToBackground()
{
	bool ret = false;

	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9)GetUseDirect3DDevice9();
	if (device == NULL) return nullptr;

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

	device->StretchRect(
		tempRender,
		nullptr,
		g_dx9_backgroundSurface,
		nullptr,
		(isSame ? D3DTEXF_POINT : D3DTEXF_LINEAR)
		);

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
	if (dx11_device == nullptr) return false;

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
		if (GetUseDirect3DDevice9() == NULL) return;
		auto renderer2d = (EffekseerRendererDX9::Renderer*)g_renderer2d;
		auto renderer3d = (EffekseerRendererDX9::Renderer*)g_renderer3d;

		if (g_dx9_backgroundTexture == NULL)
		{
			renderer2d->SetBackground(NULL);
			renderer3d->SetBackground(NULL);
			return;
		}

		CopyRenderTargetToBackground();
		renderer2d->SetBackground(g_dx9_backgroundTexture);
		renderer3d->SetBackground(g_dx9_backgroundTexture);
	}

	if (dx11_device != nullptr)
	{
		auto renderer2d = (EffekseerRendererDX11::Renderer*)g_renderer2d;
		auto renderer3d = (EffekseerRendererDX11::Renderer*)g_renderer3d;

		if (g_dx11_backGroundTexture == nullptr)
		{
			renderer2d->SetBackground(NULL);
			renderer3d->SetBackground(NULL);
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

		if (width == renderTextureDesc.Width &&
			height == renderTextureDesc.Height)
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

		renderer2d->SetBackground(g_dx11_backGroundTextureSRV);
		renderer3d->SetBackground(g_dx11_backGroundTextureSRV);
	}

}

int Effkseer_Init(int particleMax, 
	EffekseerFileOpenFunc openFunc,
	EffekseerFileReadSizeFunc readSizeFunc)
{
	LPDIRECT3DDEVICE9 dx9_device = (LPDIRECT3DDEVICE9) GetUseDirect3DDevice9();
	ID3D11Device* dx11_device = (ID3D11Device*)GetUseDirect3D11Device();
	ID3D11DeviceContext* dx11_device_context = (ID3D11DeviceContext*)GetUseDirect3D11DeviceContext();

	if (dx9_device == NULL && dx11_device == NULL) return -1;

	g_openFunc = openFunc;
	g_readSizeFunc = readSizeFunc;

	g_effectFile = new EffekseerFile();

	// レンダラー(2D)を生成する。
	if(dx9_device != NULL)
	{
		g_renderer2d = ::EffekseerRendererDX9::Renderer::Create(dx9_device, particleMax);
	}
	else
	{
		g_renderer2d = ::EffekseerRendererDX11::Renderer::Create(dx11_device, dx11_device_context, particleMax);
	}

	// マネージャー(2D)を生成する。
	g_manager2d = ::Effekseer::Manager::Create(particleMax);
	g_manager2d->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
	g_manager2d->SetEffectLoader(Effekseer::Effect::CreateEffectLoader(g_effectFile));

	// レンダラー(3D)を生成する。
	if(dx9_device != NULL)
	{
		g_renderer3d = ::EffekseerRendererDX9::Renderer::Create(dx9_device, particleMax);
	}
	else
	{
		g_renderer3d = ::EffekseerRendererDX11::Renderer::Create(dx11_device, dx11_device_context, particleMax);
	}
	
	// マネージャー(3D)を生成する。
	g_manager3d = ::Effekseer::Manager::Create(particleMax);
	g_manager3d->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
	g_manager3d->SetEffectLoader(Effekseer::Effect::CreateEffectLoader(g_effectFile));

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

	// 描画用インスタンスからテクスチャの読込機能を設定する。(2D)
	g_manager2d->SetTextureLoader(new CachedTextureLoader(g_renderer2d->CreateTextureLoader(g_effectFile)));
	g_manager2d->SetModelLoader(new CachedModelLoader(g_renderer2d->CreateModelLoader(g_effectFile)));

	// 描画用インスタンスからテクスチャの読込機能を設定する。(3D)
	g_manager3d->SetTextureLoader(new  CachedTextureLoader(g_renderer3d->CreateTextureLoader(g_effectFile)));
	g_manager3d->SetModelLoader(new CachedModelLoader(g_renderer3d->CreateModelLoader(g_effectFile)));

	return 0;
}

int Effkseer_InitServer(int port)
{
	if (g_server != NULL) return -1;
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

	if (GetScreenState(&sizeX, &sizeY, &colorBitDepth) != 0) return -1;

	sizeX *= scale;
	sizeY *= scale;

	LPDIRECT3DDEVICE9 dx9_device = (LPDIRECT3DDEVICE9) GetUseDirect3DDevice9();
	ID3D11Device* dx11_device = (ID3D11Device*)GetUseDirect3D11Device();
	ID3D11DeviceContext* dx11_device_context = (ID3D11DeviceContext*)GetUseDirect3D11DeviceContext();

	if (dx9_device != NULL)
	{
		ES_SAFE_RELEASE(g_dx9_backgroundTexture);
		ES_SAFE_RELEASE(g_dx9_backgroundSurface);

		HRESULT hr;

		hr = dx9_device->CreateTexture(
			sizeX,
			sizeY,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT,
			&g_dx9_backgroundTexture,
			NULL
			);
		if (FAILED(hr))
		{
			return -1;
		}

		g_backgroundWidth = sizeX;
		g_backgroundHeight = sizeY;
		g_dx9_backgroundTexture->GetSurfaceLevel(0, &g_dx9_backgroundSurface);
	}
	else if(dx11_device != NULL)
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
	for (auto e : effectHandleToEffect)
	{
		auto effect = e.second;
		ES_SAFE_RELEASE(effect);
	}
	effectHandleToEffect.clear();

	// エフェクト管理用インスタンスを破棄する。
	g_manager2d->Destroy();

	g_manager3d->Destroy();

	// 描画用インスタンスを破棄する。
	g_renderer2d->Destroy();

	g_renderer3d->Destroy();

	ES_SAFE_DELETE(g_effectFile);

	ES_SAFE_RELEASE(g_dx9_backgroundTexture);
	ES_SAFE_RELEASE(g_dx9_backgroundSurface);

	ES_SAFE_RELEASE(g_dx11_backGroundTexture);
	ES_SAFE_RELEASE(g_dx11_backGroundTextureSRV);
}

void Effekseer_SetGraphicsDeviceLostCallbackFunctions()
{
	// デバイスロスト時のコールバックを設定する。(フルスクリーンウインドウ切り替えのために必要)
	SetGraphicsDeviceLostCallbackFunction(Effkseer_DeviceLost, NULL);
	SetGraphicsDeviceRestoreCallbackFunction(Effkseer_DeviceRestore, NULL);
}


void Effekseer_Set2DSetting(int windowWidth, int windowHeight)
{
	if(g_renderer2d == nullptr) return;

	// 投影行列を設定を設定する。
	g_renderer2d->SetProjectionMatrix(
		::Effekseer::Matrix44().OrthographicLH(windowWidth, windowHeight, 1.0f, 400.0f));

	// カメラ行列を設定
	g_renderer2d->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtLH(
		::Effekseer::Vector3D( windowWidth / 2.0f, - windowHeight / 2.0f, -200.0f), 
		::Effekseer::Vector3D( windowWidth / 2.0f, - windowHeight / 2.0f, 200.0f), 
		::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));
}

void Effekseer_Sync3DSetting()
{
	if(g_renderer3d == nullptr) return;

	MATRIX proj = GetCameraProjectionMatrix();
	MATRIX view = GetCameraViewMatrix();

	Effekseer::Matrix44 efproj, efview;

	for(int i=0;i<4;++i){
		for(int j=0;j<4;++j){
			efproj.Values[j][i] = proj.m[j][i];
			efview.Values[j][i] = view.m[j][i];
		}
	}

	g_renderer3d->SetProjectionMatrix(efproj);
	g_renderer3d->SetCameraMatrix(efview);
}

int LoadEffekseerEffect(const char* fileName)
{
	auto fileName_ = ToWide(fileName);
	return LoadEffekseerEffect(fileName_.c_str());
}

int LoadEffekseerEffect(const wchar_t* fileName)
{
	if (g_manager2d == nullptr) return -1;

	if (effectFileNameToEffectHandle.count(fileName) > 0)
	{
		return effectFileNameToEffectHandle[fileName];
	}

	auto effect = Effekseer::Effect::Create(g_manager2d, (const EFK_CHAR*) fileName);
	if (effect == nullptr) return -1;

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
		g_server->Regist((const EFK_CHAR*) _fileName, effect);
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
			g_server->Unregist(effect);
		}

		ES_SAFE_RELEASE(effect);
	}
	
	return -1;
}

int PlayEffekseer2DEffect(int effectResourceHandle)
{
	if (g_manager2d == nullptr) return -1;

	if (effectHandleToEffect.count(effectResourceHandle) > 0)
	{
		auto effect = effectHandleToEffect[effectResourceHandle];
		return g_manager2d->Play(effect, 0, 0, 0);
	}

	return -1;
}

int PlayEffekseer3DEffect(int effectResourceHandle)
{
	if (g_manager3d == nullptr) return -1;

	if (effectHandleToEffect.count(effectResourceHandle) > 0)
	{
		auto effect = effectHandleToEffect[effectResourceHandle];
		return g_manager3d->Play(effect, 0, 0, 0);
	}

	return -1;
}

int IsEffekseer2DEffectPlaying(int playingEffectHandle)
{
	if (g_manager2d == nullptr) return -1;
	
	return g_manager2d->Exists(playingEffectHandle) ? 0 : -1;
}

int IsEffekseer3DEffectPlaying(int playingEffectHandle)
{
	if (g_manager3d == nullptr) return -1;
	
	return g_manager3d->Exists(playingEffectHandle) ? 0 : -1;
}

int StopEffekseer2DEffect(int playingEffecHandle)
{
		if (g_manager2d == nullptr) return -1;
	
		g_manager2d->StopEffect(playingEffecHandle);

		return 0;
}

int StopEffekseer3DEffect(int playingEffecHandle)
{
		if (g_manager3d == nullptr) return -1;
	
		g_manager3d->StopEffect(playingEffecHandle);

		return 0;
}

int SetPosPlayingEffekseer2DEffect(int playingEffectHandle, float x, float y, float z)
{
	if (g_manager2d == nullptr) return -1;

	g_manager2d->SetLocation(playingEffectHandle, ::Effekseer::Vector3D(x, -y, z));

	return 0;
}

int SetPosPlayingEffekseer3DEffect(int playingEffectHandle, float x, float y, float z)
{
	if (g_manager3d == nullptr) return -1;
	
	g_manager3d->SetLocation(playingEffectHandle, ::Effekseer::Vector3D(x, y, z));
	
	return 0;
}

int SetRotationPlayingEffekseer2DEffect(int playingEffectHandle, float x, float y, float z)
{
	if (g_manager2d == nullptr) return -1;

	g_manager2d->SetRotation(playingEffectHandle, -x, -y, -z);

	return 0;
}

int SetRotationPlayingEffekseer3DEffect(int playingEffectHandle, float x, float y, float z)
{
	if (g_manager3d == nullptr) return -1;

	g_manager3d->SetRotation(playingEffectHandle, x, y, z);

	return 0;
}

int SetScalePlayingEffekseer2DEffect(int playingEffectHandle, float x, float y, float z)
{
	if (g_manager2d == nullptr) return -1;
	g_manager2d->SetScale(playingEffectHandle, x, y, z);
	return 0;
}

int SetScalePlayingEffekseer3DEffect(int playingEffectHandle, float x, float y, float z)
{
	if (g_manager3d == nullptr) return -1;
	g_manager3d->SetScale(playingEffectHandle, x, y, z);
	return 0;
}

float GetSpeedPlayingEffekseer2DEffect(int playingEffectHandle)
{
	if (g_manager2d == nullptr) return 0;

	g_manager2d->GetSpeed(playingEffectHandle);

	return 0;
}

int SetSpeedPlayingEffekseer2DEffect(int playingEffectHandle, float speed)
{
	if (g_manager2d == nullptr) return -1;

	g_manager2d->SetSpeed(playingEffectHandle, speed);

	return 0;
}

float GetSpeedPlayingEffekseer3DEffect(int playingEffectHandle)
{
	if (g_manager3d == nullptr) return 0;

	g_manager3d->GetSpeed(playingEffectHandle);

	return 0;
}

int SetSpeedPlayingEffekseer3DEffect(int playingEffectHandle, float speed)
{
	if (g_manager3d == nullptr) return -1;

	g_manager3d->SetSpeed(playingEffectHandle, speed);

	return 0;
}

int SetColorPlayingEffekseer2DEffect(int playingEffectHandle, int r, int g, int b, int a)
{
	if (g_manager2d == nullptr) return -1;

	g_manager2d->SetAllColor(playingEffectHandle, Effekseer::Color(r, g, b, a));

	return 0;
}

int SetColorPlayingEffekseer3DEffect(int playingEffectHandle, int r, int g, int b, int a)
{
	if (g_manager3d == nullptr) return -1;

	g_manager3d->SetAllColor(playingEffectHandle, Effekseer::Color(r, g, b, a));

	return 0;
}

int UpdateEffekseer2D()
{
	if (g_server != nullptr)
	{
		g_server->Update();
	}

	if (g_manager2d == nullptr) return -1;
	g_manager2d->Update();
	return 0;
}

int DrawEffekseer2D()
{
	if (g_manager2d == nullptr) return -1;
	
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
	if (g_manager2d == nullptr) return -1;
	
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
	if (g_manager2d == nullptr) return -1;
	g_manager2d->DrawHandle(playingEffectHandle);

	return 0;
}

int DrawEffekseer2D_End()
{
	if (g_manager2d == nullptr) return -1;
	
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
		g_server->Update();
	}

	if (g_manager3d == nullptr) return -1;
	g_manager3d->Update();
	return 0;
}

int DrawEffekseer3D()
{
	if (g_manager3d == nullptr) return -1;
	
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
	if (g_manager3d == nullptr) return -1;

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
	if (g_manager3d == nullptr) return -1;
	g_manager3d->DrawHandle(playingEffectHandle);

	return 0;
}

int DrawEffekseer3D_End()
{
	if (g_manager3d == nullptr) return -1;

	// エフェクトの描画終了処理を行う。
	g_renderer3d->EndRendering();

	// DXライブラリの設定を戻す。
	RefreshDxLibDirect3DSetting();

	return 0;
}

::Effekseer::Manager* GetEffekseer2DManager()
{
	return g_manager2d;
}

::EffekseerRenderer::Renderer* GetEffekseer2DRenderer()
{
	return g_renderer2d;
}

::Effekseer::Manager* GetEffekseer3DManager()
{
	return g_manager3d;
}

::EffekseerRenderer::Renderer* GetEffekseer3DRenderer()
{
	return g_renderer3d;
}

::Effekseer::Effect* GetEffekseerEffect(int effectHandle)
{
	if (effectHandleToEffect.count(effectHandle) > 0)
	{
		return effectHandleToEffect[effectHandle];
	}

	return NULL;
}

void Effkseer_DeviceLost(void* data)
{
	if(GetUseDirect3DDevice9() == NULL) return;

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
	auto renderer2d = (EffekseerRendererDX9::Renderer*)g_renderer2d;
	auto renderer3d = (EffekseerRendererDX9::Renderer*)g_renderer3d;
	renderer2d->ChangeDevice(NULL);
	renderer3d->ChangeDevice(NULL);
}

void Effkseer_DeviceRestore(void* data)
{
	if(GetUseDirect3DDevice9() == NULL) return;

	// DXライブラリは回復時に内部でデバイスを再生成するので新しく設定する。
	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9) GetUseDirect3DDevice9();
	
	auto renderer2d = (EffekseerRendererDX9::Renderer*)g_renderer2d;
	auto renderer3d = (EffekseerRendererDX9::Renderer*)g_renderer3d;
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
}