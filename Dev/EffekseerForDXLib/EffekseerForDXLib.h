
#pragma once

#include "DxLib.h"
#include "Effekseer.h"
#include "EffekseerRendererDX11.h"
#include "EffekseerRendererDX9.h"

#include <string>

#if _MSC_VER >= 1900

#ifdef _WIN64

#if _MSC_VER >= 1920

#if _DEBUG
#pragma comment(lib, "Effekseer_vs2019_x64_d.lib")
#pragma comment(lib, "EffekseerRendererDX9_vs2019_x64_d.lib")
#pragma comment(lib, "EffekseerRendererDX11_vs2019_x64_d.lib")
#else
#pragma comment(lib, "Effekseer_vs2019_x64.lib")
#pragma comment(lib, "EffekseerRendererDX9_vs2019_x64.lib")
#pragma comment(lib, "EffekseerRendererDX11_vs2019_x64.lib")
#endif

#elif _MSC_VER >= 1910

#if _DEBUG
#pragma comment(lib, "Effekseer_vs2017_x64_d.lib")
#pragma comment(lib, "EffekseerRendererDX9_vs2017_x64_d.lib")
#pragma comment(lib, "EffekseerRendererDX11_vs2017_x64_d.lib")
#else
#pragma comment(lib, "Effekseer_vs2017_x64.lib")
#pragma comment(lib, "EffekseerRendererDX9_vs2017_x64.lib")
#pragma comment(lib, "EffekseerRendererDX11_vs2017_x64.lib")
#endif

#elif _MSC_VER >= 1900

#if _DEBUG
#pragma comment(lib, "Effekseer_vs2015_x64_d.lib")
#pragma comment(lib, "EffekseerRendererDX9_vs2015_x64_d.lib")
#pragma comment(lib, "EffekseerRendererDX11_vs2015_x64_d.lib")
#else
#pragma comment(lib, "Effekseer_vs2015_x64.lib")
#pragma comment(lib, "EffekseerRendererDX9_vs2015_x64.lib")
#pragma comment(lib, "EffekseerRendererDX11_vs2015_x64.lib")
#endif
#endif

#else

#if _MSC_VER >= 1920

#if _DEBUG
#pragma comment(lib, "Effekseer_vs2019_x86_d.lib")
#pragma comment(lib, "EffekseerRendererDX9_vs2019_x86_d.lib")
#pragma comment(lib, "EffekseerRendererDX11_vs2019_x86_d.lib")
#else
#pragma comment(lib, "Effekseer_vs2019_x86.lib")
#pragma comment(lib, "EffekseerRendererDX9_vs2019_x86.lib")
#pragma comment(lib, "EffekseerRendererDX11_vs2019_x86.lib")
#endif

#elif _MSC_VER >= 1910

#if _DEBUG
#pragma comment(lib, "Effekseer_vs2017_x86_d.lib")
#pragma comment(lib, "EffekseerRendererDX9_vs2017_x86_d.lib")
#pragma comment(lib, "EffekseerRendererDX11_vs2017_x86_d.lib")
#else
#pragma comment(lib, "Effekseer_vs2017_x86.lib")
#pragma comment(lib, "EffekseerRendererDX9_vs2017_x86.lib")
#pragma comment(lib, "EffekseerRendererDX11_vs2017_x86.lib")
#endif

#elif _MSC_VER >= 1900

#if _DEBUG
#pragma comment(lib, "Effekseer_vs2015_x86_d.lib")
#pragma comment(lib, "EffekseerRendererDX9_vs2015_x86_d.lib")
#pragma comment(lib, "EffekseerRendererDX11_vs2015_x86_d.lib")
#else
#pragma comment(lib, "Effekseer_vs2015_x86.lib")
#pragma comment(lib, "EffekseerRendererDX9_vs2015_x86.lib")
#pragma comment(lib, "EffekseerRendererDX11_vs2015_x86.lib")
#endif
#endif

#endif

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3d11.lib")

#ifndef _EFFEKSEER_FOR_DXLIB_LIB

