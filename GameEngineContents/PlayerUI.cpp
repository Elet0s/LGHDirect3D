#include "PreCompile.h"
#include"PlayerUI.h"
#include"GlobalContentsValue.h"
#include "Player.h"

PlayerUI::PlayerUI()
	:/*player_(),*/
	playerHpUi_(0),
	hpRedBar_(0),
	hpRedBarTregar_(false),
	ExpBlueBarTimer_(0),
	playerLevelUi_(nullptr),
	playerXindex_(0),
	playerYindex_(0),
	HitDleta_(0.f)
{

}
PlayerUI::~PlayerUI()
{

}
void PlayerUI::Start()
{
	GameEngineDirectory Dir;
	Dir.MoveParentToExistChildDirectory("ContentsResources");
	Dir.MoveToChild("ContentsResources");
	Dir.MoveToChild("UI");
	Dir.MoveToChild("PlayerUI");

	//player_ = Player::GetPlayerWeakPtr();
	hpRedBar_ = Player::GetPlayerInst().GetPlayerInfo().maxHp_;
	ExpBlueBar_ = Player::GetPlayerInst().GetPlayerInfo().maxExp_;

	playerHpUi_ = CreateComponent<GameEngineTextureRenderer>();
	playerHpUi_->SetTexture("DefaultUi.png");
	playerHpUi_->GetPixelData().mulColor_ = float4::Green; //���
	playerHpUi_->GetTransform().SetWorldScale(-100.f, 10.f, 1.f);
	playerHpUi_->ChangeCamera(CameraOrder::MidCamera);
	playerHpUi_->SetRenderingOrder(15);
	playerHpUi_->GetTransform().SetLocalPosition({0.f,100.f,-1.f });

	playerHpMax_ = CreateComponent<GameEngineTextureRenderer>();
	playerHpMax_->SetTexture("DefaultUi.png");
	playerHpMax_->GetPixelData().mulColor_ = float4::Black; //������
	playerHpMax_->GetTransform().SetLocalScale(100, 10, 1);
	playerHpMax_->ChangeCamera(CameraOrder::MidCamera);
	playerHpMax_->SetRenderingOrder(15);
	playerHpMax_->GetTransform().SetLocalPosition({ 0.f,100.f ,-3.f });

	playerHpRed_ = CreateComponent<GameEngineTextureRenderer>();
	playerHpRed_->SetTexture("DefaultUi.png");
	playerHpRed_->GetPixelData().mulColor_ = float4::Red;	//������
	playerHpRed_->GetTransform().SetWorldScale(-100.f, 10.f, 1.f);
	playerHpRed_->ChangeCamera(CameraOrder::MidCamera);
	playerHpRed_->SetRenderingOrder(15);
	playerHpRed_->GetTransform().SetLocalPosition({ 0.f,100.f,-2.f });

	playerExpMax_ = CreateComponent<GameEngineTextureRenderer>();
	playerExpMax_->SetTexture("DefaultUi.png");
	playerExpMax_->GetPixelData().mulColor_ = float4::Black; //������
	playerExpMax_->GetTransform().SetWorldScale(90, 5, 1);
	playerExpMax_->ChangeCamera(CameraOrder::MidCamera);
	playerExpMax_->SetRenderingOrder(15);
	playerExpMax_->GetTransform().SetLocalPosition({ -5.f,90.f,-1.f });


	playerExpUi_ = CreateComponent<GameEngineTextureRenderer>();
	playerExpUi_->SetTexture("DefaultUi.png");
	playerExpUi_->GetPixelData().mulColor_ = float4(0.6f, 0.6f, 1.0f, 1.0f); //�Ķ���
	playerExpUi_->GetTransform().SetLocalScale(90, 5, 1);
	playerExpUi_->GetPixelData().slice_ = float4(1.0f, 0.0f, 0.0f, 0.0f);
	playerExpUi_->ChangeCamera(CameraOrder::MidCamera);
	playerExpUi_->SetRenderingOrder(15);
	playerExpUi_->GetTransform().SetLocalPosition({ -5.f,90.f ,-3.f });

	playerExpBlue_ = CreateComponent<GameEngineTextureRenderer>();
	playerExpBlue_->SetTexture("DefaultUi.png");
	playerExpBlue_->GetPixelData().mulColor_ = float4(0.3f, 0.3f, 1.0f, 1.0f); //�Ķ���
	playerExpBlue_->GetTransform().SetWorldScale(90, 5, 1);
	playerExpBlue_->ChangeCamera(CameraOrder::MidCamera);
	playerExpBlue_->SetRenderingOrder(15);
	playerExpBlue_->GetTransform().SetLocalPosition({ -5.f,90.f ,-2.f });

	playerLevelUi_ = CreateComponent<GameEngineFontRenderer>();
	playerLevelUi_->SetSize(30.f);
	playerLevelUi_->SetLeftAndRightSort(LeftAndRightSort::Left);
	playerLevelUi_->SetText( std::to_string(Player::GetPlayerInst().GetPlayerInfo().level_), "Free Pixel");
	playerLevelUi_->SetPositionMode(FontPositionMode::World);
	playerLevelUi_->ChangeCamera(CameraOrder::MidCamera);
	playerLevelUi_->GetTransform().SetLocalPosition({ -70.f,115.f,0.f });

}
void PlayerUI::HitEffect(float _deltaTime)
{
	if (Player::GetPlayerInst().hitOnoff_ == true && playerHpUi_->GetPixelData().mulColor_ == float4::Green)
	{
		playerHpUi_->GetPixelData().mulColor_ = float4::White;//���
		Player::GetPlayerInst().hitOnoff_ = false;
	}
	else if (Player::GetPlayerInst().hitOnoff_ == true && playerHpUi_->GetPixelData().mulColor_ == float4::White)
	{
		HitDleta_ = 0;
		Player::GetPlayerInst().hitOnoff_ = false;
	}
	 if(HitDleta_ >= 0.3f && playerHpUi_->GetPixelData().mulColor_ == float4::White)
	{
		playerHpUi_->GetPixelData().mulColor_ = float4::Green;//���
		HitDleta_ = 0;
	}
}
void PlayerUI::ReduceHP(float _deltaTime)
{

	if (Player::GetPlayerInst().GetPlayerInfo().maxHp_> Player::GetPlayerInst().GetPlayerInfo().hp_)
	{
		playerHpUi_->GetPixelData().slice_ = float4(1-(Player::GetPlayerInst().GetPlayerInfo().hp_ / Player::GetPlayerInst().GetPlayerInfo().maxHp_), 0.0f, 0.0f, 0.0f);
	}
	if (hpRedBar_ > Player::GetPlayerInst().GetPlayerInfo().hp_)
	{
		hpRedBarTregar_ = true;
	}
	if (hpRedBarTregar_ == true)
	{
		if (hpRedBar_ > Player::GetPlayerInst().GetPlayerInfo().hp_)
		{
			hpRedBar_ =  GameEngineMath::Lerp(hpRedBar_,Player::GetPlayerInst().GetPlayerInfo().hp_,_deltaTime);
			playerHpRed_->GetPixelData().slice_ = float4(1-( hpRedBar_ / Player::GetPlayerInst().GetPlayerInfo().maxHp_), 0.0f, 0.0f, 0.0f);
		}
		else if (hpRedBar_ <= Player::GetPlayerInst().GetPlayerInfo().hp_)
		{
			hpRedBar_ = Player::GetPlayerInst().GetPlayerInfo().hp_;
			hpRedBarTregar_ = false;
		}

	}
}

