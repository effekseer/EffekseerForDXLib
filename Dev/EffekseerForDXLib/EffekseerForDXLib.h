
#pragma once

#include "DxLib.h"
#include "Effekseer.h"
#include "EffekseerRendererDX9.h"

#if 1900 > _MSC_VER && _MSC_VER >= 1700

#ifdef _WIN64
	#error 32bitのみ対応しています。
#else

#if _MSC_VER ==1800

#if _DEBUG
#pragma comment(lib,"Effekseer_vs2013_x86_d.lib")
#pragma comment(lib,"EffekseerRendererDX9_vs2013_x86_d.lib")
#else
#pragma comment(lib,"Effekseer_vs2013_x86.lib")
#pragma comment(lib,"EffekseerRendererDX9_vs2013_x86.lib")
#endif

#endif

#if _MSC_VER == 1700

#if _DEBUG
#pragma comment(lib,"Effekseer_vs2012_x86_d.lib")
#pragma comment(lib,"EffekseerRendererDX9_vs2012_x86_d.lib")
#else
#pragma comment(lib,"Effekseer_vs2012_x86.lib")
#pragma comment(lib,"EffekseerRendererDX9_vs2012_x86.lib")
#endif

#endif

#pragma comment(lib, "d3d9.lib" )

#ifndef _EFFEKSEER_FOR_DXLIB_LIB

#if _MSC_VER ==1800

#if _DEBUG
#pragma comment(lib,"EffekseerForDXLib_vs2013_x86_d.lib")
#else
#pragma comment(lib,"EffekseerForDXLib_vs2013_x86.lib")
#endif

#endif

#if _MSC_VER ==1700

#if _DEBUG
#pragma comment(lib,"EffekseerForDXLib_vs2012_x86_d.lib")
#else
#pragma comment(lib,"EffekseerForDXLib_vs2012_x86.lib")
#endif

#endif


#endif

#endif

#else
#error VisualStudio(C++)2012,2013のみ対応しています。
#endif

/**
	@brief	Effekseerを初期化する。
	@param	particleMax	画面に表示可能な最大パーティクル数
	@return	成功した場合は0、失敗した場合は-1を返す。
*/
int Effkseer_Init(int particleMax);

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
	@brief	Effekseerのエフェクトをメモリ上に読み込む。
	@param	fileName	efkファイルへのパス
	@return	-1以外:エフェクトのハンドル、-1:失敗
*/
int LoadEffekseerEffect(const char* fileName);

/**
	@brief	Effekseerのエフェクトをメモリ上に読み込む。
	@param	fileName	efkファイルへのパス
	@return	-1以外:エフェクトのハンドル、-1:失敗
*/
int LoadEffekseerEffect(const wchar_t* fileName);

/**
	@brief	メモリ上からEffekseerのエフェクトを削除する。
	@param	effectHandle	エフェクトのハンドル
	@return	0:成功、-1:失敗
*/
int DeleteEffekseerEffect(int effectHandle);

/**
	@brief	メモリ上のEffekseerのエフェクトを2D表示で再生する。
	@param	effectHandle	エフェクトのハンドル
	@return	再生中のエフェクトのハンドル
	@return	-1以外:再生中のエフェクトのハンドル、-1:失敗
*/
int PlayEffekseer2DEffect(int effectHandle);

/**
	@brief	メモリ上のEffekseerのエフェクトを3D表示で再生する。
	@param	effectHandle	エフェクトのハンドル
	@return	再生中のエフェクトのハンドル
	@return	-1以外:再生中のエフェクトのハンドル、-1:失敗
*/
int PlayEffekseer3DEffect(int effectHandle);

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
	@brief	Effekseerにより再生中の2Dエフェクトを更新する。
	@return	0:成功、-1:失敗
*/
int UpdateEffekseer2D();

/**
	@brief	Effekseerにより再生中の2Dエフェクトを描画する。
	@return	0:成功、-1:失敗
*/
int DrawEffekseer2D();

/**
	@brief	Effekseerにより再生中の3Dエフェクトを更新する。
	@return	0:成功、-1:失敗
*/
int UpdateEffekseer3D();

/**
	@brief	Effekseerにより再生中の3Dエフェクトを描画する。
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
::EffekseerRendererDX9::Renderer* GetEffekseer2DRenderer();

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
::EffekseerRendererDX9::Renderer* GetEffekseer3DRenderer();

/**
	@brief	デバイスロストが発生した時に呼ぶ。
	@param	Data	コールバックデータ
	@note
	Effekseer_SetGraphicsDeviceLostCallbackFunctionsを使用しない時に、
	この関数を呼び出されるように実装する必要がある。
*/
void Effkseer_DeviceLost(void *Data);

/**
	@brief	デバイスを復帰する時に呼ぶ。
	@param	Data	コールバックデータ
	Effekseer_SetGraphicsDeviceLostCallbackFunctionsを使用しない時に、
	この関数を呼び出されるように実装する必要がある。
*/
void Effkseer_DeviceRestore(void *Data);