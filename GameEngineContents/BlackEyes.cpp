#include"PreCompile.h"
#include"BlackEyes.h"
#include "GlobalContentsValue.h"

BlackEyes::BlackEyes()
{

}
BlackEyes::~BlackEyes()
{

}

void BlackEyes::Start()
{

	////////////Load////////////
	if (nullptr == GameEngineTexture::Find("BlackEyes.png"))
	{
		Monster::Start();
	}
	////////////Cut////////////
	GameEngineTexture::Cut("BlackEyes.png", 6, 1);

	monRenderer_ = CreateComponent<GameEngineTextureRenderer>();

	monRenderer_->GetTransform().SetLocalScale(100, 100, 100);
	monRenderer_->GetTransform().SetLocalPosition(0, 0, -100);
	monRenderer_->CreateFrameAnimation_CutTexture("BlackEyes", FrameAnimation_Desc("BlackEyes.png", 0, 5, 0.1f));
	monRenderer_->ChangeFrameAnimation("BlackEyes");

	monCollision_ = CreateComponent<GameEngineCollision>();
	monCollision_->GetTransform().SetLocalScale({ 100.0f, 100.0f, 100.0f });
	monCollision_->ChangeOrder(ObjectOrder::Monster);
	SummonMon();

}
void BlackEyes::Update(float _deltaTime)
{
	Chaseplayer(_deltaTime);
}
void BlackEyes::End()
{

}