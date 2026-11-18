#include <DxLib.h>
#include <string>
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

<<<<<<< Updated upstream
	//âπäy
=======
	if (enemy_) {
		currentMessage_ = gameScene_.getMessage(enemy_->GetName(), "START_BATTLE");
	}

	//Èü≥Ê•Ω
>>>>>>> Stashed changes
	SoundManager::GetInstance().Play(SoundManager::SRC::GAME_BGM, Sound::TIMES::LOOP);

	cursorImage_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CURSOR).handleId_;
	attackImage_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::COMMAND_ATTACK).handleId_;
	talkImage_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::COMMAND_TALK).handleId_;
	escapeImage_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::COMMAND_ESCAPE).handleId_;
}

void BattlePhase::Update(void)
{
<<<<<<< Updated upstream
	// ìGÇÃçXêV
=======
	// Êïµ„ÅÆÊõ¥Êñ∞
>>>>>>> Stashed changes
	if (enemy_)
	{
		enemy_->Update();
	}

<<<<<<< Updated upstream
	// É_ÉÅÅ[ÉWÉtÉâÉbÉVÉÖÇÃçXêV
=======
	// „ÉÄ„É°„Éº„Ç∏„Éï„É©„ÉÅEÇ∑„É•„ÅÆÊõ¥Êñ∞
>>>>>>> Stashed changes
	if (damageFlashTimer_ > 0.0f)
	{
		damageFlashTimer_ -= 1.0f / 60.0f;

		if (damageFlashTimer_ < 0.0f)
		{
			damageFlashTimer_ = 0.0f;
		}
	}

<<<<<<< Updated upstream
	//ÉtÉFÅ[ÉYÇ≤Ç∆ÇÃçXêVèàóùÇåƒÇ—èoÇ∑
=======
	//„Éï„Çß„Éº„Ç∫„Åî„Å®„ÅÆÊõ¥Êñ∞Âá¶ÁêÅEÇíÂëº„Å≥Âá∫„ÅÅE
>>>>>>> Stashed changes
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
	DrawFormatString(0, 150, 0xFFFFFF, "Talk Success Count: %s", currentMessage_.c_str());

<<<<<<< Updated upstream
	// ÉÅÉbÉZÅ[ÉWògÇ®ÇÊÇ—ÉAÉNÉVÉáÉìÉQÅ[ÉWÇÃï`âÊóÃàÊéwíËÅió·: âÊñ â∫ïîÅj
	int winX = 100, winY = 360, winW = 600, winH = 180;

	//ÉAÉNÉVÉáÉìÉQÅ[ÉWògÅièÌéûï\é¶Åj
	actionGauge_.Draw();

	// ìGÇï`âÊ
=======
	// „É°„ÉÅEÇª„Éº„Ç∏Êû†„Åä„Çà„Å≥„Ç¢„ÇØ„Ç∑„Éß„É≥„Ç≤„Éº„Ç∏„ÅÆÊèèÁîªÈ†òÂüüÊåÅEÆöÔºà‰æÅE ÁîªÈù¢‰∏ãÈÉ®ÅEÅE
	int winX = 100, winY = 360, winW = 600, winH = 180;

	//DrawMultilineText(275, 700, currentMessage_, GetColor(255, 255, 255), 24);
	DrawString(275, 700, currentMessage_.c_str(), GetColor(255, 255, 255));

	//„Ç¢„ÇØ„Ç∑„Éß„É≥„Ç≤„Éº„Ç∏Êû†ÅEàÂ∏∏ÊôÇË°®Á§∫ÅEÅE
	actionGauge_.Draw();

	// Êïµ„ÇíÊèèÁîª
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
		DrawString(100, GameScene::COMMAND_MENU_Y, "ÉoÉgÉãèIóπ", GetColor(255, 0, 0));
		break;
	}

	// îÌÉ_ÉÅÅ[ÉWéûÇÃê‘ÉtÉâÉbÉVÉÖ
=======
		DrawString(100, GameScene::COMMAND_MENU_Y, "„Éê„Éà„É´ÁµÇ‰∫ÅE, GetColor(255, 0, 0));
		break;
	}

	// Ë¢´„ÉÄ„É°„Éº„Ç∏ÊôÇ„ÅEËµ§„Éï„É©„ÉÅEÇ∑„É•
