#include"PreCompile.h"
#include"Bow.h"
#include"Player.h"
#include"Monster.h"

Bow::Bow()
	:projectileGroup_(),
	projectileGroupList01_(),
	projectileGroupList02_(),
	firstSerchCheak_(false),
	istarget_(false),
	targerSerchTimer_(0),
	targerSerchTimer01_(0),
	targerSerchTimer02_(0),
	targetSerchCounter_(0),
	resultCos_(),
	referenceVector_(),
	bowWeaponInfo_(),
	monsterList_(),
	minHpPair_(),
	targetInst01_(),
	targetInst02_(),
	referenceVectorList01_(),
	referenceVectorList02_(),
	passNum_()
{
	name_ = "활";
	SetName(std::string_view("Bow"));
	myRank_ = Rank::UnCommon;
	maxLevel_ = 7;
}
Bow::~Bow()
{

}
void Bow::Init()
{
	StateSet();
	std::string sDamege = std::to_string(static_cast<int>(floor(bowWeaponInfo_.weaponAtk_)));
	std::string sAttackSpeed = std::to_string(bowWeaponInfo_.weaponAtkSpeed_).substr(0, std::to_string(bowWeaponInfo_.weaponAtkSpeed_).find(".") + 3);

	etc_ = "가장가까운적에게발사합니다\n" + sDamege + " 의 피해\n" + sAttackSpeed + "초 마다 공격\n투사체 " + std::to_string(bowWeaponInfo_.weaponProjectileNum_) + "개\n ";
}
void Bow::Effect()
{
	currentlevel_ += 1;
}

