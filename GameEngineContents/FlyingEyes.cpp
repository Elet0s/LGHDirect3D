#include"PreCompile.h"
#include"FlyingEyes.h"
#include "GlobalContentsValue.h"
#include "Texture2DShadowRenderer.h"

FlyingEyes::FlyingEyes()
{

}
FlyingEyes::~FlyingEyes()
{

}
void FlyingEyes::Start()
{
	////////////Load////////////
	if (nullptr == GameEngineTexture::Find("BlackEyes.png"))
	{
		Monster::Start();
		
	}
	////////////Cut////////////
	GameEngineTexture::Cut("flyingEyes.png", 6, 1);
	


	monRenderer_ = CreateComponent<GameEngineTextureRenderer>();

	monRenderer_->GetTransform().SetLocalScale(100, 100, 100);
	monRenderer_->GetTransform().SetLocalPosition(0, 0, -100);
	monRenderer_->CreateFrameAnimation_CutTexture("flyingEyes", FrameAnimation_Desc("flyingEyes.png", 0, 5, 0.1f));
	monRenderer_->ChangeFrameAnimation("flyingEyes");

	shadowRenderer_ = CreateComponent<Texture2DShadowRenderer>();
	shadowRenderer_->SetTextureRenderer(monRenderer_);

	monCollision_ = CreateComponent<GameEngineCollision>();
	monCollision_->SetDebugSetting(CollisionType::CT_Sphere2D, float4::Red);
	monCollision_->GetTransform().SetLocalScale({ 100.0f, 50.f, 1.0f });
	monCollision_->ChangeOrder(ObjectOrder::Monster);

	monsterInfo_->atk_ = 0;
	monsterInfo_->hp_ = 10;
	monsterInfo_->maxHp_ = 10;
	monsterInfo_->baseSpeed_ = 150;
	monsterInfo_->giveExp_ = 5;

	SummonMon();

}
void FlyingEyes::Update(float _deltaTime)
{

	monCollision_->IsCollision(CollisionType::CT_Sphere2D, ObjectOrder::Monster, CollisionType::CT_Sphere2D, std::bind(&Monster::MonsterToMonsterCollision, this, std::placeholders::_1, std::placeholders::_2));
	Chaseplayer(_deltaTime);
}
void FlyingEyes::End()
{

}