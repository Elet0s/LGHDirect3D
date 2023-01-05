#include "PreCompile.h"
#include"Mouse.h"
#include "GlobalContentsValue.h"

Mouse::Mouse()
	: defaultPointerRenderer_(nullptr),
	crossHairRenderer_(nullptr),
	aimLineRenderer_(nullptr),
	mouseCollision_(nullptr),
	mousePositionInWindow_(float4::Zero),
	mousePositionInWorldSpace_(float4::Zero),
	pivotWorldPosition_(float4::Zero),
	isAiming_(true)
{

}

Mouse::~Mouse()
{

}

void Mouse::ChangeMousePointerRenderer(bool _isAiming)
{
	isAiming_ = _isAiming;
	if (true == isAiming_)
	{
		defaultPointerRenderer_->Off();
		crossHairRenderer_->On();
		aimLineRenderer_->On();
	}
	else
	{
		defaultPointerRenderer_->On();
		crossHairRenderer_->Off();
		aimLineRenderer_->Off();
	}
}

void Mouse::UpdateAimingPivot(const float4& _pivot)
{
	pivotWorldPosition_ = _pivot;
}

void Mouse::Start()
{
	//mouseCollision_ = CreateComponent<GameEngineCollision>();
	//mouseCollision_->SetDebugSetting(CollisionType::CT_OBB2D, float4::Black);
	//mouseCollision_->GetTransform().SetWorldScale( 10, 10, 10 );
	//mouseCollision_->ChangeOrder(ObjectOrder::Mouse);

	//mouseRenderer_ = CreateComponent<GameEngineTextureRenderer>();
	//mouseRenderer_->GetTransform().SetWorldScale(64, 64, 100);
	//mouseRenderer_->ChangeCamera(CameraOrder::UICamera);
	//mouseRenderer_->SetTexture("CursorSprite.png");

	this->GetTransform().SetLocalScale(float4::One);
	this->GetTransform().SetWorldScale(float4::One);

	mouseCollision_ = CreateComponent<GameEngineCollision>();
	mouseCollision_->SetDebugSetting(CollisionType::CT_OBB2D, float4::Black);
	mouseCollision_->GetTransform().SetWorldScale( 10, 10, 10 );
	mouseCollision_->GetTransform().SetLocalPosition( 20, -28, 0 );
	mouseCollision_->ChangeOrder(ObjectOrder::Mouse);

	defaultPointerRenderer_ = CreateComponent<GameEngineTextureRenderer>();
	defaultPointerRenderer_->GetTransform().SetWorldScale(40, 56, 1);
	defaultPointerRenderer_->GetTransform().SetLocalPosition( 20, -28, 0 );
	defaultPointerRenderer_->SetTexture("CursorSprite.png");
	//defaultPointerRenderer_->Off();

	crossHairRenderer_ = CreateComponent<GameEngineTextureRenderer>();
	crossHairRenderer_->GetTransform().SetWorldScale(32, 32, 1);
	crossHairRenderer_->SetTexture("CrossHair.png");
	crossHairRenderer_->Off();

	aimLineRenderer_ = CreateComponent<GameEngineTextureRenderer>();
	aimLineRenderer_->GetTransform().SetWorldScale(256, 256, 1);
	aimLineRenderer_->SetTexture("AimLine.png");
	aimLineRenderer_->Off();


}

void Mouse::Update(float _DeltaTime)
{
	//GetCurPos();
	//mouseRenderer_->GetTransform().SetLocalPosition(
	//	GetLevel()->GetUICamera()->GetMouseWorldPositionToActor().x+10.0f,
	//	GetLevel()->GetUICamera()->GetMouseWorldPositionToActor().y,
	//	-100.0f
	//);
	//mouseCollision_->GetTransform().SetLocalPosition(
	//	GetLevel()->GetUICamera()->GetMouseWorldPositionToActor().x + 10.0f,
	//	GetLevel()->GetUICamera()->GetMouseWorldPositionToActor().y, 
	//	-100.0f
	//);


	///////////////////////////////////////////////////////////////////

	//전투맵에서의 동작.
	if (CameraProjectionMode::Orthographic == this->GetLevel()->GetMainCamera()->GetProjectionMode())
	{
		float4 thisPosition = this->GetLevel()->GetUICamera()->GetMouseWorldPosition();
		thisPosition.w = 1.f;

		this->GetTransform().SetWorldPosition(thisPosition);

		if (true == isAiming_)
		{
			aimLineRenderer_->GetTransform().SetWorldPosition(
				//this->GetLevel()->GetUICameraActor()->GetTransform().GetWorldPosition()
				////나중에 플레이어포지션으로 대체.
				pivotWorldPosition_
			);


			thisPosition.z = 0.f;
			thisPosition /= thisPosition.Length();

			float aimLineAngle = 0;
			if (0 < thisPosition.x)
			{
				aimLineAngle = -acosf(thisPosition.y) * GameEngineMath::RadianToDegree;
			}
			else
			{
				aimLineAngle = acosf(thisPosition.y) * GameEngineMath::RadianToDegree;
			}


			aimLineRenderer_->GetTransform().SetWorldRotation(
				0.f, 0.f, aimLineAngle
			);


			
		}
	}
	else /*if (CameraProjectionMode::Perspective == this->GetLevel()->GetMainCamera()->GetProjectionMode())*/
	{
		//월드맵에서의 동작. 미완성.
		//this->GetTransform().SetWorldPosition(
		//	this->GetLevel()->GetMainCamera()->GetMouseWorldPosition().x,
		//	this->GetLevel()->GetMainCamera()->GetMouseWorldPosition().y,
		//	this->GetLevel()->GetMainCamera()->GetMouseWorldPosition().z * 500.f
		//);

		//this->GetTransform().SetWorldPosition(
		//	this->GetLevel()->GetMainCamera()->GetMouseWorldPosition()	// 실제로는 뷰공간좌표.
		//);
		////(1.0264, 0.5773, 1.0000)

		this->GetTransform().SetWorldPosition(
			this->GetLevel()->GetUICamera()->GetMouseWorldPosition().x,	// 실제로는 뷰공간좌표.
			this->GetLevel()->GetUICamera()->GetMouseWorldPosition().y,	// 실제로는 뷰공간좌표.
			this->GetLevel()->GetUICamera()->GetMouseWorldPosition().z + 100000.f	// 실제로는 뷰공간좌표.
		);


	}
}

void Mouse::End()
{
	//DirectX::XMPlaneIntersectLine();
	//DirectX::TriangleTests::Intersects();
}

//void Mouse::CreateMouse(GameEngineLevel* _thisLevel)
//{
//	 if (nullptr != mainMouse_)
//	 {
//		 MsgBoxAssert("마우스가 이미 존재합니다.");
//		 return;
//	 }
//	 mainMouse_ = _thisLevel->CreateActor<Mouse>(ObjectOrder::Mouse);
//	 mainMouse_->SetLevelOverOn();
//}