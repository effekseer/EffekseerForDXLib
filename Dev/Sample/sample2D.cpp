
#include "DxLib.h"

// EffekseerForDXLib.hをインクルードします。
#include "EffekseerForDXLib.h"

int sample2D()
{
	// DXライブラリの表示方法をウィンドウモードに変更する。
	ChangeWindowMode(true);

	//描画先を裏画面に変更する。
	SetDrawScreen(DX_SCREEN_BACK);

	// DirectX9を使用するようにする。(DirectX11も可)
	// Effekseerを使用するには必ず設定する。
	SetUseDirect3DVersion(DX_DIRECT3D_9);

	// DXライブラリを初期化する。
	if (DxLib_Init() == -1) return -1;

	// Effekseerを初期化する。
	// 引数には画面に表示する最大パーティクル数を設定する。
	if (Effkseer_Init(8000) == -1)
	{
		DxLib_End();
		return -1;
	}

	// フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
	// Effekseerを使用する場合は必ず設定する。
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DXライブラリのデバイスロストした時のコールバックを設定する。
	// ウインドウとフルスクリーンの切り替えが発生する場合は必ず実行する。
	// ただし、DirectX11を使用する場合は実行する必要はない。
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// Effekseerに2D描画の設定をする。
	Effekseer_Set2DSetting(640, 480);

	// エフェクトリソースを読み込む。
	int effectResourceHandle = LoadEffekseerEffect("laser.efk");

	// 何でもいいので画像を読み込む。
	int grBackgroundHandle = LoadGraph(_T("Texture/Background.png"));
	int grFrontHandle = LoadGraph(_T("Texture/Front.png"));

	// 時間を初期化する(定期的にエフェクトを再生するため)
	int time = 0;

	// フルスクリーン切り替え用フラグを設定する。(F1、F2でウインドウ、フルスクリーンを切り替えれるようにする。)
	bool isFullScreen = false;

	// エフェクトの表示する位置を設定する。
	float position_x = 100.0f;
	float position_y = 250.0f;

	// 再生中のエフェクトのハンドルを初期化する。
	int playingEffectHandle = -1;

	// Zバッファを有効にする。
	// Effekseerを使用する場合、2DゲームでもZバッファを使用する。
	SetUseZBuffer3D(TRUE);

	// Zバッファへの書き込みを有効にする。
	// Effekseerを使用する場合、2DゲームでもZバッファを使用する。
	SetWriteZBuffer3D(TRUE);

	while (!ProcessMessage() && !ClearDrawScreen() && !CheckHitKey(KEY_INPUT_ESCAPE))
	{
		// 定期的にエフェクトを再生する
		if (time % 60 == 0)
		{
			// エフェクトを再生する。
			playingEffectHandle = PlayEffekseer2DEffect(effectResourceHandle);

			// エフェクトの拡大率を設定する。
			// Effekseerで作成したエフェクトは2D表示の場合、小さすぎることが殆どなので必ず拡大する。
			SetScalePlayingEffekseer2DEffect(playingEffectHandle, 25.0f, 25.0f, 25.0f);

			// エフェクトの位置をリセットする。
			position_x = 100.0f;
		}

		// 何でもいいので画像を描画する。
		// こうして描画した後でないと、Effekseerは描画できない。
		DrawGraph(0, 0, grBackgroundHandle, TRUE);

		// 再生中のエフェクトを移動する。
		SetPosPlayingEffekseer2DEffect(playingEffectHandle, position_x, position_y, 0);
		position_x += 2.0f;

		// Effekseerにより再生中のエフェクトを更新する。
		UpdateEffekseer2D();

		// Effekseerにより再生中のエフェクトを描画する。
		DrawEffekseer2D();

		// エフェクトの上にも画像を描画できる。
		DrawGraph(0, 0, grFrontHandle, TRUE);

		// スクリーンを入れ替える。
		ScreenFlip();

		// 時間を経過させる。
		time++;

		// フルスクリーンの切り替えを行う。
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

	// エフェクトリソースを削除する。(Effekseer終了時に破棄されるので削除しなくてもいい)
	DeleteEffekseerEffect(effectResourceHandle);
	
	// Effekseerを終了する。
	Effkseer_End();

	// DXライブラリを終了する。
	DxLib_End();

	return 0;
}
