#include "PreCompile.h"
#include "GameEngineCore.h"
#include "GameEngineDevice.h"
#include "GameEngineTexture.h"
#include "GameEngineFolderTexture.h"
#include "GameEngineSampler.h"
#include "GameEngineRenderTarget.h"
#include "GameEngineVertexes.h"
#include "GameEngineConstantBuffer.h"
#include "GameEngineStructuredBuffer.h"
#include "GameEngineInstancingBuffer.h"

#include "GameEngineMesh.h"
#include "GameEngineMaterial.h"
#include "GameEngineVertexBuffer.h"
#include "GameEngineInputLayout.h"
#include "GameEngineVertexShader.h"
#include "GameEngineIndexBuffer.h"
#include "GameEngineRasterizer.h"
#include "GameEnginePixelShader.h"
#include "GameEngineDepthStencil.h"
#include "GameEngineBlend.h"
#include "GameEngineFont.h"

void EngineInputLayout()
{
	GameEngineVertex::inputLayoutInfo_.AddInputLayout(
		"POSITION",
		DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT
	);
	GameEngineVertex::inputLayoutInfo_.AddInputLayout(
		"COLOR",
		DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT
	);
	GameEngineVertex::inputLayoutInfo_.AddInputLayout(
		"TEXCOORD",
		DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT
	);

	GameEngineVertex::inputLayoutInfo_.ResetOffset();

	//�ν��Ͻ� �����Ϳ�.
	GameEngineVertex::inputLayoutInfo_.AddInputLayout(
		"ROWINDEX",
		DXGI_FORMAT::DXGI_FORMAT_R32_UINT,
		-1,
		1,
		1,
		D3D11_INPUT_PER_INSTANCE_DATA
	);
}

