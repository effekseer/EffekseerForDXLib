
#pragma once

#include "DxLib.h"
#include "Effekseer.h"
#include "EffekseerRendererDX9.h"

#if _MSC_VER == 1800

#ifdef _WIN64
	#error 32bit�̂ݑΉ����Ă��܂��B
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
#error VisualStudio(C++)2013�̂ݑΉ����Ă��܂��B
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
	@brief	Effekseer��2D�\���̐ݒ������B
	@param	windowWidth		��ʉ���
	@param	windowHeight	��ʏc��
*/
void Effekseer_Set2DSetting(int windowWidth, int windowHeight);

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