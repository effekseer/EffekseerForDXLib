
#include "EffekseerForDXLib.h"

#include <map>
#include <string>
#include <vector>

static ::Effekseer::Manager*				g_manager2d = NULL;
static ::EffekseerRendererDX9::Renderer*	g_renderer2d = NULL;

static ::Effekseer::Manager*				g_manager3d = NULL;
static ::EffekseerRendererDX9::Renderer*	g_renderer3d = NULL;

static int32_t nextEffectHandle = 0;
static std::map<std::wstring, int32_t>			effectFileNameToEffectHandle;
static std::map<int32_t, std::wstring>			effectHandleToEffectFileName;
static std::map<int32_t, ::Effekseer::Effect*>	effectHandleToEffect;

Effekseer::FileInterface*					g_effectFile = nullptr;

static std::wstring ToWide(const char* pText)
{
	int Len = ::MultiByteToWideChar(CP_ACP, 0, pText, -1, NULL, 0);

	wchar_t* pOut = new wchar_t[Len + 1];
	::MultiByteToWideChar(CP_ACP, 0, pText, -1, pOut, Len);
	std::wstring Out(pOut);
	delete [] pOut;

	return Out;
}

static std::string ToMulti(const wchar_t* pText)
{
	int Len = ::WideCharToMultiByte(932, 0,  pText, -1, NULL, 0, NULL, NULL);

	char* pOut = new char[Len + 1];
	::WideCharToMultiByte(932, 0, pText, -1, pOut, Len, NULL, NULL);
	std::string Out(pOut);
	delete [] pOut;

	return Out;
}

class EffekseerFileReader :
	public Effekseer::FileReader
{
	std::vector<uint8_t>	data;
	int32_t					position;
public:
	EffekseerFileReader(std::vector<uint8_t>& data)
	{
		this->data = data;
		position = 0;
	}

	virtual ~EffekseerFileReader() {}

	size_t Read( void* buffer, size_t size )
	{
		int32_t readable = size;
		if(data.size() - position < size) readable = data.size() - position;
	
		memcpy(buffer, &(data[position]), readable);
		position += readable;
		return readable;
	}

	void Seek(int position)
	{
		this->position = position;
		if(this->position < 0) this->position = 0;
		if(this->position > data.size()) this->position = data.size();
	}

	int GetPosition()
	{
		return position;
	}

	size_t GetLength()
	{
		return data.size();
	}
};

class EffekseerFile :
	public Effekseer::FileInterface
{
public:
	EffekseerFile() {}
	virtual ~EffekseerFile() {}

	Effekseer::FileReader* OpenRead( const EFK_CHAR* path ) 
	{
		auto path_ = ToMulti((wchar_t*)path);
		auto fileHandle = FileRead_open(path_.c_str());

		if(fileHandle == 0) return 0;

		auto size = FileRead_size(path_.c_str());
		std::vector<uint8_t> data;
		data.resize(size);
		FileRead_read(data.data(), size, fileHandle);
		FileRead_close(fileHandle);

		return new EffekseerFileReader(data);
	}

	Effekseer::FileWriter* OpenWrite( const EFK_CHAR* path )
	{
		return nullptr;
	}
};

int Effkseer_Init(int particleMax)
{
	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9) GetUseDirect3DDevice9();
	if (device == NULL) return -1;

	g_effectFile = new EffekseerFile();

	// �����_���[(2D)�𐶐�����B
	g_renderer2d = ::EffekseerRendererDX9::Renderer::Create(device, particleMax);

	// �}�l�[�W���[(2D)�𐶐�����B
	g_manager2d = ::Effekseer::Manager::Create(particleMax);
	g_manager2d->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
	g_manager2d->SetEffectLoader(Effekseer::Effect::CreateEffectLoader(g_effectFile));

	// �����_���[(3D)�𐶐�����B
	g_renderer3d = ::EffekseerRendererDX9::Renderer::Create(device, particleMax);

	// �}�l�[�W���[(3D)�𐶐�����B
	g_manager3d = ::Effekseer::Manager::Create(particleMax);
	g_manager3d->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
	g_manager3d->SetEffectLoader(Effekseer::Effect::CreateEffectLoader(g_effectFile));

	// �`����@��ݒ肷��B(2D)
	g_manager2d->SetSpriteRenderer(g_renderer2d->CreateSpriteRenderer());
	g_manager2d->SetRibbonRenderer(g_renderer2d->CreateRibbonRenderer());
	g_manager2d->SetRingRenderer(g_renderer2d->CreateRingRenderer());
	g_manager2d->SetModelRenderer(g_renderer2d->CreateModelRenderer());
	g_manager2d->SetTrackRenderer(g_renderer2d->CreateTrackRenderer());

	// �`����@��ݒ肷��B(3D)
	g_manager3d->SetSpriteRenderer(g_renderer3d->CreateSpriteRenderer());
	g_manager3d->SetRibbonRenderer(g_renderer3d->CreateRibbonRenderer());
	g_manager3d->SetRingRenderer(g_renderer3d->CreateRingRenderer());
	g_manager3d->SetModelRenderer(g_renderer3d->CreateModelRenderer());
	g_manager3d->SetTrackRenderer(g_renderer3d->CreateTrackRenderer());

	// �`��p�C���X�^���X����e�N�X�`���̓Ǎ��@�\��ݒ肷��B(2D)
	// �Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���B
	g_manager2d->SetTextureLoader(g_renderer2d->CreateTextureLoader(g_effectFile));
	g_manager2d->SetModelLoader(g_renderer2d->CreateModelLoader(g_effectFile));

	// �`��p�C���X�^���X����e�N�X�`���̓Ǎ��@�\��ݒ肷��B(3D)
	// �Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���B
	g_manager3d->SetTextureLoader(g_renderer3d->CreateTextureLoader(g_effectFile));
	g_manager3d->SetModelLoader(g_renderer3d->CreateModelLoader(g_effectFile));

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
	g_manager2d->Destroy();

	g_manager3d->Destroy();

	// �`��p�C���X�^���X��j������B
	g_renderer2d->Destory();

	g_renderer3d->Destory();

	ES_SAFE_DELETE(g_effectFile);
}

