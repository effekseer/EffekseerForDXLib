
#include "DxLib.h"

// EffekseerForDXLib.h���C���N���[�h���܂��B
#include "EffekseerForDXLib.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// DX���C�u�����̕\�����@���E�B���h�E���[�h�ɕύX����B
	ChangeWindowMode(true);

	//�`���𗠉�ʂɕύX����B
	SetDrawScreen(DX_SCREEN_BACK);

	// DirectXEx9���g�p���Ȃ��悤�ɂ���B
	// Effekseer���g�p����ɂ͕K���ݒ肷��B
	SetUseDirect3D9Ex(FALSE);

	// DX���C�u����������������B
	if (DxLib_Init() == -1) return -1;

	// Effekseer������������B
	// �����ɂ͉�ʂɕ\������ő�p�[�e�B�N������ݒ肷��B
	if (Effkseer_Init(2000) == -1)
	{
		DxLib_End();
		return -1;
	}

	// �t���X�N���[���E�C���h�E�̐؂�ւ��Ń��\�[�X��������̂�h���B
	// Effekseer���g�p����ꍇ�͕K���ݒ肷��B
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DX���C�u�����̃f�o�C�X���X�g�������̃R�[���o�b�N��ݒ肷��B
	// �E�C���h�E�ƃt���X�N���[���̐؂�ւ�����������ꍇ�͕K�����s����B
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// Effekseer��2D�`��̐ݒ������B
	Effekseer_Set2DSetting(640, 480);

	// �G�t�F�N�g��ǂݍ��ށB
	int effectHandle = LoadEffekseerEffect("test.efk");

	// ���ł������̂ŉ摜��ǂݍ��ށB
	int grHandle = LoadGraph("Texture/Background.png");

	// ���Ԃ�����������(����I�ɃG�t�F�N�g���Đ����邽��)
	int time = 0;

	// �t���X�N���[���؂�ւ��p�t���O��ݒ肷��B(F1�AF2�ŃE�C���h�E�A�t���X�N���[����؂�ւ����悤�ɂ���B)
	bool isFullScreen = false;

	// �G�t�F�N�g�̕\������ʒu��ݒ肷��B
	float position_x = 100.0f;
	float position_y = 200.0f;

	// �Đ����̃G�t�F�N�g�̃n���h��������������B
	int playingEffectHandle = -1;

	// Z�o�b�t�@��L���ɂ���B
	// Effekseer���g�p����ꍇ�A2D�Q�[���ł�Z�o�b�t�@���g�p����B
	SetUseZBuffer3D(TRUE);

	// Z�o�b�t�@�ւ̏������݂�L���ɂ���B
	// Effekseer���g�p����ꍇ�A2D�Q�[���ł�Z�o�b�t�@���g�p����B
	SetWriteZBuffer3D(TRUE);

	while (!ProcessMessage() && !ClearDrawScreen() && !CheckHitKey(KEY_INPUT_ESCAPE))
	{
		// ����I�ɃG�t�F�N�g���Đ�����
		if (time % 60 == 0)
		{
			// �G�t�F�N�g���Đ�����B
			playingEffectHandle = PlayEffekseer2DEffect(effectHandle);

			// �G�t�F�N�g�̊g�嗦��ݒ肷��B
			// Effekseer�ō쐬�����G�t�F�N�g��2D�\���̏ꍇ�A���������邱�Ƃ��w�ǂȂ̂ŕK���g�傷��B
			SetScalePlayingEffekseer2DEffect(playingEffectHandle, 20.0f, 20.0f, 20.0f);

			// �G�t�F�N�g�̈ʒu�����Z�b�g����B
			position_x = 100.0f;
		}

		// ���ł������̂�TransFrag��L���ɂ��ĉ摜��`�悷��B
		// �������ĕ`�悵����łȂ��ƁAEffekseer�͕`��ł��Ȃ��B
		DrawGraph(0, 0, grHandle, TRUE);

		// �Đ����̃G�t�F�N�g���ړ�����B
		SetPosPlayingEffekseer2DEffect(playingEffectHandle, position_x, position_y, 0);
		position_x += 2.0f;

		// Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B
		UpdateEffekseer2D();

		// Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷��B
		DrawEffekseer2D();

		// �X�N���[�������ւ���B
		ScreenFlip();

		// ���Ԃ��o�߂�����B
		time++;

		// �t���X�N���[���̐؂�ւ����s���B
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

	// �G�t�F�N�g���폜����B(Effekseer�I�����ɔj�������̂ō폜���Ȃ��Ă�����)
	DeleteEffekseerEffect(effectHandle);
	
	// Effekseer���I������B
	Effkseer_End();

	// DX���C�u�������I������B
	DxLib_End();

	return 0;
}
