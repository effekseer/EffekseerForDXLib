
#include "EffekseerForDXLib.h"

#include <map>
#include <string>

static ::Effekseer::Manager*				g_manager = NULL;
static ::EffekseerRendererDX9::Renderer*	g_renderer = NULL;

static int32_t nextEffectHandle = 0;
static std::map<std::wstring, int32_t>			effectFileNameToEffectHandle;
static std::map<int32_t, std::wstring>			effectHandleToEffectFileName;
static std::map<int32_t, ::Effekseer::Effect*>	effectHandleToEffect;

static std::wstring ToWide(const char* pText)
{
	int Len = ::MultiByteToWideChar(CP_ACP, 0, pText, -1, NULL, 0);

	wchar_t* pOut = new wchar_t[Len + 1];
	::MultiByteToWideChar(CP_ACP, 0, pText, -1, pOut, Len);
	std::wstring Out(pOut);
	delete [] pOut;

	return Out;
}

int Effkseer_Init(int particleMax)
{
	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9) GetUseDirect3DDevice9();
	if (device == NULL) return -1;

	// レンダラーを生成する。
	g_renderer = ::EffekseerRendererDX9::Renderer::Create(device, particleMax);

	// マネージャーを生成する。
	g_manager = ::Effekseer::Manager::Create(particleMax);

	// 描画方法を設定する。
	g_manager->SetSpriteRenderer(g_renderer->CreateSpriteRenderer());
	g_manager->SetRibbonRenderer(g_renderer->CreateRibbonRenderer());
	g_manager->SetRingRenderer(g_renderer->CreateRingRenderer());
	g_manager->SetModelRenderer(g_renderer->CreateModelRenderer());
	g_manager->SetTrackRenderer(g_renderer->CreateTrackRenderer());

	// 描画用インスタンスからテクスチャの読込機能を設定する。
	// 独自拡張可能、現在はファイルから読み込んでいる。
	g_manager->SetTextureLoader(g_renderer->CreateTextureLoader());
	g_manager->SetModelLoader(g_renderer->CreateModelLoader());

	return 0;
}

void Effkseer_End()
{
	// 読み込まれたエフェクトを削除する。
	for (auto e : effectHandleToEffect)
	{
		auto effect = e.second;
		ES_SAFE_RELEASE(effect);
	}
	effectHandleToEffect.clear();

	// エフェクト管理用インスタンスを破棄する。
	g_manager->Destroy();

	// 描画用インスタンスを破棄する。
	g_renderer->Destory();
}

void Effekseer_SetGraphicsDeviceLostCallbackFunctions()
{
	// デバイスロスト時のコールバックを設定する。(フルスクリーンウインドウ切り替えのために必要)
	SetGraphicsDeviceLostCallbackFunction(Effkseer_DeviceLost, NULL);
	SetGraphicsDeviceRestoreCallbackFunction(Effkseer_DeviceRestore, NULL);
}


void Effekseer_Set2DSetting(int windowWidth, int windowHeight)
{
	// 投影行列を設定を設定する。
	g_renderer->SetProjectionMatrix(
		::Effekseer::Matrix44().OrthographicRH(windowWidth, windowHeight, 1.0f, 500.0f));

	// カメラ行列を設定
	g_renderer->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtRH(
		::Effekseer::Vector3D( windowWidth / 2.0f, - windowHeight / 2.0f, -20.0f), 
		::Effekseer::Vector3D( windowWidth / 2.0f, - windowHeight / 2.0f, 20.0f), 
		::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));
}

/*
// 投影行列を設定
g_renderer->SetProjectionMatrix(
::Effekseer::Matrix44().PerspectiveFovRH(90.0f / 180.0f * 3.14f, (float) g_window_width / (float) g_window_height, 1.0f, 50.0f));

// カメラ行列を設定
g_renderer->SetCameraMatrix(
::Effekseer::Matrix44().LookAtRH(::Effekseer::Vector3D(10.0f, 5.0f, 20.0f), ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));

*/

