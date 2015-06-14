
#pragma once

#include "DxLib.h"
#include "Effekseer.h"
#include "EffekseerRendererDX9.h"

#if 1900 > _MSC_VER && _MSC_VER >= 1700

#ifdef _WIN64
	#error 32bitのみ対応しています。
#else

// DXライブラリの仕様により2012のライブラリを使用する
//#if _DEBUG
//#pragma comment(lib,"Effekseer_vs2013_x86_d.lib")
//#pragma comment(lib,"EffekseerRendererDX9_vs2013_x86_d.lib")
//#else
//#pragma comment(lib,"Effekseer_vs2013_x86.lib")
//#pragma comment(lib,"EffekseerRendererDX9_vs2013_x86.lib")
//#endif

#if _DEBUG
#pragma comment(lib,"Effekseer_vs2012_x86_d.lib")
#pragma comment(lib,"EffekseerRendererDX9_vs2012_x86_d.lib")
#else
#pragma comment(lib,"Effekseer_vs2012_x86.lib")
#pragma comment(lib,"EffekseerRendererDX9_vs2012_x86.lib")
#endif

#pragma comment(lib, "d3d9.lib" )

#ifndef _EFFEKSEER_FOR_DXLIB_LIB
#if _DEBUG
#pragma comment(lib,"EffekseerForDXLib_vs2012_x86_d.lib")
#else
#pragma comment(lib,"EffekseerForDXLib_vs2012_x86.lib")
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
	@brief	メモリ上のEffekseerのエフェクトを再生する。
	@param	effectHandle	エフェクトのハンドル
	@return	再生中のエフェクトのハンドル
	@return	-1以外:再生中のエフェクトのハンドル、-1:失敗
*/
int PlayEffekseerEffect(int effectHandle);

/**
	@brief	再生中のエフェクトの位置を設定する。
	@param	playingEffectHandle	再生中のエフェクトのハンドル
	@param	x	X座標
	@param	y	Y座標
	@param	z	Z座標
	@return	0:成功、-1:失敗
	@note
	2Dの場合、zには0を設定する。
	※エフェクトが既に再生終了していても成功を返す。
*/
int SetPosPlayingEffekseerEffect(int playingEffectHandle, float x, float y, float z);

/**
	@brief	再生中のエフェクトの拡大率を設定する。
	@param	playingEffectHandle	再生中のエフェクトのハンドル
	@param	x	X方向拡大率
	@param	y	Y方向拡大率
	@param	z	Z方向拡大率
	@return	0:成功、-1:失敗
	@note
	※エフェクトが既に再生終了していても成功を返す。
*/
int SetScalePlayingEffekseerEffect(int playingEffectHandle, float x, float y, float z);

/**
	@brief	Effekseerにより再生中のエフェクトを更新する。
	@return	0:成功、-1:失敗
*/
int UpdateEffekseer();

/**
	@brief	Effekseerにより再生中のエフェクトを描画する。
	@return	0:成功、-1:失敗
*/
int DrawEffekseer();

/**
	@brief	Effekseerのエフェクト管理クラスのインスタンスを取得する。
	@return	Effekseerのエフェクト管理クラスのインスタンス
	@note
	Effekseerの機能を直接使用しない限り、この関数は使用しない。
*/
::Effekseer::Manager* GetEffekseerManager();

/**
@brief	Effekseerのエフェクト描画クラスのインスタンスを取得する。
@return	Effekseerのエフェクト描画クラスのインスタンス
@note
Effekseerの機能を直接使用しない限り、この関数は使用しない。
*/
::EffekseerRendererDX9::Renderer* GetEffekseerRenderer();

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