void EngineSubSetting()
{
	D3D11_RASTERIZER_DESC rasterizerDesc = {};

	rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;	//�ٲٸ� �¿������ ������ �ȵ�.

	GameEngineRasterizer::Create("EngineRasterizer", rasterizerDesc);


	//typedef struct D3D11_BLEND_DESC
	//{
	//	BOOL AlphaToCoverageEnable;							���ĺ��������� ��Ƽ���ø��� �ϴ� ����� ���� �� Ŀ�������� ������ �� ����.
	//	BOOL IndependentBlendEnable;						����Ÿ�ٵ��� ������ �������� ������ �������� ������ �� �� ����. 
	//														 true: 8���� ������ �������� ����.
	//														 false: 0�� �������� �������� �����.
	// 
	//	D3D11_RENDER_TARGET_BLEND_DESC RenderTarget[8];		����Ÿ�ٿ� ����� ���ĺ����� ������ ������ ������ D3D11_RENDER_TARGET_BLEND_DESC����ü �迭.
	//														 �ִ� 8�� ����Ÿ�ٿ� ���� �ٸ� ���ĺ����� ����� ������ �� �ִ�.
	
	//	typedef struct D3D11_RENDER_TARGET_BLEND_DESC
	//	{
	//		BOOL BlendEnable;						���ĺ������� �� �� ����.
	//		D3D11_BLEND SrcBlend;					���ĺ����� ������ srcFactor�� 4�� ���Ҹ� ��� ä���� �����ϴ� �ɼ�.
	//		D3D11_BLEND DestBlend;					���ĺ����� ������ destFactor�� 4�� ���Ҹ� ��� ä���� �����ϴ� �ɼ�.
	//		D3D11_BLEND_OP BlendOp;					���ĺ����� ������ �ɼ� �κп� � ������ �������� �����ϴ� �ɼ�.
	//		D3D11_BLEND SrcBlendAlpha;				srcColor�� ���İ� ����.
	//		D3D11_BLEND DestBlendAlpha;				destColor�� ���İ� ����.
	//		D3D11_BLEND_OP BlendOpAlpha;			���ĺ����� ������ �ɼ� �׸� ���ϱ� ����. ���İ� ��꿡�� ����ȴ�.
	//		UINT8 RenderTargetWriteMask;			�������� ������ ����. RGBA�� ����� �����ؼ� ���ĺ������� ������ ���� �ִ�.
	//	} 	D3D11_RENDER_TARGET_BLEND_DESC;
	//
	//} 	D3D11_BLEND_DESC;

	D3D11_BLEND_DESC alphaBlendDesc = { 0 };

	alphaBlendDesc.AlphaToCoverageEnable = false;		//���� �� Ŀ������ ������.
	alphaBlendDesc.IndependentBlendEnable = false;		//0�� ����Ÿ���� ���ĺ����� �������� ��� ����Ÿ�ٿ� ����.
	alphaBlendDesc.RenderTarget[0].BlendEnable = true;	//���ĺ����� ��.
	alphaBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	//D3D11_COLOR_WRITE_ENABLE_ALL: RGBA ��� ���� ������ ����.

	//���ĺ����� ����.
	//output: ���� �����.
	//dest: ������� ���� ����.
	//src: ������� ����� �������� ������ ����.
	//�ɼ�: BlendOp���� �����ϴ� ������ ���� ���.
	//outputColor = (srcColor * srcFactor) �ɼ� (destColor * destFactor)

	alphaBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	//�ɼ� ������ ���ϱ�� �Ѵ�.

	alphaBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	//srcFactor�� srcColor�� ���İ��� �ϰ� ����.

	alphaBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	//destFactor�� (1 - srcColor�� ���İ�)�� �ϰ� ����.

	//�������� ���� ó��.
	alphaBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	alphaBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	alphaBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	//�� �� �ٸ� �ּ�ó���� �� ��.->�ϸ� ������ ������Ʈ ���� ����.

	GameEngineBlend::Create("AlphaBlend", alphaBlendDesc);







	D3D11_BLEND_DESC transparentBlendDesc = { 0 };

	transparentBlendDesc.AlphaToCoverageEnable = false;		//���� �� Ŀ������ ������.
	transparentBlendDesc.IndependentBlendEnable = false;		//0�� ����Ÿ���� ���ĺ����� �������� ��� ����Ÿ�ٿ� ����.
	transparentBlendDesc.RenderTarget[0].BlendEnable = true;	//���ĺ����� ��.
	transparentBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	//D3D11_COLOR_WRITE_ENABLE_ALL: RGBA ��� ���� ������ ����.

	transparentBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	//??

	transparentBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	//??

	transparentBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	//??

	//�������� ���� ó��.
	transparentBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	transparentBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	transparentBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

	GameEngineBlend::Create("TransparentBlend", transparentBlendDesc);






	//typedef struct D3D11_DEPTH_STENCIL_DESC
	//{
	//	BOOL DepthEnable;						�����׽�Ʈ Ȱ��ȭ ����.
	//	D3D11_DEPTH_WRITE_MASK DepthWriteMask;	�����׽�Ʈ ����� ���� ���� �����Ϳ� ���� �� ����??
	//	D3D11_COMPARISON_FUNC DepthFunc;		�����׽�Ʈ ���.
	//	BOOL StencilEnable;						���ٽ��׽�Ʈ�� Ȱ��ȭ ����.
	//	UINT8 StencilReadMask;					???
	//	UINT8 StencilWriteMask;					???
	//	D3D11_DEPTH_STENCILOP_DESC FrontFace;	???
	//	D3D11_DEPTH_STENCILOP_DESC BackFace;	???
	//} 	D3D11_DEPTH_STENCIL_DESC;

	D3D11_DEPTH_STENCIL_DESC engineBaseDepthStencilDesc = { 0 };

	engineBaseDepthStencilDesc.DepthEnable = true;	
	//true: �����׽�Ʈ ��.

	engineBaseDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	//D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL: ���� ���̵����Ϳ� �� ��.

	engineBaseDepthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
	//D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS: �������� ���� �������� �����Ŵ.

	engineBaseDepthStencilDesc.StencilEnable = false;
	//false: ���ٽ� �׽�Ʈ ����.

	GameEngineDepthStencil::Create("EngineBaseDepth", engineBaseDepthStencilDesc);






	D3D11_DEPTH_STENCIL_DESC alwaysDepthStencilDesc = { 0 };

	alwaysDepthStencilDesc.DepthEnable = true;
	//true: �����׽�Ʈ ��.

	alwaysDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	//D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL: ���� ���̵����Ϳ� �� ��.
	//�̰� ���η� �ٲٸ� �� �����׽�Ʈ�� ����� �� �Ǵ� �ɱ�??

	alwaysDepthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
	//D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS: �׻� �����Ŵ.

	alwaysDepthStencilDesc.StencilEnable = false;
	//false: ���ٽ� �׽�Ʈ ����.

	GameEngineDepthStencil::Create("AlwaysDepth", alwaysDepthStencilDesc);


}

