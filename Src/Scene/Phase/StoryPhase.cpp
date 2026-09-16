#include <DxLib.h>
#include "../../Manager/InputManager.h"
#include "../GameScene.h"
#include "StoryPhase.h"

StoryPhase::StoryPhase(GameScene& gameScene)
	: gameScene_(gameScene)
{
}

StoryPhase::~StoryPhase()
{
}

void StoryPhase::Init()
{
}

void StoryPhase::Update()
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsTrgDown(KEY_INPUT_RETURN))
	{
		gameScene_.ChangePhase(GameScene::QUEST_PHASE::PHASE_BATTLE);
	}
}

void StoryPhase::Draw()
{
	DrawString(100, 100, "Story Phase", GetColor(255, 255, 255));
}

void StoryPhase::Release()
{
}