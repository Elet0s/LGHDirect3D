#include "PreCompile.h"
#include "GameEngineVertexShader.h"
#include "GameEngineDevice.h"

GameEngineVertexShader::GameEngineVertexShader()
    : vertexShader_(nullptr),
    instancingVertexShader_(nullptr)

{
    shaderType_ = ShaderType::VertexShader;
}

GameEngineVertexShader::~GameEngineVertexShader()
{
    if (nullptr != vertexShader_)
    {
        vertexShader_->Release();
        vertexShader_ = nullptr;
    }    
    
    if (nullptr != instancingVertexShader_)
    {
        delete instancingVertexShader_;
        instancingVertexShader_ = nullptr;
    }   
}

void GameEngineVertexShader::Setting()
{
    if (nullptr == vertexShader_)
    {
        MsgBoxAssert("���ؽ����̴��� �����ϴ�.");
        return;
    }

    //�ι�° ���ڴ� #include�� #define�� hlsl���� ����� ����� �������� ��ü�� �־��ټ� �ִ�.
    GameEngineDevice::GetContext()->VSSetShader(//���������ο� ���ؽ����̴��� �����ϴ� �Լ�.
        vertexShader_,                     //���ؽ� ���̴� ������.
        nullptr,                           //Ŭ���� �ν��Ͻ� �������̽� �迭 �ּҰ�. ������ NULL.
        0                                  //����� Ŭ���� �ν��Ͻ� �������̽� �迭�� �ε���.
    );
}

GameEngineVertexShader* GameEngineVertexShader::Load(
    const std::string& _path,
    const std::string& _entryPoint,
    UINT _versionHigh /*= 5*/,
    UINT _versionLow /*= 0*/
)
{
    return Load(_path, GameEnginePath::GetFileName(_path), _entryPoint, _versionHigh, _versionLow);
}

GameEngineVertexShader* GameEngineVertexShader::Load(
    const std::string& _path,
    const std::string& _name,
    const std::string& _entryPoint,
    UINT _versionHigh /*= 5*/,
    UINT _versionLow /*= 0*/
)
{
    GameEngineVertexShader* newRes = CreateNamedRes(_name);
    newRes->CreateVersion("vs", _versionHigh, _versionLow);
    newRes->SetEntrtyPoint(_entryPoint);
    newRes->CompileHLSLCode(_path);
    newRes->CreateVertexShader();
    newRes->ShaderResCheck();
    return newRes;
}

void GameEngineVertexShader::InstancingShaderCompile(
    const std::string& _path, 
    const std::string& _entryPoint,
    UINT _versionHigh /*= 5*/,
    UINT _versionLow /*= 0*/
)
{
    instancingVertexShader_ = new GameEngineVertexShader(); //GameEngineRes�� ��ϵ��� �ʴ� �� ����.
    instancingVertexShader_->SetName(_entryPoint);
    instancingVertexShader_->CreateVersion("vs", _versionHigh, _versionLow);
    instancingVertexShader_->SetEntrtyPoint(_entryPoint);
    instancingVertexShader_->CompileHLSLCode(_path);
    instancingVertexShader_->CreateInstancingShader();
    instancingVertexShader_->ShaderResCheck();
}

void GameEngineVertexShader::CreateVertexShader()
{
    if (S_OK != GameEngineDevice::GetDevice()->CreateVertexShader(  //���ؽ����̴� ���� �Լ�.
        binaryCode_->GetBufferPointer(), //�����ϵ� ���̳ʸ� �ڵ�.
        binaryCode_->GetBufferSize(),    //�����ϵ� ���̳ʸ��ڵ� ũ��.
        NULL,                               //??
        &vertexShader_                      //���ؽ����̴� ������.
    ))
    {
        MsgBoxAssert("���ؽ����̴� ���� ����.");
        return;
    }
}


void GameEngineVertexShader::CreateInstancingShader()
{
    if (S_OK != GameEngineDevice::GetDevice()->CreateVertexShader(  //���ؽ����̴� ���� �Լ�.
        binaryCode_->GetBufferPointer(), //�����ϵ� ���̳ʸ� �ڵ�.
        binaryCode_->GetBufferSize(),    //�����ϵ� ���̳ʸ��ڵ� ũ��.
        NULL,                               //??
        &vertexShader_                   //�ν��Ͻ̼��̴� ������.
    ))
    {
        MsgBoxAssert("�ν��Ͻ̼��̴� ���� ����.");
        return;
    }
}