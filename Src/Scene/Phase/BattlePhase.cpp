#include <DxLib.h>
#include <string>
#include "../../Manager/InputManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SoundManager.h"
#include "../GameScene.h"
#include "../../Object/Player.h"
#include "../../Object/Enemy.h"
#include "BattlePhase.h"

BattlePhase::BattlePhase(GameScene& gameScene)
	: gameScene_(gameScene)
{
	battlePhase_ = COMMAND_SELECT;
	commandType_ = ATTACK;
	battleTurnCnt_ = 1;

	for (int i = 0; i < 7; i++)
	{
		usedEnemies_[i] = false;
	}

	selectedIndex_ = 0;
	actionGauge_.Reset();
}

BattlePhase::~BattlePhase(void)
{

}

void BattlePhase::Init(void)
{
	player_ = std::make_unique<Player>();
	player_->Init();

	enemy_ = std::make_unique<Enemy>();
	SpawnEnemy();

	//音楽
	SoundManager::GetInstance().Play(SoundManager::SRC::GAME_BGM, Sound::TIMES::LOOP);

	attackImage_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::COMMAND_ATTACK).handleId_;
	talkImage_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::COMMAND_TALK).handleId_;
	escapeImage_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::COMMAND_ESCAPE).handleId_;
}

void BattlePhase::Update(void)
{
	// 敵の更新
	if (enemy_)
	{
		enemy_->Update();
	}

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
	DrawFormatString(700,120,0xFFFFFF,"Enemy HP: %d",enemy_->GetHP());
	DrawFormatString(0,120,0xFFFFFF,"Wave Count: %d",gameScene_.GetWaveCount());

	// メッセージ枠およびアクションゲージの描画領域指定（例: 画面下部）
	int winX = 100, winY = 360, winW = 600, winH = 180;

	//アクションゲージ枠（常時表示）
	actionGauge_.Draw();

	// 敵を描画
	if (enemy_)
	{
		enemy_->Draw();
	}

	player_->Draw();

	switch (battlePhase_)
	{
	case BattlePhase::COMMAND_SELECT:
		DrawCommandSelect();
		break;
	case BattlePhase::PLAYER_TURN:
		DrawPlayerTurn();
		break;
	case BattlePhase::ENEMY_TURN:
		DrawEnemyTurn();
		break;
	case BattlePhase::BATTLE_END:
		DrawString(100, GameScene::COMMAND_MENU_Y, "バトル終了", GetColor(255, 0, 0));
		break;
	}
}

void BattlePhase::Release(void)
{
}

void BattlePhase::SpawnEnemy()
{
	Enemy::TYPE type;

	// GameSceneから現在の戦闘回数を取得
	int wave = gameScene_.GetWaveCount();

	// カエル
	if (wave == 1)
	{
		type = Enemy::TYPE::FROG;
		gameScene_.SetEnemyUsed(0);
	}
	// ボス

	else if (wave == 5)
	{
		type = Enemy::TYPE::DEVIL;
	}
	else if (wave == 6)
	{
		type = Enemy::TYPE::PIKO;
	}
	// 2～4戦目は重複なしでランダム
	else
	{
		int index;

		do
		{
			index = GetRand(5) + 1;
		} while (gameScene_.IsEnemyUsed(index));

		gameScene_.SetEnemyUsed(index);

		switch (index)
		{
		case 1:
			type = Enemy::TYPE::GOBLIN;
			break;
		case 2:
			type = Enemy::TYPE::REAPER;
			break;
		case 3:
			type = Enemy::TYPE::SLIME;
			break;
		case 4:
			type = Enemy::TYPE::DARKKNIGHT;
			break;
		case 5:
			type = Enemy::TYPE::BAT;
			break;
		case 6:
			type = Enemy::TYPE::DRAGON;
			break;
		default:
			type = Enemy::TYPE::GOBLIN;
			break;
		}
	}
	

	enemy_ = std::make_unique<Enemy>();
	enemy_->Init(type);
}

