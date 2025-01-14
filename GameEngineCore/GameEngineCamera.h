#pragma once
#include "GameEngineLevel.h"
#include "GameEngineTransformComponent.h"
#include "GameEngineLighting.h"

enum class ProjectionMode
{
	Perspective, //원근투영.
	Orthographic //직교투영.
};

class GameEngineInstanceRenderer;
class GameEngineMaterial;
class GameEngineRenderTarget;
class GameEngineRenderUnit;
class GameEngineCamera : public GameEngineTransformComponent
{
	//카메라. 
	//월드공간의 오브젝트들이 뷰 프러스텀이라고 하는 가상의 카메라 시야범위 안에 들어 왔을때, 
	// 오브젝트들을 구성하는 정점들을 뷰행렬에 맞춰서 재배치하고 투영행렬에 맞춰서 축소한 후
	// 뷰포트행렬대로 다시 확대한 만큼의 정점 좌표를 계산해서 변형된 대로 그리는 컴포넌트.

	friend GameEngineLevel;
	friend GameEngineRenderer;

public:

	GameEngineCamera();
	~GameEngineCamera();

	GameEngineCamera(const GameEngineCamera& _other) = delete;
	GameEngineCamera(GameEngineCamera&& _other) noexcept = delete;
	GameEngineCamera& operator=(const GameEngineCamera& _other) = delete;
	GameEngineCamera& operator=(GameEngineCamera&& _other) = delete;

public:
	//주어진 월드좌표를 윈도우좌표로 바꾸는 함수.
	float4 ConvertWorldPositionToScreenPosition(const float4& _worldPosition);

	//주어진 월드좌표를 뷰공간좌표로 바꾸는 함수.
	float4 ConvertWorldPositionToViewPosition(const float4& _worldPosition);

	//마우스 포인터의 위치를 윈도우 스크린 좌표로 받아오는 함수.
	float4 GetMousePositionInScreen();

	//마우스 포인터의 위치를 뷰공간 기준으로 받아오는 함수.
	float4 GetMousePositionInViewSpace();

	//마우스 포인터의 위치를 월드공간 기준으로 받아오는 함수.
	float4 GetMousePositionInWorldSpace();

	//이 카메라에서 마우스 포인터방향으로 뻗는 레이의 월드공간 방향벡터를 받아오는 함수. 
	float4 GetRayTowardMousePointer();


	//카메라의 순서를 변경하는 함수.
	void SetCameraOrder(CameraOrder _order);
	//할 일이 있을지는 모르겠지만, 런타임 중간에 카메라 오더 변경 금지.

	//인스턴싱렌더러 반환. 없다면 빈 인스턴싱렌더러를 만들어서 반환한다.
	GameEngineInstanceRenderer& GetInstanceRenderer(const std::string_view& _name);

	//카메라에 조명 추가.
	void PushLighting(GameEngineLighting* _newLighting);

public:
	void SetProjectionMode(ProjectionMode _mode)
	{
		projectionMode_ = _mode;
	}

	inline const float4& GetMouseDirection()
	{
		return mouseDirection_;
	}

	ProjectionMode GetProjectionMode()
	{
		return projectionMode_;
	}

	inline void SetProjectionSize(const float4& _size)
	{
		size_ = _size;
	}
	inline const float4& GetProjectionSize() const
	{
		return size_;
	}

	inline const float4x4& GetViewMatrix() const
	{
		return viewMatrix_;
	}

	inline const float4x4& GetProjectionMatrix() const
	{
		return projectionMatrix_;
	}

	//이 카메라의 최종 렌더타겟을 가져오는 함수.
	GameEngineRenderTarget* GetConclusionRenderTarget()
	{
		return conclusionRenderTarget_;
	}

	//이 카메라의 포워드렌더링용 렌더타겟을 가져오는 함수.
	GameEngineRenderTarget* GetForwardRenderTarget()
	{
		return forwardRenderTarget_;
	}

	//이 카메라의 디퍼드렌더링용 렌더타겟을 가져오는 함수.
	GameEngineRenderTarget* GetDeferredRenderTarget()
	{
		return deferredRenderTarget_;
	}

	//이 카메라의 GBuffer 렌더타겟을 가져오는 함수.
	GameEngineRenderTarget* GetGBufferRenderTarget()
	{
		return geometryBufferRenderTarget_;
	}

	//이 카메라가 가진 모든 조명데이터 반환.
	AllLightingDatas& GetLightingDatas()
	{
		return lightingDatasInst_;
	}