>>>>>>> Stashed changes
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

<<<<<<< Updated upstream
	// GameSceneÇ©ÇÁåªç›ÇÃêÌì¨âÒêîÇéÊìæ
=======
	// GameScene„Åã„ÇâÁèæÂú®„ÅÆÊà¶ÈóòÂõûÊï∞„ÇíÂèñÂæÅE
>>>>>>> Stashed changes
	int wave = gameScene_.GetWaveCount();
	// „Ç´„Ç®„É´
	if (wave == 1)
	{
<<<<<<< Updated upstream
		// ÉJÉGÉã
		if (wave == 1)
=======
		type = Enemy::TYPE::FROG;
		gameScene_.SetEnemyUsed(0);
	}
	// „Éú„Çπ

	else if (wave == 5)
	{
		type = Enemy::TYPE::DEVIL;
	}
	else if (wave == 6)
	{
		type = Enemy::TYPE::PIKO;
	}
	// 2ÅEÅEÊà¶ÁõÆ„ÅØÈáçË§ÅEÅ™„Åó„Åß„É©„É≥„ÉÄ„É†
	else
	{
		int index;

		while (true)
>>>>>>> Stashed changes
		{

		} do if (gameScene_.IsStory())
		{
			// ÅEΩJÅEΩGÅEΩÅEΩ
			if (wave == 1)
			{
				type = Enemy::TYPE::FROG;
			}
			else if (wave == 2)
			{
				type = Enemy::TYPE::REAPER;
			}
			else if (wave == 3)
			{
				type = Enemy::TYPE::SLIME;
			}
			else if (wave == 4)
			{
				type = Enemy::TYPE::DARKKNIGHT;
			}
			else if (wave == 5)
			{
				type = Enemy::TYPE::DEVIL;
			}
			else if (wave == 6)
			{
				type = Enemy::TYPE::PIKO;
			}
		}
	}
	else
	{
		// ÉJÉGÉã
		if (wave == 1)
		{
			type = Enemy::TYPE::FROG;
			gameScene_.SetEnemyUsed(0);
		}
		// É{ÉX

		else if (wave == 5)
		{
			type = Enemy::TYPE::DEVIL;
		}

		// 2Å`4êÌñ⁄ÇÕèdï°Ç»ÇµÇ≈ÉâÉìÉ_ÉÄ
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
	}

	enemy_ = std::make_unique<Enemy>();
	enemy_->Init(type);
}