#ifdef _WIN64

#if _MSC_VER >= 1920
#if _DEBUG
#pragma comment(lib, "EffekseerForDXLib_vs2019_x64_d.lib")
#else
#pragma comment(lib, "EffekseerForDXLib_vs2019_x64.lib")
#endif
#elif _MSC_VER >= 1910
#if _DEBUG
#pragma comment(lib, "EffekseerForDXLib_vs2017_x64_d.lib")
#else
#pragma comment(lib, "EffekseerForDXLib_vs2017_x64.lib")
#endif
#elif _MSC_VER >= 1900
#if _DEBUG
#pragma comment(lib, "EffekseerForDXLib_vs2015_x64_d.lib")
#else
#pragma comment(lib, "EffekseerForDXLib_vs2015_x64.lib")
#endif
#endif

#else

#if _MSC_VER >= 1920
#if _DEBUG
#pragma comment(lib, "EffekseerForDXLib_vs2019_x86_d.lib")
#else
#pragma comment(lib, "EffekseerForDXLib_vs2019_x86.lib")
#endif
#elif _MSC_VER >= 1910
#if _DEBUG
#pragma comment(lib, "EffekseerForDXLib_vs2017_x86_d.lib")
#else
#pragma comment(lib, "EffekseerForDXLib_vs2017_x86.lib")
#endif
#elif _MSC_VER >= 1900
#if _DEBUG
#pragma comment(lib, "EffekseerForDXLib_vs2015_x86_d.lib")
#else
#pragma comment(lib, "EffekseerForDXLib_vs2015_x86.lib")
#endif
#endif

#endif

#endif

#else
#error VisualStudio(C++)2017,2019のみ対応しています。
#endif

// ================================================================================================================================ //
// 内部処理用
// ================================================================================================================================ //

#ifdef _UNICODE
inline int __Effekseer_FileRead_open(const char* filePath)
{
	int Len = ::MultiByteToWideChar(CP_ACP, 0, filePath, -1, NULL, 0);

	wchar_t* pOut = new wchar_t[Len + 1];
	::MultiByteToWideChar(CP_ACP, 0, filePath, -1, pOut, Len);
	std::wstring Out(pOut);
	delete[] pOut;

	return FileRead_open(Out.c_str(), 0);
}

inline LONGLONG __Effekseer_FileRead_size(const char* filePath)
{
	int Len = ::MultiByteToWideChar(CP_ACP, 0, filePath, -1, NULL, 0);

	wchar_t* pOut = new wchar_t[Len + 1];
	::MultiByteToWideChar(CP_ACP, 0, filePath, -1, pOut, Len);
	std::wstring Out(pOut);
	delete[] pOut;

	return FileRead_size(Out.c_str());
}
#else
inline int __Effekseer_FileRead_open(const char* filePath) { return FileRead_open(filePath, 0); }

inline LONGLONG __Effekseer_FileRead_size(const char* FilePath) { return FileRead_size(FilePath); }
#endif

typedef int (*EffekseerFileOpenFunc)(const char* filePath);
typedef LONGLONG (*EffekseerFileReadSizeFunc)(const char* filePath);

// ================================================================================================================================ //
// ここまで
// ================================================================================================================================ //

/**
	@brief	Effekseerを初期化する。
	@param	particleMax	画面に表示可能な最大パーティクル数
	@param	openFunc	内部処理用(文字コード処理用)のための引数。必ずデフォルト引数を使用する。
	@param	readSizeFunc 	内部処理用(文字コード処理用)のための引数。必ずデフォルト引数を使用する。
	@return	成功した場合は0、失敗した場合は-1を返す。
*/
int Effekseer_Init(int particleMax,
				   EffekseerFileOpenFunc openFunc = __Effekseer_FileRead_open,
				   EffekseerFileReadSizeFunc readSizeFunc = __Effekseer_FileRead_size);

/**
	@brief	非推奨(Effekseer_Initを使用してください)
*/
int Effkseer_Init(int particleMax,
				  EffekseerFileOpenFunc openFunc = __Effekseer_FileRead_open,
				  EffekseerFileReadSizeFunc readSizeFunc = __Effekseer_FileRead_size);

