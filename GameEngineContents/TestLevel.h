#pragma once
#include "GlobalContentsValue.h"

class Mouse;
class FieldRenderingActor;
class TestLevel : public GameEngineLevel
{
public:
	TestLevel();
	~TestLevel();

	TestLevel(const TestLevel& _Other) = delete;
	TestLevel(TestLevel&& _Other) noexcept = delete;
	TestLevel& operator=(const TestLevel& _Other) = delete;
	TestLevel& operator=(TestLevel&& _Other) noexcept = delete;

	Mouse* GetMousePointer()
	{
		return mousePointer_;
	}

	StageType GetstageType()
	{
		return stageType_;
	}


	CombatType GetCombatType()
	{
		return combatType_;
	}

protected:
	void Start() override;
	void Update(float _deltaTime) override;
	void End() override {};
	void LevelStartEvent() override;
	void LevelEndEvent() override;


private:
	void MouseMoveCamera();
	void PlayerMoveCamera();

	void StageMonsterManager();

private:
	FieldRenderingActor* fieldRenderingActor_;
	GameEngineLighting* testLevelLighting_;
	Mouse* mousePointer_;
	class StageUI* stageUI_;

	StageType stageType_;
	CombatType combatType_;
	float stageManagerTimer_;
	int killCount_;
	float timeLimit_;
	size_t summonCounter_;
};
