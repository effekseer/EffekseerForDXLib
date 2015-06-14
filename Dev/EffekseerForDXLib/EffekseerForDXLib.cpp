
#include "EffekseerForDXLib.h"

#include <map>
#include <string>

static ::Effekseer::Manager*				g_manager = NULL;
static ::EffekseerRendererDX9::Renderer*	g_renderer = NULL;

static int32_t nextEffectHandle = 0;
static std::map<std::wstring, int32_t>			effectFileNameToEffectHandle;
static std::map<int32_t, std::wstring>			effectHandleToEffectFileName;
static std::map<int32_t, ::Effekseer::Effect*>	effectHandleToEffect;

static std::wstring ToWide(const char* pText)
{
	int Len = ::MultiByteToWideChar(CP_ACP, 0, pText, -1, NULL, 0);

	wchar_t* pOut = new wchar_t[Len + 1];
	::MultiByteToWideChar(CP_ACP, 0, pText, -1, pOut, Len);
	std::wstring Out(pOut);
	delete [] pOut;

	return Out;
}

int Effkseer_Init(int particleMax)
{
	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9) GetUseDirect3DDevice9();
	if (device == NULL) return -1;

	// �����_���[�𐶐�����B
	g_renderer = ::EffekseerRendererDX9::Renderer::Create(device, particleMax);

	// �}�l�[�W���[�𐶐�����B
	g_manager = ::Effekseer::Manager::Create(particleMax);

	// �`����@��ݒ肷��B
	g_manager->SetSpriteRenderer(g_renderer->CreateSpriteRenderer());
	g_manager->SetRibbonRenderer(g_renderer->CreateRibbonRenderer());
	g_manager->SetRingRenderer(g_renderer->CreateRingRenderer());
	g_manager->SetModelRenderer(g_renderer->CreateModelRenderer());
	g_manager->SetTrackRenderer(g_renderer->CreateTrackRenderer());

	// �`��p�C���X�^���X����e�N�X�`���̓Ǎ��@�\��ݒ肷��B
	// �Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���B
	g_manager->SetTextureLoader(g_renderer->CreateTextureLoader());
	g_manager->SetModelLoader(g_renderer->CreateModelLoader());

	return 0;
}

void Effkseer_End()
{
	// �ǂݍ��܂ꂽ�G�t�F�N�g���폜����B
	for (auto e : effectHandleToEffect)
	{
		auto effect = e.second;
		ES_SAFE_RELEASE(effect);
	}
	effectHandleToEffect.clear();

	// �G�t�F�N�g�Ǘ��p�C���X�^���X��j������B
	g_manager->Destroy();

	// �`��p�C���X�^���X��j������B
	g_renderer->Destory();
}

void Effekseer_SetGraphicsDeviceLostCallbackFunctions()
{
	// �f�o�C�X���X�g���̃R�[���o�b�N��ݒ肷��B(�t���X�N���[���E�C���h�E�؂�ւ��̂��߂ɕK�v)
	SetGraphicsDeviceLostCallbackFunction(Effkseer_DeviceLost, NULL);
	SetGraphicsDeviceRestoreCallbackFunction(Effkseer_DeviceRestore, NULL);
}


void Effekseer_Set2DSetting(int windowWidth, int windowHeight)
{
	// ���e�s���ݒ��ݒ肷��B
	g_renderer->SetProjectionMatrix(
		::Effekseer::Matrix44().OrthographicRH(windowWidth, windowHeight, 1.0f, 500.0f));

	// �J�����s���ݒ�
	g_renderer->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtRH(
		::Effekseer::Vector3D( windowWidth / 2.0f, - windowHeight / 2.0f, -20.0f), 
		::Effekseer::Vector3D( windowWidth / 2.0f, - windowHeight / 2.0f, 20.0f), 
		::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));
}

/*
// ���e�s���ݒ�
g_renderer->SetProjectionMatrix(
::Effekseer::Matrix44().PerspectiveFovRH(90.0f / 180.0f * 3.14f, (float) g_window_width / (float) g_window_height, 1.0f, 50.0f));

// �J�����s���ݒ�
g_renderer->SetCameraMatrix(
::Effekseer::Matrix44().LookAtRH(::Effekseer::Vector3D(10.0f, 5.0f, 20.0f), ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));

*/

