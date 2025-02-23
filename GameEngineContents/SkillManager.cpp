#include "PreCompile.h"
#include "SkillManager.h"

#include"Cleaver.h"
#include"Kunai.h"
#include"Pike.h"
#include "Spear.h"

#include"DeathAura.h"
#include"Firering.h"

#include"ThunderStaf.h"
#include"WindsBlade.h"

#include "Shuriken.h"

#include"Katana.h"
#include"Crossbow.h"
#include"Bow.h"

// Rank : Spotted [0]

#include "LowPressure.h"
#include "Healthy.h"
#include "Durability.h"
#include "Durability.h"
#include "StoneSkin.h"
#include "SharpeningStone.h"
#include "Cardio.h"

// Rank : Nomal [1]

#include "Agility.h"
#include "Void.h"
#include "Student.h"
#include "IronWill.h"
#include "HealingFactor.h"
#include "BlackBlood.h"
#include "LightArmor.h"
#include "SharpEdge.h"
#include "Range.h"
#include "TensString.h"
#include "Momentum.h"
#include "Muscle.h"
#include "Compromise.h"
#include "Practice.h"
#include "DamascusSteel.h"

// Rank : UnCommon [2]
#include "Dash.h"
#include "IronSkin.h"
#include "SlipperySkin.h"
#include "Impenetrable.h"
#include "Aiming.h"
#include "Area.h"
#include "Quick.h"
#include "Clumsy.h"
#include "TradeOff.h"

// Rank : Rare [3]
#include "DashCooldown.h"
#include "Tower.h"
#include "SteelSkin.h"
#include "Celerity.h"
#include "GodsWrath.h"
#include "Penetration.h"

// Rank : Epic [4]
#include "SingleHanded.h"
#include "LightHand.h"
#include "Hypersonic.h"
#include "StoneHanded.h"
#include "Castle.h"



#include "Player.h"

SkillManager::SkillManager()
{
	
}
SkillManager::~SkillManager()
{
}

