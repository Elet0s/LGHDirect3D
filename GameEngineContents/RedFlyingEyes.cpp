#include"PreCompile.h"
#include"RedFlyingEyes.h"
#include "GlobalContentsValue.h"
RedFlyingEyes::RedFlyingEyes()
{

}
RedFlyingEyes::~RedFlyingEyes()
{

}
void RedFlyingEyes::Start()
{
	////////////Load////////////
	if (nullptr == GameEngineTexture::Find("BlackEyes.png"))
	{
		Monster::Start();
	}
	////////////Cut////////////
	GameEngineTexture::Cut("RedFlyingEyes.png", 6, 1);


	monRenderer_ = CreateComponent<GameEngineTextureRenderer>();

	monRenderer_->GetTransform().SetLocalScale(100, 100, 100);
	monRenderer_->GetTransform().SetLocalPosition(0, 0, -100);
	monRenderer_->CreateFrameAnimation_CutTexture("RedFlyingEyes", FrameAnimation_Desc("RedFlyingEyes.png", 0, 5, 0.1f));
	monRenderer_->ChangeFrameAnimation("RedFlyingEyes");

	monCollision_ = CreateComponent<GameEngineCollision>();
	monCollision_->GetTransform().SetLocalScale({ 100.0f, 100.0f, 100.0f });
	monCollision_->ChangeOrder(ObjectOrder::Monster);
	SummonMon();

}
void RedFlyingEyes::Update(float _deltaTime)
{
	Chaseplayer(_deltaTime);
}
void RedFlyingEyes::End()
{

}