
#include "DxLib.h"

// EffekseerForDXLib.hをインクルードします。
#include "EffekseerForDXLib.h"

int network()
{
	// このソースコードを実行しつつ、EffekseerでLaser01.efkefcを開き、編集してネットワークの送信ボタンを押してください。

	// ネットワークのサンプルのためDirectX11固定かつ、処理の説明は一部省く。

	// DXライブラリとEffekseerを初期化する。
	ChangeWindowMode(true);
	SetDrawScreen(DX_SCREEN_BACK);
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	SetAlwaysRunFlag(TRUE);
	if (DxLib_Init() == -1)
		return -1;

	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
		return -1;
	}

	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// ネットワーク機能を有効にする。
	// エフェクトを読み込む前に実行する。
	Effekseer_StartNetwork(60000);

	// エフェクトリソースを読み込む。
	// 読み込む時に大きさを指定する。
	int effectResourceHandle = LoadEffekseerEffect("Laser01.efkefc", 1.0f);

	// 何でもいいので画像を読み込む。
	int grBackgroundHandle = LoadGraph(_T("Texture/Background.png"));
	int grFrontHandle = LoadGraph(_T("Texture/Front.png"));

	// 時間を初期化する(定期的にエフェクトを再生するため)
	int time = 0;

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
		SetCameraPositionAndTarget_UpVecY(VGet(10, 10, -20), VGet(0, 0, 0));
		SetupCamera_Perspective(60.0f * DX_PI_F / 180.0f);
		SetCameraNearFar(1.0f, 150.0f);

		// DXライブラリのカメラとEffekseerのカメラを同期する。
		Effekseer_Sync3DSetting();

		// 定期的にエフェクトを再生する
		if (time % 60 == 0)
		{
			// エフェクトを再生する。
			playingEffectHandle = PlayEffekseer3DEffect(effectResourceHandle);

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

		// 3Dを表示する。
		DrawCapsule3D(
			VGet(0.0f, 100.0f, 0.0f), VGet(0.0f, -100.0f, 0.0f), 6.0f, 16, GetColor(100, 100, 100), GetColor(255, 255, 255), TRUE);

		// Effekseerにより再生中のエフェクトを描画する。
		DrawEffekseer3D();

		// エフェクトの上にも画像を描画できる。
		DrawGraph(0, 0, grFrontHandle, TRUE);

		// スクリーンを入れ替える。
		ScreenFlip();

		// 時間を経過させる。
		time++;
	}

	// エフェクトリソースを削除する。(Effekseer終了時に破棄されるので削除しなくてもいい)
	DeleteEffekseerEffect(effectResourceHandle);

	// Effekseerを終了する。
	Effkseer_End();

	// DXライブラリを終了する。
	DxLib_End();

	return 0;
}