void PlayerUI::GainExp(float _deltaTime)
{
	if (Player::GetPlayerInst().GetPlayerInfo().maxExp_ > Player::GetPlayerInst().GetPlayerInfo().exp_)
	{
		playerExpMax_->GetPixelData().slice_ = float4(Player::GetPlayerInst().GetPlayerInfo().exp_ / Player::GetPlayerInst().GetPlayerInfo().maxExp_, 0.0f, 0.0f, 0.0f);
		ExpBlueBarTimer_ += _deltaTime;
	}

	 if (ExpBlueBarTimer_ >0.3f)
	{
		playerExpUi_->GetPixelData().slice_ = float4(Player::GetPlayerInst().GetPlayerInfo().exp_ / Player::GetPlayerInst().GetPlayerInfo().maxExp_, 0.0f, 0.0f, 0.0f);
		ExpBlueBarTimer_ = 0;
	}

}
void PlayerUI::Update(float _deltaTime)
{
	GetTransform().SetWorldPosition(Player::GetPlayerInst().GetTransform().GetWorldPosition());

	playerLevelUi_->SetText(std::to_string(Player::GetPlayerInst().GetPlayerInfo().level_), "Free Pixel");

	HitDleta_ += _deltaTime;
	HitEffect(_deltaTime);
	ReduceHP(_deltaTime);
	GainExp(_deltaTime);
}
void PlayerUI::End()
{

}