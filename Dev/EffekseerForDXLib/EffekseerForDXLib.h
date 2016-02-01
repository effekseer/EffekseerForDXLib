
#pragma once

#include "DxLib.h"
#include "Effekseer.h"
#include "EffekseerRendererDX9.h"

#include <string>

#if _MSC_VER >= 1700

#ifdef _WIN64
	#error 32bit�̂ݑΉ����Ă��܂��B
#else

#if _MSC_VER ==1900

#if _DEBUG
#pragma comment(lib,"Effekseer_vs2015_x86_d.lib")
#pragma comment(lib,"EffekseerRendererDX9_vs2015_x86_d.lib")
#else
#pragma comment(lib,"Effekseer_vs2015_x86.lib")
#pragma comment(lib,"EffekseerRendererDX9_vs2015_x86.lib")
#endif

#endif

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

#if _MSC_VER ==1900

#if _DEBUG
#pragma comment(lib,"EffekseerForDXLib_vs2015_x86_d.lib")
#else
#pragma comment(lib,"EffekseerForDXLib_vs2015_x86.lib")
#endif

#endif

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
#error VisualStudio(C++)2012,2013�̂ݑΉ����Ă��܂��B
#endif

// ================================================================================================================================ //
// ���������p
// ================================================================================================================================ //

#ifdef _UNICODE
inline int __Effekseer_FileRead_open(const char* filePath) {
	int Len = ::MultiByteToWideChar(CP_ACP, 0, filePath, -1, NULL, 0);

	wchar_t* pOut = new wchar_t[Len + 1];
	::MultiByteToWideChar(CP_ACP, 0, filePath, -1, pOut, Len);
	std::wstring Out(pOut);
	delete [] pOut;

	return FileRead_open(Out.c_str(), 0);
}

inline LONGLONG	__Effekseer_FileRead_size(const char *filePath) {
	int Len = ::MultiByteToWideChar(CP_ACP, 0, filePath, -1, NULL, 0);

	wchar_t* pOut = new wchar_t[Len + 1];
	::MultiByteToWideChar(CP_ACP, 0, filePath, -1, pOut, Len);
	std::wstring Out(pOut);
	delete [] pOut;

	return FileRead_size(Out.c_str());
}
#else
inline int __Effekseer_FileRead_open(const char* filePath)
{
	return FileRead_open(filePath, 0);
}

inline LONGLONG	__Effekseer_FileRead_size(const char *FilePath)
{
	return FileRead_size(FilePath);
}
#endif

typedef int(*EffekseerFileOpenFunc) (const char* filePath);
typedef LONGLONG(*EffekseerFileReadSizeFunc) (const char* filePath);

// ================================================================================================================================ //
// �����܂�
// ================================================================================================================================ //


/**
	@brief	Effekseer������������B
	@param	particleMax	��ʂɕ\���\�ȍő�p�[�e�B�N����
	@param	openFunc	���������p(�����R�[�h�����p)�̂��߂̈����B�K���f�t�H���g�������g�p����B
	@param	readSizeFunc 	���������p(�����R�[�h�����p)�̂��߂̈����B�K���f�t�H���g�������g�p����B
	@return	���������ꍇ��0�A���s�����ꍇ��-1��Ԃ��B
*/
int Effkseer_Init(int particleMax, 
	EffekseerFileOpenFunc openFunc = __Effekseer_FileRead_open,
	EffekseerFileReadSizeFunc readSizeFunc = __Effekseer_FileRead_size);

/**
	@brief	Effekseer�̃l�b�g���[�N�@�\������������B
	@param	port	�g�p����|�[�g
	@return	���������ꍇ��0�A���s�����ꍇ��-1��Ԃ��B
	@note
	�l�b�g���[�N�@�\��L���ɂ���ƁADX���C�u�����̋N������Effekseer�̃c�[������G�t�F�N�g��ύX�ł���悤�ɂȂ�B
*/
int Effkseer_InitServer(int port);

/**
@brief	Effekseer�̘c�݋@�\������������B
@param	scale	�E�C���h�E�T�C�Y�ɑ΂���c�݂Ɏg�p����o�b�t�@�̊g�嗦(1.0���f�t�H���g)
@return	���������ꍇ��0�A���s�����ꍇ��-1��Ԃ��B
*/
int Effekseer_InitDistortion(float scale = 1.0f);

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
	@brief	Effekseer��3D�\���̐ݒ��DX���C�u������3D�\���̐ݒ�ɓ���������B
*/
void Effekseer_Sync3DSetting();

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
	@brief	���������Effekseer�̃G�t�F�N�g��2D�\���ōĐ�����B
	@param	effectHandle	�G�t�F�N�g�̃n���h��
	@return	-1�ȊO:�Đ����̃G�t�F�N�g�̃n���h���A-1:���s
*/
int PlayEffekseer2DEffect(int effectHandle);