void EngineTextureLoad()
{

	//typedef struct D3D11_SAMPLER_DESC
	//	D3D11_FILTER Filter;					�ؽ��ĸ� ���ø��Ҷ� ������ ����.
	//	D3D11_TEXTURE_ADDRESS_MODE AddressU;	UV��ǥ 0~1������ ���η� ��� �޽��� ���� ó�� ���.
	//	D3D11_TEXTURE_ADDRESS_MODE AddressV;	UV��ǥ 0~1������ ���η� ��� �޽��� ���� ó�� ���.
	//	D3D11_TEXTURE_ADDRESS_MODE AddressW;	??
	//	FLOAT MipLODBias;						�Ӹ� ���� ������??
	// 
	//	UINT MaxAnisotropy;						���漺 ���͸� �ִ밪. 1~16 ������ ����. 1�� ���漺 ���͸� ��� ����.
	//�ִ밪�� 16���� �ϸ� 1/16�� ��ҵ� �Ӹ� �̹������� ���� ����Ѵٴ� �ǰ�??
	//Filter�� D3D11_FILTER_ANISOTROPIC, D3D11_FILTER_COMPARISON_ANISOTROPIC�϶��� ����.
	// 
	//	D3D11_COMPARISON_FUNC ComparisonFunc;	���ø� �����͸� ���� ���ø� �����Ϳ� ���� �� ����Ǵ� �ɼ�??
	//���� �ɼ��� D3D11_FILTER_COMPARISON_~ ���� ������ ���� ����.
	// 
	//	FLOAT BorderColor[4];					rgba 0~1 ���������� ��輱 ������ ���Ѵ�.
	//AddressU, AddressV, AddressW �� D3D11_TEXTURE_ADDRESS_BORDER�� ������ ������ ����ȴ�.
	// 
	//	FLOAT MinLOD;							�Ӹ� ���� ���� ���� ����. 0�̸� ���� ũ�� ���� �Ӹ� ����.
	//	FLOAT MaxLOD;							�Ӹ� ���� ���� �ְ� ����. 0�̸� ���� ũ�� ���� �Ӹ� ����. �ݵ�� MinLOD���� ũ�ų� ���� ���̾�� �Ѵ�.
	//											
	//} 	D3D11_SAMPLER_DESC;


	//�Ӹ�: ���� �ؽ�ó�� �ػ󵵸� ���� ���� ���� ������ �̸� ����� �����صΰ� �ʿ��� �� ������ ���� ��. 
	// Ŀ�ٶ� ���� �ؽ�ó�� ����Ѵٸ� ī�޶󿡼� �ָ� �ִ� �ؽ�ó�� ��� �ؽ�ó�� ������ ������ �а� �Ǿ ���⿡�� ���� �ʰ� 
	// �޸� ĳ���� ȿ���� �ް��ϰ� ��������. ���� �ؽ�ó �ܰ迡�� �ٿ���ϸ��� ���� ��Ƽ�ٸ������ ���� �ʿ䰡 �ִµ�, 
	// �̰��� �̸� �صδ� ���� �Ӹ���, �Ӹ����� ���� �ؽ�ó ������ �Ӹ��̴�.

	D3D11_SAMPLER_DESC pointSamplerDesc = { };

	pointSamplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT;
	//D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT: 
	// ���(Minification), Ȯ��(Magnification), �� ���� ���ø��� ���� ������ ���ø����� �Ѵ�.
	//���伥 Ȯ��/����� �ִ�����(Nearest Neighbor. �ֺ� ���� ���� �ʰ� �� �� �ϳ��� ��� ��) �ɼǰ� ���� ���.
	
	//D3D11_FILTER::D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR:
	//���(Minification), Ȯ��(Magnification)�� ������ ���ø�����, �� ���� ���ø��� ���������Ѵ�.

	//D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR:
	//���(Minification), Ȯ��(Magnification), �� ���� ���ø��� ���� �������� �Ѵ�. 
	
	//D3D11_FILTER::D3D11_FILTER_ANISOTROPIC:
	// ���漺(ANISOTROPIC) ���͸�: �����¿츦 ������ ������ Ȯ�� ����� �Ӹ� �̹����� ����ϴ� ���� �ƴ϶�, 
	// ���ϳ� �¿� ������ �ٸ� ������ Ȯ�� ����� �Ӹ��̹����� ����ؼ� ī�޶� �پ��� ��ġ�� �������� �׿� �´� ������ �Ӹ��� ����ϴ� ���.
	
	//�� �ܿ��� ���� ���͸� �ɼǵ��� �ִ�. 


	//typedef
	//enum D3D11_TEXTURE_ADDRESS_MODE			UV��ǥ���� 0~1�� ��� ��ǥ�� �޽� ������ ���� ���.
	//{
	//	D3D11_TEXTURE_ADDRESS_WRAP = 1,			�ؽ��ĸ� �״�� �����ؼ� �׸���.	
	//	D3D11_TEXTURE_ADDRESS_MIRROR = 2,		�¿�/���� ����� �׸���.
	//	D3D11_TEXTURE_ADDRESS_CLAMP = 3,		�׸��� �ʴ´�.
	//	D3D11_TEXTURE_ADDRESS_BORDER = 4,		�ؽ��� �ش� ���� ��輱�� ���� �÷� �׸���
	//	D3D11_TEXTURE_ADDRESS_MIRROR_ONCE = 5	�¿�/���ϸ� ����� �ѹ��� �׸���.
	//} 	D3D11_TEXTURE_ADDRESS_MODE;

	pointSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	pointSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	pointSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;

	//D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP�� �⺻�� ����: 
	// �Ǽ� ������ UV���� 1�� �ʰ��ص� �� �׸��� �ʰ� ������ �����Ƿ� ���� ����ϰ� ���´�.


	pointSamplerDesc.MipLODBias = 0.f;
	pointSamplerDesc.MaxAnisotropy = 1;	//1: ���漺 �Ӹ� ���� ����.

	//	typedef 
	//	enum D3D11_COMPARISON_FUNC
	//	{
	//		D3D11_COMPARISON_NEVER = 1,			�ƹ��͵� �����Ű�� ����.
	//		D3D11_COMPARISON_LESS = 2,			���� ���� �͸� ���.
	//		D3D11_COMPARISON_EQUAL = 3,			���� ���� �͸� ���.
	//		D3D11_COMPARISON_LESS_EQUAL = 4,	���� �۰ų� ���� �͸� ���.
	//		D3D11_COMPARISON_GREATER = 5,		���� ū �͸� ���.
	//		D3D11_COMPARISON_NOT_EQUAL = 6,		���� ���� ���� �͸� ���.
	//		D3D11_COMPARISON_GREATER_EQUAL = 7,	���� ũ�ų� ���� �͸� ���.
	//		D3D11_COMPARISON_ALWAYS = 8			������ ���.
	//	} 	D3D11_COMPARISON_FUNC;	

	pointSamplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
	//���ʿ� ���� �ɼ��� ������ �ʴ� �ɼ����� �߱� ������ �� ��� ������ ������� �ʴ´�.

	pointSamplerDesc.MinLOD = -FLT_MAX;
	pointSamplerDesc.MaxLOD = FLT_MAX;

	GameEngineSampler::Create("EngineSampler_Point", pointSamplerDesc);	


	D3D11_SAMPLER_DESC linearSamplerDesc = {};
	linearSamplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	linearSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	linearSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	linearSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	linearSamplerDesc.MipLODBias = 0.f;
	linearSamplerDesc.MaxAnisotropy = 1;
	linearSamplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
	linearSamplerDesc.MinLOD = -FLT_MAX;
	linearSamplerDesc.MaxLOD = FLT_MAX;
	GameEngineSampler::Create("EngineSampler_Linear", linearSamplerDesc);

	//D3D11 ERROR: ID3D11DeviceContext::DrawIndexed: 
	//The Pixel Shader unit expects a Sampler configured for default filtering to be set at Slot 0, 
	//but the sampler bound at this slot is configured for comparison filtering.  
	//This mismatch will produce undefined behavior if the sampler is used (e.g. it is not skipped due to shader code branching).
	//[ EXECUTION ERROR #390: DEVICE_DRAW_SAMPLER_MISMATCH]
	//->���÷� ���� �ɼ��� D3D11_FILTER::D3D11_FILTER_COMPARISON_~���� ����������, 
	// HLSL�ڵ�����δ� �� ���� �ɼ��� Ȱ������ �ʾ��� �� ������ ������ �Ѵ�..


	GameEngineDirectory currentDir;

	currentDir.MoveParentToExistChildDirectory("GameEngineResources");
	currentDir.MoveToChild("GameEngineResources");
	currentDir.MoveToChild("Texture");

	std::vector<GameEngineFile> textures = currentDir.GetAllFiles();

	for (size_t i = 0; i < textures.size(); i++)
	{
		GameEngineTexture::Load(textures[i].GetFullPath());
	}
}

