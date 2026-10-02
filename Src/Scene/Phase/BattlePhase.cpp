#include <DxLib.h>
#include "../../Manager/InputManager.h"
#include "../GameScene.h"
#include "BattlePhase.h"

BattlePhase::BattlePhase(GameScene& gameScene)
	: gameScene_(gameScene)
{
	battlePhase_ = COMMAND_SELECT;
	commandType_ = ATTACK;
	battleTurnCnt_ = 1;
}

BattlePhase::~BattlePhase(void)
{
}

void BattlePhase::Init(void)
{
	
}

void BattlePhase::Update(void)
{
	//フェーズごとの更新処理を呼び出す
	switch (battlePhase_)
	{
	case BattlePhase::COMMAND_SELECT:
		UpdateCommandSelect();
		break;
	case BattlePhase::PLAYER_TURN:
		UpdatePlayerTurn();
		break;
	case BattlePhase::ENEMY_TURN:
		UpdateEnemyTurn();
		break;
	case BattlePhase::BATTLE_END:
		UpdateBattleEnd();
		break;
	}
}

void BattlePhase::Draw(void)
{
	DrawFormatString(100, 50, 0xFFFFFF, "Battle Turn: %d", battleTurnCnt_);
	DrawString(100, 100, "Battle Phase", GetColor(255, 0, 0));
	DrawFormatString(100, 150, 0xFFFFFF, "Battle Phase: %d", battlePhase_);
	switch (battlePhase_)
	{
	case BattlePhase::COMMAND_SELECT:
		DrawCommandSelect();
		break;
	case BattlePhase::PLAYER_TURN:
		DrawPlayerTrun();
		break;
	case BattlePhase::ENEMY_TURN:
		DrawEnemyTrun();
		break;
	case BattlePhase::BATTLE_END:
		DrawString(100, GameScene::COMMAND_MENU_Y, "バトル終了", GetColor(255, 0, 0));
		break;
	}
}

void BattlePhase::Release(void)
{
}

void BattlePhase::UpdateCommandSelect(void)
{
	//コマンド選択の処理
	if(ins_.IsTrgDown(KEY_INPUT_Q))
	{
		commandType_ = ATTACK;
		battlePhase_ = PLAYER_TURN;
	}
	if (ins_.IsTrgDown(KEY_INPUT_W))
	{
		commandType_ = TALK;
		battlePhase_ = PLAYER_TURN;
	}
	if (ins_.IsTrgDown(KEY_INPUT_E))
	{
		commandType_ = RUN;
		battlePhase_ = PLAYER_TURN;
	}
}

void BattlePhase::UpdatePlayerTurn(void)
{
	//プレイヤーのターンの処理
	switch (commandType_)
	{
	case BattlePhase::ATTACK:
		ProsesSelectAttack();
		break;
	case BattlePhase::TALK:
		ProsesSelectTalk();
		break;
	case BattlePhase::RUN:
		ProsesSelectRun();
		break;
	}
}

void BattlePhase::UpdateEnemyTurn(void)
{
}

void BattlePhase::UpdateBattleEnd(void)
{
	if (ins_.IsTrgDown(KEY_INPUT_RETURN))
	{
		gameScene_.ChangePhase(GameScene::QUEST_PHASE::PHASE_STORY);
	}
}

void BattlePhase::DrawCommandSelect(void)
{
	//コマンド選択の描画処理
	DrawString(GameScene::COMMAND_MENU_X, GameScene::COMMAND_MENU_Y, "1.こうげき　2.はなす　3.にげる", GetColor(255, 255, 255));
}

void BattlePhase::DrawPlayerTrun(void)
{
	//プレイヤーのターンの描画処理
	DrawFormatString(100, GameScene::COMMAND_MENU_Y, 0xFFFFFF, "CommandType: %d", commandType_);
}

void BattlePhase::DrawEnemyTrun(void)
{
	//敵のターンの描画処理
}

void BattlePhase::ProsesSelectAttack(void)
{
	if (ins_.IsTrgDown(KEY_INPUT_RETURN))
	{
		battleTurnCnt_++;
		battlePhase_ = COMMAND_SELECT;
		//battlePhase_ = ENEMY_TURN;
	}
}

void BattlePhase::ProsesSelectTalk(void)
{
	if (ins_.IsTrgDown(KEY_INPUT_RETURN))
	{
		battleTurnCnt_++;
		battlePhase_ = COMMAND_SELECT;
		//battlePhase_ = ENEMY_TURN;
	}
}

void BattlePhase::ProsesSelectRun(void)
{
	if (ins_.IsTrgDown(KEY_INPUT_RETURN))
	{
		//ターンが経過するごとに逃げれる確率が下がる
		int runRate = RUN_SUCCESS_RATE + (battleTurnCnt_ * 5);

		if (runRate <= GetRand(99))
		{
			battlePhase_ = BATTLE_END;
		}
		else
		{
			battleTurnCnt_++;
			battlePhase_ = COMMAND_SELECT;
		}
	}
}
