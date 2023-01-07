#pragma once

class Mouse : public GameEngineActor
{

public:
	Mouse();
	~Mouse();

	Mouse(const Mouse& _Other) = delete;
	Mouse(Mouse&& _Other) noexcept = delete;
	Mouse& operator=(const Mouse& _Other) = delete;
	Mouse& operator=(Mouse&& _Other) noexcept = delete;

public:
	void ChangeMousePointerRenderer(bool _isAiming);
	void UpdatePivotPosition(const float4& _pivot);
	bool IsPointing(const float4x4& _worldWorldMatrix, const float4& _pivot);

public:
	//inline void GetCurPos()
	//{
	//	POINT Pos_;
	//	GetCursorPos(&Pos_);
	//	mousePositionInWorldSpace_.x =(GetLevel()->GetUICamera()->GetMouseWorldPositionToActor().x);
	//	mousePositionInWorldSpace_.y =(GetLevel()->GetUICamera()->GetMouseWorldPositionToActor().y);
	//	ScreenToClient(GameEngineWindow::GetHWND(), &Pos_);
	//	mousePositionInWindow_.x = static_cast<float>(Pos_.x);
	//	mousePositionInWindow_.y = static_cast<float>(Pos_.y);
	//}

	//static std::shared_ptr<Mouse> GetMouseInfo()
	//{
	//	return mainMouse_;
	//}

	//inline float4  IsCurPosToWindow()
	//{
	//	return mousePositionInWindow_;
	//}

	//inline float4  IsCurPosToClient()
	//{
	//	 return mousePositionInWorldSpace_;
	//}


protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:
	std::shared_ptr<GameEngineTextureRenderer> defaultPointerRenderer_;
	std::shared_ptr<GameEngineTextureRenderer> crossHairRenderer_;
	std::shared_ptr<GameEngineTextureRenderer> aimLineRenderer_;
	std::shared_ptr<GameEngineCollision> mouseCollision_;

	float4 mousePositionInWindow_;		//윈도우 좌표계 마우스 위치.
	float4 mousePositionInWorldSpace_;	//월드스페이스 마우스 위치.
	float4 pivotWorldPosition_;	//에임라인 피봇의 월드 포지션.
	bool isAiming_;	//true: 방향지정 투사체용 마우스포인터로 변경. false: 기본 마우스포인터 사용.

	float4 localVertexPosition_[4];
};