void SkillManager::CreatePlayerAllSkill()
{
	skillList_.resize(6);

	skillList_[5].reserve(12);
	{
		Skill* Skill00 = myLevel_->CreateActor<Cleaver>(ObjectOrder::Skill, "Cleaver");
		skillList_[5].push_back(Skill00);
		Skill* Skill01 = myLevel_->CreateActor<Kunai>(ObjectOrder::Skill, "Kunai");
		skillList_[5].push_back(Skill01);
		Skill* Skill02 = myLevel_->CreateActor<Pike>(ObjectOrder::Skill, "Pike");
		skillList_[5].push_back(Skill02);
		Skill* Skill03 = myLevel_->CreateActor<Spear>(ObjectOrder::Skill, "Spear");
		skillList_[5].push_back(Skill03);
		Skill* Skill04 = myLevel_->CreateActor<DeathAura>(ObjectOrder::Skill, "DeathAura");
		skillList_[5].push_back(Skill04);

		Skill* Skill05 = myLevel_->CreateActor<Firering>(ObjectOrder::Skill, "Firering");
		skillList_[5].push_back(Skill05);
		Skill* Skill06 = myLevel_->CreateActor<ThunderStaf>(ObjectOrder::Skill, "ThunderStaf");
		skillList_[5].push_back(Skill06);
		Skill* Skill07 = myLevel_->CreateActor<WindsBlade>(ObjectOrder::Skill, "WindBlade #44160");
		skillList_[5].push_back(Skill07);
		Skill* Skill08 = myLevel_->CreateActor<Bow>(ObjectOrder::Skill, "Bow");
		skillList_[5].push_back(Skill08);

		Skill* Skill09 = myLevel_->CreateActor<Crossbow>(ObjectOrder::Skill, "Crossbow");
		skillList_[5].push_back(Skill09);
		Skill* Skill10 = myLevel_->CreateActor<Katana>(ObjectOrder::Skill, "Katana");
		skillList_[5].push_back(Skill10);
		Skill* Skill11 = myLevel_->CreateActor<Shuriken>(ObjectOrder::Skill, "Shuriken");
		skillList_[5].push_back(Skill11);
	}


	// 더럽혀진 랭크 
	skillList_[0].reserve(6);
	{
		Skill* Skill00 = myLevel_->CreateActor<LowPressure>(ObjectOrder::Skill,"LowPressure");
		skillList_[0].push_back(Skill00);

		Skill* Skill01 = myLevel_->CreateActor<Healthy>(ObjectOrder::Skill, "HealingFactor");
		skillList_[0].push_back(Skill01);

		Skill* Skill02 = myLevel_->CreateActor<Durability>(ObjectOrder::Skill, "Durability");
		skillList_[0].push_back(Skill02);

		Skill* Skill04 = myLevel_->CreateActor<StoneSkin>(ObjectOrder::Skill, "StoneSkin");
		skillList_[0].push_back(Skill04);

		Skill* Skill05 = myLevel_->CreateActor<SharpeningStone>(ObjectOrder::Skill, "SharpeningStone");
		skillList_[0].push_back(Skill05);

		Skill* Skill06 = myLevel_->CreateActor<Cardio>(ObjectOrder::Skill, "Cardio");
		skillList_[0].push_back(Skill06);
	}

	// 노말 랭크
	skillList_[1].reserve(16);
	{
		Skill* Skill00 = myLevel_->CreateActor<Agility>(ObjectOrder::Skill, "Agility");
		skillList_[1].push_back(Skill00);
		
		Skill* Skill01 = myLevel_->CreateActor<Void>(ObjectOrder::Skill, "Void");
		skillList_[1].push_back(Skill01);

		Skill* Skill02 = myLevel_->CreateActor<Student>(ObjectOrder::Skill, "Student");
		skillList_[1].push_back(Skill02);

		Skill* Skill03 = myLevel_->CreateActor<IronWill>(ObjectOrder::Skill, "IronWill");
		skillList_[1].push_back(Skill03);

		Skill* Skill04 = myLevel_->CreateActor<HealingFactor>(ObjectOrder::Skill, "HealingFactor");
		skillList_[1].push_back(Skill04);

		Skill* Skill06 = myLevel_->CreateActor<BlackBlood>(ObjectOrder::Skill, "BlackBlood");
		skillList_[1].push_back(Skill06);

		Skill* Skill07 = myLevel_->CreateActor<LightArmor>(ObjectOrder::Skill, "LightArmor");
		skillList_[1].push_back(Skill07);

		Skill* Skill08 = myLevel_->CreateActor<SharpEdge>(ObjectOrder::Skill, "SharpEdge");
		skillList_[1].push_back(Skill08);

		Skill* Skill09 = myLevel_->CreateActor<Range>(ObjectOrder::Skill, "Range");
		skillList_[1].push_back(Skill09);

		Skill* Skill10 = myLevel_->CreateActor<TensString>(ObjectOrder::Skill, "TensString");
		skillList_[1].push_back(Skill10);

		Skill* Skill11 = myLevel_->CreateActor<Momentum>(ObjectOrder::Skill, "Momentum");
		skillList_[1].push_back(Skill11);

		Skill* Skill12 = myLevel_->CreateActor<Muscle>(ObjectOrder::Skill, "Muscle");
		skillList_[1].push_back(Skill12);

		Skill* Skill13 = myLevel_->CreateActor<Compromise>(ObjectOrder::Skill, "Compromise");
		skillList_[1].push_back(Skill13);

		Skill* Skill15 = myLevel_->CreateActor<Practice>(ObjectOrder::Skill, "Practice");
		skillList_[1].push_back(Skill15);

		Skill* Skill16 = myLevel_->CreateActor<DamascusSteel>(ObjectOrder::Skill, "DamascusSteel");
		skillList_[1].push_back(Skill16);
	}


	// 언커먼 랭크
	skillList_[2].reserve(9);
	{
		Skill* Skill00 = myLevel_->CreateActor<Dash>(ObjectOrder::Skill, "Dash");
		skillList_[2].push_back(Skill00);

		Skill* Skill01 = myLevel_->CreateActor<IronSkin>(ObjectOrder::Skill, "IronSkin");
		skillList_[2].push_back(Skill01);

		Skill* Skill02 = myLevel_->CreateActor<SlipperySkin>(ObjectOrder::Skill, "SlipperySkin");
		skillList_[2].push_back(Skill02);

		Skill* Skill03 = myLevel_->CreateActor<Impenetrable>(ObjectOrder::Skill, "Impenetrable");
		skillList_[2].push_back(Skill03);

		Skill* Skill04 = myLevel_->CreateActor<Aiming>(ObjectOrder::Skill, "Aiming");
		skillList_[2].push_back(Skill04);

		Skill* Skill05 = myLevel_->CreateActor<Area>(ObjectOrder::Skill, "Area");
		skillList_[2].push_back(Skill05);

		Skill* Skill06 = myLevel_->CreateActor<Quick>(ObjectOrder::Skill, "Quick");
		skillList_[2].push_back(Skill06);

		Skill* Skill07 = myLevel_->CreateActor<Clumsy>(ObjectOrder::Skill, "Clumsy");
		skillList_[2].push_back(Skill07);

		Skill* Skill08 = myLevel_->CreateActor<TradeOff>(ObjectOrder::Skill, "TradeOff");
		skillList_[2].push_back(Skill08);
	}

	// 레어 랭크
	skillList_[3].reserve(6);
	{
		Skill* Skill00 = myLevel_->CreateActor<DashCooldown>(ObjectOrder::Skill, "DashCooldown");
		skillList_[3].push_back(Skill00);

		Skill* Skill01 = myLevel_->CreateActor<Tower>(ObjectOrder::Skill, "Tower");
		skillList_[3].push_back(Skill01);

		Skill* Skill02 = myLevel_->CreateActor<SteelSkin>(ObjectOrder::Skill, "SteelSkin");
		skillList_[3].push_back(Skill02);


		Skill* Skill03 = myLevel_->CreateActor<Celerity>(ObjectOrder::Skill, "Celerity");
		skillList_[3].push_back(Skill03);


		Skill* Skill04 = myLevel_->CreateActor<GodsWrath>(ObjectOrder::Skill, "GodsWrath");
		skillList_[3].push_back(Skill04);

		Skill* Skill05 = myLevel_->CreateActor<Penetration>(ObjectOrder::Skill, "Penetration");
		skillList_[3].push_back(Skill05);

		
	}

	// 에픽 랭크
	skillList_[4].reserve(4);
	{
		Skill* Skill00 = myLevel_->CreateActor<SingleHanded>(ObjectOrder::Skill, "SingleHanded");
		skillList_[4].push_back(Skill00);

		Skill* Skill01 = myLevel_->CreateActor<LightHand>(ObjectOrder::Skill, "LightHand");
		skillList_[4].push_back(Skill01);

		Skill* Skill02 = myLevel_->CreateActor<Hypersonic>(ObjectOrder::Skill, "Hypersonic");
		skillList_[4].push_back(Skill02);

		Skill* Skill03 = myLevel_->CreateActor<Castle>(ObjectOrder::Skill, "Castle");
		skillList_[4].push_back(Skill03);
	}
}

