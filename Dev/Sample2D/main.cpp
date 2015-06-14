
#include "DxLib.h"

// EffekseerForDXLib.h���C���N���[�h���܂��B
#include "EffekseerForDXLib.h"

static int g_window_width = 640;
static int g_window_height = 480;
static ::Effekseer::Manager*			g_manager = NULL;
static ::EffekseerRendererDX9::Renderer*	g_renderer = NULL;
static ::Effekseer::Effect*				g_effect = NULL;
static ::Effekseer::Handle				g_handle = -1;

void DeviceLostFunction(void *Data)
{
	printf("Start LostFunction\n");
	// �f�o�C�X���X�g�������������ɌĂԁB
	g_renderer->OnLostDevice();

	// �ǂݍ��񂾃G�t�F�N�g�̃��\�[�X�͑S�Ĕj������B
	g_effect->UnloadResources();

	// DX���C�u�����͓����Ńf�o�C�X���̂��������Ă���̂�NULL��ݒ肷��B
	g_renderer->ChangeDevice(NULL);

	printf("End LostFunction\n");
}

void DeviceRestoreFunction(void *Data)
{
	printf("Start RestoreFunction\n");

	// DX���C�u�����͉񕜎��ɓ����Ńf�o�C�X���Đ�������̂ŐV�����ݒ肷��B
	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9) GetUseDirect3DDevice9();
	g_renderer->ChangeDevice(device);

	// �G�t�F�N�g�̃��\�[�X���ēǂݍ��݂���B
	g_effect->ReloadResources();

	// �f�o�C�X�����A����Ƃ��ɌĂ�
	g_renderer->OnResetDevice();
	printf("End RestoreFunction\n");
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
{
	// DX���C�u�����̕\�����@���E�B���h�E���[�h�ɕύX����B
	ChangeWindowMode(true);

	// DX���C�u����������������B
	if (DxLib_Init() == -1 || SetDrawScreen(DX_SCREEN_BACK) != 0) return -1;

	// �t���X�N���[���E�C���h�E�̐؂�ւ��Ń��\�[�X��������̂�h���B
	// Effekseer���g�p����ꍇ�͕K���ݒ肷��B
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// Effekseer������������B
	// �����ɂ͉�ʂɕ\������ő�p�[�e�B�N������ݒ肷��B
	if (Effkseer_Init(2000) == -1)
	{
		DxLib_End();
		return -1;
	}

	// Effekseer��2D�`��̐ݒ������B
	Effekseer_Set2DSetting(640, 480);

	g_renderer = GetEffekseerRenderer();
	g_manager = GetEffekseerManager();

	// �f�o�C�X���X�g���̃R�[���o�b�N�ݒ�(�t���X�N���[���E�C���h�E�؂�ւ��̂��߂ɕK�v)
	SetGraphicsDeviceLostCallbackFunction(DeviceLostFunction, NULL);
	SetGraphicsDeviceRestoreCallbackFunction(DeviceRestoreFunction, NULL);

	// �G�t�F�N�g�̓Ǎ�
	g_effect = Effekseer::Effect::Create(g_manager, (const EFK_CHAR*) L"test.efk");

	// ���ł���������摜�Ǎ�
	int grHandle = LoadGraph(L"Texture/Particle01.png");

	// ���Ԃ̐ݒ�
	int time = 0;

	bool isFullScreen = false;

	while (!ProcessMessage() && !ClearDrawScreen() && !CheckHitKey(KEY_INPUT_ESCAPE))
	{
		// ����I�ɃG�t�F�N�g���Đ�
		if (time % 60 == 0)
		{
			// �G�t�F�N�g�̍Đ�
			g_handle = g_manager->Play(g_effect, 0, 0, 0);
		}

		// ���ł������̂�TransFrag��L���ɂ��ĉ摜��`�悷��B
		// �������ĕ`�悵����łȂ��ƁAEffekseer�͕`��ł��Ȃ��B
		DrawGraph(0, 0, grHandle, TRUE);

		// ���_�o�b�t�@�ɗ��܂������_�f�[�^��f���o��
		RenderVertex();

		// �G�t�F�N�g�̈ړ��������s��
		g_manager->AddLocation(g_handle, ::Effekseer::Vector3D(0.2f, 0.0f, 0.0f));

		// �G�t�F�N�g�̍X�V�������s��
		g_manager->Update();

		// �G�t�F�N�g�̕`��J�n�������s���B
		g_renderer->BeginRendering();

		// �G�t�F�N�g�̕`����s���B
		g_manager->Draw();

		// �G�t�F�N�g�̕`��I���������s���B
		g_renderer->EndRendering();

		// DX���C�u�����̐ݒ��߂��B
		RefreshDxLibDirect3DSetting();

		// �X�N���[������ւ�
		ScreenFlip();

		// ���Ԍo��
		time++;

		// �t���X�N���[���؂�ւ�
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

	// �G�t�F�N�g�̔j��
	ES_SAFE_RELEASE(g_effect);

	// Effekseer���I������B
	Effkseer_End();

	// DX���C�u�������I������B
	DxLib_End();

	return 0;
}
