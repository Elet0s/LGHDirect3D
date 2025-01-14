﻿#include "PreCompile.h"
#include "GameEngineStatusWindow.h"
#include "ShaderTestLevel.h"
#include "ClearLevel.h"
#include "Mouse.h"

void GameEngineImageShotWindow::RenderTextureSetting(ImTextureID _renderTexture, ImVec2 _size)
{
	renderTexture_ = _renderTexture;
	size_ = _size;
}

void GameEngineImageShotWindow::Initialize(GameEngineLevel* _level)
{
}

void GameEngineImageShotWindow::OnGUI(GameEngineLevel* _level, float _deltaTime)
{
	if (true == ImGui::ImageButton(renderTexture_, size_))
	{
		this->Off();
	}
}

std::map<std::string, GameEngineRenderTarget*> GameEngineStatusWindow::debugRenderTargets_;

GameEngineStatusWindow::GameEngineStatusWindow()
{
}

GameEngineStatusWindow::~GameEngineStatusWindow()
{
}

void GameEngineStatusWindow::AddDebugRenderTarget(
	const std::string& _renderTargetName,
	GameEngineRenderTarget* _renderTarget
)
{
	if (debugRenderTargets_.end() != debugRenderTargets_.find(_renderTargetName))
	{
		MsgBoxAssertString(_renderTargetName + ": 이미 같은 이름의 렌더타겟이 존재합니다.");
		return;
	}

	debugRenderTargets_.insert(std::make_pair(_renderTargetName, _renderTarget));
}

void GameEngineStatusWindow::Initialize(GameEngineLevel* _level)
{
}

void GameEngineStatusWindow::OnGUI(GameEngineLevel* _level, float _deltaTime)
{
	std::string currentFPS = "Current FPS : " + std::to_string(GameEngineTime::GetFPS());
	ImGui::Text(currentFPS.c_str());

	std::string currentDeltaTime = "Current DeltaTime : " + std::to_string(GameEngineTime::GetInst().GetDeltaTime());
	ImGui::Text(currentDeltaTime.c_str());


	std::string currentCameraPos = "CameraPos : " 
		+ std::to_string(GEngine::GetCurrentLevel()->GetMainCameraActor()->GetTransform().GetWorldPosition().x)
		+ ", "
		+ std::to_string(GEngine::GetCurrentLevel()->GetMainCameraActor()->GetTransform().GetWorldPosition().y)
		+ ", "
		+ std::to_string(GEngine::GetCurrentLevel()->GetMainCameraActor()->GetTransform().GetWorldPosition().z);
	ImGui::Text(currentCameraPos.c_str());


	//if (nullptr != reinterpret_cast<ClearLevel*>(GEngine::GetCurrentLevel()))
	//{
	//	std::string currentMousePos = "MouseWorldPosition : ";
	//	currentMousePos += std::to_string(reinterpret_cast<ClearLevel*>(GEngine::GetCurrentLevel())->GetMousePointer()->GetTransform().GetWorldPosition().x);
	//	currentMousePos += ", ";
	//	currentMousePos += std::to_string(reinterpret_cast<ClearLevel*>(GEngine::GetCurrentLevel())->GetMousePointer()->GetTransform().GetWorldPosition().y);
	//	currentMousePos += ", ";
	//	currentMousePos += std::to_string(reinterpret_cast<ClearLevel*>(GEngine::GetCurrentLevel())->GetMousePointer()->GetTransform().GetWorldPosition().z);

	//	ImGui::Text(currentMousePos.c_str());
	//}



	if (true == ImGui::Button("Collision Debug Switch"))
	{
		GEngine::CollisionDebugSwitch();
	}

	if (true == ImGui::Button("FreeCamera On/Off"))
	{
		GEngine::GetCurrentLevel()->GetMainCameraActor()->FreeCameraModeOnOff();
		GEngine::GetCurrentLevel()->GetCameraActor(CameraOrder::MidCamera)->FreeCameraModeOnOff();
	}


	ImGui::Text("Level Selection");
	for (std::pair<std::string, GameEngineLevel*> levelPair : GameEngineCore::allLevels_)
	{
		if (true == ImGui::Button(levelPair.first.c_str()))
		{
			GameEngineCore::ChangeLevel(levelPair.first);
		}

		ImGui::SameLine();
	}

	ImGui::NewLine();

	ImGui::Text("All RenderTargets");

	for (std::pair<std::string, GameEngineRenderTarget*> renderTargetPair : debugRenderTargets_)
	{
		if (true == ImGui::TreeNodeEx(renderTargetPair.first.c_str(), 0))
		{
			GameEngineRenderTarget* renderTarget = renderTargetPair.second;

			for (ID3D11ShaderResourceView* shaderResourceView : renderTarget->shaderResourceViews_)
			{
				float4 renderTargetScale = GameEngineWindow::GetInst().GetScale() * 0.2f;

				if (true == ImGui::ImageButton(static_cast<ImTextureID>(shaderResourceView),
					{ renderTargetScale.x, renderTargetScale.y })
					)
				{
					GameEngineImageShotWindow* newImageShotWindow
						= GameEngineGUI::CreateGUIWindow<GameEngineImageShotWindow>("Image Shot", nullptr);

					newImageShotWindow->RenderTextureSetting(static_cast<ImTextureID>(shaderResourceView),
						{ GameEngineWindow::GetInst().GetScale().x, GameEngineWindow::GetInst().GetScale().y });
				}

			}
			ImGui::TreePop();
		}
	}

	debugRenderTargets_.clear();
}