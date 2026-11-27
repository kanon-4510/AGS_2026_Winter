#include <DxLib.h>
#include <string>
#include <sstream>
#include "../../Manager/InputManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Manager/SceneManager.h"
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

	//バトルBGM再生
	SoundManager::GetInstance().Play(SoundManager::SRC::GAME_BGM, Sound::TIMES::LOOP);

	battleImage_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::GAME_SCENE).handleId_;

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

	// バトルフェーズの更新
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
	DrawFormatString(0,120,0xFFFFFF,"Wave Count: %d",gameScene_.GetWaveCount());;

	DrawFormatString(0, 200, 0xFFFFFF, "combo:%d", comboCount_);

	DrawGraph(0, 0, battleImage_, true);

	// メッセージボックスの描画
	int winX = 100, winY = 360, winW = 600, winH = 180;

	//DrawMultilineText(275, 700, currentMessage_, GetColor(255, 255, 255), 24);
	DrawString(275, 700, currentMessage_.c_str(), GetColor(255, 255, 255));

	//アクションゲージ枠（常時表示）
	actionGauge_.Draw();

	//敵の描画
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
	SoundManager::GetInstance().Stop(SoundManager::SRC::GAME_BGM);
}

void BattlePhase::SpawnEnemy()
{
	Enemy::TYPE type;

	// 現在のWave数
	int wave = gameScene_.GetWaveCount();

	// ストーリーモード
	if (gameScene_.IsStory())
	{
		switch (wave)
		{
		case 1:
			type = Enemy::TYPE::FROG;
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
			type = Enemy::TYPE::DEVIL;
			break;

		case 6:
			type = Enemy::TYPE::PIKO;
			break;

		default:
			type = Enemy::TYPE::FROG;
			break;
		}
	}
	// バトルモード
	else
	{
		// Wave 1：カエル
		if (wave == 1)
		{
			type = Enemy::TYPE::FROG;
			gameScene_.SetEnemyUsed(0);
		}
		// Wave 5：ボス
		else if (wave == 5)
		{
			type = Enemy::TYPE::DEVIL;
		}
		// Wave 2～4：重複なしでランダム
		else
		{
			int index;

			do
			{
				index = GetRand(4) + 1;
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

			default:
				type = Enemy::TYPE::GOBLIN;
				break;
			}
		}
	}

	enemy_ = std::make_unique<Enemy>();
	enemy_->Init(type);
}