void EngineRenderingPipeLine()
{

	GameEngineMaterial* newRenderingPipeLine1 
		= GameEngineMaterial::Create("Color");
	newRenderingPipeLine1->SetVertexShader("Color.hlsl");
	newRenderingPipeLine1->SetPixelShader("Color.hlsl");
	newRenderingPipeLine1->SetRasterizer("EngineRasterizer");
	newRenderingPipeLine1->SetBlend_OutputMerger("AlphaBlend");
	newRenderingPipeLine1->SetDepthStencil_OutputMerger("EngineBaseDepth");


	GameEngineMaterial* newRenderingPipeLine2 
		= GameEngineMaterial::Create("Texture");
	newRenderingPipeLine2->SetVertexShader("Texture.hlsl");
	newRenderingPipeLine2->SetPixelShader("Texture.hlsl");
	newRenderingPipeLine2->SetRasterizer("EngineRasterizer");
	newRenderingPipeLine2->SetBlend_OutputMerger("AlphaBlend");
	newRenderingPipeLine2->SetDepthStencil_OutputMerger("EngineBaseDepth");


	GameEngineMaterial* newRenderingPipeLine3 
		= GameEngineMaterial::Create("TextureAtlas");
	newRenderingPipeLine3->SetVertexShader("TextureAtlas.hlsl");
	newRenderingPipeLine3->SetPixelShader("TextureAtlas.hlsl");
	newRenderingPipeLine3->SetRasterizer("EngineRasterizer");
	newRenderingPipeLine3->SetBlend_OutputMerger("AlphaBlend");
	newRenderingPipeLine3->SetDepthStencil_OutputMerger("EngineBaseDepth");


	GameEngineMaterial* newRenderingPipeLine4 
		= GameEngineMaterial::Create("3DDebug");
	newRenderingPipeLine4->SetVertexShader("Debug3D.hlsl");
	newRenderingPipeLine4->SetPixelShader("Debug3D.hlsl");
	newRenderingPipeLine4->SetRasterizer("EngineRasterizer");
	newRenderingPipeLine4->SetBlend_OutputMerger("AlphaBlend");
	newRenderingPipeLine4->SetDepthStencil_OutputMerger("AlwaysDepth");


	GameEngineMaterial* newRenderingPipeLine5 
		= GameEngineMaterial::Create("DebugTexture");
	newRenderingPipeLine5->SetVertexShader("DebugTexture.hlsl");
	newRenderingPipeLine5->SetPixelShader("DebugTexture.hlsl");
	newRenderingPipeLine5->SetRasterizer("EngineRasterizer");
	newRenderingPipeLine5->SetBlend_OutputMerger("AlphaBlend");
	newRenderingPipeLine5->SetDepthStencil_OutputMerger("EngineBaseDepth");


	GameEngineMaterial* newRenderingPipeLine6 
		= GameEngineMaterial::Create("TargetMerge");
	newRenderingPipeLine6->SetVertexShader("TargetMerge.hlsl");
	newRenderingPipeLine6->SetPixelShader("TargetMerge.hlsl");
	newRenderingPipeLine6->SetRasterizer("EngineRasterizer");
	newRenderingPipeLine6->SetBlend_OutputMerger("AlphaBlend");
	newRenderingPipeLine6->SetDepthStencil_OutputMerger("AlwaysDepth");


	GameEngineMaterial* newRenderingPipeLine7 
		= GameEngineMaterial::Create("Blur");
	newRenderingPipeLine7->SetVertexShader("Blur.hlsl"); 
	newRenderingPipeLine7->SetPixelShader("Blur.hlsl");
	newRenderingPipeLine7->SetRasterizer("EngineRasterizer");
	newRenderingPipeLine7->SetBlend_OutputMerger("AlphaBlend");
	newRenderingPipeLine7->SetDepthStencil_OutputMerger("EngineBaseDepth");
}

