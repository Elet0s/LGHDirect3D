#pragma once
class Player;
class PlayerUI : public GameEngineActor
{
public:
	PlayerUI();
	~PlayerUI();


	PlayerUI(const PlayerUI& _Other) = delete;
	PlayerUI(PlayerUI&& _Other) noexcept = delete;
	PlayerUI& operator=(const PlayerUI& _Other) = delete;
	PlayerUI& operator=(PlayerUI&& _Other) noexcept = delete;
protected:
	void Start() override;
	void Update(float _deltaTime) override;
	void End() override;

private:
	std::weak_ptr<Player> player_;
	std::shared_ptr<GameEngineTextureRenderer> playerHpUi_;
};