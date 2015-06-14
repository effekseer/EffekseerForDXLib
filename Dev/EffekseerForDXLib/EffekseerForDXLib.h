
#pragma once

#include "DxLib.h"
#include "Effekseer.h"
#include "EffekseerRendererDX9.h"

#if _MSC_VER == 1800

#ifdef _WIN64
	#error 32bitのみ対応しています。
#else

#if _DEBUG
#pragma comment(lib,"Effekseer_vs2013_x86_d.lib")
#pragma comment(lib,"EffekseerRendererDX9_vs2013_x86_d.lib")
#else
#pragma comment(lib,"Effekseer_vs2013_x86.lib")
#pragma comment(lib,"EffekseerRendererDX9_vs2013_x86.lib")
#endif

#pragma comment(lib, "d3d9.lib" )

#endif

#else
#error VisualStudio(C++)2013のみ対応しています。
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
	@brief	Effekseerに2D表示の設定をする。
	@param	windowWidth		画面横幅
	@param	windowHeight	画面縦幅
*/
void Effekseer_Set2DSetting(int windowWidth, int windowHeight);

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