void Effekseer_SetGraphicsDeviceLostCallbackFunctions()
{
	// �f�o�C�X���X�g���̃R�[���o�b�N��ݒ肷��B(�t���X�N���[���E�C���h�E�؂�ւ��̂��߂ɕK�v)
	SetGraphicsDeviceLostCallbackFunction(Effkseer_DeviceLost, NULL);
	SetGraphicsDeviceRestoreCallbackFunction(Effkseer_DeviceRestore, NULL);
}


void Effekseer_Set2DSetting(int windowWidth, int windowHeight)
{
	if(g_renderer2d == nullptr) return;

	// ���e�s���ݒ��ݒ肷��B
	g_renderer2d->SetProjectionMatrix(
		::Effekseer::Matrix44().OrthographicLH(windowWidth, windowHeight, 1.0f, 400.0f));

	// �J�����s���ݒ�
	g_renderer2d->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtLH(
		::Effekseer::Vector3D( windowWidth / 2.0f, - windowHeight / 2.0f, -200.0f), 
		::Effekseer::Vector3D( windowWidth / 2.0f, - windowHeight / 2.0f, 200.0f), 
		::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));
}

void Effekseer_Sync3DSetting()
{
	if(g_renderer3d == nullptr) return;

	MATRIX proj = GetCameraProjectionMatrix();
	MATRIX view = GetCameraViewMatrix();

	Effekseer::Matrix44 efproj, efview;

	for(int i=0;i<4;++i){
		for(int j=0;j<4;++j){
			efproj.Values[j][i] = proj.m[j][i];
			efview.Values[j][i] = view.m[j][i];
		}
	}

	g_renderer3d->SetProjectionMatrix(efproj);
	g_renderer3d->SetCameraMatrix(efview);
}

int LoadEffekseerEffect(const char* fileName)
{
	auto fileName_ = ToWide(fileName);
	return LoadEffekseerEffect(fileName_.c_str());
}