	void SetFarZ(float _farZ)
	{
		this->farZ_ = _farZ;
	}


protected:
	void Start();

private:
	//이 카메라가 가진 렌더러들에게 이 카메라가 가진 렌더타겟에 렌더링 정보를 적용시키게 하는 함수.
	void Render(float _deltaTime);

	//주어진 렌더러를 이 카메라에 등록하는 함수.
	void PushRenderer(GameEngineRenderer* _renderer);

	//렌더러를 등록 해제하는 함수. 직접 삭제하는 함수가 아님에 주의할 것.
	void Release(float _deltaTime);

	void Update(float _dletaTime) override;

	//이 카메라의 렌더러들을 다른 레벨의 카메라로 옮기는 함수.
	void OverRenderer(GameEngineCamera* _nextCamera);

	//렌더링 순서 변경.
	void ChangeRenderingOrder(GameEngineRenderer* _renderer, int _newRenderingOrder);

	//마우스 포인터 이동방향 업데이트 함수.
	void UpdateMouseDirection();

private:
	std::map<int, std::list<GameEngineRenderer*>> allRenderers_;	//이 카메라가 가진 모든 렌더러들.
	//렌더링오더로 먼저 분류한 후 렌더 직전에 zsort() 함수로 z값 순서대로 정렬한다. 

	std::map<std::string, GameEngineInstanceRenderer> allInstancingRenderers_;

	//비정렬 맵(Unordered Map): 들어오는 키값을 해시함수를 거쳐서 나온 숫자로 바꿔서, 
	// 그 값을 인덱스로 하는 배열 내 원소로 데이터를 저장하는 방식의 컨테이너. 
	//장점으로는 검색 속도가 '대체로' 다른 컨테이너들보다 빠르다고 한다(항상 그렇진 않음).
	//단점으로는 이름대로 키값 정렬이 되지 않고, 순회 효율이 떨어지며, 해시 충돌이 많이 발생할 수록 검색 효율이 떨어져서,
	// 최악의 경우 선형 시간복잡도를 보이게 된다고 한다.

	//해시 충돌: 다른 입력값을 해시 함수에 넣었는데 같은 결과값이 나와서 두 입력값을 구분할 수 없게 되는 현상.
	// 해시 충돌 확률이 낮을 수록 더 좋은 해시 함수라고 한다.


	float4x4 viewMatrix_;
	//뷰행렬: 월드공간의 원점 기준으로 배치된 여러 정점들을, 카메라 위치가 원점이 된 뷰공간 안에 어떻게 재배치해서, 
	// 월드공간에 배치되었던 그대로 화면 안에 보이게 할 지 계산하는데 필요한 행렬. 
	//월드 전체의 오브젝트를 재배치하는 특성상, 뷰행렬은 월드 전체에 적용된다.
	//카메라가 뷰행렬만큼 이동 회전했다고 생각하지만 실제로는 카메라는 가만히 있고,
	// 오브젝트들이 뷰행렬의 반대로 같은 값만큼 움직인 것이다.
	// 그래서 실제 뷰행렬로 사용되는건 구한 행렬의 역행렬이다.

	float4x4 projectionMatrix_;
	//투영행렬: 뷰행렬로 재배치된 뷰공간 안의 오브젝트들을 2차원으로 변환하는 과정을 투영변환이라고 하는데, 
	// 이 과정에 필요한 행렬. 크게 직교투영과 원근투영 두가지 투영방식으로 나눈다. 
	//투영행렬을 거치고 난 모든 카메라의 시야범위(보통 절두체(frustum) 형태를 가지기 때문에 뷰 절두체, 뷰 프러스텀이라고 한다)
	// 안에 들어온 정점들의 좌표는 -1 <= x <= 1, -1 <= y <= 1, 0 <= z <=1 범위로 들어오게 된다. 
	// 이러한 좌표계를 NDC(Normalized Device Coodinate)좌표계라고 한다.
	//뷰 프러스텀 밖에 있어서 저 값을 벗어난 정점들은 투영 대상에서 제외되어 화면에 그려지지 않는다.

	//직교투영: 근평면과의 거리 상관없이 오브젝트 자체의 크기만 반영해서 -1~1 범위로 축소하는 투영. 소실점이 없다.
	// 정점좌표 벡터의 w값은 항상 1로 고정된다. 그래서 신경 쓸 필요 없다.

	//원근투영: 근평면에서 떨어진 거리에 비례해서 가까운 오브젝트는 크게, 멀리 있는 오브젝트는 작게 조정해서
	// -1~1 범위로 축소하는 투영방식. 소실점이 있다.
	//정점좌표 벡터의 w값은 뷰공간 내 z값에 따라서 0~1사이의 값을 가진다. 
	//투영행렬까지 곱한 정점좌표 벡터를 이 w값으로 나누어 주어야 비로소 투영행렬 적용이 끝난다.

