#include <DxLib.h>
#include "../GameScene.h"
#include "../../Object/Player.h"
#include "BattlePhase.h"

BattlePhase::BattlePhase(GameScene& gameScene)
	: gameScene_(gameScene)
{
	battlePhase_ = COMMAND_SELECT;
	commandType_ = ATTACK;
	battleTurnCnt_ = 1;
	actionGauge_.Reset();
}

BattlePhase::~BattlePhase(void)
{

}

void BattlePhase::Init(void)
{
	player_ = std::make_unique<Player>();
	player_->Init();
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
	DrawFormatString(700, 120, 0xFFFFFF, "Enemy HP: %d", EnemyHP);

	// メッセージ枠およびアクションゲージの描画領域指定（例: 画面下部）
	int winX = 100, winY = 360, winW = 600, winH = 180;

	//アクションゲージ枠（常時表示）
	actionGauge_.Draw();

	player_->Draw();

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
	// キーが押されたときだけ各コマンド処理を呼び出す
	if (ins_.IsTrgDown(KEY_INPUT_A))
	{
		commandType_ = ATTACK;
		battlePhase_ = PLAYER_TURN;
		actionGauge_.Start(ActionGauge::BASE_GOOD_WIDTH, ActionGauge::BASE_GREAT_WIDTH, ActionGauge::BASE_PERFECT_WIDTH); // 攻撃ゲージ開始
	}
	else if (ins_.IsTrgDown(KEY_INPUT_S))
	{
		commandType_ = TALK;
		battlePhase_ = PLAYER_TURN;
	}
	else if (ins_.IsTrgDown(KEY_INPUT_D))
	{
		commandType_ = RUN;
		battlePhase_ = PLAYER_TURN;
	}
}

void BattlePhase::UpdatePlayerTurn(void)
{
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
	float deltaTime = 1.0f / 60.0f;
	actionGauge_.Update(deltaTime);

	// タイミングよくボタンを押してガード
	if (ins_.IsTrgDown(KEY_INPUT_SPACE))
	{
		GaugeState state = actionGauge_.PressButton();
		float guardRate = ActionGauge::GetResultRate(state);

		// TODO: 被ダメージ処理を記述
		player_->Defense(guardRate);
		player_->Damage(10); // 仮のダメージ計算例
	}
	// 時間切れ（ガード失敗）
	else if (!actionGauge_.IsVisible())
	{
	}

	if(ins_.IsTrgDown(KEY_INPUT_RETURN))
	{
		battleTurnCnt_++;
		battlePhase_ = COMMAND_SELECT;
	}
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
	DrawString(100, 130, "プレイヤーターン",0xFFFFFF);
	DrawFormatString(100, GameScene::COMMAND_MENU_Y, 0xFFFFFF, "CommandType: %d", commandType_);
}

void BattlePhase::DrawEnemyTrun(void)
{
	//敵のターンの描画処理
	DrawString(100, 130, "敵のターン", 0xFFFFFF);
}

void BattlePhase::ProsesSelectAttack(void)
{
	// バーがまだ動いている最中の処理
	if (actionGauge_.IsMoving())
	{
		float deltaTime = 1.0f / 60.0f;
		actionGauge_.Update(deltaTime);

		if (ins_.IsTrgDown(KEY_INPUT_SPACE))
		{
			// ボタンが押されたらバーがその場に止まる（表示は残る）
			GaugeState state = actionGauge_.PressButton();
			float rate = ActionGauge::GetResultRate(state);
			int power = player_->Attack(rate);

			EnemyHP -= power; // 仮のダメージ計算例
		}
	}
	// バーが止まった後の処理（止まったバーが画面に見えている状態）
	else
	{
		if (ins_.IsTrgDown(KEY_INPUT_RETURN))
		{
			battleTurnCnt_++;

			// 次のターン用ゲージを開始（ここで新しい表示・移動にリセットされる）
			actionGauge_.Start(ActionGauge::BASE_GOOD_WIDTH, ActionGauge::BASE_GREAT_WIDTH, ActionGauge::BASE_PERFECT_WIDTH);
			battlePhase_ = ENEMY_TURN;
		}
	}
}

void BattlePhase::ProsesSelectTalk(void)
{
	if (ins_.IsTrgDown(KEY_INPUT_RETURN))
	{
		battleTurnCnt_++;
		//battlePhase_ = COMMAND_SELECT;
		actionGauge_.Start(ActionGauge::BASE_GOOD_WIDTH, ActionGauge::BASE_GREAT_WIDTH, ActionGauge::BASE_PERFECT_WIDTH);
		battlePhase_ = ENEMY_TURN;
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
			actionGauge_.Start(ActionGauge::BASE_GOOD_WIDTH, ActionGauge::BASE_GREAT_WIDTH, ActionGauge::BASE_PERFECT_WIDTH);
			battlePhase_ = ENEMY_TURN;
		}
	}
}
