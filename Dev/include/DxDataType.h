// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�f�[�^�^�C�v��`�w�b�_�t�@�C��
// 
// 				Ver 3.15a
// 
// -------------------------------------------------------------------------------

#ifndef __DXDATATYPE_H__
#define __DXDATATYPE_H__

// �C���N���[�h ------------------------------------------------------------------
#include "DxCompileConfig.h"
#include <stdio.h>

#ifdef __WINDOWS__
#include "DxDataTypeWin.h"
#endif

#ifdef __PSVITA
#include "DxDataTypePSVita.h"
#endif

#ifdef __PS4
#include "DxDataTypePS4.h"
#endif

#ifdef __ANDROID
#include "DxDataTypeAndroid.h"
#endif

#ifdef DX_USE_NAMESPACE

namespace DxLib
{

#endif // DX_USE_NAMESPACE

// �}�N����` --------------------------------------------------------------------

#define SETRECT( Dest, Left, Top, Right, Bottom )	\
	( Dest ).left   = Left ;\
	( Dest ).top    = Top ;\
	( Dest ).right  = Right ;\
	( Dest ).bottom = Bottom ;

// �\���̒�` --------------------------------------------------------------------

// �q�f�a�`�F�\����
struct RGBCOLOR
{
	unsigned char			Blue, Green, Red, Alpha ;
} ;

// �e�[�u��-----------------------------------------------------------------------

// �������ϐ��錾 --------------------------------------------------------------

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

#ifdef DX_USE_NAMESPACE

}

#endif // DX_USE_NAMESPACE

#endif // __DXDATATYPE_H__
