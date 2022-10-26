#pragma once

//class MyLock
//{
//public:
//	std::mutex& LockInst;
//
//public:
//	MyLock(std::mutex& _Lock)
//		: LockInst(_Lock)
//	{
//		LockInst.lock();
//	}
//
//	~MyLock()
//	{
//		LockInst.unlock();
//	}
//};

template<typename ResType>
class GameEngineRes: public GameEngineNameObject
{
	//���ҽ� Ŭ�������� �ߺ��Ǵ� �ڵ带 ��Ȱ���ϱ� ���� ���ø� ������ Ŭ����.

	//���ø� Ŭ������ ����ϸ�, ��ӹ��� Ŭ���� ������ �� Ŭ������ ��������°Ͱ� ����.
	// �׷��Ƿ�, �Ʒ� �����̳ʵ� ��ӹ��� Ŭ���� ������ŭ �����ȴ�.


protected:
	GameEngineRes(): isOriginal_(true) {}
	~GameEngineRes() {}	//ResourceDestroy()�Լ����� �� ���ҽ�Ŭ���� �Ҹ��ڵ��� ���� ȣ���ϹǷ� virtual�� ������ �ʾƵ� ���� ����.

	GameEngineRes(const GameEngineRes& _other) = delete;
	GameEngineRes(GameEngineRes&& _other) noexcept = delete;
	GameEngineRes& operator=(const GameEngineRes& _other) = delete;
	GameEngineRes& operator=(GameEngineRes&& _other) = delete;

public:
	static ResType* Find(const std::string& _resName)
	{
		std::string uppercaseResName = GameEngineString::ToUpperReturn(_resName);

		typename std::map<std::string, ResType*>::iterator findIter;
		{
			std::lock_guard<std::mutex> lockInst(namedResLock_);
			//std::lock_guard<std::mutex>: std::mutex�� ���� Ŭ����.
			//���������� �����Ǹ鼭 ������ �Ű������� �־��� std::mutex ��ü�� lock() �Լ��� ȣ���ؼ� �޸𸮿� ���������� �ɰ�,
			//�������� ������ �Ҹ�� �� �Ҹ��ڿ��� std::mutex�� unlock() �Լ��� ȣ���ؼ� �޸𸮿� �ɸ� ���������� �����ϰ� �Ѵ�.
			//�׷��� �ѹ� ������ �� �θ� �ڵ����� ��ݰ� ������ �� �ش�.

			findIter = namedRes_.find(uppercaseResName);
			if (namedRes_.end() == findIter)
			{
				return  nullptr;
			}
		}

		return findIter->second;
	}

	//���ҽ� ��ü ���� �Լ�. �� ���α׷��� ��� ���ҽ��� �� �Լ��� ����/�����Ǿ�߸� �Ѵ�.
	static void ResourceDestroy()
	{
		for (std::pair<std::string, ResType*> resPair : namedRes_)
		{
			if (nullptr != resPair.second)
			{
				delete resPair.second;
				resPair.second = nullptr;
			}
		}

		for (ResType* res : unnamedRes_)
		{
			if (nullptr != res)
			{
				delete res;
				res = nullptr;
			}
		}

		//��� ���ҽ��� ���α׷� ���������� �� ������ �ذ�Ǵ� ��������,
		//�׷��� ���� �� �ִ°� �ִ��� �� ���Ƶд�.
	}

	bool IsOriginal()
	{
		return isOriginal_;
	}

	void SetPath(const std::string& _path)
	{
		path_ = _path;
	}

	const std::string& GetPath()
	{
		return path_;
	}

protected:
	static ResType* CreateNamedRes(const std::string& _resName = "")
	{
		ResType* newRes = CreateRes(_resName);

		std::pair<std::map<std::string, ResType*>::iterator, bool> insertResult;

		{
			std::lock_guard<std::mutex> lockInst(namedResLock_);
			//std::lock_guard<std::mutex>: std::mutex�� ���� Ŭ����.
			//���������� �����Ǹ鼭 ������ �Ű������� �־��� std::mutex ��ü�� lock() �Լ��� ȣ���ؼ� �޸𸮿� ���������� �ɰ�,
			//�������� ������ �Ҹ�� �� �Ҹ��ڿ��� std::mutex�� unlock() �Լ��� ȣ���ؼ� �޸𸮿� �ɸ� ���������� �����ϰ� �Ѵ�.
			//�׷��� �ѹ� ������ �� �θ� �ڵ����� ��ݰ� ������ �� �ش�.
			insertResult = namedRes_.insert(std::make_pair(newRes->GetNameConstRef(), newRes));
		}

		if (false == insertResult.second)
		{
			MsgBoxAssertString(newRes->GetNameConstRef() + ": �̹� �����ϴ� ���ҽ� �̸��Դϴ�.");
			return nullptr;
		}

		return newRes;
	}

	static ResType* CreateUnnamedRes()
	{
		ResType* newRes = CreateRes();
		std::lock_guard<std::mutex> lockInst(unnamedResLock_);
		unnamedRes_.push_back(newRes);
		return newRes;
	}

	static ResType* CreateRes(const std::string& _resName = "")
	{
		std::string uppercaseResName = GameEngineString::ToUpperReturn(_resName);

		ResType* newRes = new ResType();
		newRes->SetName(uppercaseResName);

		return newRes;
	}

protected:
	bool isOriginal_;

private:
	std::string path_;

	static std::map<std::string, ResType*> namedRes_;
	static std::list<ResType*> unnamedRes_;

	static std::mutex namedResLock_;
	static std::mutex unnamedResLock_;

	//std::mutex: ���� ��������� ������ �� �ִ� Ư�� �޸� ������ �Ѱ� �����常 ������ �� �ְ� �ؼ�
	// ���� ����(Race Condition) �߻��� �����ϴ� Ŭ����.
	// ��� �޸𸮴� ��ǻ� �̱۽����� ���°� �ǹǷ� ��Ƽ�������� �ǹ̰� ������Ƿ� �����ؼ� �ȵȴ�.
	//lock()�Լ��� �޸𸮸� ��� ���¿��� �ѹ� �� lock() �Լ��� ȣ���ϰų� unlock()�Լ� ȣ���� �ؾ������
	//��� ������ �ȵǴ� deadlock���°� �Ǿ� �ٽ� �� �޸𸮿� ������ �� ���� �ȴ�.


};

//���ø��� ���� ���� ��������� �ʱ�ȭ�� ���⼭ �Ѵ�.
template<typename ResType>
std::map<std::string, ResType*> GameEngineRes<ResType>::namedRes_;

template<typename ResType>
std::list<ResType*> GameEngineRes<ResType>::unnamedRes_;

template<typename ResType>
std::mutex GameEngineRes<ResType>::namedResLock_;

template<typename ResType>
std::mutex GameEngineRes<ResType>::unnamedResLock_;