void BattlePhase::UpdateCommandSelect(void)
{
	// コマンド選択の更新
	if (ins_.IsTrgDown(KEY_INPUT_RIGHT))
	{
		selectedIndex_ = (selectedIndex_ + 1) % MAX_COMMANDS;
		// カーソル移動
		SoundManager::GetInstance().Play(SoundManager::SRC::CURSOR_SE, Sound::TIMES::FORCE_ONCE);
	}
	else if(ins_.IsTrgDown(KEY_INPUT_LEFT))
	{
		selectedIndex_ = (selectedIndex_ - 1 + MAX_COMMANDS) % MAX_COMMANDS;
		// カーソル移動
		SoundManager::GetInstance().Play(SoundManager::SRC::CURSOR_SE, Sound::TIMES::FORCE_ONCE);
	}

	if (ins_.IsTrgDown(KEY_INPUT_RETURN))
	{
		// コマンド選択の決定
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

		// 決定音再生
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
	// 敵のターンの更新
	if (actionGauge_.IsMoving())
	{
		float deltaTime = 1.0f / 60.0f;
		actionGauge_.Update(deltaTime);

		if (ins_.IsTrgDown(KEY_INPUT_RETURN))
		{
			// ボタンが押された
			GaugeState state = actionGauge_.PressButton();

			// 結果を取得
			float rate = ActionGauge::GetResultRate(state);

			// 防御
			player_->Defense(rate);

			// プレイヤーのHPを取得
			int beforeHP = player_->GetHP();

			// 敵の攻撃を受ける
			player_->Damage(enemy_->GetATK());

			// プレイヤーのHPを取得
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
		// ボタンが押されなかった場合
		else if (!actionGauge_.IsMoving())
		{
			// 防御を無効化
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
	// ボタンが押されなかった場合
	else
	{
		if (ins_.IsTrgDown(KEY_INPUT_RETURN))
		{
			battleTurnCnt_++;

			actionGauge_.Reset();

			battlePhase_ = COMMAND_SELECT;
		}
	}
}

void BattlePhase::UpdateBattleEnd(void)
{
	comboCount_ = 0;//コンボ回数をリセット
	if (ins_.IsTrgDown(KEY_INPUT_RETURN))
	{
		//if(ストーリーモードかどうか)
		if (gameScene_.IsStory())
		{
			gameScene_.ChangePhase(GameScene::QUEST_PHASE::PHASE_STORY);

			// 決定音再生
			SoundManager::GetInstance().Play(SoundManager::SRC::SELECT_SE, Sound::TIMES::FORCE_ONCE);
		}
		else
		{
			// Wave 5の通常敵
			if (gameScene_.GetWaveCount() >= 5)
			{
				SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::CLEAR);
			}
			else
			{
				gameScene_.NextWave();

				gameScene_.ChangePhase(GameScene::QUEST_PHASE::PHASE_BATTLE);
			}

			SoundManager::GetInstance().Play(SoundManager::SRC::SELECT_SE,Sound::TIMES::FORCE_ONCE);
		}
	}
}

void BattlePhase::DrawCommandSelect(void)
{
	// コマンド選択の描画
	DrawGraph(ARROW_POS_X + ARROW_OFFSET * selectedIndex_, ARROW_POS_Y, cursorImage_, true);
	DrawGraph(GameScene::COMMAND_ATTACK_X, GameScene::COMMAND_MENU_Y, attackImage_, true);
	DrawGraph(GameScene::COMMAND_TALK_X, GameScene::COMMAND_MENU_Y, talkImage_, true);
	DrawGraph(GameScene::COMMAND_ESCAPE_X, GameScene::COMMAND_MENU_Y, escapeImage_, true);
}

void BattlePhase::DrawPlayerTurn(void)
{
	// プレイヤーのターンの描画
	DrawString(100, 130, "プレイヤーのターン",0xFFFFFF);
	DrawFormatString(100, GameScene::COMMAND_MENU_Y, 0xFFFFFF, "CommandType: %d", commandType_);
}

void BattlePhase::DrawEnemyTurn(void)
{
	// 敵のターンの描画
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
	// 敵が倒された場合は戦闘終了
	if (enemy_->IsDead())
	{
		gameScene_.RecordBattleResult(GameScene::BATTLE_RESULT::RESULT_ATTACK);
		battlePhase_ = BATTLE_END;

		SoundManager::GetInstance().Stop(SoundManager::SRC::GAME_BGM);
		return;
	}

	// 攻撃のステップに応じた処理
	switch (attackStep_)
	{
	case AttackStep::MOVING_GAUGE:
		ProcessAttackGaugeInput();
		break;

	case AttackStep::WAIT_FOR_MESSAGE:
		ProcessAttackMessageWait();
		break;
	}
}

void BattlePhase::ProsesSelectTalk(void)
{
	// 話すコマンドのステップに応じた処理
	switch (talkStep_)
	{
	case TalkStep::SELECT_TALK:
		ProcessTalkExecution();
		break;

	case TalkStep::WAIT_FOR_MESSAGE:
		ProcessTalkMessageWait();
		break;
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

			SoundManager::GetInstance().Stop(SoundManager::SRC::GAME_BGM);
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

// バーが動いている間の処理
void BattlePhase::ProcessAttackGaugeInput(void)
{

	if (actionGauge_.IsMoving())
	{
		if (ins_.IsTrgDown(KEY_INPUT_RETURN))
		{
			// ボタンが押された場合
			++comboCount_;//連続成功回数を増やす
			GaugeState state = actionGauge_.PressButton();
			float rate = ActionGauge::GetResultRate(state);
			int power = player_->Attack(rate);
			int attack = power * GetComboMultiplier();//コンボ倍率を適用
			enemy_->Damage(attack);

			if (power > 0)
			{
				SoundManager::GetInstance().Play(SoundManager::SRC::ATTACK_SE, Sound::TIMES::FORCE_ONCE);
				DamageTalk(); // ダメージに応じたセリフをセット
			}
			else
			{
				SoundManager::GetInstance().Play(SoundManager::SRC::MISS_SE, Sound::TIMES::FORCE_ONCE);
				comboCount_ = 0; //コンボ回数をリセット
				currentMessage_ = "攻撃を外した！";
			}

			// 次のステップ（メッセージ確認待ち）へ移行
			attackStep_ = AttackStep::WAIT_FOR_MESSAGE;
		}
		else
		{
			// ボタンが押されていない間はバーを進める
			float deltaTime = 1.0f / 60.0f;
			actionGauge_.Update(deltaTime);
			
			//Updateした結果、もしバーが端まで到達して自動で停止していた場合！
			if (!actionGauge_.IsMoving())
			{
				// 自動的に「ミス（未操作）」としてダメージ0の処理を行う
				int power = 0;
				enemy_->Damage(power);
				comboCount_ = 0; //コンボ回数をリセット

				SoundManager::GetInstance().Play(SoundManager::SRC::MISS_SE, Sound::TIMES::FORCE_ONCE);

				// 自動的に次のステップ（メッセージ確認待ち）へ進む！
				attackStep_ = AttackStep::WAIT_FOR_MESSAGE;
			}
		}
	}
}

//バーが動いている間の処理
void BattlePhase::ProcessAttackMessageWait(void)
{
	if (ins_.IsTrgDown(KEY_INPUT_RETURN))
	{
		SoundManager::GetInstance().Play(SoundManager::SRC::SELECT_SE, Sound::TIMES::FORCE_ONCE);

		// 撃破していた場合は戦闘終了へ
		if (enemy_->IsDead())
		{
			gameScene_.RecordBattleResult(GameScene::BATTLE_RESULT::RESULT_ATTACK);
			battlePhase_ = BATTLE_END;
			return;
		}

		// 次回のためにステップを最初（バー移動）にリセット
		attackStep_ = AttackStep::MOVING_GAUGE;

		// ターンを回して敵のターンへ移行
		battleTurnCnt_++;
		actionGauge_.Start(enemy_->GetGaugeGoodWidth(), enemy_->GetGaugeGreatWidth(), enemy_->GetGaugePerfectWidth());
		battlePhase_ = ENEMY_TURN;
	}
}

// 話すコマンドの処理
void BattlePhase::ProcessTalkExecution(void)
{
	//敵のHPが満タンの状態で話すと説得成功回数が増える
	if (enemy_->GetHP() == enemy_->GetMaxHP())
	{
		talkSuccessCount_++;

		//4回に達した場合は説得成功！
		if (talkSuccessCount_ >= 4)
		{
			currentMessage_ = gameScene_.getMessage(enemy_->GetName(), "ON_TALK", talkSuccessCount_ - 1);
			gameScene_.RecordBattleResult(GameScene::BATTLE_RESULT::RESULT_TALK);
			SoundManager::GetInstance().Stop(SoundManager::SRC::GAME_BGM);
			battlePhase_ = BATTLE_END;
			return;
		}
		else
		{
			//CSVから該当回数のセリフを取得して画面にセット
			currentMessage_ = gameScene_.getMessage(enemy_->GetName(), "ON_TALK", talkSuccessCount_ - 1);
		}
	}
	else
	{
		//攻撃してHPが減っている状態のとき
		currentMessage_ = "相手は警戒して話を聞いてくれない……！";
	}

	// 次のステップ（セリフ確認待ち）へ移行
	talkStep_ = TalkStep::WAIT_FOR_MESSAGE;
}

// 話すコマンドのセリフ確認後の処理
void BattlePhase::ProcessTalkMessageWait(void)
{
	if (ins_.IsTrgDown(KEY_INPUT_RETURN))
	{
		SoundManager::GetInstance().Play(SoundManager::SRC::SELECT_SE, Sound::TIMES::FORCE_ONCE);

		//次回のためにステップを最初に戻す
		talkStep_ = TalkStep::SELECT_TALK;

		// ターンを進めて敵のターンへ移行
		battleTurnCnt_++;
		actionGauge_.Start(enemy_->GetGaugeGoodWidth(), enemy_->GetGaugeGreatWidth(), enemy_->GetGaugePerfectWidth());
		battlePhase_ = ENEMY_TURN;
	}
}

void BattlePhase::DamageTalk(void)
{
	damageCount_++;

	if (damageCount_ == 1)
	{
		currentMessage_ = gameScene_.getMessage(enemy_->GetName(), "ON_DAMAGE", 0);
	}

	if (enemy_->GetHP() <= enemy_->GetMaxHP() / 2 && !isHalfHpTalkDone_)
	{
		currentMessage_ = gameScene_.getMessage(enemy_->GetName(), "ON_DAMAGE", 1);
		isHalfHpTalkDone_ = true;
	}
}

float BattlePhase::GetComboMultiplier() const
{
	if (comboCount_ <= 1) {
		return 1.0f;
	}

	float bonus = 1.0f + COMBO_BONUS * static_cast<float>(comboCount_ - 1);
	return (bonus < MAX_COMBO) ? bonus : MAX_COMBO;
}
