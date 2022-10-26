#pragma once
#include "GameEngineActor.h"
#include "GameEngineCamera.h"

class GameEngineCameraActor: public GameEngineActor
{

public:

	GameEngineCameraActor();
	~GameEngineCameraActor();

	GameEngineCameraActor(const GameEngineCameraActor& _other) = delete;
	GameEngineCameraActor(GameEngineCameraActor&& _other) noexcept = delete;
	GameEngineCameraActor& operator=(const GameEngineCameraActor& _other) = delete;
	GameEngineCameraActor& operator=(GameEngineCameraActor&& _other) = delete;

public:
	void Start() override;
	void Update(float _deltaTime) override;
	void End() override;
	void FreeCameraModeOnOff();

public:
	GameEngineCamera* GetCameraComponent()
	{
		return cameraComponent_;
	}
	inline bool IsFreeCameraMode()
	{
		return freeCameraMode_;
	}

private:
	GameEngineCamera* cameraComponent_;
	bool freeCameraMode_;
	float cameraMovingSpeed_;
	float rotationSpeed_;

	CameraProjectionMode prevProjectionMode_;
	GameEngineTransform originalTransform_;

};