int LoadEffekseerEffect(const char* fileName)
{
	auto fileName_ = ToWide(fileName);
	return LoadEffekseerEffect(fileName_.c_str());
}

int LoadEffekseerEffect(const wchar_t* fileName)
{
	if (g_manager == nullptr) return -1;

	if (effectFileNameToEffectHandle.count(fileName) > 0)
	{
		return effectFileNameToEffectHandle[fileName];
	}

	auto effect = Effekseer::Effect::Create(g_manager, (const EFK_CHAR*) fileName);
	if (effect == nullptr) return -1;

	int32_t handle = nextEffectHandle;
	nextEffectHandle++;

	effectFileNameToEffectHandle[fileName] = handle;
	effectHandleToEffectFileName[handle] = fileName;
	effectHandleToEffect[handle] = effect;

	return handle;
}

int DeleteEffekseerEffect(int effectHandle)
{
	if (effectHandleToEffectFileName.count(effectHandle) > 0)
	{
		auto fileName = effectHandleToEffectFileName[effectHandle];
		auto effect = effectHandleToEffect[effectHandle];
		effectFileNameToEffectHandle.erase(fileName);
		effectHandleToEffect.erase(effectHandle);
		effectHandleToEffectFileName.erase(effectHandle);

		ES_SAFE_RELEASE(effect);
	}
	
	return -1;
}

int PlayEffekseerEffect(int effectHandle)
{
	if (g_manager == nullptr) return -1;

	if (effectHandleToEffect.count(effectHandle) > 0)
	{
		auto effect = effectHandleToEffect[effectHandle];
		return g_manager->Play(effect, 0, 0, 0);
	}

	return -1;
}

int SetPosPlayingEffekseerEffect(int playingEffectHandle, float x, float y, float z)
{
	if (g_manager == nullptr) return -1;
	g_manager->SetLocation(playingEffectHandle, ::Effekseer::Vector3D(x, y, z));
	return 0;
}

int SetScalePlayingEffekseerEffect(int playingEffectHandle, float x, float y, float z)
{
	if (g_manager == nullptr) return -1;
	g_manager->SetScale(playingEffectHandle, x, y, z);
	return 0;
}

int UpdateEffekseer()
{
	if (g_manager == nullptr) return -1;
	g_manager->Update();
	return 0;
}

int DrawEffekseer()
{
	if (g_manager == nullptr) return -1;
	
	// ���_�o�b�t�@�ɗ��܂������_�f�[�^��f���o���B
	RenderVertex();

	// �G�t�F�N�g�̕`��J�n�������s���B
	g_renderer->BeginRendering();

	// �G�t�F�N�g�̕`����s���B
	g_manager->Draw();

	// �G�t�F�N�g�̕`��I���������s���B
	g_renderer->EndRendering();

	// DX���C�u�����̐ݒ��߂��B
	RefreshDxLibDirect3DSetting();

	return 0;
}

::Effekseer::Manager* GetEffekseerManager()
{
	return g_manager;
}

::EffekseerRendererDX9::Renderer* GetEffekseerRenderer()
{
	return g_renderer;
}

void Effkseer_DeviceLost(void* data)
{
	// �f�o�C�X���X�g�������������ɌĂԁB
	g_renderer->OnLostDevice();

	// �ǂݍ��񂾃G�t�F�N�g�̃��\�[�X�͑S�Ĕj������B
	for (auto e : effectHandleToEffect)
	{
		auto effect = e.second;
		effect->UnloadResources();
	}

	// DX���C�u�����͓����Ńf�o�C�X���̂��������Ă���̂�NULL��ݒ肷��B
	g_renderer->ChangeDevice(NULL);
}

void Effkseer_DeviceRestore(void* data)
{
	// DX���C�u�����͉񕜎��ɓ����Ńf�o�C�X���Đ�������̂ŐV�����ݒ肷��B
	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9) GetUseDirect3DDevice9();
	g_renderer->ChangeDevice(device);

	// �G�t�F�N�g�̃��\�[�X���ēǂݍ��݂���B
	for (auto e : effectHandleToEffect)
	{
		auto effect = e.second;
		effect->ReloadResources();
	}

	// �f�o�C�X�����A����Ƃ��ɌĂ�
	g_renderer->OnResetDevice();
}