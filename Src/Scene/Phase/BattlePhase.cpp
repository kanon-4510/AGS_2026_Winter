#include <DxLib.h>
#include <string>
#include <sstream>
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

	if (enemy_) {
		currentMessage_ = gameScene_.getMessage(enemy_->GetName(), "START_BATTLE");
	}

	//音楽
	SoundManager::GetInstance().Play(SoundManager::SRC::GAME_BGM, Sound::TIMES::LOOP);

	cursorImage_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CURSOR).handleId_;
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

	// ダメージフラッシュの更新
	if (damageFlashTimer_ > 0.0f)
	{
		damageFlashTimer_ -= 1.0f / 60.0f;

		if (damageFlashTimer_ < 0.0f)
		{
			damageFlashTimer_ = 0.0f;
		}
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
	//DrawFormatString(0, 150, 0xFFFFFF, "Talk Success Count: %s", currentMessage_.c_str());

	// メッセージ枠およびアクションゲージの描画領域指定（例: 画面下部）
	int winX = 100, winY = 360, winW = 600, winH = 180;

	//DrawMultilineText(275, 700, currentMessage_, GetColor(255, 255, 255), 24);
	DrawString(275, 700, currentMessage_.c_str(), GetColor(255, 255, 255));

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

	// 被ダメージ時の赤フラッシュ
	if (damageFlashTimer_ > 0.0f)
	{
		int screenWidth;
		int screenHeight;

		GetDrawScreenSize(&screenWidth, &screenHeight);

		float rate = damageFlashTimer_ / DAMAGE_FLASH_DURATION;

		int alpha =static_cast<int>(DAMAGE_FLASH_MAX_ALPHA * rate);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

		DrawBox(0,0,screenWidth,screenHeight,GetColor(255, 0, 0),TRUE);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
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

			// ダメージを受ける前のHP
			int beforeHP = player_->GetHP();

			// 敵の攻撃力を使ってダメージ計算
			player_->Damage(enemy_->GetATK());

			// ダメージを受けた後のHP
			int afterHP = player_->GetHP();

			int damage = beforeHP - afterHP;

			if (damage > 0)
			{
				damageFlashTimer_ = DAMAGE_FLASH_DURATION;

				SoundManager::GetInstance().Play(SoundManager::SRC::CRITICALATTACK_SE,Sound::TIMES::FORCE_ONCE);
			}
			else
			{
				SoundManager::GetInstance().Play(SoundManager::SRC::PARRY_SE,Sound::TIMES::FORCE_ONCE);
			}
		}
		// バーが最後まで動いてもEnterを押さなかった場合
		else if (!actionGauge_.IsMoving())
		{
			// 防御失敗として扱う
			player_->Defense(0.0f);

			int beforeHP = player_->GetHP();

			player_->Damage(enemy_->GetATK());

			int afterHP = player_->GetHP();

			if (beforeHP > afterHP)
			{
				damageFlashTimer_ = DAMAGE_FLASH_DURATION;
			}

			SoundManager::GetInstance().Play(SoundManager::SRC::CRITICALATTACK_SE,Sound::TIMES::FORCE_ONCE);
		}
	}
	// バーが止まった後
	else
	{
		if (ins_.IsTrgDown(KEY_INPUT_RETURN))
		{
			battleTurnCnt_++;

			actionGauge_.Reset();

			battlePhase_ = COMMAND_SELECT;

			SoundManager::GetInstance().Play(SoundManager::SRC::SELECT_SE,Sound::TIMES::FORCE_ONCE);
		}
	}
}

void BattlePhase::UpdateBattleEnd(void)
{
	if (ins_.IsTrgDown(KEY_INPUT_RETURN))
	{
		//if(ストーリーモードの時)
		if (gameScene_.IsStory())
		{
			gameScene_.ChangePhase(GameScene::QUEST_PHASE::PHASE_STORY);
			SoundManager::GetInstance().Stop(SoundManager::SRC::GAME_BGM);
			
			// 決定音
			SoundManager::GetInstance().Play(SoundManager::SRC::SELECT_SE, Sound::TIMES::FORCE_ONCE);
		}
		else
		{
			gameScene_.NextWave();
			gameScene_.ChangePhase(GameScene::QUEST_PHASE::PHASE_BATTLE);
		}
		
	}
}

