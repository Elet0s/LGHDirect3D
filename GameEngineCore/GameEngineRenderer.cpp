#include "PreCompile.h"
#include "GameEngineRenderer.h"
#include "GameEngineActor.h"
#include "GameEngineInputLayout.h"
#include "GameEngineDevice.h"
#include "GameEngineCamera.h"

GameEngineRenderUnit::GameEngineRenderUnit()
	: parentRenderer_(),
	mesh_(nullptr),
	inputLayout_(nullptr),
	material_(nullptr),
	topology_(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
	specialRenderingFunction_(nullptr)
{
	SetMesh("Rect");
}

GameEngineRenderUnit::~GameEngineRenderUnit()
{
}

GameEngineRenderUnit::GameEngineRenderUnit(const GameEngineRenderUnit& _other)
{
	this->parentRenderer_ = _other.parentRenderer_;
	this->mesh_ = _other.mesh_;
	this->inputLayout_ = _other.inputLayout_;
	this->material_ = _other.material_;
	this->topology_ = _other.topology_;

	this->shaderResourceHelper_.ResourceCheck(this->material_);
}

void GameEngineRenderUnit::SetMesh(const std::string_view& _meshName)
{
	this->mesh_ = GameEngineMesh::Find(_meshName);

	if (nullptr == mesh_)
	{
		MsgBoxAssertString(std::string(_meshName) + ": 그런 이름의 메쉬가 존재하지 않습니다.");
		return;
	}

	if (nullptr == inputLayout_ && nullptr != material_)
	{
		inputLayout_ = GameEngineInputLayout::Create(
			this->mesh_->GetInputLayoutDesc(),
			this->material_->GetVertexShader()
		);
		//메쉬의 버텍스버퍼와 렌더링 파이프라인의 버텍스셰이더가 모두 준비되면 인풋 레이아웃을 생성한다.
		//어떤게 먼저 준비될 지 모르므로 메쉬가 먼저 준비되는 경우와 렌더링 파이프라인이 먼저 준비되는 경우 
		// 두가지 모두 대비한다.
	}
}

void GameEngineRenderUnit::SetMesh(GameEngineMesh* _mesh)
{
	if (nullptr == _mesh)
	{
		MsgBoxAssert("메쉬가 존재하지 않습니다.");
		return;
	}

	this->mesh_ = _mesh;

	if (nullptr == inputLayout_ && nullptr != material_)
	{
		inputLayout_ = GameEngineInputLayout::Create(
			this->mesh_->GetInputLayoutDesc(),
			this->material_->GetVertexShader()
		);
		//메쉬의 버텍스버퍼와 렌더링 파이프라인의 버텍스셰이더가 모두 준비되면 인풋 레이아웃을 생성한다.
		//어떤게 먼저 준비될 지 모르므로 메쉬가 먼저 준비되는 경우와 렌더링 파이프라인이 먼저 준비되는 경우 
		// 두가지 모두 대비한다.
	}
}

void GameEngineRenderUnit::SetMaterial(const std::string_view& _materialName)
{
	material_ = GameEngineMaterial::Find(_materialName);

	if (nullptr == material_)
	{
		MsgBoxAssertString(std::string(_materialName) + ": 그런 이름의 마테리얼이 존재하지 않습니다.");
		return;
	}

	if (nullptr == inputLayout_ && nullptr != mesh_)
	{
		inputLayout_ = GameEngineInputLayout::Create(
			this->mesh_->GetInputLayoutDesc(),
			this->material_->GetVertexShader()
		);
		//메쉬의 버텍스버퍼와 렌더링 파이프라인의 버텍스셰이더가 모두 준비되면 인풋 레이아웃을 생성한다.
		//어떤게 먼저 준비될 지 모르므로 메쉬가 먼저 준비되는 경우와 렌더링 파이프라인이 먼저 준비되는 경우 
		// 두가지 모두 대비한다.
	}

	shaderResourceHelper_.ResourceCheck(this->material_);
}

void GameEngineRenderUnit::EngineShaderResourceSetting(GameEngineRenderer* _parentRenderer)
{
	if (nullptr == _parentRenderer)
	{
		return;
	}
	parentRenderer_ = _parentRenderer;

	if (true == this->shaderResourceHelper_.IsConstantBuffer("ALLLIGHTINGDATAS"))
	{
		shaderResourceHelper_.SetConstantBuffer_Link(
			"ALLLIGHTINGDATAS",
			&this->parentRenderer_->GetCamera()->GetLightingDatas(),
			sizeof(AllLightingDatas)
		);
	}
	if (true == this->shaderResourceHelper_.IsConstantBuffer("TRANSFORMDATA"))
	{
		shaderResourceHelper_.SetConstantBuffer_Link(
			"TRANSFORMDATA",
			&this->parentRenderer_->GetTransformData(),
			sizeof(TransformData)
		);
	}
	if (true == this->shaderResourceHelper_.IsConstantBuffer("RENDEROPTION"))
	{
		this->shaderResourceHelper_.SetConstantBuffer_Link(
			"RENDEROPTION",
			&this->parentRenderer_->renderOptionInst_,
			sizeof(RenderOption)
		);
	}
}

void GameEngineRenderUnit::Render(float _deltaTime)
{

	if (nullptr == this->mesh_)
	{
		MsgBoxAssert("메쉬가 없습니다. 렌더링을 할 수 없습니다.");
		return;
	}

	if (nullptr == this->inputLayout_)
	{
		MsgBoxAssert("인풋 레이아웃이 없습니다. 렌더링을 할 수 없습니다.");
		return;
	}

	if (nullptr == this->material_)
	{
		MsgBoxAssert("마테리얼이 없습니다. 렌더링을 할 수 없습니다.");
		return;
	}


	mesh_->Set();

	inputLayout_->Set();

	GameEngineDevice::GetDC()->IASetPrimitiveTopology(topology_);

	material_->Set();

	shaderResourceHelper_.SetAllResources();

	mesh_->Render();

	shaderResourceHelper_.ResetAllResources();
}

GameEngineMesh* GameEngineRenderUnit::GetMesh()
{
	return this->mesh_;
}

GameEngineMaterial* GameEngineRenderUnit::GetMaterial()
{
	return this->material_;
}

void GameEngineRenderUnit::SetRenderer(GameEngineRenderer* _parentRenderer)
{
	parentRenderer_ = _parentRenderer;
	if (nullptr == parentRenderer_)
	{
		MsgBoxAssert("존재하지 않는 렌더러입니다.");
		return;
	}

	EngineShaderResourceSetting(_parentRenderer);
}

GameEngineRenderer::GameEngineRenderer()
	: camera_(),
	renderOptionInst_(),
	cameraOrder_(CameraOrder::MainCamera),
	renderingOrder_(0)
{
}

GameEngineRenderer::~GameEngineRenderer()
{
}

void GameEngineRenderer::ChangeCamera(CameraOrder _order)
{
	this->GetActor()->GetLevel()->PushRenderer(this, _order);
}

void GameEngineRenderer::SetRenderingOrder(int _renderingOrder)
{
	camera_->ChangeRenderingOrder(this, _renderingOrder);
}

void GameEngineRenderer::Start()
{
	GameEngineRenderer::PushRendererToMainCamera();
}

void GameEngineRenderer::PushRendererToMainCamera()
{
	this->GetRoot<GameEngineActor>()->GetLevel()->PushRendererToMainCamera(this);
}

void GameEngineRenderer::PushRendererToUICamera()
{
	this->GetActor()->GetLevel()->PushRendererToUICamera(this);
}

//void GameEngineRenderer::Render(float _deltaTime)
//{
//	//임시 렌더링 코드.
//	GameEngineVertexBuffer* renderVertex = GameEngineVertexBuffer::Find("BoxVertex");
//	GameEngineIndexBuffer* renderIndex = GameEngineIndexBuffer::Find("BoxIndex");
//
//	std::vector<POINT> drawVertex;
//	drawVertex.resize(renderIndex->indexes_.size());
//
//	std::vector<float4> copiedVertex;
//	copiedVertex.resize(renderIndex->indexes_.size());
//
//	for (size_t i = 0; i < renderIndex->indexes_.size(); i++)
//	{
//		int triIndex = renderIndex->indexes_[i];
//		copiedVertex[i] = renderVertex->vertexes_[triIndex];
//		//tempIndex에 저장된 인덱스를 triIndex라는 이름으로 옮긴 후 
//		//triIndex에 대응되는 버텍스 정보를 순서대로 copiedVertex로 옮긴다.
//
//		copiedVertex[i] *= this->GetTransform().GetWorldViewProjection();	
//		//최종 world행렬에 뷰행렬, 투영행렬까지 적용된 값을 copiedVertex[i]에 반영한다.
//
//		copiedVertex[i] /=  copiedVertex[i].w;
//		//투영행렬의 결과값이 저장된 w수치만큼 copiedVertex의 좌표값을 나누어 축소시킨다.
//		
//		copiedVertex[i] *= tempViewport_;
//		//임시뷰포트 행렬을 적용해서 원하는 크기대로 다시 확대된 좌표를 정점에 입력한다.
//	
//		drawVertex[i] = copiedVertex[i].ConvertToWindowsPoint();
//		//입력된 좌표대로 POINT 구조체를 만들어서 전달.
//	}
//
//	for (size_t i = 0; i < drawVertex.size(); i += 3)
//	{
//		Polygon(GameEngineWindow::GetInst().GetHDC(), &drawVertex[i], 3);
//	}
//}

