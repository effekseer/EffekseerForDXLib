
#include "DxLib.h"

// EffekseerForDXLib.hをインクルードします。
#include "EffekseerForDXLib.h"

int sample3D()
{
	// DXライブラリの表示方法をウィンドウモードに変更する。
	ChangeWindowMode(true);

	//描画先を裏画面に変更する。
	SetDrawScreen(DX_SCREEN_BACK);

	// DirectX9を使用するようにする。
	// Effekseerを使用するには必ず設定する。
	SetUseDirect3DVersion(DX_DIRECT3D_9);

	// DXライブラリを初期化する。
	if (DxLib_Init() == -1) return -1;

	// Effekseerを初期化する。
	// 引数には画面に表示する最大パーティクル数を設定する。
	if (Effkseer_Init(2000) == -1)
	{
		DxLib_End();
		return -1;
	}

	// フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
	// Effekseerを使用する場合は必ず設定する。
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DXライブラリのデバイスロストした時のコールバックを設定する。
	// ウインドウとフルスクリーンの切り替えが発生する場合は必ず実行する。
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// エフェクトを読み込む。
	int effectHandle = LoadEffekseerEffect("laser.efk");

	// 何でもいいので画像を読み込む。
	int grBackgroundHandle = LoadGraph(_T("Texture/Background.png"));
	int grFrontHandle = LoadGraph(_T("Texture/Front.png"));

	// 時間を初期化する(定期的にエフェクトを再生するため)
	int time = 0;

	// フルスクリーン切り替え用フラグを設定する。(F1、F2でウインドウ、フルスクリーンを切り替えれるようにする。)
	bool isFullScreen = false;

	// エフェクトの表示する位置を設定する。
	float position_x = 0.0f;
	float position_y = 0.0f;

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
		// DXライブラリのカメラを設定する。
		SetCameraPositionAndTarget_UpVecY(VGet(10,10,-20),VGet(0,0,0));
		SetupCamera_Perspective( 60.0f * DX_PI_F / 180.0f ) ;
		SetCameraNearFar(1.0f, 150.0f);

		// DXライブラリのカメラとEffekseerのカメラを同期する。
		Effekseer_Sync3DSetting();

		// 定期的にエフェクトを再生する
		if (time % 60 == 0)
		{
			// エフェクトを再生する。
			playingEffectHandle = PlayEffekseer3DEffect(effectHandle);

			// エフェクトの位置をリセットする。
			position_x = 0.0f;
		}

		// 何でもいいので画像を描画する。
		// こうして描画した後でないと、Effekseerは描画できない。
		DrawGraph(0, 0, grBackgroundHandle, TRUE);

		// 再生中のエフェクトを移動する。
		SetPosPlayingEffekseer3DEffect(playingEffectHandle, position_x, position_y, 0);
		position_x += 0.2f;

		// Effekseerにより再生中のエフェクトを更新する。
		UpdateEffekseer3D();

		// Effekseerにより再生中のエフェクトを描画する。
		DrawEffekseer3D();

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

	// エフェクトを削除する。(Effekseer終了時に破棄されるので削除しなくてもいい)
	DeleteEffekseerEffect(effectHandle);
	
	// Effekseerを終了する。
	Effkseer_End();

	// DXライブラリを終了する。
	DxLib_End();

	return 0;
}