/**
	@brief	Effekseerのネットワーク機能を初期化する。
	@param	port	使用するポート
	@return	成功した場合は0、失敗した場合は-1を返す。
	@note
	ネットワーク機能を有効にすると、DXライブラリの起動中にEffekseerのツールからエフェクトを変更できるようになる。
*/
int Effekseer_StartNetwork(int port);

/**
	@brief	非推奨(Effekseer_StartNetworkを使用してください)
*/
int Effkseer_InitServer(int port);

/**
@brief	Effekseerの歪み機能を初期化する。
@param	scale	ウインドウサイズに対する歪みに使用するバッファの拡大率(1.0がデフォルト)
@return	成功した場合は0、失敗した場合は-1を返す。
*/
int Effekseer_InitDistortion(float scale = 1.0f);

/**
	@brief	Effekseerを終了する。
*/
void Effkseer_End();

/**
	@brief	DXライブラリのデバイスがロストした時のコールバックを設定する。
*/
void Effekseer_SetGraphicsDeviceLostCallbackFunctions();

/**
	@brief	Effekseerに2D表示の設定をする。
	@param	windowWidth		画面横幅
	@param	windowHeight	画面縦幅
*/
void Effekseer_Set2DSetting(int windowWidth, int windowHeight);

/**
	@brief	Effekseerに3D表示の設定をDXライブラリの3D表示の設定に同期させる。
*/
void Effekseer_Sync3DSetting();

/**
	@brief	Effekseerのエフェクトリソースをメモリ上に読み込む。
	@param	fileName	efkファイルへのパス
	@param	magnification	拡大率、指定された値に拡大してエフェクトが読み込まれる。
	@return	-1以外:エフェクトリソースのハンドル、-1:失敗
*/
int LoadEffekseerEffect(const char* fileName, float magnification = 1.0f);

/**
	@brief	Effekseerのエフェクトリソースをメモリ上に読み込む。
	@param	fileName	efkファイルへのパス
	@param	magnification	拡大率、指定された値に拡大してエフェクトが読み込まれる。
	@return	-1以外:エフェクトリソースのハンドル、-1:失敗
*/
int LoadEffekseerEffect(const wchar_t* fileName, float magnification = 1.0f);

/**
	@brief	メモリ上からEffekseerのエフェクトリソースを削除する。
	@param	effectHandle	エフェクトリソースのハンドル
	@return	0:成功、-1:失敗
	@note
	LoadEffekseerEffectが返したエフェクトリソースのハンドルを引数に指定する。
*/
int DeleteEffekseerEffect(int effectResourceHandle);

/**
	@brief	メモリ上のEffekseerのエフェクトリソースを2D表示で再生する。
	@param	effectHandle	エフェクトリソースのハンドル
	@return	-1以外:エフェクトのハンドル、-1:失敗
*/
int PlayEffekseer2DEffect(int effectResourceHandle);

/**
	@brief	メモリ上のEffekseerのエフェクトリソースを3D表示で再生する。
	@param	effectHandle	エフェクトリソースのハンドル
	@return	-1以外:エフェクトのハンドル、-1:失敗
*/
int PlayEffekseer3DEffect(int effectResourceHandle);

/**
	@brief	2D表示のエフェクトが再生中か取得する。
	@param	playingEffectHandle	エフェクトのハンドル
	@return	再生中かどうか?
	@return	0:再生中、-1:再生されていない、もしくは再生終了
*/
int IsEffekseer2DEffectPlaying(int playingEffectHandle);

/**
	@brief	3D表示のエフェクトが再生中か取得する。
	@param	playingEffectHandle	エフェクトのハンドル
	@return	再生中かどうか?
	@return	0:再生中、-1:再生されていない、もしくは再生終了
*/
int IsEffekseer3DEffectPlaying(int playingEffectHandle);