void EngineMesh()
{
	//�⺻ �簢��.
	std::vector<GameEngineVertex> rectVertex;
	rectVertex.reserve(4);
	rectVertex.push_back({ float4(-0.5f, 0.5f), float4(), float4(0, 0) });	//0�� ��.
	rectVertex.push_back({ float4(0.5f, 0.5f), float4(), float4(1, 0) });	//1�� ��.
	rectVertex.push_back({ float4(0.5f, -0.5f), float4(), float4(1, 1) });	//2�� ��.
	rectVertex.push_back({ float4(-0.5f, -0.5f), float4(), float4(0, 1) });	//3�� ��.
	GameEngineVertexBuffer::Create("RectVertex", rectVertex);

	std::vector<int> rectIndex;
	rectIndex.reserve(6);
	//1�� �ﰢ��.
	rectIndex.push_back(0);
	rectIndex.push_back(1);
	rectIndex.push_back(2);
	//2�� �ﰢ��.
	rectIndex.push_back(0);
	rectIndex.push_back(2);
	rectIndex.push_back(3);
	GameEngineIndexBuffer::Create("RectIndex", rectIndex);

	GameEngineMesh::Create("Rect", "RectVertex", "RectIndex");





	//������ ��ü ũ�� �簢��.
	std::vector<GameEngineVertex> fullrectVertex;
	fullrectVertex.reserve(4);
	fullrectVertex.push_back({ float4(-1.f, 1.f), float4(), float4(0, 0) });	//0�� ��.
	fullrectVertex.push_back({ float4(1.f, 1.f), float4(), float4(1, 0) });		//1�� ��.
	fullrectVertex.push_back({ float4(1.f, -1.f), float4(), float4(1, 1) });	//2�� ��.
	fullrectVertex.push_back({ float4(-1.f, -1.f), float4(), float4(0, 1) });	//3�� ��.
	GameEngineVertexBuffer::Create("FullrectVertex", fullrectVertex);

	std::vector<int> fullrectIndex;
	fullrectIndex.reserve(6);
	//1�� �ﰢ��.
	fullrectIndex.push_back(0);
	fullrectIndex.push_back(1);
	fullrectIndex.push_back(2);
	//2�� �ﰢ��.
	fullrectIndex.push_back(0);
	fullrectIndex.push_back(2);
	fullrectIndex.push_back(3);
	GameEngineIndexBuffer::Create("FullrectIndex", fullrectIndex);

	GameEngineMesh::Create("Fullrect", "FullrectVertex", "FullrectIndex");




	//�⺻ ����ü.
	std::vector<GameEngineVertex> boxVertex;
	boxVertex.reserve(8);
	boxVertex.push_back({ float4(-0.5f, 0.5f, -0.5f), float4(), float4(0, 0)});		//0�� ��.
	boxVertex.push_back({ float4(0.5f, 0.5f, -0.5f), float4(), float4(1, 0)});		//1�� ��.
	boxVertex.push_back({ float4(0.5f, -0.5f, -0.5f), float4(), float4(1, 1)});		//2�� ��.
	boxVertex.push_back({ float4(-0.5f, -0.5f, -0.5f), float4(), float4(0, 1)});	//3�� ��.

	boxVertex.push_back({ float4(0.5f, 0.5f, 0.5f), float4(), float4(0, 1)});		//4�� ��.
	boxVertex.push_back({ float4(-0.5f, 0.5f, 0.5f), float4(), float4(1, 1)});		//5�� ��.
	boxVertex.push_back({ float4(-0.5f, -0.5f, 0.5f), float4(), float4(1, 0)});		//6�� ��.
	boxVertex.push_back({ float4(0.5f, -0.5f, 0.5f), float4(), float4(0, 0)});		//7�� ��.
	GameEngineVertexBuffer::Create("BoxVertex", boxVertex);

	std::vector<int> boxIndex;
	rectIndex.reserve(36);

	//���� 1�� �ﰢ��.
	boxIndex.push_back(0);
	boxIndex.push_back(1);
	boxIndex.push_back(2);
	//���� 2�� �ﰢ��.
	boxIndex.push_back(0);
	boxIndex.push_back(2);
	boxIndex.push_back(3);
	//�ĸ� 1�� �ﰢ��.
	boxIndex.push_back(4);
	boxIndex.push_back(5);
	boxIndex.push_back(6);
	//�ĸ� 2�� �ﰢ��.
	boxIndex.push_back(4);
	boxIndex.push_back(6);
	boxIndex.push_back(7);

	//�¸� 1�� �ﰢ��.
	boxIndex.push_back(5);
	boxIndex.push_back(0);
	boxIndex.push_back(3);
	//�¸� 2�� �ﰢ��.
	boxIndex.push_back(5);
	boxIndex.push_back(3);
	boxIndex.push_back(6);
	//��� 1�� �ﰢ��.
	boxIndex.push_back(1);
	boxIndex.push_back(4);
	boxIndex.push_back(7);
	//��� 2�� �ﰢ��.
	boxIndex.push_back(1);
	boxIndex.push_back(7);
	boxIndex.push_back(2);

	//��� 1�� �ﰢ��.
	boxIndex.push_back(5);
	boxIndex.push_back(4);
	boxIndex.push_back(1);
	//��� 2�� �ﰢ��.
	boxIndex.push_back(5);
	boxIndex.push_back(1);
	boxIndex.push_back(0);
	//�ϸ� 1�� �ﰢ��.
	boxIndex.push_back(2);
	boxIndex.push_back(3);
	boxIndex.push_back(6);
	//�ϸ� 2�� �ﰢ��.
	boxIndex.push_back(2);
	boxIndex.push_back(6);
	boxIndex.push_back(7);

	GameEngineIndexBuffer::Create("BoxIndex", boxIndex);

	GameEngineMesh::Create("Box", "BoxVertex", "BoxIndex");




	GameEngineFont::Load("����");
}

