#include "PreCompile.h"
#include "LoginLevel.h"
#include "LoginUI.h"
#include "Enums.h"
#include "Player.h"
#include "GlobalContentsValue.h"

LoginLevel::LoginLevel()
{
}

LoginLevel::~LoginLevel()
{
}

void LoginLevel::Start()
{
	if (false == GameEngineInput::GetInst()->IsKey("P"))
	{
		GameEngineInput::GetInst()->CreateKey("LevelChange", 'P');
	}

	//GameEngineCameraActor* tempCameraActor = CreateActor<GameEngineCameraActor>(GameObjectGroup::UI);
	//tempCameraActor->GetTransform().SetLocalPosition(0, 0, -100);
	//tempCameraActor->GetCameraComponent()->SetProjectionMode(CameraProjectionMode::Orthographic);
	//[1]  [0]  [0]   [0]
	//[0]  [1]  [0]   [0]
	//[0]  [0]  [1]   [0]	���� ���� �ִ�.
	//[0]  [0]  [-100][0]	�ڷ� 100��ŭ ��������.

	//loadingThread_.Start("LoginLoading", ThreadLoading);

}

void LoginLevel::Update(float _deltaTime)
{
	if (true == GameEngineInput::GetInst()->IsDown("LevelChange"))
	{
		GEngine::ChangeLevel("Play");
	}

	if (true == GameEngineInput::GetInst()->IsDown("FreeCameraModeOnOff"))
	{
		GetMainCameraActor()->FreeCameraModeOnOff();
	}
}

void LoginLevel::End()
{
}

void LoginLevel::LevelStartEvent()
{
}