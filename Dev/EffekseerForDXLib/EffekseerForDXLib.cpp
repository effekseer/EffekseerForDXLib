
#include "EffekseerForDXLib.h"

static ::Effekseer::Manager*				g_manager = NULL;
static ::EffekseerRendererDX9::Renderer*	g_renderer = NULL;

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
	// エフェクト管理用インスタンスを破棄する。
	g_manager->Destroy();

	// 描画用インスタンスを破棄する。
	g_renderer->Destory();
}

void Effekseer_Set2DSetting(int windowWidth, int windowHeight)
{
	// 投影行列を設定を設定する。
	g_renderer->SetProjectionMatrix(
		::Effekseer::Matrix44().OrthographicRH(windowWidth, windowHeight, 1.0f, 500.0f));

	// カメラ行列を設定
	g_renderer->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtRH(
		::Effekseer::Vector3D( - windowWidth / 2.0f, - windowHeight / 2.0f, -20.0f), 
		::Effekseer::Vector3D( - windowWidth / 2.0f, - windowHeight / 2.0f, 20.0f), 
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

::Effekseer::Manager* GetEffekseerManager()
{
	return g_manager;
}

::EffekseerRendererDX9::Renderer* GetEffekseerRenderer()
{
	return g_renderer;
}