void Bow::Start()
{
	
	projectileGroupList01_.reserve(20);
	projectileGroupList02_.reserve(20);
	passNum_.reserve(40);
	for (size_t i = 0; i < 40; i++) // 처음부터 최대갯수 모두 만들어서 가지고 있을 것 
	{
		passNum_.push_back(0);
		projectileGroup_.first = CreateComponent<GameEngineTextureRenderer>();
		projectileGroup_.first->GetTransform().SetWorldScale(30, 30, 0);
		projectileGroup_.first->SetTexture("Arrow.png");
		projectileGroup_.first->Off();

		projectileGroup_.second = CreateComponent<GameEngineCollision>();
		projectileGroup_.second->SetDebugSetting(CollisionType::CT_Sphere2D, float4::Blue);
		projectileGroup_.second->GetTransform().SetWorldScale(20, 20, 0);
		projectileGroup_.second->ChangeOrder(ObjectOrder::Projectile);
		projectileGroup_.second->SetCollisionMode(CollisionMode::Multiple);
		projectileGroup_.second->Off();

		if (i < 20)
		{
			projectileGroupList01_.push_back(projectileGroup_);
		}
		else if (i < 40)
		{
			projectileGroupList02_.push_back(projectileGroup_);
		}
	}
	targerSerchTimer01_ = targerSerchTimer_;
	targerSerchTimer02_ = targerSerchTimer01_ - 0.1f;

	valueSoulCard_ = SoulCard::Bow;
	Off();
}
void Bow::Update(float _deltaTime)
{
	StateSet();

	SerchTarget();
	ProjectileSort();
	RenderRotate();
	TimerUpdater(_deltaTime);
	RangeCheak(_deltaTime);

	ColCheak();
	TarGetInitialization();

}
void Bow::End()
{

}
void Bow::StateSet()
{
	PlayerInfo* Info = &Player::GetPlayerInst().GetPlayerInfo();
	PlayerPassiveInfo* PInfo = &Player::GetPlayerInst().GetPlayerPassiveInfo();

	bowWeaponInfo_.weaponAtk_ = round((5.f + (2.f * currentlevel_)) * Info->atk_ * PInfo->atkMultiple_Result / 100);
	bowWeaponInfo_.weaponAtkSpeed_ = 150.f / (Info->attackSpeed_ * PInfo->attackSpeed_Result);
	bowWeaponInfo_.weaponPassNum_ = 6 + 7 * currentlevel_ + Info->passProjectile_;
	bowWeaponInfo_.weaponSize_ = 1 * Info->projectileSize_ * PInfo->projectileSize_Result / 100;
	bowWeaponInfo_.weaponDuration_ = 100 * Info->projectileduration_ * PInfo->projectileDuration_Result / 100;
	bowWeaponInfo_.weaponSpeed_ = 1000 * Info->projectilespeed_ * PInfo->projectileSpeed_Result / 100;
	bowWeaponInfo_.weaponProjectileNum_ = 1 + Info->addProjectile_;
	bowWeaponInfo_.weponConsecutiveAtkNum_ = 1;

	if (currentlevel_ < 3)
	{
		bowWeaponInfo_.weaponAtkSpeed_ = 125.f / (Info->attackSpeed_ * PInfo->attackSpeed_Result);
		bowWeaponInfo_.weaponProjectileNum_ = 1 + Info->addProjectile_;
	}
	else if (currentlevel_ < 5)
	{
		bowWeaponInfo_.weaponAtkSpeed_ = 100.f / (Info->attackSpeed_ * PInfo->attackSpeed_Result);
		bowWeaponInfo_.weaponProjectileNum_ = 2 + Info->addProjectile_;
	}
	else
	{
		bowWeaponInfo_.weaponAtkSpeed_ = 50.f / (Info->attackSpeed_ * PInfo->attackSpeed_Result);
		bowWeaponInfo_.weaponProjectileNum_ = 3 + Info->addProjectile_;
	}

}
void Bow::SerchTarget()
{
	if (targerSerchTimer_ > 3.f)
	{
		targetSerchCounter_ = 0;
		monsterList_ = Monster::GetMonsterList();
		targetInst01_.clear();
		for (size_t n = 0; n < bowWeaponInfo_.weaponProjectileNum_; n++)//한번에 던지는 투사체 갯수만큼 반복할것임
		{
			for (size_t i = 0; i < monsterList_.size(); i++)
			{
				if (monsterList_[i]->IsSummoned() == true && monsterList_[i]->isTarget_ == false)
				{
					targetSerchCounter_ += 1;
					if (monsterList_[i]->GetMonsterInfo().hp_ > 0 && firstSerchCheak_ == false)//hp0이상, 첫번째 순번일경우
					{
						minHpPair_ = std::make_pair(i, monsterList_[i]->GetMonsterInfo().hp_);
						firstSerchCheak_ = true;
					}
					else if (minHpPair_.second > monsterList_[i]->GetMonsterInfo().hp_)//현재검사중인 몬스터 체력이 더 낮다면
					{
						minHpPair_ = std::make_pair(i, monsterList_[i]->GetMonsterInfo().hp_);
					}
				}
				if (i == monsterList_.size() - 1)
				{
					targetInst01_.push_back(minHpPair_);//타겟리스트에 추가
					monsterList_[minHpPair_.first]->isTarget_ = true;
					firstSerchCheak_ = false;
					istarget_ = true;
				}
				if (targetSerchCounter_ == 0)
				{
					istarget_ = false;
				}
			}
		}

		if (bowWeaponInfo_.weponConsecutiveAtkNum_ >= 2)
		{
			targetInst02_ = targetInst01_;
		}
	}
}
void Bow::ProjectileSort()
{
	if (istarget_ == true)
	{
		if (targerSerchTimer01_ > 3.f)
		{
			for (size_t i = 0; i < projectileGroupList01_.size(); i++)
			{
				if (targetInst01_.size() > i) // 타겟수만큼 필요
				{
					passNum_[i] = bowWeaponInfo_.weaponPassNum_;
					projectileGroupList01_[i].first->On();
					projectileGroupList01_[i].second->On();
					projectileGroupList01_[i].first->GetTransform().SetWorldPosition(Player::GetPlayerInst().GetTransform().GetWorldPosition() + (float4(0, 0, -219)));
					projectileGroupList01_[i].second->GetTransform().SetWorldPosition(Player::GetPlayerInst().GetTransform().GetWorldPosition());
				}
				else	 if (projectileGroupList01_[i].first->IsUpdate() == true)
				{
					projectileGroupList01_[i].first->Off();
					projectileGroupList01_[i].second->Off();
				}
			}
		}
		if (targerSerchTimer02_ > 3.f)
		{
			for (size_t i = 0; i < projectileGroupList02_.size(); i++)
			{
				if (targetInst02_.size() > i) // 타겟수만큼 필요
				{
					passNum_[i+20] = bowWeaponInfo_.weaponPassNum_;
					projectileGroupList02_[i].first->On();
					projectileGroupList02_[i].second->On();
					projectileGroupList02_[i].first->GetTransform().SetWorldPosition(Player::GetPlayerInst().GetTransform().GetWorldPosition() + (float4(0, 0, -219)));
					projectileGroupList02_[i].second->GetTransform().SetWorldPosition(Player::GetPlayerInst().GetTransform().GetWorldPosition());
				}
				else	 if (projectileGroupList02_[i].first->IsUpdate() == true)
				{
					projectileGroupList02_[i].first->Off();
					projectileGroupList02_[i].second->Off();
				}
			}
		}
	}
}
void Bow::RenderRotate()
{
	if (istarget_ == true)
	{
		if (targerSerchTimer01_ > 3.f)
		{
			referenceVectorList01_.clear();
			monsterList_ = Monster::GetMonsterList();

			for (size_t i = 0; i < targetInst01_.size(); i++)
			{
				float Mx = monsterList_[targetInst01_[i].first]->GetTransform().GetWorldPosition().x;
				float My = monsterList_[targetInst01_[i].first]->GetTransform().GetWorldPosition().y;
				float Px = Player::GetPlayerInst().GetTransform().GetWorldPosition().x;
				float Py = Player::GetPlayerInst().GetTransform().GetWorldPosition().y;//몬스터 옮겨진 위치로 가야함
				referenceVector_.x = (Mx - Px); //방향 구하는 공식
				referenceVector_.y = (My - Py);
				referenceVector_.w = 0;
				referenceVectorList01_.push_back(referenceVector_);

				projectileGroupList01_[i].first->GetTransform().SetWorldRotation(60, 0, -atan2f(Mx - Px, My - Py) * GameEngineMath::RadianToDegree);
			}
		}
		if (targerSerchTimer02_ > 3.f)
		{
			referenceVectorList02_.clear();
			monsterList_ = Monster::GetMonsterList();

			for (size_t i = 0; i < targetInst02_.size(); i++)
			{
				float Mx = monsterList_[targetInst02_[i].first]->GetTransform().GetWorldPosition().x;
				float My = monsterList_[targetInst02_[i].first]->GetTransform().GetWorldPosition().y;
				float Px = Player::GetPlayerInst().GetTransform().GetWorldPosition().x;
				float Py = Player::GetPlayerInst().GetTransform().GetWorldPosition().y;//몬스터 옮겨진 위치로 가야함
				referenceVector_.x = (Mx - Px); //방향 구하는 공식
				referenceVector_.y = (My - Py);
				referenceVector_.w = 0;
				referenceVectorList02_.push_back(referenceVector_);

				projectileGroupList02_[i].first->GetTransform().SetWorldRotation(60, 0, -atan2f(Mx - Px, My - Py) * GameEngineMath::RadianToDegree);
			}
		}
	}
}
void Bow::RangeCheak(float _deltaTime)
{
	if (istarget_ == true)
	{
		if (targerSerchTimer01_ <= 2.8f)
		{
			for (size_t i = 0; i < targetInst01_.size(); i++)
			{
				projectileGroupList01_[i].first->GetTransform().SetWorldMove(referenceVectorList01_[i].Normalize3D() * _deltaTime * bowWeaponInfo_.weaponSpeed_);
				projectileGroupList01_[i].second->GetTransform().SetWorldMove(referenceVectorList01_[i].Normalize3D() * _deltaTime * bowWeaponInfo_.weaponSpeed_);
			}
		}
		else
		{
			for (size_t i = 0; i < targetInst01_.size(); i++)
			{
				projectileGroupList01_[i].first->Off();
				projectileGroupList01_[i].second->Off();
			}
		}
		if (targerSerchTimer02_ <= 2.8f)
		{
			for (size_t i = 0; i < targetInst02_.size(); i++)
			{
				projectileGroupList02_[i].first->GetTransform().SetWorldMove(referenceVectorList02_[i].Normalize3D() * _deltaTime * bowWeaponInfo_.weaponSpeed_);
				projectileGroupList02_[i].second->GetTransform().SetWorldMove(referenceVectorList02_[i].Normalize3D() * _deltaTime * bowWeaponInfo_.weaponSpeed_);
			}
		}
		else
		{
			for (size_t i = 0; i < targetInst02_.size(); i++)
			{
				projectileGroupList02_[i].first->Off();
				projectileGroupList02_[i].second->Off();
			}
		}
	}
}

