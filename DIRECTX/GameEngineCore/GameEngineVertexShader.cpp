#include "PreCompile.h"
#include "GameEngineVertexShader.h"
#include <GameEngineBase/GameEngineString.h>

GameEngineVertexShader::GameEngineVertexShader() 
	: ShaderPtr(nullptr)
	// , InstancingShaderPtr(nullptr)
{
	ShaderSettingType = ShaderType::Vertex;
}

GameEngineVertexShader::~GameEngineVertexShader() 
{
	//if (nullptr != InstancingShaderPtr)
	//{
	//	InstancingShaderPtr->Release();
	//	InstancingShaderPtr = nullptr;
	//}
	//if (nullptr != InstancingShaderPtr)
	//{
	//	InstancingBinaryPtr->Release();
	//	InstancingBinaryPtr = nullptr;
	//}

	if (nullptr != InstancingVertexShader)
	{
		delete InstancingVertexShader;
	}

	if (nullptr != ShaderPtr)
	{
		ShaderPtr->Release();
		ShaderPtr = nullptr;
	}

}

// Color.hlsl
// ���� ���̴����ϳ���
// ���ؽ����̴��� 2�� �ۼ����մϴ�.

// Color_VS

void GameEngineVertexShader::Setting()
{
	if (nullptr == ShaderPtr)
	{
		MsgBoxAssert("���̴� ���� ����");
	}

	// �ι�° ���ڴ� #include�� #define�� hlsl���� ����� ����� �������� ��ü�� �־��ټ� �ִ�.
	GameEngineDevice::GetContext()->VSSetShader(ShaderPtr, nullptr, 0);
}

GameEngineVertexShader* GameEngineVertexShader::Load(std::string _Path, std::string _EntryPoint, UINT _VersionHigh /*= 5*/, UINT _VersionLow /*= 0*/)
{
	return Load(_Path, GameEnginePath::GetFileName(_Path), _EntryPoint, _VersionHigh, _VersionLow);
}


GameEngineVertexShader* GameEngineVertexShader::Load(std::string _Path, std::string _Name, std::string _EntryPoint, UINT _VersionHigh = 5, UINT _VersionLow = 0)
{
	GameEngineVertexShader* NewRes = CreateResName(_Name);
	NewRes->ShaderCompile(_Path, _EntryPoint, _VersionHigh, _VersionLow);

	return NewRes;
}


void GameEngineVertexShader::ShaderCompile(std::string _Path, std::string _EntryPoint, UINT _VersionHigh, UINT _VersionLow)
{
	CreateVersion("vs", _VersionHigh, _VersionLow);
	SetEntryPoint(_EntryPoint);

	unsigned int Flag = 0;

#ifdef _DEBUG
	Flag = D3D10_SHADER_DEBUG;
#endif

	// ���̴������� �⺻������ ����� �Ʒ��� ���� ��ġ�� �Ǿ ���� �Ǿ��ִµ�
	// 1000
	// 0100
	// 2010
	// 0301

	// 1020
	// 0103
	// 0010
	// 0001


	Flag |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

	ID3DBlob* Error;

	std::wstring UnicodePath = GameEngineString::AnsiToUnicodeReturn(_Path);

	// ���̴� 
	if (D3DCompileFromFile(
		UnicodePath.c_str(), // ���� ���
		nullptr,  // ��ũ�� ()
		D3D_COMPILE_STANDARD_FILE_INCLUDE,  // ��� ()
		_EntryPoint.c_str(), // ������ COLOR_VS(
		Version.c_str(),  // vs_5_0
		Flag,
		0,
		&BinaryPtr,
		&Error)
		)
	{
		std::string ErrorText = reinterpret_cast<char*>(Error->GetBufferPointer());
		MsgBoxAssertString(ErrorText);
		return;
	}

	// �̹� �� �����ϵ� ���̴� �ڵ��� ���̳ʸ��� �־��༭ �����ϴ� ����� �٤��ϴ�.
	if (S_OK != GameEngineDevice::GetDevice()->CreateVertexShader(
		BinaryPtr->GetBufferPointer(),
		BinaryPtr->GetBufferSize(),
		nullptr,
		&ShaderPtr))
	{
		MsgBoxAssert("���ؽ� ���̴� �ڵ� ������ �����߽��ϴ�.");
	}

	ShaderResCheck();
}

void GameEngineVertexShader::InstancingShaderCompile(std::string _Path, std::string _EntryPoint, UINT _VersionHigh, UINT _VersionLow)
{
	CreateVersion("vs", _VersionHigh, _VersionLow);
	SetEntryPoint(_EntryPoint);

	unsigned int Flag = 0;

#ifdef _DEBUG
	Flag = D3D10_SHADER_DEBUG;
#endif

	// ���̴������� �⺻������ ����� �Ʒ��� ���� ��ġ�� �Ǿ ���� �Ǿ��ִµ�
	// 1000
	// 0100
	// 2010
	// 0301

	// 1020
	// 0103
	// 0010
	// 0001

	InstancingVertexShader = new GameEngineVertexShader();
	InstancingVertexShader->SetName(_EntryPoint);

	Flag |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

	ID3DBlob* Error;

	std::wstring UnicodePath = GameEngineString::AnsiToUnicodeReturn(_Path);

	// ���̴� 
	if (D3DCompileFromFile(
		UnicodePath.c_str(), // ���� ���
		nullptr,  // ��ũ�� ()
		D3D_COMPILE_STANDARD_FILE_INCLUDE,  // ��� ()
		_EntryPoint.c_str(), // ������ COLOR_VS(
		Version.c_str(),  // vs_5_0
		Flag,
		0,
		&InstancingVertexShader->BinaryPtr,
		&Error)
		)
	{
		std::string ErrorText = reinterpret_cast<char*>(Error->GetBufferPointer());
		MsgBoxAssertString(ErrorText);
		return;
	}

	// �̹� �� �����ϵ� ���̴� �ڵ��� ���̳ʸ��� �־��༭ �����ϴ� ����� �٤��ϴ�.
	if (S_OK != GameEngineDevice::GetDevice()->CreateVertexShader(
		InstancingVertexShader->BinaryPtr->GetBufferPointer(),
		InstancingVertexShader->BinaryPtr->GetBufferSize(),
		nullptr,
		&InstancingVertexShader->ShaderPtr))
	{
		MsgBoxAssert("���ؽ� ���̴� �ڵ� ������ �����߽��ϴ�.");
	}

	InstancingVertexShader->ShaderResCheck();

}