int LoadEffekseerEffect(const wchar_t* fileName)
{
	if (g_manager2d == nullptr) return -1;

	if (effectFileNameToEffectHandle.count(fileName) > 0)
	{
		return effectFileNameToEffectHandle[fileName];
	}

	auto effect = Effekseer::Effect::Create(g_manager2d, (const EFK_CHAR*) fileName);
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

int PlayEffekseer2DEffect(int effectHandle)
{
	if (g_manager2d == nullptr) return -1;

	if (effectHandleToEffect.count(effectHandle) > 0)
	{
		auto effect = effectHandleToEffect[effectHandle];
		return g_manager2d->Play(effect, 0, 0, 0);
	}

	return -1;
}

int PlayEffekseer3DEffect(int effectHandle)
{
	if (g_manager3d == nullptr) return -1;

	if (effectHandleToEffect.count(effectHandle) > 0)
	{
		auto effect = effectHandleToEffect[effectHandle];
		return g_manager3d->Play(effect, 0, 0, 0);
	}

	return -1;
}

int SetPosPlayingEffekseer2DEffect(int playingEffectHandle, float x, float y, float z)
{
	if (g_manager2d == nullptr) return -1;

	g_manager2d->SetLocation(playingEffectHandle, ::Effekseer::Vector3D(x, -y, z));

	return 0;
}

int SetPosPlayingEffekseer3DEffect(int playingEffectHandle, float x, float y, float z)
{
	if (g_manager3d == nullptr) return -1;
	
	g_manager3d->SetLocation(playingEffectHandle, ::Effekseer::Vector3D(x, y, z));
	
	return 0;
}

int SetRotationPlayingEffekseer2DEffect(int playingEffectHandle, float x, float y, float z)
{
	if (g_manager2d == nullptr) return -1;

	g_manager2d->SetRotation(playingEffectHandle, -x, -y, -z);

	return 0;
}

int SetRotationPlayingEffekseer3DEffect(int playingEffectHandle, float x, float y, float z)
{
	if (g_manager3d == nullptr) return -1;

	g_manager3d->SetRotation(playingEffectHandle, x, y, z);

	return 0;
}

int SetScalePlayingEffekseer2DEffect(int playingEffectHandle, float x, float y, float z)
{
	if (g_manager2d == nullptr) return -1;
	g_manager2d->SetScale(playingEffectHandle, x, y, z);
	return 0;
}

int SetScalePlayingEffekseer3DEffect(int playingEffectHandle, float x, float y, float z)
{
	if (g_manager3d == nullptr) return -1;
	g_manager3d->SetScale(playingEffectHandle, x, y, z);
	return 0;
}

int UpdateEffekseer2D()
{
	if (g_manager2d == nullptr) return -1;
	g_manager2d->Update();
	return 0;
}

int DrawEffekseer2D()
{
	if (g_manager2d == nullptr) return -1;
	
	// ���_�o�b�t�@�ɗ��܂������_�f�[�^��f���o���B
	RenderVertex();

	// �G�t�F�N�g�̕`��J�n�������s���B
	g_renderer2d->BeginRendering();

	// �G�t�F�N�g�̕`����s���B
	g_manager2d->Draw();

	// �G�t�F�N�g�̕`��I���������s���B
	g_renderer2d->EndRendering();

	// DX���C�u�����̐ݒ��߂��B
	RefreshDxLibDirect3DSetting();

	return 0;
}

int DrawEffekseer2D_Begin()
{
	if (g_manager2d == nullptr) return -1;
	
	// ���_�o�b�t�@�ɗ��܂������_�f�[�^��f���o���B
	RenderVertex();

	// �G�t�F�N�g�̕`��J�n�������s���B
	g_renderer2d->BeginRendering();
	
	return 0;
}

int DrawEffekseer2D_Draw(int playingEffectHandle)
{
	if (g_manager2d == nullptr) return -1;
	g_manager2d->DrawHandle(playingEffectHandle);

	return 0;
}

int DrawEffekseer2D_End()
{
	if (g_manager2d == nullptr) return -1;
	
	// �G�t�F�N�g�̕`��I���������s���B
	g_renderer2d->EndRendering();

	// DX���C�u�����̐ݒ��߂��B
	RefreshDxLibDirect3DSetting();

	return 0;
}

int UpdateEffekseer3D()
{
	if (g_manager3d == nullptr) return -1;
	g_manager3d->Update();
	return 0;
}

int DrawEffekseer3D()
{
	if (g_manager3d == nullptr) return -1;
	
	// ���_�o�b�t�@�ɗ��܂������_�f�[�^��f���o���B
	RenderVertex();

	// �G�t�F�N�g�̕`��J�n�������s���B
	g_renderer3d->BeginRendering();

	// �G�t�F�N�g�̕`����s���B
	g_manager3d->Draw();

	// �G�t�F�N�g�̕`��I���������s���B
	g_renderer3d->EndRendering();

	// DX���C�u�����̐ݒ��߂��B
	RefreshDxLibDirect3DSetting();

	return 0;
}

::Effekseer::Manager* GetEffekseer2DManager()
{
	return g_manager2d;
}

::EffekseerRendererDX9::Renderer* GetEffekseer2DRenderer()
{
	return g_renderer2d;
}

::Effekseer::Manager* GetEffekseer3DManager()
{
	return g_manager3d;
}

::EffekseerRendererDX9::Renderer* GetEffekseer3DRenderer()
{
	return g_renderer3d;
}

void Effkseer_DeviceLost(void* data)
{
	// �f�o�C�X���X�g�������������ɌĂԁB
	g_renderer2d->OnLostDevice();
	g_renderer3d->OnLostDevice();

	// �ǂݍ��񂾃G�t�F�N�g�̃��\�[�X�͑S�Ĕj������B
	for (auto e : effectHandleToEffect)
	{
		auto effect = e.second;
		effect->UnloadResources();
	}

	// DX���C�u�����͓����Ńf�o�C�X���̂��������Ă���̂�NULL��ݒ肷��B
	g_renderer2d->ChangeDevice(NULL);
	g_renderer3d->ChangeDevice(NULL);
}

void Effkseer_DeviceRestore(void* data)
{
	// DX���C�u�����͉񕜎��ɓ����Ńf�o�C�X���Đ�������̂ŐV�����ݒ肷��B
	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9) GetUseDirect3DDevice9();
	g_renderer2d->ChangeDevice(device);
	g_renderer3d->ChangeDevice(device);

	// �G�t�F�N�g�̃��\�[�X���ēǂݍ��݂���B
	for (auto e : effectHandleToEffect)
	{
		auto effect = e.second;
		effect->ReloadResources();
	}

	// �f�o�C�X�����A����Ƃ��ɌĂ�
	g_renderer2d->OnResetDevice();
	g_renderer3d->OnResetDevice();
}