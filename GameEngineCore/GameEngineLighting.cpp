#include "PreCompile.h"
#include "GameEngineLighting.h"
#include "GameEngineCamera.h"
#include "GameEngineRenderTarget.h"

GameEngineLighting::GameEngineLighting()/*: lightingViewport_()*/
{
	//lightingData_.shadowDepthRenderTargetWidth_ = GameEngineWindow::GetScale().x * 1.5f;
	//lightingData_.shadowDepthRenderTargetHeight_ = GameEngineWindow::GetScale().y * 1.5f;
	//lightingData_.lightingViewFrustumNearZ_ = 0.1f;
	//lightingData_.lightingViewFrustumFarZ_ = 1000.f;
	////그림자 렌더타겟에 그림자를 그리는데 필요한 정보 입력.
}

GameEngineLighting::~GameEngineLighting()
{
}

void GameEngineLighting::Start()
{
	//lightingViewport_.TopLeftX = 0.f;
	//lightingViewport_.TopLeftY = 0.f;
	//lightingViewport_.Width = lightingData_.shadowDepthRenderTargetWidth_;
	//lightingViewport_.Height = lightingData_.shadowDepthRenderTargetHeight_;
	//lightingViewport_.MinDepth = 0.f;
	//lightingViewport_.MaxDepth = 1.f;
	////조명 뷰포트 초기화.

	//shadowDepthRenderTarget_ = GameEngineRenderTarget::Create("ShadowRenderTarget");
	//shadowDepthRenderTarget_->CreateRenderTargetTexture(
	//	float4(lightingData_.shadowDepthRenderTargetWidth_, lightingData_.shadowDepthRenderTargetHeight_),
	//	DXGI_FORMAT_R32_FLOAT,	//4색이 다 필요한게 아니라 0~1 사이 그림자 농도만 있으면 충분하므로 이 포맷으로 설정.
	//	float4::Zero
	//);
	//shadowDepthRenderTarget_->CreateDepthTexture(0);
	////그림자 정보를 받을 렌더타겟뷰와 셰이더리소스뷰, 깊이스텐실뷰 생성.
}

void GameEngineLighting::Update(float _deltaTime)
{
	this->lightingData_.lightingPosition_ = this->GetTransform().GetWorldPosition();
	this->lightingData_.lightingDirection_ = this->GetTransform().GetForwardVector();
	this->lightingData_.inverseLightingDirection_ = this->GetTransform().GetBackVector();

	this->lightingData_.lightingDirection_.w = 0.f;
	this->lightingData_.inverseLightingDirection_.w = 0.f;
	//조명 방향벡터와 역방향 벡터는 방향벡터이므로 w가 0이어야 한다.

	//lightingData_.lightingViewMatrix_.LookToLH(
	//	this->GetTransform().GetWorldPosition(),
	//	this->GetTransform().GetForwardVector(),
	//	this->GetTransform().GetUpVector()
	//);
	//lightingData_.inverseLightingViewMatrix_ = lightingData_.lightingViewMatrix_.InverseReturn();
	////조명 뷰행렬과 역행렬 설정.

	//lightingData_.lightingProjectionMatrix_.ProjectOrthographicLH(
	//	lightingData_.shadowDepthRenderTargetWidth_,
	//	lightingData_.shadowDepthRenderTargetHeight_,
	//	lightingData_.lightingViewFrustumNearZ_,
	//	lightingData_.lightingViewFrustumFarZ_
	//);
	//lightingData_.inverseLightingProjectionMatrix_ = lightingData_.lightingProjectionMatrix_.InverseReturn();
	////조명 투영행렬과 역행렬 설정.

	//lightingData_.lightingViewProjectionMatrix_ 
	//	= lightingData_.lightingViewMatrix_ * lightingData_.lightingProjectionMatrix_;
}

void GameEngineLighting::UpdataLightingData(GameEngineCamera* _camera)
{
	this->lightingData_.cameraViewMatrix_ = _camera->GetViewMatrix();
	this->lightingData_.inverseCameraViewMatrix_ = this->lightingData_.cameraViewMatrix_.InverseReturn();

	this->GetTransform().SetViewMatrix(this->lightingData_.cameraViewMatrix_);
	this->GetTransform().SetProjectionMatrix(_camera->GetProjectionMatrix());
	this->GetTransform().CalculateWorldViewProjection();
	//조명 액터의 트랜스폼에 뷰행렬, 투영행렬 적용.

	this->lightingData_.viewLightingPosition_
		= this->lightingData_.lightingPosition_ * this->lightingData_.cameraViewMatrix_;
	this->lightingData_.viewLightingDirection_
		= this->lightingData_.lightingDirection_ * this->lightingData_.cameraViewMatrix_;
	this->lightingData_.inverseViewLightingDirection_
		= this->lightingData_.inverseLightingDirection_ * this->lightingData_.cameraViewMatrix_;
	//조명데이터에도 카메라의 뷰행렬'만' 적용.

	this->lightingData_.viewLightingDirection_.Normalize3D();
	this->lightingData_.inverseViewLightingDirection_.Normalize3D();
	//조명의 뷰공간 방향벡터와 역방향벡터 정규화.

	this->lightingData_.viewLightingDirection_.w = 0.f;
	this->lightingData_.inverseViewLightingDirection_.w = 0.f;
	//조명의 뷰공간 방향벡터와 역방향벡터도 방향벡터이므로 w를 0으로 만든다.

	//this->lightingData_.viewSpaceCameraPosition_
	//	= _camera.lock()->GetTransform().GetWorldPosition() * _camera.lock()->GetViewMatrix();
	//뷰공간 카메라위치는 (0, 0, 0) 고정 아닌가?? 굳이 계산할 필요가 있나??
}

//void GameEngineLighting::SetShadowDepthRenderTarget()
//{
//	shadowDepthRenderTarget_->Clear();
//	shadowDepthRenderTarget_->Setting();
//}