int LoadEffekseerEffect(const char* fileName)
{
	auto fileName_ = ToWide(fileName);
	return LoadEffekseerEffect(fileName_.c_str());
}

int LoadEffekseerEffect(const wchar_t* fileName)
{
	if (g_manager == nullptr) return -1;

	if (effectFileNameToEffectHandle.count(fileName) > 0)
	{
		return effectFileNameToEffectHandle[fileName];
	}

	auto effect = Effekseer::Effect::Create(g_manager, (const EFK_CHAR*) fileName);
	if (effect == nullptr) return -1;

	int32_t handle = nextEffectHandle;
	nextEffectHandle++;

	effectFileNameToEffectHandle[fileName] = handle;
	effectHandleToEffectFileName[handle] = fileName;
	effectHandleToEffect[handle] = effect;

	return handle;
}

int DeleteEffekseerEffect(int effectHandle)
{
	if (effectHandleToEffectFileName.count(effectHandle) > 0)
	{
		auto fileName = effectHandleToEffectFileName[effectHandle];
		auto effect = effectHandleToEffect[effectHandle];
		effectFileNameToEffectHandle.erase(fileName);
		effectHandleToEffect.erase(effectHandle);
		effectHandleToEffectFileName.erase(effectHandle);

		ES_SAFE_RELEASE(effect);
	}
	
	return -1;
}

int PlayEffekseerEffect(int effectHandle)
{
	if (g_manager == nullptr) return -1;

	if (effectHandleToEffect.count(effectHandle) > 0)
	{
		auto effect = effectHandleToEffect[effectHandle];
		return g_manager->Play(effect, 0, 0, 0);
	}

	return -1;
}

int SetPosPlayingEffekseerEffect(int playingEffectHandle, float x, float y, float z)
{
	if (g_manager == nullptr) return -1;
	g_manager->SetLocation(playingEffectHandle, ::Effekseer::Vector3D(x, y, z));
	return 0;
}

int SetScalePlayingEffekseerEffect(int playingEffectHandle, float x, float y, float z)
{
	if (g_manager == nullptr) return -1;
	g_manager->SetScale(playingEffectHandle, x, y, z);
	return 0;
}

int UpdateEffekseer()
{
	if (g_manager == nullptr) return -1;
	g_manager->Update();
	return 0;
}

int DrawEffekseer()
{
	if (g_manager == nullptr) return -1;
	
	// 頂点バッファに溜まった頂点データを吐き出す。
	RenderVertex();

	// エフェクトの描画開始処理を行う。
	g_renderer->BeginRendering();

	// エフェクトの描画を行う。
	g_manager->Draw();

	// エフェクトの描画終了処理を行う。
	g_renderer->EndRendering();

	// DXライブラリの設定を戻す。
	RefreshDxLibDirect3DSetting();

	return 0;
}

::Effekseer::Manager* GetEffekseerManager()
{
	return g_manager;
}

::EffekseerRendererDX9::Renderer* GetEffekseerRenderer()
{
	return g_renderer;
}

void Effkseer_DeviceLost(void* data)
{
	// デバイスロストが発生した時に呼ぶ。
	g_renderer->OnLostDevice();

	// 読み込んだエフェクトのリソースは全て破棄する。
	for (auto e : effectHandleToEffect)
	{
		auto effect = e.second;
		effect->UnloadResources();
	}

	// DXライブラリは内部でデバイス自体を消去しているのでNULLを設定する。
	g_renderer->ChangeDevice(NULL);
}

void Effkseer_DeviceRestore(void* data)
{
	// DXライブラリは回復時に内部でデバイスを再生成するので新しく設定する。
	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9) GetUseDirect3DDevice9();
	g_renderer->ChangeDevice(device);

	// エフェクトのリソースを再読み込みする。
	for (auto e : effectHandleToEffect)
	{
		auto effect = e.second;
		effect->ReloadResources();
	}

	// デバイスが復帰するときに呼ぶ
	g_renderer->OnResetDevice();
}