	//원근 투영에 탄젠트를 쓰는 이유: 
	// 특정 위치의 정점을 투영변환 후 뷰포트 변환하는 과정을 거칠 때, 
	// 정점 좌표의 x값 / z값, y값 / z값의 비율을 만드는 탄젠트 각도를 저장해 두었다면, 
	// z값 * 아크 탄젠트(각도) 공식을 통해 새로운 z값이 얼마인지에 상관없이,
	// 뷰 스페이스내 평면에 투영변환 이전의 원래 상대좌표를 유지한 상태의 정점을 화면에 그릴 수 있다.
	//그 탄젠트 각도는 정점 좌표 벡터의 w에 저장된다.



	ProjectionMode projectionMode_;	//직교투영/원근투영 설정. 

	float4 size_;	//투영행렬을 계산할 기준 평면 == 내가 사용중인 윈도우의 크기. 근평면이라는 보장은 없음.
	float nearZ_;		//원근 중심에서 근평면까지의 거리. 
	float farZ_;		//원근 중심에서 원평면까지의 거리.
	float fovAngleY_;	//FoV(Field of View): 화각, 시야 범위. Y이므로 세로길이를 형성하는 각도.

	float4x4 viewportMatrix_;
	//뷰포트행렬: 투영변환을 통해 축소시킨 뷰 공간의 오브젝트들을 원하는 크기만큼 다시 확대시키는 뷰포트 변환을 할 때 필요한 행렬.

	//typedef struct D3D11_VIEWPORT
	//{
	//	FLOAT TopLeftX;		뷰포트 변환할 윈도우의 좌상단 x좌표.
	//	FLOAT TopLeftY;		뷰포트 변환할 윈도우의 좌상단 y좌표.
	//	FLOAT Width;		뷰포트 변환할 윈도우의 가로길이.
	//	FLOAT Height;		뷰포트 변환할 윈도우의 세로길이.
	//	FLOAT MinDepth;		뷰포트 최소깊이(0~1). 웬만하면 0으로 하는것이 좋다.
	//	FLOAT MaxDepth;		뷰포트 최대깊이(0~1). 웬만하면 1로 하는것이 좋다.
	//} 	D3D11_VIEWPORT;

	D3D11_VIEWPORT viewportDesc_;
	//래스터라이저 단계에서 뷰포트 변환할 때 필요한 윈도우 위치, 크기 정보를 저장하는 D3D11_VIEWPORT구조체 객체.
	//렌더링 파이프라인상 순서는 래스터라이저이지만 뷰포트 변환할 때 필요한 윈도우 크기 정보를 전부 카메라가 가지고 있고,
	// 변경할 일도 거의 없으므로 래스터라이저 대신 카메라가 가진다.


	float4 prevMousePosition_;	//이전 마우스포인터 위치.
	float4 mouseDirection_;		//마우스포인터가 이동한 방향.

	//최종 결과물을 받는 렌더타겟.
	GameEngineRenderTarget* conclusionRenderTarget_;

	//포워드 렌더링용 렌더타겟.
	GameEngineRenderTarget* forwardRenderTarget_;

	//디퍼드 렌더링용 렌더타겟.
	GameEngineRenderTarget* deferredRenderTarget_;

	//지오메트리 버퍼 렌더타겟(이하 g버퍼).
	GameEngineRenderTarget* geometryBufferRenderTarget_;
	//오브젝트의 깊이값도 여기에 저장한다.

	//그림자 깊이값 저장용 렌더타겟.
	GameEngineRenderTarget* shadowDepthRenderTarget_;

	//빛 적용 배율값 저장용 렌더타겟.
	GameEngineRenderTarget* lightRatioBufferRenderTarget_;

	//빛정보 저장용 렌더타겟.
	GameEngineRenderTarget* lightDataBufferRenderTarget_;


	class GameEngineRenderUnit* lightRatioRenderUnit_;	//빛 배율 저장용 렌더유닛.

	class GameEngineRenderUnit* lightDataRenderUnit_;	//빛 정보 계산용 렌더유닛.

	class GameEngineRenderUnit* mergeRenderUnit_;	//최종 통합 렌더유닛.





	std::set<GameEngineLighting*> allLightings_;	//모든 조명 정보.

	AllLightingDatas lightingDatasInst_;	//이 카메라의 모든 조명 데이터들. 
};