/**
	@brief	���������Effekseer�̃G�t�F�N�g��3D�\���ōĐ�����B
	@param	effectHandle	�G�t�F�N�g�̃n���h��
	@return	-1�ȊO:�Đ����̃G�t�F�N�g�̃n���h���A-1:���s
*/
int PlayEffekseer3DEffect(int effectHandle);

/**
	@brief	2D�\���̃G�t�F�N�g���Đ������擾����B
	@param	effectHandle	�G�t�F�N�g�̃n���h��
	@return	�Đ������ǂ���?
	@return	0:�Đ����A-1:�Đ�����Ă��Ȃ��A�������͍Đ��I��
*/
int IsEffekseer2DEffectPlaying(int effectHandle);

/**
	@brief	3D�\���̃G�t�F�N�g���Đ������擾����B
	@param	effectHandle	�G�t�F�N�g�̃n���h��
	@return	�Đ������ǂ���?
	@return	0:�Đ����A-1:�Đ�����Ă��Ȃ��A�������͍Đ��I��
*/
int IsEffekseer3DEffectPlaying(int effectHandle);

/**
	@brief	�Đ�����2D�\���̃G�t�F�N�g�̈ʒu��ݒ肷��B
	@param	playingEffectHandle	�Đ����̃G�t�F�N�g�̃n���h��
	@param	x	X���W
	@param	y	Y���W
	@return	0:�����A-1:���s
	@note
	���G�t�F�N�g�����ɍĐ��I�����Ă��Ă�������Ԃ��B
*/
int SetPosPlayingEffekseer2DEffect(int playingEffectHandle, float x, float y, float z);

/**
	@brief	�Đ�����3D�\���̃G�t�F�N�g�̈ʒu��ݒ肷��B
	@param	playingEffectHandle	�Đ����̃G�t�F�N�g�̃n���h��
	@param	x	X���W
	@param	y	Y���W
	@param	z	Z���W
	@return	0:�����A-1:���s
	@note
	���G�t�F�N�g�����ɍĐ��I�����Ă��Ă�������Ԃ��B
*/
int SetPosPlayingEffekseer3DEffect(int playingEffectHandle, float x, float y, float z);

/**
@brief	�Đ�����2D�\���̃G�t�F�N�g�̊p�x��ݒ肷��B
@param	playingEffectHandle	�Đ����̃G�t�F�N�g�̃n���h��
@param	x	X���p�x(���W�A��)
@param	y	Y���p�x(���W�A��)
@param	y	Y���p�x(���W�A��)
@return	0:�����A-1:���s
@note
��]�̕����͎��v���ł���B
��]�̏��Ԃ� Z����] �� X����] �� Y����]�ł���B
���G�t�F�N�g�����ɍĐ��I�����Ă��Ă�������Ԃ��B
*/
int SetRotationPlayingEffekseer2DEffect(int playingEffectHandle, float x, float y, float z);

/**
@brief	�Đ�����3D�\���̃G�t�F�N�g�̊p�x��ݒ肷��B
@param	playingEffectHandle	�Đ����̃G�t�F�N�g�̃n���h��
@param	x	X���p�x(���W�A��)
@param	y	Y���p�x(���W�A��)
@param	y	Y���p�x(���W�A��)
@return	0:�����A-1:���s
@note
��]�̕����͎��v���ł���B
��]�̏��Ԃ� Z����] �� X����] �� Y����]�ł���B
���G�t�F�N�g�����ɍĐ��I�����Ă��Ă�������Ԃ��B
*/
int SetRotationPlayingEffekseer3DEffect(int playingEffectHandle, float x, float y, float z);

/**
	@brief	�Đ�����2D�\���̃G�t�F�N�g�̊g�嗦��ݒ肷��B
	@param	playingEffectHandle	�Đ����̃G�t�F�N�g�̃n���h��
	@param	x	X�����g�嗦
	@param	y	Y�����g�嗦
	@param	z	Z�����g�嗦
	@return	0:�����A-1:���s
	@note
	���G�t�F�N�g�����ɍĐ��I�����Ă��Ă�������Ԃ��B
*/
int SetScalePlayingEffekseer2DEffect(int playingEffectHandle, float x, float y, float z);

/**
	@brief	�Đ�����3D�\���̃G�t�F�N�g�̊g�嗦��ݒ肷��B
	@param	playingEffectHandle	�Đ����̃G�t�F�N�g�̃n���h��
	@param	x	X�����g�嗦
	@param	y	Y�����g�嗦
	@param	z	Z�����g�嗦
	@return	0:�����A-1:���s
	@note
	���G�t�F�N�g�����ɍĐ��I�����Ă��Ă�������Ԃ��B
*/
int SetScalePlayingEffekseer3DEffect(int playingEffectHandle, float x, float y, float z);

