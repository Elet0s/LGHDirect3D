#include "PreCompile.h"
#include "GameEngineRenderer.h"
#include "GameEngineActor.h"
#include "GameEngineInstancing.h"
#include "GameEngineInputLayout.h"
#include "GameEngineDevice.h"

GameEngineRenderUnit::GameEngineRenderUnit()
	: parentRenderer_(nullptr),
	mesh_(nullptr),
	inputLayout_(nullptr),
	renderingPipeLine_(nullptr),
	topology_(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
	SetMesh("Rect");
}

GameEngineRenderUnit::~GameEngineRenderUnit()
{
}

void GameEngineRenderUnit::SetPipeLine(const std::string& _renderingPipeLineName)
{
	renderingPipeLine_ = GameEngineMaterial::Find(_renderingPipeLineName);

	if (nullptr == renderingPipeLine_)
	{
		MsgBoxAssertString(_renderingPipeLineName + ": �׷� �̸��� ������ ������������ �������� �ʽ��ϴ�.");
		return;
	}

	if (nullptr == inputLayout_ && nullptr != mesh_)
	{
		inputLayout_ = GameEngineInputLayout::Create(
			this->mesh_->GetInputLayoutDesc(),
			this->renderingPipeLine_->GetVertexShader()
		);
		//�޽��� ���ؽ����ۿ� ������ ������������ ���ؽ����̴��� ��� �غ�Ǹ� ��ǲ ���̾ƿ��� �����Ѵ�.
		//��� ���� �غ�� �� �𸣹Ƿ� �޽��� ���� �غ�Ǵ� ���� ������ ������������ ���� �غ�Ǵ� ��� 
		// �ΰ��� ��� ����Ѵ�.
	}

	shaderResourceHelper_.ResourceCheck(this->renderingPipeLine_);
}

void GameEngineRenderUnit::SetMesh(const std::string& _meshName)
{
	mesh_ = GameEngineMesh::Find(_meshName);

	if (nullptr == mesh_)
	{
		MsgBoxAssertString(_meshName + ": �׷� �̸��� �޽��� �������� �ʽ��ϴ�.");
		return;
	}

	if (nullptr == inputLayout_ && nullptr != renderingPipeLine_)
	{
		inputLayout_ = GameEngineInputLayout::Create(
			this->mesh_->GetInputLayoutDesc(),
			this->renderingPipeLine_->GetVertexShader()
		);
		//�޽��� ���ؽ����ۿ� ������ ������������ ���ؽ����̴��� ��� �غ�Ǹ� ��ǲ ���̾ƿ��� �����Ѵ�.
		//��� ���� �غ�� �� �𸣹Ƿ� �޽��� ���� �غ�Ǵ� ���� ������ ������������ ���� �غ�Ǵ� ��� 
		// �ΰ��� ��� ����Ѵ�.
	}
}

void GameEngineRenderUnit::EngineShaderResourceSetting(GameEngineRenderer* _parentRenderer)
{
	if (nullptr == _parentRenderer)
	{
		return;
	}
	parentRenderer_ = _parentRenderer;

	if (true == this->shaderResourceHelper_.IsConstantBuffer("TRANSFORMDATA"))
	{
		this->shaderResourceHelper_.SetConstantBuffer_Link(
			"TRANSFORMDATA",
			&this->parentRenderer_->GetTransformData(),
			sizeof(this->parentRenderer_->GetTransformData())
		);
	}
	if (true == this->shaderResourceHelper_.IsConstantBuffer("RENDEROPTION"))
	{
		this->shaderResourceHelper_.SetConstantBuffer_Link(
			"RENDEROPTION",
			&this->parentRenderer_->renderOption_,
			sizeof(RenderOption)
		);
	}
}

void GameEngineRenderUnit::Render(float _deltaTime)
{
	if (nullptr == this->renderingPipeLine_)
	{
		MsgBoxAssert("������ ������������ �����ϴ�. �������� �� �� �����ϴ�.");
		return;
	}	
	
	if (nullptr == this->mesh_)
	{
		MsgBoxAssert("�޽��� �����ϴ�. �������� �� �� �����ϴ�.");
		return;
	}	
	
	if (nullptr == this->inputLayout_)
	{
		MsgBoxAssert("��ǲ ���̾ƿ��� �����ϴ�. �������� �� �� �����ϴ�.");
		return;
	}

	mesh_->Setting();

	inputLayout_->Setting();

	GameEngineDevice::GetContext()->IASetPrimitiveTopology(topology_);

	renderingPipeLine_->Setting();

	shaderResourceHelper_.AllResourcesSetting();

	mesh_->Render();

	shaderResourceHelper_.AllResourcesReset();
}

