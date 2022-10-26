#include "PreCompile.h"
#include "GameEngineThread.h"
#include "GameEngineDebug.h"
#include "GameEngineString.h"

GameEngineThread::GameEngineThread()
{
}

GameEngineThread::GameEngineThread(
	const std::string& _threadName,
	std::function<void(GameEngineThread*)> _callback
)
{
	this->Start(_threadName, _callback);
}

GameEngineThread::~GameEngineThread()
{
	this->Join();
}

void GameEngineThread::Start(const std::string& _threadName, std::function<void(GameEngineThread*)> _callback)
{
	this->workFunction_ = _callback;
	//�� �����尡 ������ �Լ��� �����Ѵ�.

	this->thread_ = std::thread(GameEngineThreadFunction, this, _threadName);
	//std::thread�� �����ϸ鼭 ������ �Լ��� GameEngineThreadFunction()��, �־��� �Ű������� 
	// �� GameEngineThread ��ü�� �ٿ��� ������ �̸��� �־��ش�. ���� �־��� �Լ��� ���� ȣ������ �ʴ°Ϳ� ������ ��.
	//�̷��� ������ ������� true == joinable ���·� �����ȴ�.
	//�⺻ �����ڷ� ������ ������� false == joinable ���·� �����ȴ�. 
	// �Ƹ� � �Լ��� �Լ������Ͱ� �����忡 ����Ǿ�߸� true == joinable�� �Ǵ°� ����.
}

void GameEngineThread::Join()
{
	if (nullptr != this->workFunction_)
	{
		Sleep(1);	
		this->thread_.join();	//�� �������� �۾��� ���߰� �θ� �����尡 ����ɶ����� ����ϰ� �ϴ� �Լ�.
		//�� �Լ��� ���� ȣ��Ǽ� �����带 ���� ������ ���̵� 0���� �����߸� std::thread�� �Ҹ��ڰ� ����� ����� �� �ִ�.

		this->workFunction_ = nullptr;

		//if (true == this->thread_.joinable())
		//{
		//	MsgBoxAssert("�� �������� join()�� ����� �̷������ �ʾҽ��ϴ�.");
		//}
		//std::thread�� �Ҹ��ڿ� �̹� �̷� ��쿡 ������ �߻���Ű�� �ڵ尡 �ִٰ� �Ѵ�.
	}
}

void GameEngineThread::GameEngineThreadFunction(GameEngineThread* _thisThread, const std::string& _threadName)
{
	//����� �������� ����.

	SetThreadDescription(		//�����忡 ������ ������ ���ڿ��� �ٿ��ִ� �Լ�. ���⼭�� �̸��� �ٿ��ش�.
		GetCurrentThread(),		//���� �������� �ڵ��� ��ȯ�ϴ� �Լ�.
		GameEngineString::AnsiToUnicodeReturn(_threadName).c_str()	//�����忡 �ٿ��� ���ڿ�. ���⼭�� ������ �̸�.
	);

	_thisThread->workFunction_(_thisThread);	//_thisThread�� �����Ų �Լ� ����.
}