void BattlePhase::DrawCommandSelect(void)
{
	//コマンド選択の描画処理
	DrawGraph(ARROW_POS_X + ARROW_OFFSET * selectedIndex_, ARROW_POS_Y, cursorImage_, true);
	DrawGraph(GameScene::COMMAND_ATTACK_X, GameScene::COMMAND_MENU_Y, attackImage_, true);
	DrawGraph(GameScene::COMMAND_TALK_X, GameScene::COMMAND_MENU_Y, talkImage_, true);
	DrawGraph(GameScene::COMMAND_ESCAPE_X, GameScene::COMMAND_MENU_Y, escapeImage_, true);
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

void BattlePhase::DrawMultilineText(int x, int y, const std::string& text, unsigned int color, int lineHeight)
{
	std::stringstream ss(text);
	std::string line;
	int currentY = y;

	while (std::getline(ss, line, '\n'))
	{
		DrawString(x, currentY, line.c_str(), color);
		currentY += lineHeight;
	}
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
			else
			{
				SoundManager::GetInstance().Play(SoundManager::SRC::MISS_SE, Sound::TIMES::FORCE_ONCE);
			}
		}
		else
		{
			//ボタンが押されていないフレームだけ、バーを進める
			float deltaTime = 1.0f / 60.0f;
			actionGauge_.Update(deltaTime);
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
		// 敵のHPが満タンの時だけ、説得（トーク）が進む
		if (enemy_->GetHP() == enemy_->GetMaxHP())
		{
			talkSuccessCount_++;

			// "ON_TALK" のセリフリストをGameSceneから取得
			auto talkList = gameScene_.getMessage(enemy_->GetName(), "ON_TALK");

			// 4回に達した、またはCSVのセリフをすべて喋り終えた場合
			if (talkSuccessCount_ >= 4 || (!talkList.empty() && talkSuccessCount_ >= static_cast<int>(talkList.size())))
			{
				currentMessage_ = "話を聞いてくれるようだ";

				//説得完了で戦績記録 ＆ 戦闘終了
				gameScene_.RecordBattleResult(GameScene::BATTLE_RESULT::RESULT_TALK);
				battlePhase_ = BATTLE_END;
			}
			else
			{
				// 回数に応じた行をリストから取得（1回目なら0番目、2回目なら1番目…）
				int index = talkSuccessCount_ - 1;
				if (!talkList.empty() && index < static_cast<int>(talkList.size()))
				{
					currentMessage_ = talkList[index];
				}
				else
				{
					// 万が一CSVにデータが足りない場合のフォールバック
					currentMessage_ = "相手は少し話に耳を傾けている…";
				}

				battleTurnCnt_++;
				actionGauge_.Start(enemy_->GetGaugeGoodWidth(), enemy_->GetGaugeGreatWidth(), enemy_->GetGaugePerfectWidth());
				battlePhase_ = ENEMY_TURN;
			}
		}
		else
		{
			//攻撃してHPが減っている状態で「はなす」を選んだ場合
			currentMessage_ = "相手は警戒して話を聞いてくれない……！";

			battleTurnCnt_++;
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
			currentMessage_ = gameScene_.getMessage(enemy_->GetName(), "ON_ESCAPE");

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

void BattlePhase::DamageTalk(void)
{
	damageCount_++;

	// GameScene経由で "ON_DAMAGE" のセリフリストをすべて取得
	// （※ GameScene側に getMessageList が用意されている前提）
	auto dmgList = gameScene_.getMessage(enemy_->GetName(), "ON_DAMAGE");

	// 1. 初めてダメージを受けたとき（1回目のダメージ）
	if (damageCount_ == 1 && !dmgList.empty())
	{
		currentMessage_ = dmgList[0];
	}

	// 2. HPが半分以下になった瞬間（かつ、まだ再生していない場合）
	if (enemy_->GetHP() <= enemy_->GetMaxHP() / 2 && !isHalfHpTalkDone_ && dmgList.size() >= 2)
	{
		currentMessage_ = dmgList[1];
		isHalfHpTalkDone_ = true; // 2回再生されないようにロック
	}
}
