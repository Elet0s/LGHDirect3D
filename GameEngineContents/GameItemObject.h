#pragma once
#include "GlobalContentsValue.h"

class GameItemObjectManager;
class Player;
class GameItemObject : public GameEngineActor
{
	friend   GameItemObjectManager;
	friend   Player;
public:
	GameItemObject();
	~GameItemObject();


	GameItemObject(const GameItemObject& _Other) = delete;
	GameItemObject(GameItemObject&& _Other) noexcept = delete;
	GameItemObject& operator=(const GameItemObject& _Other) = delete;
	GameItemObject& operator=(GameItemObject&& _Other) noexcept = delete;

public:
	bool chasePlayer_;
	ItemObjectOrder GetObjectOrder();
protected:
	void Start() override;
	void Update(float _deltaTime) override;
	void End() override;

	void LevelEndEvent() override;
	void ChasePlayer(float _deltaTime);
private:
	float ox_;
	float oy_;
	float px_;
	float py_;

	float4 range_;
	float4 resultVector_;

	ItemObjectOrder itemObjectOrder_;
	GameEngineTextureRenderer* itemObjectRenderer_;
	GameEngineCollision* itemObjectCol_;//닿으면 효과발생

};