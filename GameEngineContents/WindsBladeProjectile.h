#pragma once
class WindsBladeProjectile : public GameEngineActor
{
public:
	WindsBladeProjectile();
	~WindsBladeProjectile();

	WindsBladeProjectile(const WindsBladeProjectile& _Other) = delete;
	WindsBladeProjectile(WindsBladeProjectile&& _Other) noexcept = delete;
	WindsBladeProjectile& operator=(const WindsBladeProjectile& _Other) = delete;
	WindsBladeProjectile& operator=(WindsBladeProjectile&& _Other) noexcept = delete;

	CollisionReturn ProjectileToMonster(std::shared_ptr<GameEngineCollision> _This, std::shared_ptr<GameEngineCollision> _Other);

	void ProjectileSet(float _atk, float _speed);
public:
	bool posSet_;
protected:
	void Start() override;
	void Update(float _deltaTime) override;
	void End() override;
	void Shoothing(float _deltaTime);
	void TimeOff(float _deltaTime);
	void Rotate();
private:
	std::shared_ptr < GameEngineTextureRenderer> projectileRen_;
	std::shared_ptr < GameEngineCollision> projectileCol_;
	float timer_;
	float projectileatk_;
	float projectilespeed_;
	float px_;
	float py_;
	float mpx_;
	float mpy_;
	float4 range_;
	float4 resultVector_;
	bool shoothing_;
	float4 mouseAimPos_;
	float4 playerPos_;
	float angle_;
};