void BattlePhase::UpdateCommandSelect(void)
{
<<<<<<< Updated upstream
	//è\éöÉLÅ[Ç≈ÉRÉ}ÉìÉhëIëÇçsÇ§èàóù
	if (ins_.IsTrgDown(KEY_INPUT_RIGHT))
	{
		selectedIndex_ = (selectedIndex_ + 1) % MAX_COMMANDS;
		//ÉJÅ[É\Éãâπ
=======
	//ÂçÅÂ≠ó„Ç≠„Éº„Åß„Ç≥„Éû„É≥„ÉâÈÅ∏Êäû„ÇíË°å„ÅÜÂá¶ÁêÅE
	if (ins_.IsTrgDown(KEY_INPUT_RIGHT))
	{
		selectedIndex_ = (selectedIndex_ + 1) % MAX_COMMANDS;
		//„Ç´„Éº„ÇΩ„É´Èü≥
>>>>>>> Stashed changes
		SoundManager::GetInstance().Play(SoundManager::SRC::CURSOR_SE, Sound::TIMES::FORCE_ONCE);
	}
	else if(ins_.IsTrgDown(KEY_INPUT_LEFT))
	{
		selectedIndex_ = (selectedIndex_ - 1 + MAX_COMMANDS) % MAX_COMMANDS;
<<<<<<< Updated upstream
		//ÉJÅ[É\Éãâπ
=======
		//„Ç´„Éº„ÇΩ„É´Èü≥
>>>>>>> Stashed changes
		SoundManager::GetInstance().Play(SoundManager::SRC::CURSOR_SE, Sound::TIMES::FORCE_ONCE);
	}

	if (ins_.IsTrgDown(KEY_INPUT_RETURN))
	{
<<<<<<< Updated upstream
		// ÉLÅ[Ç™âüÇ≥ÇÍÇΩÇ∆Ç´ÇæÇØäeÉRÉ}ÉìÉhèàóùÇåƒÇ—èoÇ∑
=======
		// „Ç≠„Éº„ÅåÊäº„Åï„Çå„Åü„Å®„Åç„Å†„ÅëÂêÑ„Ç≥„Éû„É≥„ÉâÂÅEÁêÅEÇíÂëº„Å≥Âá∫„ÅÅE
>>>>>>> Stashed changes
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

<<<<<<< Updated upstream
		//åàíËâπ
=======
		//Ê±∫ÂÆöÈü≥
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
	// ÉoÅ[Ç™Ç‹ÇæìÆÇ¢ÇƒÇ¢ÇÈç≈íÜ
=======
	// „Éê„ÅE„Åå„Åæ„Å†Âãï„ÅÑ„Å¶„ÅÅEÇãÊúÄ‰∏≠
>>>>>>> Stashed changes
	if (actionGauge_.IsMoving())
	{
		float deltaTime = 1.0f / 60.0f;
		actionGauge_.Update(deltaTime);

		if (ins_.IsTrgDown(KEY_INPUT_RETURN))
		{
<<<<<<< Updated upstream
			// ÉQÅ[ÉWÇé~ÇﬂÇÈ
			GaugeState state = actionGauge_.PressButton();

			// ñhå‰ó¶ÇéÊìæ
			float rate = ActionGauge::GetResultRate(state);

			// ñhå‰èàóù
			player_->Defense(rate);

			// É_ÉÅÅ[ÉWÇéÛÇØÇÈëOÇÃHP
			int beforeHP = player_->GetHP();

			// ìGÇÃçUåÇóÕÇégÇ¡ÇƒÉ_ÉÅÅ[ÉWåvéZ
			player_->Damage(enemy_->GetATK());

			// É_ÉÅÅ[ÉWÇéÛÇØÇΩå„ÇÃHP
=======
			// „Ç≤„Éº„Ç∏„ÇíÊ≠¢„ÇÅ„Çã
			GaugeState state = actionGauge_.PressButton();

			// Èò≤Âæ°ÁéÅEÇíÂèñÂæÅE
			float rate = ActionGauge::GetResultRate(state);

			// Èò≤Âæ°Âá¶ÁêÅE
			player_->Defense(rate);

			// „ÉÄ„É°„Éº„Ç∏„ÇíÂèó„Åë„ÇãÂâç„ÅEHP
			int beforeHP = player_->GetHP();

			// Êïµ„ÅÆÊîªÊíÅEäõ„Çí‰Ωø„Å£„Å¶„ÉÄ„É°„Éº„Ç∏Ë®àÁÆÅE
			player_->Damage(enemy_->GetATK());

			// „ÉÄ„É°„Éº„Ç∏„ÇíÂèó„Åë„ÅüÂæå„ÅEHP
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
		// ÉoÅ[Ç™ç≈å„Ç‹Ç≈ìÆÇ¢ÇƒÇ‡EnterÇâüÇ≥Ç»Ç©Ç¡ÇΩèÍçá
		else if (!actionGauge_.IsMoving())
		{
			// ñhå‰é∏îsÇ∆ÇµÇƒàµÇ§
=======
		// „Éê„ÅE„ÅåÊúÄÂæå„Åæ„ÅßÂãï„ÅÑ„Å¶„ÇÅEnter„ÇíÊäº„Åï„Å™„Åã„Å£„ÅüÂ†¥ÂêÅE
		else if (!actionGauge_.IsMoving())
		{
			// Èò≤Âæ°Â§±Êïó„Å®„Åó„Å¶Êâ±„ÅÅE
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
	// ÉoÅ[Ç™é~Ç‹Ç¡ÇΩå„
=======
	// „Éê„ÅE„ÅåÊ≠¢„Åæ„Å£„ÅüÂæÅE
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
		//if(ÉXÉgÅ[ÉäÅ[ÉÇÅ[ÉhÇÃéû)
=======
		//if(„Çπ„Éà„ÅE„É™„Éº„É¢„Éº„Éâ„ÅEÊôÅE
>>>>>>> Stashed changes
		if (gameScene_.IsStory())
		{
			gameScene_.ChangePhase(GameScene::QUEST_PHASE::PHASE_STORY);
<<<<<<< HEAD
			SoundManager::GetInstance().Stop(SoundManager::SRC::GAME_BGM);
			
			// Ê±∫ÂÆöÈü≥
=======

<<<<<<< Updated upstream
			// åàíËâπ
=======
			// ÅEΩÅEΩÅEΩËâπ
>>>>>>> Scene
>>>>>>> Stashed changes
			SoundManager::GetInstance().Play(SoundManager::SRC::SELECT_SE, Sound::TIMES::FORCE_ONCE);
		}
		else
		{
			// Wave 5ÇÉNÉäÉAÇµÇΩÇÁÉQÅ[ÉÄÉNÉäÉA
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
<<<<<<< Updated upstream
	//ÉRÉ}ÉìÉhëIëÇÃï`âÊèàóù
=======
	//„Ç≥„Éû„É≥„ÉâÈÅ∏Êäû„ÅEÊèèÁîªÂá¶ÁêÅE
>>>>>>> Stashed changes
	DrawGraph(ARROW_POS_X + ARROW_OFFSET * selectedIndex_, ARROW_POS_Y, cursorImage_, true);
	DrawGraph(GameScene::COMMAND_ATTACK_X, GameScene::COMMAND_MENU_Y, attackImage_, true);
	DrawGraph(GameScene::COMMAND_TALK_X, GameScene::COMMAND_MENU_Y, talkImage_, true);
	DrawGraph(GameScene::COMMAND_ESCAPE_X, GameScene::COMMAND_MENU_Y, escapeImage_, true);
}

void BattlePhase::DrawPlayerTurn(void)
{
<<<<<<< Updated upstream
	//ÉvÉåÉCÉÑÅ[ÇÃÉ^Å[ÉìÇÃï`âÊèàóù
	DrawString(100, 130, "ÉvÉåÉCÉÑÅ[É^Å[Éì",0xFFFFFF);
=======
	//„Éó„É¨„Ç§„É§„Éº„ÅÆ„Çø„Éº„É≥„ÅÆÊèèÁîªÂá¶ÁêÅE
	DrawString(100, 130, "„Éó„É¨„Ç§„É§„Éº„Çø„Éº„É≥",0xFFFFFF);
>>>>>>> Stashed changes
	DrawFormatString(100, GameScene::COMMAND_MENU_Y, 0xFFFFFF, "CommandType: %d", commandType_);
}

void BattlePhase::DrawEnemyTurn(void)
{
<<<<<<< Updated upstream
	//ìGÇÃÉ^Å[ÉìÇÃï`âÊèàóù
	DrawString(100, 130, "ìGÇÃÉ^Å[Éì", 0xFFFFFF);
=======
	//Êïµ„ÅÆ„Çø„Éº„É≥„ÅÆÊèèÁîªÂá¶ÁêÅE
	DrawString(100, 130, "Êïµ„ÅÆ„Çø„Éº„É≥", 0xFFFFFF);
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
>>>>>>> Stashed changes
}

void BattlePhase::ProsesSelectAttack(void)
{
<<<<<<< Updated upstream
=======
	//„Åô„Åß„Å´Êïµ„ÇíÂÄí„Åó„Å¶„ÅÅEÇãÂ†¥Âêà„ÅEÂç≥Â∫ß„ÅÆÁµÇ‰∫ÅEà§ÂÆÅE
>>>>>>> Stashed changes
	if (enemy_->IsDead())
	{
		//çUåÇåÇîjåãâ Ç GameScene Ç…ãLò^ÅI
		gameScene_.RecordBattleResult(GameScene::BATTLE_RESULT::RESULT_ATTACK);
		//êÌì¨èIóπâÊñ Ç÷à⁄çs
		battlePhase_ = BATTLE_END;

		SoundManager::GetInstance().Stop(SoundManager::SRC::GAME_BGM);
		return;
	}

<<<<<<< Updated upstream
	// ÉoÅ[Ç™Ç‹ÇæìÆÇ¢ÇƒÇ¢ÇÈç≈íÜÇÃèàóù
	if (actionGauge_.IsMoving())
=======
	// „Çπ„ÉÅEÉÉ„Éó„Åî„Å®„Å´Âá¶ÁêÅEÇíÂÆåÂÅEÂàÅEõ¢
	switch (attackStep_)
>>>>>>> Stashed changes
	{
		if (ins_.IsTrgDown(KEY_INPUT_RETURN))
		{
			// É{É^ÉìÇ™âüÇ≥ÇÍÇΩÇÁÉoÅ[Ç™ÇªÇÃèÍÇ…é~Ç‹ÇÈ
			GaugeState state = actionGauge_.PressButton();

<<<<<<< Updated upstream
=======
<<<<<<< HEAD
	case AttackStep::WAIT_FOR_MESSAGE:
		ProcessAttackMessageWait();
		break;
=======
>>>>>>> Stashed changes
			float rate = ActionGauge::GetResultRate(state);

			int power = player_->Attack(rate);

<<<<<<< Updated upstream
			// ìGÇ…É_ÉÅÅ[ÉWÇó^Ç¶ÇÈ
			enemy_->Damage(power);

			// É_ÉÅÅ[ÉWÇó^Ç¶ÇÁÇÍÇΩÇ∆Ç´ÇæÇØçUåÇâπÇñ¬ÇÁÇ∑
=======
			// ÅEΩGÅEΩ…É_ÅEΩÅEΩÅEΩ[ÅEΩWÅEΩÅEΩ^ÅEΩÅEΩÅEΩÅEΩ
			enemy_->Damage(power);

			// ÅEΩ_ÅEΩÅEΩÅEΩ[ÅEΩWÅEΩÅEΩ^ÅEΩÅEΩÅEΩÅEΩÍÇΩÅEΩ∆ÇÔøΩÅEΩÅEΩÅEΩÅEΩÅEΩUÅEΩÅEΩÅEΩÅEΩÅEΩÅEΩ¬ÇÁÇ∑
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
			//É{É^ÉìÇ™âüÇ≥ÇÍÇƒÇ¢Ç»Ç¢ÉtÉåÅ[ÉÄÇæÇØÅAÉoÅ[ÇêiÇﬂÇÈ
=======
			//ÅEΩ{ÅEΩ^ÅEΩÅEΩÅEΩÅEΩÅEΩÅEΩÅEΩÅEΩÅEΩÅEΩƒÇÔøΩÅEΩ»ÇÔøΩÅEΩtÅEΩÅEΩÅEΩ[ÅEΩÅEΩÅEΩÅEΩÅEΩÅEΩÅEΩAÅEΩoÅEΩ[ÅEΩÅEΩiÅEΩﬂÇÔøΩ
>>>>>>> Stashed changes
			float deltaTime = 1.0f / 60.0f;
			actionGauge_.Update(deltaTime);
		}
	}
	else
	{
		if (ins_.IsTrgDown(KEY_INPUT_RETURN))
		{
<<<<<<< Updated upstream
			//ìGÇì|ÇµÇƒÇ¢ÇΩèÍçá
			if (enemy_->IsDead())
			{
				//çUåÇåÇîjåãâ Ç GameScene Ç…ãLò^ÅI
				gameScene_.RecordBattleResult(GameScene::BATTLE_RESULT::RESULT_ATTACK);

				//åàíËâπ
				SoundManager::GetInstance().Play(SoundManager::SRC::SELECT_SE, Sound::TIMES::FORCE_ONCE);

				//êÌì¨èIóπâÊñ Ç÷à⁄çs
=======
			//ÅEΩGÅEΩÅEΩ|ÅEΩÅEΩÅEΩƒÇÔøΩÅEΩÅEΩÅEΩÍçÅE
			if (enemy_->IsDead())
			{
				//ÅEΩUÅEΩÅEΩÅEΩÅEΩÅEΩjÅEΩÅEΩÅEΩ ÇÔøΩ GameScene ÅEΩ…ãLÅEΩ^ÅEΩI
				gameScene_.RecordBattleResult(GameScene::BATTLE_RESULT::RESULT_ATTACK);

				//ÅEΩÅEΩÅEΩËâπ
				SoundManager::GetInstance().Play(SoundManager::SRC::SELECT_SE, Sound::TIMES::FORCE_ONCE);

				//ÅEΩ˙Ø¨ÅEΩIÅEΩÅEΩÅEΩÅEΩ Ç÷à⁄çs
>>>>>>> Stashed changes
				battlePhase_ = BATTLE_END;

				return;
			}

			battleTurnCnt_++;

			actionGauge_.Start(enemy_->GetGaugeGoodWidth(), enemy_->GetGaugeGreatWidth(), enemy_->GetGaugePerfectWidth());

			battlePhase_ = ENEMY_TURN;
		}
<<<<<<< Updated upstream
=======
>>>>>>> Scene
>>>>>>> Stashed changes
	}
}

void BattlePhase::ProsesSelectTalk(void)
{
<<<<<<< Updated upstream
	if (ins_.IsTrgDown(KEY_INPUT_RETURN))
=======
	// „Çπ„ÉÅEÉÉ„Éó„Åî„Å®„Å´Âá¶ÁêÅEÇíÂàÅE≤ÅE
	switch (talkStep_)
>>>>>>> Stashed changes
	{
		if (enemy_->GetHP() == enemy_->GetMaxHP())
		{
			talkSuccessCount_++;
		}

		if (talkSuccessCount_ >= 4)
		{
			currentMessage_ = "ëäéËÇÕêÌà”Çëré∏ÇµÇΩÅI";

			//ê‡ìæäÆóπÇ≈êÌê—ãLò^ Åï êÌì¨èIóπ
			gameScene_.RecordBattleResult(GameScene::BATTLE_RESULT::RESULT_TALK);
			battlePhase_ = BATTLE_END;
		}
		else
		{
			currentMessage_ = "ëäéËÇÕè≠ÇµòbÇ…é®ÇåXÇØÇƒÇ¢ÇÈÅc";

			battleTurnCnt_++;
			//battlePhase_ = COMMAND_SELECT;
			actionGauge_.Start(enemy_->GetGaugeGoodWidth(), enemy_->GetGaugeGreatWidth(), enemy_->GetGaugePerfectWidth());
			battlePhase_ = ENEMY_TURN;
		}

		// åàíËâπ
		SoundManager::GetInstance().Play(SoundManager::SRC::SELECT_SE, Sound::TIMES::FORCE_ONCE);
	}
}

void BattlePhase::ProsesSelectRun(void)
{
	if (ins_.IsTrgDown(KEY_INPUT_RETURN))
	{
<<<<<<< Updated upstream
		// É^Å[ÉìÇ™åoâﬂÇ∑ÇÈÇ≤Ç∆Ç…ì¶Ç∞ÇÁÇÍÇÈämó¶Ç™ïœÇÌÇÈ
		int runRate = RUN_SUCCESS_RATE + (battleTurnCnt_ * 5);

		// ì¶ëñê¨å˜îªíËÅiê¨å˜éûÅj
		if (runRate <= GetRand(99))
		{
			//ì¶ëñåãâ Ç GameScene Ç…ãLò^ÅI
=======
		// „Çø„Éº„É≥„ÅåÁµåÈÅé„Åô„Çã„Åî„Å®„Å´ÈÄÅEÅí„Çâ„Çå„ÇãÁ¢∫ÁéÅEÅåÂ§â„Çè„ÇÅE
		int runRate = RUN_SUCCESS_RATE + (battleTurnCnt_ * 5);

		// ÈÄÅEµ∞ÊàêÂäüÂà§ÂÆöÔºàÊÅEÂäüÊôÇÅEÅE
		if (runRate <= GetRand(99))
		{
			currentMessage_ = gameScene_.getMessage(enemy_->GetName(), "ON_ESCAPE");

			//ÈÄÅEµ∞ÁµêÊûú„ÇÅEGameScene „Å´Ë®òÈå≤ÅEÅE
>>>>>>> Stashed changes
			gameScene_.RecordBattleResult(GameScene::BATTLE_RESULT::RESULT_ESCAPE);

			battlePhase_ = BATTLE_END;
		}
<<<<<<< Updated upstream
		// é∏îséû
=======
		// Â§±ÊïóÊôÇ
>>>>>>> Stashed changes
		else
		{
			actionGauge_.Start(enemy_->GetGaugeGoodWidth(), enemy_->GetGaugeGreatWidth(), enemy_->GetGaugePerfectWidth());
			battlePhase_ = ENEMY_TURN;
		}

<<<<<<< Updated upstream
		// åàíËâπ
		SoundManager::GetInstance().Play(SoundManager::SRC::SELECT_SE, Sound::TIMES::FORCE_ONCE);
	}
}
=======
		// Ê±∫ÂÆöÈü≥
		SoundManager::GetInstance().Play(SoundManager::SRC::SELECT_SE, Sound::TIMES::FORCE_ONCE);
	}
}

// „Éê„ÅE„ÅåÂãï„ÅÅEÅ¶„ÅÅEÇãÈñì„ÅEÂá¶ÁêÅE
void BattlePhase::ProcessAttackGaugeInput(void)
{

	if (actionGauge_.IsMoving())
	{
		if (ins_.IsTrgDown(KEY_INPUT_RETURN))
		{
			// Ê±∫ÂÆö„ÅE„Çø„É≥„ÅåÊäº„Åï„Çå„Åü„Çâ„Éê„ÅE„ÇíÊ≠¢„ÇÅ„Å¶„ÉÄ„É°„Éº„Ç∏Ë®àÁÆÅE
			GaugeState state = actionGauge_.PressButton();
			float rate = ActionGauge::GetResultRate(state);
			int power = player_->Attack(rate);
			enemy_->Damage(power);

			if (power > 0)
			{
				SoundManager::GetInstance().Play(SoundManager::SRC::ATTACK_SE, Sound::TIMES::FORCE_ONCE);
				DamageTalk(); // „ÉÄ„É°„Éº„Ç∏„Å´Âøú„Åò„Åü„Çª„É™„Éï„Çí„Çª„ÉÅEÉà
			}
			else
			{
				SoundManager::GetInstance().Play(SoundManager::SRC::MISS_SE, Sound::TIMES::FORCE_ONCE);
				currentMessage_ = "ÊîªÊíÅEÅE„Éü„Çπ„Åó„ÅüÅEÅE;
			}

			// Ê¨°„ÅÆ„Çπ„ÉÅEÉÉ„ÉóÔºà„É°„ÉÅEÇª„Éº„Ç∏Á¢∫Ë™çÂæÅEÅ°ÅEâ„Å∏ÁßªË°ÅE
			attackStep_ = AttackStep::WAIT_FOR_MESSAGE;
		}
		else
		{
			// „Éú„Çø„É≥„ÅåÊäº„Åï„Çå„Å¶„ÅÅEÅ™„ÅÅEñì„ÅØ„Éê„ÅE„ÇíÈÄ≤„ÇÅ„Çã
			float deltaTime = 1.0f / 60.0f;
			actionGauge_.Update(deltaTime);
		}
	}
}

//„Éê„ÅE„ÅåÂãï„ÅÅEÅ¶„ÅÅEÇãÈñì„ÅEÂá¶ÁêÅE
void BattlePhase::ProcessAttackMessageWait(void)
{
	if (ins_.IsTrgDown(KEY_INPUT_RETURN))
	{
		SoundManager::GetInstance().Play(SoundManager::SRC::SELECT_SE, Sound::TIMES::FORCE_ONCE);

		// ÊíÅE†¥„Åó„Å¶„ÅÅEÅüÂ†¥Âêà„ÅEÊà¶ÈóòÁµÇ‰∫ÅEÅ∏
		if (enemy_->IsDead())
		{
			gameScene_.RecordBattleResult(GameScene::BATTLE_RESULT::RESULT_ATTACK);
			battlePhase_ = BATTLE_END;
			return;
		}

		// Ê¨°Âõû„ÅE„Åü„ÇÅ„Å´„Çπ„ÉÅEÉÉ„Éó„ÇíÊúÄÂàùÔºà„Éê„ÉºÁßªÂãïÔºâ„Å´„É™„Çª„ÉÅEÉà
		attackStep_ = AttackStep::MOVING_GAUGE;

		// „Çø„Éº„É≥„ÇíÂõû„Åó„Å¶Êïµ„ÅÆ„Çø„Éº„É≥„Å∏ÁßªË°ÅE
		battleTurnCnt_++;
		actionGauge_.Start(enemy_->GetGaugeGoodWidth(), enemy_->GetGaugeGreatWidth(), enemy_->GetGaugePerfectWidth());
		battlePhase_ = ENEMY_TURN;
	}
}

// Ë©±„Åô„Ç≥„Éû„É≥„Éâ„ÅEÂá¶ÁêÅE
void BattlePhase::ProcessTalkExecution(void)
{
	//Êïµ„ÅÆHP„ÅåÊ∫Ä„Çø„É≥„ÅÆÁä∂ÊÖã„ÅßË©±„Åô„Å®Ë™¨ÂæóÊÅEÂäüÂõûÊï∞„ÅåÂ¢ó„Åà„ÇÅE
	if (enemy_->GetHP() == enemy_->GetMaxHP())
	{
		talkSuccessCount_++;

		// 4Âõû„Å´ÈÅî„Åó„ÅüÂ†¥Âêà„ÅEË™¨ÂæóÊÅEÂäüÔºÅE
		if (talkSuccessCount_ >= 4)
		{
			currentMessage_ = gameScene_.getMessage(enemy_->GetName(), "ON_TALK", talkSuccessCount_ - 1);
			gameScene_.RecordBattleResult(GameScene::BATTLE_RESULT::RESULT_TALK);
			battlePhase_ = BATTLE_END;
			return;
		}
		else
		{
			// CSV„Åã„ÇâË©≤ÂΩìÂõûÊï∞„ÅÆ„Çª„É™„Éï„ÇíÂèñÂæó„Åó„Å¶ÁîªÈù¢„Å´„Çª„ÉÅEÉà
			currentMessage_ = gameScene_.getMessage(enemy_->GetName(), "ON_TALK", talkSuccessCount_ - 1);
		}
	}
	else
	{
		// ÊîªÊíÅEÅó„Å¶HP„ÅåÊ∏õ„Å£„Å¶„ÅÅEÇãÁä∂ÊÖã„ÅE„Å®„ÅÅE
		currentMessage_ = "Áõ∏Êâã„ÅEË≠¶Êàí„Åó„Å¶Ë©±„ÇíËÅû„ÅÅEÅ¶„Åè„Çå„Å™„ÅÅEÄ¶‚Ä¶ÅEÅE;
	}

	SoundManager::GetInstance().Play(SoundManager::SRC::SELECT_SE, Sound::TIMES::FORCE_ONCE);

	// Ê¨°„ÅÆ„Çπ„ÉÅEÉÉ„ÉóÔºà„Çª„É™„ÉïÁ¢∫Ë™çÂæÅEÅ°ÅEâ„Å∏ÁßªË°ÅE
	talkStep_ = TalkStep::WAIT_FOR_MESSAGE;
}

// Ë©±„Åô„Ç≥„Éû„É≥„Éâ„ÅE„Çª„É™„ÉïÁ¢∫Ë™çÂæå„ÅEÂá¶ÁêÅE
void BattlePhase::ProcessTalkMessageWait(void)
{
	if (ins_.IsTrgDown(KEY_INPUT_RETURN))
	{
		SoundManager::GetInstance().Play(SoundManager::SRC::SELECT_SE, Sound::TIMES::FORCE_ONCE);

		// Ê¨°Âõû„ÅE„Åü„ÇÅ„Å´„Çπ„ÉÅEÉÉ„Éó„ÇíÊúÄÂàù„Å´Êàª„ÅÅE
		talkStep_ = TalkStep::SELECT_TALK;

		// „Çø„Éº„É≥„ÇíÈÄ≤„ÇÅ„Å¶Êïµ„ÅÆ„Çø„Éº„É≥„Å∏ÁßªË°ÅE
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
>>>>>>> Stashed changes