void ShaderCompile()
{
	GameEngineDirectory engineResourceDir;   

	engineResourceDir.MoveParentToExistChildDirectory("GameEngineResources");
	engineResourceDir.MoveToChild("GameEngineResources");
	engineResourceDir.MoveToChild("GameEngineShader");

	std::vector<GameEngineFile> shaders = engineResourceDir.GetAllFiles("hlsl");

	for (size_t i = 0; i < shaders.size(); i++)
	{
		GameEngineShader::AutoCompile(shaders[i].GetFullPath());
	}
}

void GameEngineCore::EngineResourceInitialize()
{
	EngineTextureLoad();	//���÷��� �����ϰ� �ؽ�ó�� �ҷ����� �Լ�.
	EngineInputLayout();	//���� �⺻���� ��ǲ ���̾ƿ��� �����ϴ� �Լ�.
	EngineMesh();			//���� �⺻���� �簢���� ����ü �޽��� �����ϴ� �Լ�.
	EngineSubSetting();		//���� �⺻���� �����Ͷ�����, ������, ���̽��ٽ��� �����ϴ� �Լ�.
	ShaderCompile();		//���� �⺻���� HLSL�ڵ带 �������ؼ� ���̴��� ���̴����ҽ��� ����, �����ϴ� �Լ�.

	EngineRenderingPipeLine();	//���� �⺻���� ������ ���������ε��� �����ϴ� �Լ�.
}