/**
	@brief	2D表示のエフェクトを停止する。
	@param	effectHandle	エフェクトのハンドル
	@return	成否
	@return	0:成功、-1:失敗
*/
int StopEffekseer2DEffect(int playingEffecHandle);

/**
	@brief	3D表示のエフェクトを停止する。
	@param	effectHandle	エフェクトのハンドル
	@return	成否
	@return	0:成功、-1:失敗
*/
int StopEffekseer3DEffect(int playingEffecHandle);

/**
	@brief	再生中の2D表示のエフェクトの位置を設定する。
	@param	playingEffectHandle	再生中のエフェクトのハンドル
	@param	x	X座標
	@param	y	Y座標
	@return	0:成功、-1:失敗
	@note
	※エフェクトが既に再生終了していても成功を返す。
*/
int SetPosPlayingEffekseer2DEffect(int playingEffectHandle, float x, float y, float z);

/**
	@brief	再生中の3D表示のエフェクトの位置を設定する。
	@param	playingEffectHandle	再生中のエフェクトのハンドル
	@param	x	X座標
	@param	y	Y座標
	@param	z	Z座標
	@return	0:成功、-1:失敗
	@note
	※エフェクトが既に再生終了していても成功を返す。
*/
int SetPosPlayingEffekseer3DEffect(int playingEffectHandle, float x, float y, float z);

/**
@brief	再生中の2D表示のエフェクトの角度を設定する。
@param	playingEffectHandle	再生中のエフェクトのハンドル
@param	x	X軸角度(ラジアン)
@param	y	Y軸角度(ラジアン)
@param	y	Y軸角度(ラジアン)
@return	0:成功、-1:失敗
@note
回転の方向は時計回りである。
回転の順番は Z軸回転 → X軸回転 → Y軸回転である。
※エフェクトが既に再生終了していても成功を返す。
*/
int SetRotationPlayingEffekseer2DEffect(int playingEffectHandle, float x, float y, float z);

/**
@brief	再生中の3D表示のエフェクトの角度を設定する。
@param	playingEffectHandle	再生中のエフェクトのハンドル
@param	x	X軸角度(ラジアン)
@param	y	Y軸角度(ラジアン)
@param	y	Y軸角度(ラジアン)
@return	0:成功、-1:失敗
@note
回転の方向は時計回りである。
回転の順番は Z軸回転 → X軸回転 → Y軸回転である。
※エフェクトが既に再生終了していても成功を返す。
*/
int SetRotationPlayingEffekseer3DEffect(int playingEffectHandle, float x, float y, float z);

/**
	@brief	再生中の2D表示のエフェクトの拡大率を設定する。
	@param	playingEffectHandle	再生中のエフェクトのハンドル
	@param	x	X方向拡大率
	@param	y	Y方向拡大率
	@param	z	Z方向拡大率
	@return	0:成功、-1:失敗
	@note
	※エフェクトが既に再生終了していても成功を返す。
*/
int SetScalePlayingEffekseer2DEffect(int playingEffectHandle, float x, float y, float z);

/**
	@brief	再生中の3D表示のエフェクトの拡大率を設定する。
	@param	playingEffectHandle	再生中のエフェクトのハンドル
	@param	x	X方向拡大率
	@param	y	Y方向拡大率
	@param	z	Z方向拡大率
	@return	0:成功、-1:失敗
	@note
	※エフェクトが既に再生終了していても成功を返す。
*/
int SetScalePlayingEffekseer3DEffect(int playingEffectHandle, float x, float y, float z);

/**
	@brief	再生中の2D表示のエフェクトの再生速度を取得する。
	@param	playingEffectHandle	再生中のエフェクトのハンドル
	@return	再生速度
*/
float GetSpeedPlayingEffekseer2DEffect(int playingEffectHandle);

/**
	@brief	再生中の2D表示のエフェクトの再生速度を設定する。
	@param	playingEffectHandle	再生中のエフェクトのハンドル
	@param	speed	再生速度
	@return	0:成功、-1:失敗
	@note
	※エフェクトが既に再生終了していても成功を返す。
*/
int SetSpeedPlayingEffekseer2DEffect(int playingEffectHandle, float speed);

