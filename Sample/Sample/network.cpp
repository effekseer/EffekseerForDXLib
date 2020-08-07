
#include "DxLib.h"

// EffekseerForDXLib.h���C���N���[�h���܂��B
#include "EffekseerForDXLib.h"

int network()
{
	// ���̃\�[�X�R�[�h�����s���AEffekseer��Laser01.efkefc���J���A�ҏW���ăl�b�g���[�N�̑��M�{�^���������Ă��������B

	// �l�b�g���[�N�̃T���v���̂���DirectX11�Œ肩�A�����̐����͈ꕔ�Ȃ��B

	// DX���C�u������Effekseer������������B
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

	// �l�b�g���[�N�@�\��L���ɂ���B
	// �G�t�F�N�g��ǂݍ��ޑO�Ɏ��s����B
	Effekseer_StartNetwork(60000);

	// �G�t�F�N�g���\�[�X��ǂݍ��ށB
	// �ǂݍ��ގ��ɑ傫�����w�肷��B
	int effectResourceHandle = LoadEffekseerEffect("Laser01.efkefc", 1.0f);

	// ���ł������̂ŉ摜��ǂݍ��ށB
	int grBackgroundHandle = LoadGraph(_T("Texture/Background.png"));
	int grFrontHandle = LoadGraph(_T("Texture/Front.png"));

	// ���Ԃ�����������(����I�ɃG�t�F�N�g���Đ����邽��)
	int time = 0;

	// �G�t�F�N�g�̕\������ʒu��ݒ肷��B
	float position_x = 0.0f;
	float position_y = 0.0f;

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
		// DX���C�u�����̃J������ݒ肷��B
		SetCameraPositionAndTarget_UpVecY(VGet(10, 10, -20), VGet(0, 0, 0));
		SetupCamera_Perspective(60.0f * DX_PI_F / 180.0f);
		SetCameraNearFar(1.0f, 150.0f);

		// DX���C�u�����̃J������Effekseer�̃J�����𓯊�����B
		Effekseer_Sync3DSetting();

		// ����I�ɃG�t�F�N�g���Đ�����
		if (time % 60 == 0)
		{
			// �G�t�F�N�g���Đ�����B
			playingEffectHandle = PlayEffekseer3DEffect(effectResourceHandle);

			// �G�t�F�N�g�̈ʒu�����Z�b�g����B
			position_x = 0.0f;
		}

		// ���ł������̂ŉ摜��`�悷��B
		// �������ĕ`�悵����łȂ��ƁAEffekseer�͕`��ł��Ȃ��B
		DrawGraph(0, 0, grBackgroundHandle, TRUE);

		// �Đ����̃G�t�F�N�g���ړ�����B
		SetPosPlayingEffekseer3DEffect(playingEffectHandle, position_x, position_y, 0);
		position_x += 0.2f;

		// Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B
		UpdateEffekseer3D();

		// 3D��\������B
		DrawCapsule3D(
			VGet(0.0f, 100.0f, 0.0f), VGet(0.0f, -100.0f, 0.0f), 6.0f, 16, GetColor(100, 100, 100), GetColor(255, 255, 255), TRUE);

		// Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷��B
		DrawEffekseer3D();

		// �G�t�F�N�g�̏�ɂ��摜��`��ł���B
		DrawGraph(0, 0, grFrontHandle, TRUE);

		// �X�N���[�������ւ���B
		ScreenFlip();

		// ���Ԃ��o�߂�����B
		time++;
	}

	// �G�t�F�N�g���\�[�X���폜����B(Effekseer�I�����ɔj�������̂ō폜���Ȃ��Ă�����)
	DeleteEffekseerEffect(effectResourceHandle);

	// Effekseer���I������B
	Effkseer_End();

	// DX���C�u�������I������B
	DxLib_End();

	return 0;
}