GameEngineMaterial* GameEngineRenderUnit::GetPipeLine()
{
	return this->renderingPipeLine_;
}

GameEngineMaterial* GameEngineRenderUnit::GetClonePipeLine()
{
	if (false == renderingPipeLine_->IsOriginal())
	{
		return renderingPipeLine_;
	}

	renderingPipeLine_ = ClonePipeLine(renderingPipeLine_);
	return renderingPipeLine_;
}

GameEngineMaterial* GameEngineRenderUnit::ClonePipeLine(GameEngineMaterial* _original)
{
	GameEngineMaterial* clone = GameEngineMaterial::Create();
	clone->Copy(_original);
	return clone;
}

void GameEngineRenderUnit::SetRenderer(GameEngineRenderer* _parentRenderer)
{
	parentRenderer_ = _parentRenderer;
	if (nullptr == parentRenderer_)
	{
		MsgBoxAssert("�������� �ʴ� �������Դϴ�.");
		return;
	}

	EngineShaderResourceSetting(_parentRenderer);
}

GameEngineRenderer::GameEngineRenderer()
	: camera_(nullptr),
	renderOption_(),
	cameraOrder_(CameraOrder::MainCamera),
	renderingOrder_(0),
	isInstancing_(false)
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

bool GameEngineRenderer::IsInstancing(GameEngineMaterial* _pipeLine)
{
	std::unordered_map<GameEngineMaterial*, GameEngineInstancing>::iterator instancingIter
		= this->camera_->instancingMap_.find(_pipeLine);

	if (this->camera_->instancingMap_.end() == instancingIter)
	{
		return false;
	}

	return true == isInstancing_ && GameEngineInstancing::minInstancingCount_ <= instancingIter->second.count_;
}

void GameEngineRenderer::InstancingDataSetting(GameEngineMaterial* _pipeLine)
{
	int instancingIndex = this->camera_->PushInstancingIndex(_pipeLine);

	GameEngineInstancing* instancing = camera_->GetInstancing(_pipeLine);

	if (nullptr == instancing)
	{
		MsgBoxAssert("�ν��Ͻ��� ���� ������ �ν��Ͻ� �غ�� �Ǿ����� �ʽ��ϴ�.");
		return;
	}

	if (true == instancing->shaderResourceHelper_.IsStructuredBuffer("allInstancingTransformDatas"))
	{
		GameEngineStructuredBufferSetter* sBufferSetter 
			= instancing->shaderResourceHelper_.GetStructuredBufferSetter("allInstancingTransformDatas");

		sBufferSetter->Push(this->GetTransformData(), instancingIndex);
	}
}

void GameEngineRenderer::Start()
{
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
//	//�ӽ� ������ �ڵ�.
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
//		//tempIndex�� ����� �ε����� triIndex��� �̸����� �ű� �� 
//		//triIndex�� �����Ǵ� ���ؽ� ������ ������� copiedVertex�� �ű��.
//
//		copiedVertex[i] *= this->GetTransform().GetWorldViewProjection();	
//		//���� world��Ŀ� �����, ������ı��� ����� ���� copiedVertex[i]�� �ݿ��Ѵ�.
//
//		copiedVertex[i] /=  copiedVertex[i].w;
//		//��������� ������� ����� w��ġ��ŭ copiedVertex�� ��ǥ���� ������ ��ҽ�Ų��.
//		
//		copiedVertex[i] *= tempViewport_;
//		//�ӽú���Ʈ ����� �����ؼ� ���ϴ� ũ���� �ٽ� Ȯ��� ��ǥ�� ������ �Է��Ѵ�.
//	
//		drawVertex[i] = copiedVertex[i].ConvertToWindowsPoint();
//		//�Էµ� ��ǥ��� POINT ����ü�� ���� ����.
//	}
//
//	for (size_t i = 0; i < drawVertex.size(); i += 3)
//	{
//		Polygon(GameEngineWindow::GetInst().GetHDC(), &drawVertex[i], 3);
//	}
//}
