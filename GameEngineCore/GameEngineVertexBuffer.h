#pragma once
#include "GameEngineRes.h"

class GameEngineInputLayoutDesc;
class GameEngineVertexBuffer: public GameEngineRes<GameEngineVertexBuffer>
{
	
	//���ؽ�: ����, ������. �ٰ������� �ΰ� �̻��� ���� ������ ��. ��� �� ���·� �׸��� ������ ������ �� �ִ�.
	//����� ���ؽ��� ��ġ �����ۿ� ������ ���� DirectX���� ��ġ���� �Ӹ� �ƴ϶� ��, ��, �ؽ�ó ���� ������ ���� �����ؼ�, 
	//FVF(Flexible Vertex Format)�̶�� �������� ��Ƽ� �����ϰ� �ִ�.

	friend GameEngineRes<GameEngineVertexBuffer>;	
	//GameEngineVertexBufferŬ������ �����̺� �Ҹ��ڸ� GameEngineResŬ�������� ȣ���ϱ� ���� ���.


	//�׷��� ���ҽ��� �� ����(���ؽ� ����, �ε��� ����, ��� ����, ��Ʈ�� ��� ����)�� 
	// ���ҽ��� ���̵� ������ ���������ο� ������ �� �ִ�.
private:
	GameEngineVertexBuffer();
	~GameEngineVertexBuffer();	
	//�ܺο��� ���ڴ�� ���ҽ��� ����/�����ϴ°� ���� ���ؼ� ������/�Ҹ��ڸ� �����̺����� �����ؼ� �ܺ� ������ ���´�.
	//�� �����ӿ�ũ�� ���ҽ��� �ݵ�� �Ҹ��ڰ� �ƴ϶� ResourceDestroy()�Լ����� �����ؾ� �Ѵ�.
	//���α׷� ���������� ���ҽ������� ���ϸ� ������ �������� �׷��� �ִ��� ���Ƶд�.

	GameEngineVertexBuffer(const GameEngineVertexBuffer& _other) = delete;
	GameEngineVertexBuffer(GameEngineVertexBuffer&& _other) noexcept = delete;
	GameEngineVertexBuffer& operator=(const GameEngineVertexBuffer& _other) = delete;
	GameEngineVertexBuffer& operator=(GameEngineVertexBuffer&& _other) = delete;


public:
	static GameEngineVertexBuffer* Create(
		const std::string& _name,
		const void* _data,
		UINT _vertexSize,
		UINT _vertexCount,
		const GameEngineInputLayoutDesc& _info
	);
	void Setting();

public:
	template<typename VertexType>
	static GameEngineVertexBuffer* Create(
		const std::string& _name,
		const std::vector<VertexType>& _vertex
	)
	{
		return Create(
			_name,
			&_vertex[0],
			static_cast<UINT>(sizeof(VertexType)),
			static_cast<UINT>(_vertex.size()),
			VertexType::inputLayoutInfo_
		);
	}

	const GameEngineInputLayoutDesc& GetInputLayoutDesc() const
	{
		if (nullptr == this->inputLayoutDesc_)
		{
			MsgBoxAssert("������ ��ǲ ���̾ƿ� ������ �����ϴ�.");
		}

		return *inputLayoutDesc_;
	}

	inline ID3D11Buffer* GetBuffer()
	{
		return vertexBuffer_;
	}

	inline unsigned int GetVertexSize()
	{
		return vertexSize_;
	}

private:
	void CreateVertexBuffer(
		const void* _initialData,
		UINT _vertexSize,
		UINT _vertexCount
	);

private:
	ID3D11Buffer* vertexBuffer_;				//���ؽ� ������ ������ ���� �������̽�.

//���� ���� ���� ����.
	D3D11_SUBRESOURCE_DATA resData_;	//������ �ʱⰪ.
	D3D11_BUFFER_DESC vertexBufferDesc_;		//������ ���� ������.

	UINT vertexSize_;		//���� �Ѱ��� ũ��.
	UINT vertexCount_;		//���� ����.
	UINT offset_;			//������ ������ ������, ���� ���������κ����� �Ÿ�(byte).

	const GameEngineInputLayoutDesc* inputLayoutDesc_;


};