CollisionReturn Bow::ProjectileToMonsterCollision(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	dynamic_cast<Monster*>(_Other->GetActor())->flash_ = true;
	for (size_t i = 0; i < 40; i++)
	{
		if (i < 20)
		{
			if (projectileGroupList01_[i].second == _This)
			{

				if (dynamic_cast<Bow*>(_This->GetActor())->passNum_[i] > 0)
				{
					dynamic_cast<Bow*>(_This->GetActor())->passNum_[i] -= 1;
				}
				if (dynamic_cast<Bow*>(_This->GetActor())->passNum_[i] == 0)
				{
					projectileGroupList01_[i].first->Off();
					projectileGroupList01_[i].second->Off();
				}
			}
		}
		else if (i < 40)
		{
			if (projectileGroupList02_[i - 20].second == _This)
			{
				if (dynamic_cast<Bow*>(_This->GetActor())->passNum_[i] > 0)
				{
					dynamic_cast<Bow*>(_This->GetActor())->passNum_[i] -= 1;
				}
				if (dynamic_cast<Bow*>(_This->GetActor())->passNum_[i] == 0)
				{
					projectileGroupList02_[i - 20].first->Off();
					projectileGroupList02_[i - 20].second->Off();
				}
			}
		}
	}
	dynamic_cast<Monster*>(_Other->GetActor())->GetMonsterInfo().hp_ -= bowWeaponInfo_.weaponAtk_; //데미지줌
	return CollisionReturn::Stop;
}