void BattlePhase::UpdateCommandSelect(void)
{
	//十字キーでコマンド選択を行う処理
	if (ins_.IsTrgDown(KEY_INPUT_RIGHT))
	{
		selectedIndex_ = (selectedIndex_ + 1) % MAX_COMMANDS;
		//カーソル音
		SoundManager::GetInstance().Play(SoundManager::SRC::CURSOR_SE, Sound::TIMES::FORCE_ONCE);
	}
	else if(ins_.IsTrgDown(KEY_INPUT_LEFT))
	{
		selectedIndex_ = (selectedIndex_ - 1 + MAX_COMMANDS) % MAX_COMMANDS;
		//カーソル音
		SoundManager::GetInstance().Play(SoundManager::SRC::CURSOR_SE, Sound::TIMES::FORCE_ONCE);
	}

	if (ins_.IsTrgDown(KEY_INPUT_RETURN))
	{
		// キーが押されたときだけ各コマンド処理を呼び出す
		if (selectedIndex_ == 0)
		{
			commandType_ = ATTACK;
			battlePhase_ = PLAYER_TURN;
			actionGauge_.Start(enemy_->GetGaugeGoodWidth(), enemy_->GetGaugeGreatWidth(), enemy_->GetGaugePerfectWidth());
		}
		else if (selectedIndex_ == 1)
		{
			commandType_ = TALK;
			battlePhase_ = PLAYER_TURN;
		}
		else if (selectedIndex_ == 2)
		{
			commandType_ = RUN;
			battlePhase_ = PLAYER_TURN;
		}

		//決定音
		SoundManager::GetInstance().Play(SoundManager::SRC::SELECT_SE, Sound::TIMES::FORCE_ONCE);

		selectedIndex_ = 0;
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
	// バーがまだ動いている最中
	if (actionGauge_.IsMoving())
	{
		float deltaTime = 1.0f / 60.0f;
		actionGauge_.Update(deltaTime);

		if (ins_.IsTrgDown(KEY_INPUT_RETURN))
		{
			// ゲージを止める
			GaugeState state = actionGauge_.PressButton();

			// 防御率を取得
			float rate = ActionGauge::GetResultRate(state);

			// 防御処理
			player_->Defense(rate);

			// 敵の攻撃力を使ってダメージ計算
			player_->Damage(enemy_->GetATK());

			// 敵からのダメージ音
			SoundManager::GetInstance().Play(SoundManager::SRC::CRITICALATTACK_SE, Sound::TIMES::FORCE_ONCE);
		}
	}
	// バーが止まった後
	else
	{
		if (ins_.IsTrgDown(KEY_INPUT_RETURN))
		{
			battleTurnCnt_++;

			actionGauge_.Start(enemy_->GetGaugeGoodWidth(), enemy_->GetGaugeGreatWidth(), enemy_->GetGaugePerfectWidth());

			battlePhase_ = COMMAND_SELECT;

			// 決定音
			SoundManager::GetInstance().Play(SoundManager::SRC::SELECT_SE, Sound::TIMES::FORCE_ONCE);
		}
	}
}

void BattlePhase::UpdateBattleEnd(void)
{
	if (ins_.IsTrgDown(KEY_INPUT_RETURN))
	{
		gameScene_.ChangePhase(GameScene::QUEST_PHASE::PHASE_STORY);
		SoundManager::GetInstance().Stop(SoundManager::SRC::GAME_BGM);

		// 決定音
		SoundManager::GetInstance().Play(SoundManager::SRC::SELECT_SE, Sound::TIMES::FORCE_ONCE);
	}
}

void BattlePhase::DrawCommandSelect(void)
{
	//コマンド選択の描画処理
	DrawGraph(GameScene::COMMAND_ATTACK_X, GameScene::COMMAND_MENU_Y, attackImage_, true);
	DrawGraph(GameScene::COMMAND_TALK_X, GameScene::COMMAND_MENU_Y, talkImage_, true);
	DrawGraph(GameScene::COMMAND_ESCAPE_X, GameScene::COMMAND_MENU_Y, escapeImage_, true);
	DrawFormatString(ARROW_POS_X + ARROW_OFFSET * selectedIndex_, GameScene::COMMAND_MENU_Y, 0xffffff, "→");
}

void BattlePhase::DrawPlayerTurn(void)
{
	//プレイヤーのターンの描画処理
	DrawString(100, 130, "プレイヤーターン",0xFFFFFF);
	DrawFormatString(100, GameScene::COMMAND_MENU_Y, 0xFFFFFF, "CommandType: %d", commandType_);
}

void BattlePhase::DrawEnemyTurn(void)
{
	//敵のターンの描画処理
	DrawString(100, 130, "敵のターン", 0xFFFFFF);
}

void BattlePhase::ProsesSelectAttack(void)
{
	if (enemy_->IsDead())
	{
		//攻撃撃破結果を GameScene に記録！
		gameScene_.RecordBattleResult(GameScene::BATTLE_RESULT::RESULT_ATTACK);
		//戦闘終了画面へ移行
		battlePhase_ = BATTLE_END;
		return;
	}

	// バーがまだ動いている最中の処理
	if (actionGauge_.IsMoving())
	{
		float deltaTime = 1.0f / 60.0f;
		actionGauge_.Update(deltaTime);

		if (ins_.IsTrgDown(KEY_INPUT_RETURN))
		{
			// ボタンが押されたらバーがその場に止まる
			GaugeState state = actionGauge_.PressButton();

			float rate = ActionGauge::GetResultRate(state);

			int power = player_->Attack(rate);

			// 敵にダメージを与える
			enemy_->Damage(power);

			// ダメージを与えられたときだけ攻撃音を鳴らす
			if (power > 0)
			{
				SoundManager::GetInstance().Play(SoundManager::SRC::ATTACK_SE,Sound::TIMES::FORCE_ONCE);
			}
		}
	}
	else
	{
		if (ins_.IsTrgDown(KEY_INPUT_RETURN))
		{
			 //敵を倒していた場合
			if (enemy_->IsDead())
			{
				//攻撃撃破結果を GameScene に記録！
				gameScene_.RecordBattleResult(GameScene::BATTLE_RESULT::RESULT_ATTACK);

				//決定音
				SoundManager::GetInstance().Play(SoundManager::SRC::SELECT_SE, Sound::TIMES::FORCE_ONCE);

				//戦闘終了画面へ移行
				battlePhase_ = BATTLE_END;
				return;
			}

        battleTurnCnt_++;

		actionGauge_.Start(enemy_->GetGaugeGoodWidth(), enemy_->GetGaugeGreatWidth(), enemy_->GetGaugePerfectWidth());

        battlePhase_ = ENEMY_TURN;
    }
	}
}

void BattlePhase::ProsesSelectTalk(void)
{
	if (ins_.IsTrgDown(KEY_INPUT_RETURN))
	{
		if (enemy_->GetHP() == enemy_->GetMaxHP())
		{
			talkSuccessCount_++;
		}

		if (talkSuccessCount_ >= 1)
		{
			currentMessage_ = "相手は戦意を喪失した！";

			//説得完了で戦績記録 ＆ 戦闘終了
			gameScene_.RecordBattleResult(GameScene::BATTLE_RESULT::RESULT_TALK);
			battlePhase_ = BATTLE_END;
		}
		else
		{
			currentMessage_ = "相手は少し話に耳を傾けている…";

			battleTurnCnt_++;
			//battlePhase_ = COMMAND_SELECT;
			actionGauge_.Start(enemy_->GetGaugeGoodWidth(), enemy_->GetGaugeGreatWidth(), enemy_->GetGaugePerfectWidth());
			battlePhase_ = ENEMY_TURN;
		}

		// 決定音
		SoundManager::GetInstance().Play(SoundManager::SRC::SELECT_SE, Sound::TIMES::FORCE_ONCE);
	}
}

void BattlePhase::ProsesSelectRun(void)
{
	if (ins_.IsTrgDown(KEY_INPUT_RETURN))
	{
		// ターンが経過するごとに逃げられる確率が変わる
		int runRate = RUN_SUCCESS_RATE + (battleTurnCnt_ * 5);

		// 逃走成功判定（成功時）
		if (runRate <= GetRand(99))
		{
			//逃走結果を GameScene に記録！
			gameScene_.RecordBattleResult(GameScene::BATTLE_RESULT::RESULT_ESCAPE);

			battlePhase_ = BATTLE_END;
		}
		// 失敗時
		else
		{
			actionGauge_.Start(enemy_->GetGaugeGoodWidth(), enemy_->GetGaugeGreatWidth(), enemy_->GetGaugePerfectWidth());
			battlePhase_ = ENEMY_TURN;
		}

		// 決定音
		SoundManager::GetInstance().Play(SoundManager::SRC::SELECT_SE, Sound::TIMES::FORCE_ONCE);
	}
}
