#include <DxLib.h>
#include "../../Manager/InputManager.h"
#include "../GameScene.h"
#include "BattlePhase.h"

BattlePhase::BattlePhase(GameScene& gameScene)
	: gameScene_(gameScene)
{
	battleStep_ = BATTLE_STEP::COMMAND_SELECT;
	commandSelect_ = COMMAND_SELECT::ATTACK;
	
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

	TurnManager();

	if (ins.IsTrgDown(KEY_INPUT_RETURN))
	{
		gameScene_.ChangePhase(GameScene::QUEST_PHASE::PHASE_STORY);
	}
}

void BattlePhase::Draw()
{
	DrawString(0, 0, "Battle Phase", GetColor(255, 0, 0));
	DrawCommandMenu(0, 40, { "こうげき", "話す", "逃げる" }, static_cast<int>(commandSelect_));
}

void BattlePhase::Release()
{
}

void BattlePhase::TurnManager(void)
{
	switch (battleStep_)
	{
	case BattlePhase::BATTLE_STEP::COMMAND_SELECT:
		CommandSelect();
		break;
	case BattlePhase::BATTLE_STEP::PLAYER_ATTACK:
		PlayerAttack();
		break;
	case BattlePhase::BATTLE_STEP::ENEMY_ATTACK:
		EnemyAttack();
		break;
	case BattlePhase::BATTLE_STEP::MAX:
		break;
	default:
		break;
	}
}

void BattlePhase::CommandSelect(void)
{
	// コマンド選択の処理
	int commandIndex = static_cast<int>(commandSelect_);
	ProcessCommandMenuSelection(commandIndex, static_cast<int>(COMMAND_SELECT::MAX));
	commandSelect_ = static_cast<COMMAND_SELECT>(commandIndex);
}

void BattlePhase::PlayerAttack(void)
{
}

void BattlePhase::EnemyAttack(void)
{
}


void BattlePhase::DrawCommandMenu(int x, int y, const std::vector<std::string>& labels, int cursor, int interval)
{
	for (int i = 0; i < (int)labels.size(); ++i) {
		int color = GetColor(255, 255, 255);
		int selectColor = GetColor(255, 255, 0); //選択中は黄色にする
		DrawFormatString(x, y + i * interval, (cursor == i ? selectColor : color), labels[i].c_str());
	}
}

void BattlePhase::ProcessCommandMenuSelection(int& cursor, int maxItems)
{
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_UP) ||
		InputManager::GetInstance().IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DG_UP))
	{
		cursor = (cursor - 1 + maxItems) % maxItems; //上にループ
	}
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_DOWN) ||
		InputManager::GetInstance().IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DG_DOWN))
	{
		cursor = (cursor + 1) % maxItems; //下にループ
	}
}