void Bow::ColCheak()
{
	for (size_t i = 0; i < 30; i++)
	{
		if (i < 10)
		{
			projectileGroupList01_[i].second->IsCollision(CollisionType::CT_Sphere2D, ObjectOrder::Monster, CollisionType::CT_Sphere2D, std::bind(&Bow::ProjectileToMonsterCollision, this, std::placeholders::_1, std::placeholders::_2));
		}
		else if (i < 20)
		{
			projectileGroupList02_[i - 10].second->IsCollision(CollisionType::CT_Sphere2D, ObjectOrder::Monster, CollisionType::CT_Sphere2D, std::bind(&Bow::ProjectileToMonsterCollision, this, std::placeholders::_1, std::placeholders::_2));
		}
	}
}
void Bow::TarGetInitialization()
{
	if (Monster::GetMonsterList().size() != 0)
	{
		for (size_t i = 0; i < Monster::GetMonsterList().size() - 1; i++)
		{
			if (Monster::GetMonsterList()[i]->isTarget_ == true)
			{
				Monster::GetMonsterList()[i]->isTarget_ = false;
			}
		}
	}
}
void Bow::TimerUpdater(float _deltaTime)
{
	if (targerSerchTimer_ > 3.f)
	{
		targerSerchTimer_ = 0;
	}
	else
	{
		targerSerchTimer_ += _deltaTime;
	}

	if (targerSerchTimer01_ > 3.f)
	{
		targerSerchTimer01_ = 0;
	}
	else
	{
		targerSerchTimer01_ += _deltaTime;
	}

	if (targerSerchTimer02_ > 3.f)
	{
		targerSerchTimer02_ = 0;
	}
	else
	{
		targerSerchTimer02_ += _deltaTime;
	}
}