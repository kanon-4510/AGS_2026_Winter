#include <DxLib.h>
#include "../../Manager/InputManager.h"
#include "../GameScene.h"
#include "BattlePhase.h"

BattlePhase::BattlePhase(GameScene& gameScene)
	: gameScene_(gameScene)
{

}

BattlePhase::~BattlePhase()
{
}

void BattlePhase::Init()
{
}

void BattlePhase::Update()
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsTrgDown(KEY_INPUT_RETURN))
	{
		gameScene_.ChangePhase(GameScene::QUEST_PHASE::PHASE_STORY);
	}
}

void BattlePhase::Draw()
{
	DrawString(100, 100, "Battle Phase", GetColor(255, 0, 0));
}

void BattlePhase::Release()
{
}