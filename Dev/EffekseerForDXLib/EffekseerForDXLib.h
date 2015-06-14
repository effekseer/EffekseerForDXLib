
#pragma once

#include "DxLib.h"
#include "Effekseer.h"
#include "EffekseerRendererDX9.h"

#if 1900 > _MSC_VER && _MSC_VER >= 1700

#ifdef _WIN64
	#error 32bit�̂ݑΉ����Ă��܂��B
#else

// DX���C�u�����̎d�l�ɂ��2012�̃��C�u�������g�p����
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
#error VisualStudio(C++)2012,2013�̂ݑΉ����Ă��܂��B
#endif

/**
	@brief	Effekseer������������B
	@param	particleMax	��ʂɕ\���\�ȍő�p�[�e�B�N����
	@return	���������ꍇ��0�A���s�����ꍇ��-1��Ԃ��B
*/
int Effkseer_Init(int particleMax);

/**
	@brief	Effekseer���I������B
*/
void Effkseer_End();

/**
	@brief	DX���C�u�����̃f�o�C�X�����X�g�������̃R�[���o�b�N��ݒ肷��B
*/
void Effekseer_SetGraphicsDeviceLostCallbackFunctions();

/**
	@brief	Effekseer��2D�\���̐ݒ������B
	@param	windowWidth		��ʉ���
	@param	windowHeight	��ʏc��
*/
void Effekseer_Set2DSetting(int windowWidth, int windowHeight);

/**
	@brief	Effekseer�̃G�t�F�N�g����������ɓǂݍ��ށB
	@param	fileName	efk�t�@�C���ւ̃p�X
	@return	-1�ȊO:�G�t�F�N�g�̃n���h���A-1:���s
*/
int LoadEffekseerEffect(const char* fileName);

/**
	@brief	Effekseer�̃G�t�F�N�g����������ɓǂݍ��ށB
	@param	fileName	efk�t�@�C���ւ̃p�X
	@return	-1�ȊO:�G�t�F�N�g�̃n���h���A-1:���s
*/
int LoadEffekseerEffect(const wchar_t* fileName);

/**
	@brief	�������ォ��Effekseer�̃G�t�F�N�g���폜����B
	@param	effectHandle	�G�t�F�N�g�̃n���h��
	@return	0:�����A-1:���s
*/
int DeleteEffekseerEffect(int effectHandle);

/**
	@brief	���������Effekseer�̃G�t�F�N�g���Đ�����B
	@param	effectHandle	�G�t�F�N�g�̃n���h��
	@return	�Đ����̃G�t�F�N�g�̃n���h��
	@return	-1�ȊO:�Đ����̃G�t�F�N�g�̃n���h���A-1:���s
*/
int PlayEffekseerEffect(int effectHandle);

/**
	@brief	�Đ����̃G�t�F�N�g�̈ʒu��ݒ肷��B
	@param	playingEffectHandle	�Đ����̃G�t�F�N�g�̃n���h��
	@param	x	X���W
	@param	y	Y���W
	@param	z	Z���W
	@return	0:�����A-1:���s
	@note
	2D�̏ꍇ�Az�ɂ�0��ݒ肷��B
	���G�t�F�N�g�����ɍĐ��I�����Ă��Ă�������Ԃ��B
*/
int SetPosPlayingEffekseerEffect(int playingEffectHandle, float x, float y, float z);

/**
	@brief	�Đ����̃G�t�F�N�g�̊g�嗦��ݒ肷��B
	@param	playingEffectHandle	�Đ����̃G�t�F�N�g�̃n���h��
	@param	x	X�����g�嗦
	@param	y	Y�����g�嗦
	@param	z	Z�����g�嗦
	@return	0:�����A-1:���s
	@note
	���G�t�F�N�g�����ɍĐ��I�����Ă��Ă�������Ԃ��B
*/
int SetScalePlayingEffekseerEffect(int playingEffectHandle, float x, float y, float z);

/**
	@brief	Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B
	@return	0:�����A-1:���s
*/
int UpdateEffekseer();

/**
	@brief	Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷��B
	@return	0:�����A-1:���s
*/
int DrawEffekseer();

/**
	@brief	Effekseer�̃G�t�F�N�g�Ǘ��N���X�̃C���X�^���X���擾����B
	@return	Effekseer�̃G�t�F�N�g�Ǘ��N���X�̃C���X�^���X
	@note
	Effekseer�̋@�\�𒼐ڎg�p���Ȃ�����A���̊֐��͎g�p���Ȃ��B
*/
::Effekseer::Manager* GetEffekseerManager();

/**
@brief	Effekseer�̃G�t�F�N�g�`��N���X�̃C���X�^���X���擾����B
@return	Effekseer�̃G�t�F�N�g�`��N���X�̃C���X�^���X
@note
Effekseer�̋@�\�𒼐ڎg�p���Ȃ�����A���̊֐��͎g�p���Ȃ��B
*/
::EffekseerRendererDX9::Renderer* GetEffekseerRenderer();

/**
	@brief	�f�o�C�X���X�g�������������ɌĂԁB
	@param	Data	�R�[���o�b�N�f�[�^
	@note
	Effekseer_SetGraphicsDeviceLostCallbackFunctions���g�p���Ȃ����ɁA
	���̊֐����Ăяo�����悤�Ɏ�������K�v������B
*/
void Effkseer_DeviceLost(void *Data);

/**
	@brief	�f�o�C�X�𕜋A���鎞�ɌĂԁB
	@param	Data	�R�[���o�b�N�f�[�^
	Effekseer_SetGraphicsDeviceLostCallbackFunctions���g�p���Ȃ����ɁA
	���̊֐����Ăяo�����悤�Ɏ�������K�v������B
*/
void Effkseer_DeviceRestore(void *Data);