/**
	@brief	Effekseer�ɂ��Đ�����2D�G�t�F�N�g���X�V����B
	@return	0:�����A-1:���s
*/
int UpdateEffekseer2D();

/**
	@brief	Effekseer�ɂ��Đ�����2D�G�t�F�N�g��S�ĕ`�悷��B
	@return	0:�����A-1:���s
	@note
	DrawEffekseer2D_Begin�ADrawEffekseer2D_Draw�ADrawEffekseer2D_End�Ƃ͕��p�ł��Ȃ��B
*/
int DrawEffekseer2D();

/**
	@brief	Effekseer�ɂ��Đ�����2D�G�t�F�N�g�̕`����J�n����B
	@return	0:�����A-1:���s
	@note
	����̃G�t�F�N�g�݂̂�`�悵�������Ɏg�p����B
	�G�t�F�N�g�̕`��J�n�������s���B
	���̊֐������s���Ă���DrawEffekseer2D_End�����s�����܂ŁADrawEffekseer2D_Draw�����s�ł���B
	DrawEffekseer2D�Ƃ͕��p�ł��Ȃ��B
*/
int DrawEffekseer2D_Begin();

/**
	@brief	�w�肳�ꂽEffekseer�ɂ��Đ�����2D�G�t�F�N�g��`�悷��B
	@param	playingEffectHandle	�Đ����̃G�t�F�N�g�̃n���h��
	@return	0:�����A-1:���s
	@note
	����̃G�t�F�N�g�݂̂�`�悵�������Ɏg�p����B
	����̃G�t�F�N�g��`�悷��B
	DrawEffekseer2D�Ƃ͕��p�ł��Ȃ��B
*/
int DrawEffekseer2D_Draw(int playingEffectHandle);

/**
	@brief	Effekseer�ɂ��Đ�����2D�G�t�F�N�g�̕`����I������B
	@return	0:�����A-1:���s
	@note
	����̃G�t�F�N�g�݂̂�`�悵�������Ɏg�p����B
	�G�t�F�N�g�̕`��I���������s���B
	DrawEffekseer2D_Begin�����s���Ă��炪���̊֐������s�����܂ŁADrawEffekseer2D_Draw�����s�ł���B
	DrawEffekseer2D�Ƃ͕��p�ł��Ȃ��B
*/
int DrawEffekseer2D_End();
/**
	@brief	Effekseer�ɂ��Đ�����3D�G�t�F�N�g���X�V����B
	@return	0:�����A-1:���s
*/
int UpdateEffekseer3D();

/**
	@brief	Effekseer�ɂ��Đ�����3D�G�t�F�N�g��S�ĕ`�悷��B
	@return	0:�����A-1:���s
*/
int DrawEffekseer3D();

/**
	@brief	Effekseer��2D�G�t�F�N�g�Ǘ��N���X�̃C���X�^���X���擾����B
	@return	Effekseer��2D�G�t�F�N�g�Ǘ��N���X�̃C���X�^���X
	@note
	Effekseer�̋@�\�𒼐ڎg�p���Ȃ�����A���̊֐��͎g�p���Ȃ��B
*/
::Effekseer::Manager* GetEffekseer2DManager();

/**
@brief	Effekseer��2D�G�t�F�N�g�`��N���X�̃C���X�^���X���擾����B
@return	Effekseer��2D�G�t�F�N�g�`��N���X�̃C���X�^���X
@note
Effekseer�̋@�\�𒼐ڎg�p���Ȃ�����A���̊֐��͎g�p���Ȃ��B
*/
::EffekseerRendererDX9::Renderer* GetEffekseer2DRenderer();

/**
	@brief	Effekseer��3D�G�t�F�N�g�Ǘ��N���X�̃C���X�^���X���擾����B
	@return	Effekseer��3D�G�t�F�N�g�Ǘ��N���X�̃C���X�^���X
	@note
	Effekseer�̋@�\�𒼐ڎg�p���Ȃ�����A���̊֐��͎g�p���Ȃ��B
*/
::Effekseer::Manager* GetEffekseer3DManager();

/**
@brief	Effekseer��3D�G�t�F�N�g�`��N���X�̃C���X�^���X���擾����B
@return	Effekseer��3D�G�t�F�N�g�`��N���X�̃C���X�^���X
@note
Effekseer�̋@�\�𒼐ڎg�p���Ȃ�����A���̊֐��͎g�p���Ȃ��B
*/
::EffekseerRendererDX9::Renderer* GetEffekseer3DRenderer();

/**
@brief	Effekseer�̃G�t�F�N�g�N���X�̃C���X�^���X���擾����B
@param	effectHandle	�G�t�F�N�g�̃n���h��
@return	Effekseer�̃G�t�F�N�g�N���X�̃C���X�^���X
*/
::Effekseer::Effect* GetEffekseerEffect(int effectHandle);

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