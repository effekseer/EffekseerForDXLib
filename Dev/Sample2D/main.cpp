
#include "DxLib.h"

// EffekseerForDXLib.hをインクルードします。
#include "EffekseerForDXLib.h"

static int g_window_width = 640;
static int g_window_height = 480;
static ::Effekseer::Manager*			g_manager = NULL;
static ::EffekseerRendererDX9::Renderer*	g_renderer = NULL;
static ::Effekseer::Effect*				g_effect = NULL;
static ::Effekseer::Handle				g_handle = -1;

void DeviceLostFunction(void *Data)
{
	printf("Start LostFunction\n");
	// デバイスロストが発生した時に呼ぶ。
	g_renderer->OnLostDevice();

	// 読み込んだエフェクトのリソースは全て破棄する。
	g_effect->UnloadResources();

	// DXライブラリは内部でデバイス自体を消去しているのでNULLを設定する。
	g_renderer->ChangeDevice(NULL);

	printf("End LostFunction\n");
}

void DeviceRestoreFunction(void *Data)
{
	printf("Start RestoreFunction\n");

	// DXライブラリは回復時に内部でデバイスを再生成するので新しく設定する。
	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9) GetUseDirect3DDevice9();
	g_renderer->ChangeDevice(device);

	// エフェクトのリソースを再読み込みする。
	g_effect->ReloadResources();

	// デバイスが復帰するときに呼ぶ
	g_renderer->OnResetDevice();
	printf("End RestoreFunction\n");
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
{
	// DXライブラリの表示方法をウィンドウモードに変更する。
	ChangeWindowMode(true);

	// DXライブラリを初期化する。
	if (DxLib_Init() == -1 || SetDrawScreen(DX_SCREEN_BACK) != 0) return -1;

	// フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
	// Effekseerを使用する場合は必ず設定する。
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// Effekseerを初期化する。
	// 引数には画面に表示する最大パーティクル数を設定する。
	if (Effkseer_Init(2000) == -1)
	{
		DxLib_End();
		return -1;
	}

	// Effekseerに2D描画の設定をする。
	Effekseer_Set2DSetting(640, 480);

	g_renderer = GetEffekseerRenderer();
	g_manager = GetEffekseerManager();

	// デバイスロスト時のコールバック設定(フルスクリーンウインドウ切り替えのために必要)
	SetGraphicsDeviceLostCallbackFunction(DeviceLostFunction, NULL);
	SetGraphicsDeviceRestoreCallbackFunction(DeviceRestoreFunction, NULL);

	// エフェクトの読込
	g_effect = Effekseer::Effect::Create(g_manager, (const EFK_CHAR*) L"test.efk");

	// 何でもいいから画像読込
	int grHandle = LoadGraph(L"Texture/Particle01.png");

	// 時間の設定
	int time = 0;

	bool isFullScreen = false;

	while (!ProcessMessage() && !ClearDrawScreen() && !CheckHitKey(KEY_INPUT_ESCAPE))
	{
		// 定期的にエフェクトを再生
		if (time % 60 == 0)
		{
			// エフェクトの再生
			g_handle = g_manager->Play(g_effect, 0, 0, 0);
		}

		// 何でもいいのでTransFragを有効にして画像を描画する。
		// こうして描画した後でないと、Effekseerは描画できない。
		DrawGraph(0, 0, grHandle, TRUE);

		// 頂点バッファに溜まった頂点データを吐き出す
		RenderVertex();

		// エフェクトの移動処理を行う
		g_manager->AddLocation(g_handle, ::Effekseer::Vector3D(0.2f, 0.0f, 0.0f));

		// エフェクトの更新処理を行う
		g_manager->Update();

		// エフェクトの描画開始処理を行う。
		g_renderer->BeginRendering();

		// エフェクトの描画を行う。
		g_manager->Draw();

		// エフェクトの描画終了処理を行う。
		g_renderer->EndRendering();

		// DXライブラリの設定を戻す。
		RefreshDxLibDirect3DSetting();

		// スクリーン入れ替え
		ScreenFlip();

		// 時間経過
		time++;

		// フルスクリーン切り替え
		if (CheckHitKey(KEY_INPUT_F1) && !isFullScreen)
		{
			ChangeWindowMode(FALSE);
			SetDrawScreen(DX_SCREEN_BACK);
			isFullScreen = true;
		}
		if (CheckHitKey(KEY_INPUT_F2) && isFullScreen)
		{
			ChangeWindowMode(TRUE);
			SetDrawScreen(DX_SCREEN_BACK);
			isFullScreen = false;
		}
	}

	// エフェクトの破棄
	ES_SAFE_RELEASE(g_effect);

	// Effekseerを終了する。
	Effkseer_End();

	// DXライブラリを終了する。
	DxLib_End();

	return 0;
}