void SkillManager::SkillLevelCheak()
{
	if (skillList_[5][0]->currentlevel_ > 0 && skillList_[5][0]->IsUpdate() == false) //cleaver
	{
		skillList_[5][0]->On();
	}
	if (skillList_[5][1]->currentlevel_ > 0 && skillList_[5][1]->IsUpdate() == false) //kunai
	{
		skillList_[5][1]->On();
	}
	if (skillList_[5][2]->currentlevel_ > 0 && skillList_[5][2]->IsUpdate() == false) //pike
	{
		skillList_[5][2]->On();
	}
	if (skillList_[5][3]->currentlevel_ > 0 && skillList_[5][3]->IsUpdate() == false) //spear
	{
		skillList_[5][3]->On();
	}
	if (skillList_[5][4]->currentlevel_ > 0 && skillList_[5][4]->IsUpdate() == false) //DeathAura
	{
		skillList_[5][4]->On();
	}

	if (skillList_[5][5]->currentlevel_ > 0 && skillList_[5][5]->IsUpdate() == false) //FireAura
	{
		skillList_[5][5]->On();
	}

	
	if (skillList_[5][6]->currentlevel_ > 0 && skillList_[5][6]->IsUpdate() == false) //thunderStaf
	{
		skillList_[5][6]->On();
	}

	if (skillList_[5][7]->currentlevel_ > 0 && skillList_[5][7]->IsUpdate() == false) //WindsBlads
	{
		skillList_[5][7]->On();
	}
	if (skillList_[5][8]->currentlevel_ > 0 && skillList_[5][8]->IsUpdate() == false) //Bow
	{
		skillList_[5][8]->On();
		for (size_t i = 0; i < 40; i++)
			if (i < 20)
			{
				dynamic_cast<Bow*>(skillList_[5][8])->projectileGroupList01_[i].second->GetTransform().SetWorldPosition(Player::GetPlayerInst().GetTransform().GetWorldPosition().x, Player::GetPlayerInst().GetTransform().GetWorldPosition().y, -220);
				dynamic_cast<Bow*>(skillList_[5][8])->projectileGroupList01_[i].first->GetTransform().SetWorldPosition(Player::GetPlayerInst().GetTransform().GetWorldPosition().x, Player::GetPlayerInst().GetTransform().GetWorldPosition().y, -220);
			}
			else if (i < 40)
			{
				dynamic_cast<Bow*>(skillList_[5][8])->projectileGroupList02_[i - 20].first->GetTransform().SetWorldPosition(Player::GetPlayerInst().GetTransform().GetWorldPosition().x, Player::GetPlayerInst().GetTransform().GetWorldPosition().y, -220);
				dynamic_cast<Bow*>(skillList_[5][8])->projectileGroupList02_[i - 20].second->GetTransform().SetWorldPosition(Player::GetPlayerInst().GetTransform().GetWorldPosition().x, Player::GetPlayerInst().GetTransform().GetWorldPosition().y, -220);
			}
	}
	if (skillList_[5][9]->currentlevel_ > 0 && skillList_[5][9]->IsUpdate() == false)
	{
		skillList_[5][9]->On();
		for (size_t i = 0; i < 20; i++)
		{
			dynamic_cast<Crossbow*>(skillList_[5][9])->projectileGroupList_[i].first->GetTransform().SetWorldPosition(Player::GetPlayerInst().GetTransform().GetWorldPosition().x, Player::GetPlayerInst().GetTransform().GetWorldPosition().y, -220);
			dynamic_cast<Crossbow*>(skillList_[5][9])->projectileGroupList_[i].second->GetTransform().SetWorldPosition(Player::GetPlayerInst().GetTransform().GetWorldPosition().x, Player::GetPlayerInst().GetTransform().GetWorldPosition().y, -220);
		}
	}


	if (skillList_[5][10]->currentlevel_ > 0 && skillList_[5][10]->IsUpdate() == false)//katana
	{
		skillList_[5][10]->On();
	}

	if (skillList_[5][11]->currentlevel_ > 0 && skillList_[5][11]->IsUpdate() == false) //shuriken
	{
		skillList_[5][11]->On();
		//플레이어 위치로 이동시키고 on
		for (size_t i = 0; i < 60; i++)
		{
			if (i < 20)
			{
				dynamic_cast<Shuriken*>(skillList_[5][11])->projectileGroupList01_[i].first->GetTransform().SetWorldPosition(Player::GetPlayerInst().GetTransform().GetWorldPosition().x, Player::GetPlayerInst().GetTransform().GetWorldPosition().y, -220);
				dynamic_cast<Shuriken*>(skillList_[5][11])->projectileGroupList01_[i].second->GetTransform().SetWorldPosition(Player::GetPlayerInst().GetTransform().GetWorldPosition().x, Player::GetPlayerInst().GetTransform().GetWorldPosition().y, -220);
			}
			else if (i < 40)
			{
				dynamic_cast<Shuriken*>(skillList_[5][11])->projectileGroupList02_[i - 20].first->GetTransform().SetWorldPosition(Player::GetPlayerInst().GetTransform().GetWorldPosition().x, Player::GetPlayerInst().GetTransform().GetWorldPosition().y, -220);
				dynamic_cast<Shuriken*>(skillList_[5][11])->projectileGroupList02_[i - 20].second->GetTransform().SetWorldPosition(Player::GetPlayerInst().GetTransform().GetWorldPosition().x, Player::GetPlayerInst().GetTransform().GetWorldPosition().y, -220);
			}
			else if (i < 60)
			{
				dynamic_cast<Shuriken*>(skillList_[5][11])->projectileGroupList03_[i - 40].first->GetTransform().SetWorldPosition(Player::GetPlayerInst().GetTransform().GetWorldPosition().x, Player::GetPlayerInst().GetTransform().GetWorldPosition().y, -220);
				dynamic_cast<Shuriken*>(skillList_[5][11])->projectileGroupList03_[i - 40].second->GetTransform().SetWorldPosition(Player::GetPlayerInst().GetTransform().GetWorldPosition().x, Player::GetPlayerInst().GetTransform().GetWorldPosition().y, -220);
			}
		}
	}
}