/**
	@brief	再生中の3D表示のエフェクトの再生速度を取得する。
	@param	playingEffectHandle	再生中のエフェクトのハンドル
	@return	再生速度
*/
float GetSpeedPlayingEffekseer3DEffect(int playingEffectHandle);

/**
	@brief	再生中の3D表示のエフェクトの再生速度を設定する。
	@param	playingEffectHandle	再生中のエフェクトのハンドル
	@param	speed	再生速度
	@return	0:成功、-1:失敗
	@note
	※エフェクトが既に再生終了していても成功を返す。
*/
int SetSpeedPlayingEffekseer3DEffect(int playingEffectHandle, float speed);

/**
@brief	再生中の2D表示のエフェクトの色を設定する。
@param	playingEffectHandle	再生中のエフェクトのハンドル
@param	speed	再生速度
@return	0:成功、-1:失敗
@note
※エフェクトが既に再生終了していても成功を返す。
*/
int SetColorPlayingEffekseer2DEffect(int playingEffectHandle, int r, int g, int b, int a);

/**
	@brief	再生中の3D表示のエフェクトの色を設定する。
	@param	playingEffectHandle	再生中のエフェクトのハンドル
	@param	speed	再生速度
	@return	0:成功、-1:失敗
	@note
	※エフェクトが既に再生終了していても成功を返す。
*/
int SetColorPlayingEffekseer3DEffect(int playingEffectHandle, int r, int g, int b, int a);

/**
	@brief	再生中にエフェクトのパラメーターを変更する動的パラメーターを取得する。
*/
float GetDynamicInput2DEffect(int playingEffectHandle, int32_t index);

/**
	@brief	再生中にエフェクトのパラメーターを変更する動的パラメーターを設定する。
*/
void SetDynamicInput2DEffect(int playingEffectHandle, int32_t index, float value);

/**
	@brief	再生中にエフェクトのパラメーターを変更する動的パラメーターを取得する。
*/
float GetDynamicInput3DEffect(int playingEffectHandle, int32_t index);

/**
	@brief	再生中にエフェクトのパラメーターを変更する動的パラメーターを設定する。
*/
void SetDynamicInput3DEffect(int playingEffectHandle, int32_t index, float value);

/**
	@brief	Effekseerにより再生中の2Dエフェクトを更新する。
	@return	0:成功、-1:失敗
*/
int UpdateEffekseer2D();

/**
	@brief	Effekseerにより再生中の2Dエフェクトを全て描画する。
	@return	0:成功、-1:失敗
	@note
	DrawEffekseer2D_Begin、DrawEffekseer2D_Draw、DrawEffekseer2D_Endとは併用できない。
*/
int DrawEffekseer2D();

/**
	@brief	Effekseerにより再生中の2Dエフェクトの描画を開始する。
	@return	0:成功、-1:失敗
	@note
	特定のエフェクトのみを描画したい時に使用する。
	エフェクトの描画開始処理を行う。
	この関数を実行してからDrawEffekseer2D_Endが実行されるまで、DrawEffekseer2D_Drawを実行できる。
	DrawEffekseer2Dとは併用できない。
*/
int DrawEffekseer2D_Begin();

/**
	@brief	指定されたEffekseerにより再生中の2Dエフェクトを描画する。
	@param	playingEffectHandle	再生中のエフェクトのハンドル
	@return	0:成功、-1:失敗
	@note
	特定のエフェクトのみを描画したい時に使用する。
	特定のエフェクトを描画する。
	DrawEffekseer2Dとは併用できない。
*/
int DrawEffekseer2D_Draw(int playingEffectHandle);

/**
	@brief	Effekseerにより再生中の2Dエフェクトの描画を終了する。
	@return	0:成功、-1:失敗
	@note
	特定のエフェクトのみを描画したい時に使用する。
	エフェクトの描画終了処理を行う。
	DrawEffekseer2D_Beginを実行してからがこの関数が実行されるまで、DrawEffekseer2D_Drawを実行できる。
	DrawEffekseer2Dとは併用できない。
*/
int DrawEffekseer2D_End();
/**
	@brief	Effekseerにより再生中の3Dエフェクトを更新する。
	@return	0:成功、-1:失敗
*/
int UpdateEffekseer3D();

