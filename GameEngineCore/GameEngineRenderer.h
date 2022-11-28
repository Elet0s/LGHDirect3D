#pragma once
#include "GameEngineTransformComponent.h"
#include "GameEngineCamera.h"
#include "GameEngineMaterial.h"
#include "GameEngineShaderResourceHelper.h"
#include "GameEngineMesh.h"

struct RenderOption
{
    float deltaTime_ = 0.f;
    float sumDeltaTime_ = 0.f;
    int isAnimation_ = 0;
    int vertexInversion_ = 1;   //그림자용 정점 좌우 뒤집힘 표시 변수. 좌우 뒤집어야 하면 -1 대입.
    float pivotPosX_ = 0.f;     //피봇포스 X
    float pivotPosY_ = 0.f;     //피봇포스 Y
    float shadowAngle_ = 30.f;  //그림자 각도. 기본값 30도.
    int bytePad1_ = 0;          //바이트패드.
};

class GameEngineRenderUnit : public std::enable_shared_from_this<GameEngineRenderUnit>
{
    //이 프레임워크에서 렌더링에 필수적인 두가지 요소인 렌더링 파이프라인과 셰이더리소스헬퍼를 
    // 한데 모아 렌더러를 통해 관리하기 편하기 위해 만든 클래스.
    //다양한 종류의 메쉬를 세팅해서 사용하기 좀 더 편해진 체제.

    friend class GameEngineRenderer;
    //셰이더리소스헬퍼 가져다 써야해서 프렌드.

public:
    GameEngineRenderUnit();
    ~GameEngineRenderUnit();

    GameEngineRenderUnit(const GameEngineRenderUnit& _other);
    //GameEngineRenderUnit(GameEngineRenderUnit&& _other) noexcept = delete;

private:
    GameEngineRenderUnit& operator=(const GameEngineRenderUnit& _other) = delete;
    GameEngineRenderUnit& operator=(GameEngineRenderUnit&& _other) = delete;

public:
    //렌더유닛에 메쉬를 지정하는 함수. 
    void SetMesh(const std::string_view& _meshName);
    void SetMesh(std::shared_ptr<GameEngineMesh> _mesh);

    //렌더유닛에 마테리얼을 지정하는 함수.
    void SetMaterial(const std::string_view& _materialName);

    //새 부모 렌더러를 지정하고 렌더유닛이 가진 셰이더리소스헬퍼에
    // 엔진 기본제공 상수버퍼인 "TRANSFORMDATA"와 "RENDEROPTION"을 등록하는 함수.
    void EngineShaderResourceSetting(std::shared_ptr<GameEngineRenderer> _parentRenderer);

    //렌더유닛의 셰이더리소스헬퍼에 저장된 셰이더리소스들을 렌더링 파이프라인을 통해 
    // 부모 렌더러가 등록된 카메라의 렌더타겟에 옮기는 함수.
    void Render(float _deltaTime);

    void RenderInstancing(
        float _deltaTime,
        size_t _instancingDataCount,
        std::shared_ptr<class GameEngineInstancingBuffer> _instancingBuffer
    );

    std::shared_ptr<GameEngineMesh> GetMesh();
    std::shared_ptr<GameEngineMaterial> GetMaterial();

    std::shared_ptr<GameEngineMaterial> GetCloneMaterial();

    std::shared_ptr<GameEngineMaterial> CloneMaterial(std::shared_ptr<GameEngineMaterial> _original);

    //렌더유닛에 부모 렌더러를 지정하고 EngineShaderResourceSetting() 함수를 호출해서 엔진 기본 상수버퍼를 등록하는 함수.
    void SetRenderer(std::shared_ptr<GameEngineRenderer> _parentRenderer);


public:
    inline GameEngineShaderResourceHelper& GetShaderResourceHelper()
    {
        return this->shaderResourceHelper_;
    }

    //이 렌더유닛을 일반 렌더링 모드로 전환하는 함수.
    inline void On()
    {
        isOn_ = true;
    }

    //이 렌더유닛을 인스턴스 렌더링 모드로 전환하는 함수.
    inline void Off()
    {
        isOn_ = false;
    }


private:
    bool isOn_;     //true: 일반 렌더링 모드, false: 인스턴스 렌더링 모드.

    std::weak_ptr<GameEngineRenderer> parentRenderer_;    //이 렌더유닛을 가진 부모 렌더러.

    std::shared_ptr<GameEngineMesh> mesh_;                  //

    std::shared_ptr<GameEngineInputLayout> inputLayout_;    //

    std::shared_ptr<GameEngineMaterial> material_;    //셰이더리소스들을 렌더타겟에 그릴 마테리얼.

    D3D11_PRIMITIVE_TOPOLOGY topology_;     //

    GameEngineShaderResourceHelper shaderResourceHelper_;   //셰이더리소스들을 가진 셰이더리소스 헬퍼.
    //값형인 이유: 렌더유닛마다 각각의 셰이더리소스헬퍼를 가지게 하기 위해서.
};

class GameEngineShaderResourceHelper;
class GameEngineMaterial;
class GameEngineRenderer : public GameEngineTransformComponent
{
    //모든 렌더러가 공통적으로 가져야 하는 기능만을 최소한도로 가진 인터페이스 클래스.
    //실질적인 역할은 트랜스폼 정보를 가지고 게임엔진카메라에 등록되어 렌더링 대상이 되고, 
    //자식 렌더러들의 형식을 규정한다.

    friend GameEngineLevel;
    friend class GameEngineCamera;

public:

    GameEngineRenderer();
    ~GameEngineRenderer();

    GameEngineRenderer(const GameEngineRenderer& _other) = delete;
    GameEngineRenderer(GameEngineRenderer&& _other) noexcept = delete;
    GameEngineRenderer& operator=(const GameEngineRenderer& _other) = delete;
    GameEngineRenderer& operator=(GameEngineRenderer&& _other) = delete;

public:
    void ChangeCamera(CameraOrder _order);
    void SetRenderingOrder(int _renderingOrder);

public:
    inline int GetRenderingOrder()
    {
        return renderingOrder_;
    }

    virtual void InstancingOn()
    {
        isInstancing_ = true;
    }

public:
    RenderOption renderOptionInst_;


protected:
    virtual void Start();
    virtual void Render(float _deltaTime) = 0;

    void PushRendererToMainCamera();	//렌더러가 메인카메라에 등록하는 함수.
    void PushRendererToUICamera();		//렌더러가 UI카메라에 등록하는 함수.

protected:
    std::weak_ptr<class GameEngineCamera> camera_;    //렌더러가 등록된 카메라.

private:
    CameraOrder cameraOrder_;
    int renderingOrder_;
    bool isInstancing_;
};

