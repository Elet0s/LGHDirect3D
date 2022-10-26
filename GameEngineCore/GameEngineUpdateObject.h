#pragma once

class GameEngineUpdateObject: public GameEngineDebugObject
{
	//�� Ŭ������ ���� ������ ����?? �ݵ�� �����ص� ��.


public:

	GameEngineUpdateObject();
	virtual ~GameEngineUpdateObject();

	GameEngineUpdateObject(const GameEngineUpdateObject& _other) = delete;
	GameEngineUpdateObject(GameEngineUpdateObject&& _other) noexcept = delete;
	GameEngineUpdateObject& operator=(const GameEngineUpdateObject& _other) = delete;
	GameEngineUpdateObject& operator=(GameEngineUpdateObject&& _other) = delete;


public:
	//�� ������Ʈ���� �θ� �޾��ְ�, �θ� ������Ʈ�� children_ ����Ʈ�� �� ������Ʈ�� ����ϴ� �Լ�.
	virtual void SetParent(GameEngineUpdateObject* _newParent);
	
	//�� ������Ʈ�� �� �ڽĵ��� ���� �����ϴ� �Լ�.
	virtual void ReleaseHierarchy(); 

	// �� ������Ʈ�� �����ӱ����ȿ��� ���� ��ġ�� ����.
	virtual void Update(float _deltaTime) = 0;

	void AllUpdate(float _deltaTime);

	virtual void LevelStartEvent() {};
	virtual void LevelEndEvent() {};

public:
	inline void On()
	{
		this->isUpdate_ = true;
		AllOnEvent();
	}

	inline void Off()
	{
		this->isUpdate_ = false;
		AllOffEvent();
	}

	inline void OnOffSwitch()
	{
		this->isUpdate_ = !this->isUpdate_;
		isUpdate_ == true ? OnEvent() : OffEvent();
	}

	//Ư�� ������Ʈ�� ������Ʈ������ �ƴ��� �޾ƺ��� �Լ�.
	inline bool IsUpdate()
	{
		if (nullptr == parent_)
		{
			return this->isUpdate_ && false == this->isDead_;
		}
		else
		{
			return this->isUpdate_ && false == this->isDead_ && true == parent_->IsUpdate();
			//�ڱ� �ڽ��� isUpdate_ == true���� �θ� ������Ʈ�� isUpdate_ == false�� ������Ʈ���� ���ܵȴ�. 
		}
	}

	inline bool& IsUpdateRef()
	{
		return isUpdate_;
	}

	inline bool IsDead()
	{
		if (nullptr == parent_)
		{
			return this->isDead_;
		}
		else
		{
			return this->isDead_ || true == parent_->IsDead();
			//�ڱ� �ڽ��� isDead_ == false���� �θ� ������Ʈ�� isUpdate_ == true�� ��� �����ް� �����ȴ�.
		}
	}

	//Ư�� ������Ʈ�� ������ �������κ��� �󸶳� �ð��� �������� �����ϴ� �Լ�.
	void AddAccTime(float _deltaTime)
	{
		this->accTime_ += _deltaTime;
	}

	//������ �������κ��� �󸶸�ŭ�� �ð��� �������� �޾ƺ��� �Լ�.
	float GetAccTime()
	{
		return this->accTime_;
	}

	//Ư�� ������Ʈ�� ������ �ð� �ʱ�ȭ.
	void ResetAccTime()
	{
		this->accTime_ = 0.0f;
	}

	//Ư�� ������Ʈ�� ��� ������� ������ �Լ�.
	inline void Death()
	{
		this->isDead_ = true;
	}

	//�Է��� �ð��� ���� �� Ư�� ������Ʈ�� ��� �����ϴ� �Լ�.
	inline void Death(float _time)
	{
		this->isReleaseUpdate_ = true;
		this->deadTime_ = _time;
	}

	//Ư�� �ð� �� ��� �����Ǵ� ������Ʈ�� ���� �ð��� ������ �����ϴ� �Լ�.
	void ReleaseUpdate(float _deltaTime)
	{
		if (false == isReleaseUpdate_)
		{
			return;
		}

		this->deadTime_ -= _deltaTime;

		if (0.0f >= deadTime_)
		{
			this->isDead_ = true;
		}
	}