void GameEngineCore::EngineResourceDestroy()
{
	//�簢��, ����ü, �����ؽ��� ���, ���� ���ؿ��� �⺻������ �����Ǿ�� �ϴ� ���ҽ��� �����ϴ� �Լ�.
	GameEngineMaterial::ResourceDestroy();

	GameEngineInputLayout::ResourceDestroy();
	GameEngineVertexBuffer::ResourceDestroy();
	GameEngineVertexShader::ResourceDestroy();
	GameEngineIndexBuffer::ResourceDestroy();
	GameEngineRasterizer::ResourceDestroy();
	GameEnginePixelShader::ResourceDestroy();
	GameEngineDepthStencil::ResourceDestroy();
	GameEngineBlend::ResourceDestroy();

	GameEngineMesh::ResourceDestroy();

	GameEngineConstantBuffer::ResourceDestroy();
	GameEngineStructuredBuffer::ResourceDestroy();
	GameEngineInstancingBuffer::ResourceDestroy();
	GameEngineRenderTarget::ResourceDestroy();
	GameEngineTexture::ResourceDestroy();
	GameEngineFolderTexture::ResourceDestroy();
	GameEngineSampler::ResourceDestroy();
	GameEngineSound::ResourceDestroy();
	GameEngineFont::ResourceDestroy();

	GameEngineDevice::Destroy();	
	//��� ���ҽ����� ���̷�ƮX ����̽��� ������ �־�� ������ �� �����Ƿ�,
	// ���ҽ��� ���� �ı��� �Ŀ� ����̽��� �����Ѵ�.
}