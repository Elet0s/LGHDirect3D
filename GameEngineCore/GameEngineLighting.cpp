#include "PreCompile.h"
#include "GameEngineLighting.h"
#include "GameEngineCamera.h"

GameEngineLighting::GameEngineLighting()
{
}

GameEngineLighting::~GameEngineLighting()
{
}

void GameEngineLighting::Update(float _deltaTime)
{
	this->lightingData_.lightingPos_ = this->GetTransform().GetWorldPosition();
	this->lightingData_.lightingDirection_ = this->GetTransform().GetForwardVector();
	this->lightingData_.lightingInversedDirection_ = this->GetTransform().GetBackVector();

	this->lightingData_.lightingDirection_.w = 0.f;
	this->lightingData_.lightingInversedDirection_.w = 0.f;
	//���� ���⺤�Ϳ� ������ ���ʹ� ���⺤���̹Ƿ� w�� 0�̾�� �Ѵ�.
}

void GameEngineLighting::UpdataLightingData(std::weak_ptr<GameEngineCamera> _camera)
{
	this->GetTransform().SetViewMatrix(_camera.lock()->GetViewMatrix());
	this->GetTransform().SetProjectionMatrix(_camera.lock()->GetProjectionMatrix());
	this->GetTransform().CalculateWorldViewProjection();
	//���� ������ Ʈ�������� �����, ������� ����.

	this->lightingData_.viewLightingPos_
		= this->lightingData_.lightingPos_ * _camera.lock()->GetViewMatrix();
	this->lightingData_.viewLightingDirection_
		= this->lightingData_.lightingDirection_ * _camera.lock()->GetViewMatrix();
	this->lightingData_.viewLightingInversedDirection_
		= this->lightingData_.lightingInversedDirection_ * _camera.lock()->GetViewMatrix();
	//���������Ϳ��� �����'��' ����.

	this->lightingData_.viewLightingDirection_.Normalize3D();
	this->lightingData_.viewLightingInversedDirection_.Normalize3D();
	//������ ����� ���⺤�Ϳ� �����⺤�� ����ȭ.

	this->lightingData_.viewLightingDirection_.w = 0.f;
	this->lightingData_.viewLightingInversedDirection_.w = 0.f;
	//������ ����� ���⺤�Ϳ� �����⺤�͵� ���⺤���̹Ƿ� w�� 0���� �����.
}