	inline int GetOrder()
	{
		return this->order_;
	}

	//������Ʈ ���� ����.
	virtual inline void SetOrder(int _order)
	{
		this->order_ = _order;
	}

	//����ȯ �θ� �ޱ�.
	template<typename ParentType>
	ParentType* GetParent()
	{
		return dynamic_cast<ParentType*>(parent_);
	}

	//GameEngineUpdateObject* ���·� �θ� �ޱ�.
	GameEngineUpdateObject* GetParent()
	{
		return parent_;
	}

	//���� �ٷ� �Ʒ� ��ϵ� ������Ʈ���� �Ž��� �ö�, �� ������Ʈ�� ����ȯ�ؼ� ��ȯ�޴� �Լ�.
	template<typename ParentType>
	ParentType* GetRoot()
	{
		return dynamic_cast<ParentType*>(GetRoot());
	}

	//���� �ٷ� �Ʒ� ��ϵ� ������Ʈ���� �Ž��� �ö�, �� ������Ʈ�� ��ȯ�޴� �Լ�.
	GameEngineUpdateObject* GetRoot()
	{
		GameEngineUpdateObject* currentObject = this;

		while (nullptr != currentObject->GetParent())
		{
			currentObject = currentObject->GetParent();
		}
		return currentObject;
	}



protected:
	//�� ������Ʈ�� ������Ʈ�� �����Ҷ� ȣ���ϴ� �Լ�.
	virtual void OnEvent() {}

	//�� ������Ʈ�� ������Ʈ�� ������ �� ȣ���ϴ� �Լ�.
	virtual void OffEvent() {}

	//�� ������Ʈ�� �����ɶ� ��ġ�� �������� �����ϴ� �Լ�.
	virtual void Start() = 0;

	// �� ������Ʈ�� �޸𸮰� ������ �� ��ġ�� �������� �����ϴ� �Լ�. ���� �����ھ��� End()�ܿ� ȣ����� ����.
	virtual void End() = 0;

	void AllLevelStartEvent();
	void AllLevelEndEvent();

	void AllOnEvent();		//
	void AllOffEvent();		//

	//����������� ������Ʈ�� �� �ڽĵ��� ������Ʈ �������� ��� ���� ��� ����Ʈ�� ����ϴ� �Լ�.
	//���⿡ ���� ������Ʈ���� ���� �������� �����ȴ�. �ٷ� �������� �ʴ°Ϳ� ������ ��.
	void ReleaseObject(std::list<GameEngineUpdateObject*>& _releaseList);	

	//�� ������Ʈ�� �θ� ������Ʈ�� children_����Ʈ���� �����ϴ� �Լ�.
	virtual void DetachObject();

protected:
	//�� ������Ʈ�� �ڽĵ� �� Ư�� ������ �ڽĵ鸸 �޾ƺ��� �Լ�.
	template<typename ConvertType>
	std::list<ConvertType*> GetConvertedChildren()
	{
		std::list<ConvertType*> newList;

		for (GameEngineUpdateObject* child : children_)
		{
			ConvertType* convertedChild = dynamic_cast<ConvertType*>(child);
			if (nullptr != convertedChild)
			{
				newList.push_back(convertedChild);
			}
		}

		return newList;
	}

protected:
	std::list<GameEngineUpdateObject*> children_;	//�ڽ� ������Ʈ��.

private:



private:
	int order_;			//������Ʈ ����.
	bool isReleaseUpdate_;	//true: Ư�� ������Ʈ�� ���� �ð��� ���.
	float deadTime_;	//���� ������Ʈ�� ��� ���� �ð�. �� �ð��� 0�̳� ������ �Ǹ� ���.
	float accTime_;		//������Ʈ ���� �������κ��� ���� �ð�.

	bool isUpdate_;	//true: ������Ʈ ����. false: ������Ʈ ����.
	bool isDead_;	//true: ��� ���� ����. false: ��� ���� �� ����. ���ó���� �ƴ� ��� �����ӿ� ����.

	GameEngineUpdateObject* parent_;	//�θ� ������Ʈ.


};