/**
@brief	Effekseerにより再生中の3Dエフェクトの描画を開始する。
@return	0:成功、-1:失敗
@note
特定のエフェクトのみを描画したい時に使用する。
エフェクトの描画開始処理を行う。
この関数を実行してからDrawEffekseer3D_Endが実行されるまで、DrawEffekseer3D_Drawを実行できる。
DrawEffekseer3Dとは併用できない。
*/
int DrawEffekseer3D_Begin();

/**
@brief	指定されたEffekseerにより再生中の3Dエフェクトを描画する。
@param	playingEffectHandle	再生中のエフェクトのハンドル
@return	0:成功、-1:失敗
@note
特定のエフェクトのみを描画したい時に使用する。
特定のエフェクトを描画する。
DrawEffekseer3Dとは併用できない。
*/
int DrawEffekseer3D_Draw(int playingEffectHandle);

/**
@brief	Effekseerにより再生中の3Dエフェクトの描画を終了する。
@return	0:成功、-1:失敗
@note
特定のエフェクトのみを描画したい時に使用する。
エフェクトの描画終了処理を行う。
DrawEffekseer3D_Beginを実行してからがこの関数が実行されるまで、DrawEffekseer3D_Drawを実行できる。
DrawEffekseer3Dとは併用できない。
*/
int DrawEffekseer3D_End();

/**
	@brief	Effekseerにより再生中の3Dエフェクトを全て描画する。
	@return	0:成功、-1:失敗
*/
int DrawEffekseer3D();

/**
	@brief	Effekseerの2Dエフェクト管理クラスのインスタンスを取得する。
	@return	Effekseerの2Dエフェクト管理クラスのインスタンス
	@note
	Effekseerの機能を直接使用しない限り、この関数は使用しない。
*/
::Effekseer::Manager* GetEffekseer2DManager();

/**
@brief	Effekseerの2Dエフェクト描画クラスのインスタンスを取得する。
@return	Effekseerの2Dエフェクト描画クラスのインスタンス
@note
Effekseerの機能を直接使用しない限り、この関数は使用しない。
*/
::EffekseerRenderer::Renderer* GetEffekseer2DRenderer();

/**
	@brief	Effekseerの3Dエフェクト管理クラスのインスタンスを取得する。
	@return	Effekseerの3Dエフェクト管理クラスのインスタンス
	@note
	Effekseerの機能を直接使用しない限り、この関数は使用しない。
*/
::Effekseer::Manager* GetEffekseer3DManager();

/**
@brief	Effekseerの3Dエフェクト描画クラスのインスタンスを取得する。
@return	Effekseerの3Dエフェクト描画クラスのインスタンス
@note
Effekseerの機能を直接使用しない限り、この関数は使用しない。
*/
::EffekseerRenderer::Renderer* GetEffekseer3DRenderer();

/**
@brief	Effekseerのエフェクトクラスのインスタンスを取得する。
@param	effectHandle	エフェクトのハンドル
@return	Effekseerのエフェクトクラスのインスタンス
*/
::Effekseer::Effect* GetEffekseerEffect(int effectHandle);

/**
	@brief	デバイスロストが発生した時に呼ぶ。
	@param	Data	コールバックデータ
	@note
	Effekseer_SetGraphicsDeviceLostCallbackFunctionsを使用しない時に、
	この関数を呼び出されるように実装する必要がある。
*/
void Effkseer_DeviceLost(void* Data);

/**
	@brief	デバイスを復帰する時に呼ぶ。
	@param	Data	コールバックデータ
	Effekseer_SetGraphicsDeviceLostCallbackFunctionsを使用しない時に、
	この関数を呼び出されるように実装する必要がある。
*/
void Effkseer_DeviceRestore(void* Data);