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

	//���y
	SoundManager::GetInstance().Play(SoundManager::SRC::GAME_BGM, Sound::TIMES::LOOP);

	cursorImage_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CURSOR).handleId_;
	attackImage_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::COMMAND_ATTACK).handleId_;
	talkImage_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::COMMAND_TALK).handleId_;
	escapeImage_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::COMMAND_ESCAPE).handleId_;
}

void BattlePhase::Update(void)
{
	// �G�̍X�V
	if (enemy_)
	{
		enemy_->Update();
	}

	// �_���[�W�t���b�V���̍X�V
	if (damageFlashTimer_ > 0.0f)
	{
		damageFlashTimer_ -= 1.0f / 60.0f;

		if (damageFlashTimer_ < 0.0f)
		{
			damageFlashTimer_ = 0.0f;
		}
	}

	//�t�F�[�Y���Ƃ̍X�V������Ăяo��
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

	// ���b�Z�[�W�g����уA�N�V�����Q�[�W�̕`��̈�w��i��: ��ʉ����j
	int winX = 100, winY = 360, winW = 600, winH = 180;

	//DrawMultilineText(275, 700, currentMessage_, GetColor(255, 255, 255), 24);
	DrawString(275, 700, currentMessage_.c_str(), GetColor(255, 255, 255));

	//�A�N�V�����Q�[�W�g�i�펞�\���j
	actionGauge_.Draw();

	// �G��`��
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
		DrawString(100, GameScene::COMMAND_MENU_Y, "�o�g���I��", GetColor(255, 0, 0));
		break;
	}

	// ��_���[�W���̐ԃt���b�V��
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

	// GameScene���猻�݂̐퓬�񐔂�擾
	int wave = gameScene_.GetWaveCount();

	if (gameScene_.IsStory())
	{
		// �J�G��
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
	else
	{
		// �J�G��
		if (wave == 1)
		{
			type = Enemy::TYPE::FROG;
			gameScene_.SetEnemyUsed(0);
		}
		// �{�X

		else if (wave == 5)
		{
			type = Enemy::TYPE::DEVIL;
		}

		// 2�`4��ڂ͏d���Ȃ��Ń����_��
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
	//�\���L�[�ŃR�}���h�I���s������
	if (ins_.IsTrgDown(KEY_INPUT_RIGHT))
	{
		selectedIndex_ = (selectedIndex_ + 1) % MAX_COMMANDS;
		//�J�[�\����
		SoundManager::GetInstance().Play(SoundManager::SRC::CURSOR_SE, Sound::TIMES::FORCE_ONCE);
	}
	else if(ins_.IsTrgDown(KEY_INPUT_LEFT))
	{
		selectedIndex_ = (selectedIndex_ - 1 + MAX_COMMANDS) % MAX_COMMANDS;
		//�J�[�\����
		SoundManager::GetInstance().Play(SoundManager::SRC::CURSOR_SE, Sound::TIMES::FORCE_ONCE);
	}

	if (ins_.IsTrgDown(KEY_INPUT_RETURN))
	{
		// �L�[�������ꂽ�Ƃ������e�R�}���h������Ăяo��
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

		//���艹
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
	// �o�[���܂������Ă���Œ�
	if (actionGauge_.IsMoving())
	{
		float deltaTime = 1.0f / 60.0f;
		actionGauge_.Update(deltaTime);

		if (ins_.IsTrgDown(KEY_INPUT_RETURN))
		{
			// �Q�[�W��~�߂�
			GaugeState state = actionGauge_.PressButton();

			// �h�䗦��擾
			float rate = ActionGauge::GetResultRate(state);

			// �h�䏈��
			player_->Defense(rate);

			// �_���[�W��󂯂�O��HP
			int beforeHP = player_->GetHP();

			// �G�̍U���͂�g���ă_���[�W�v�Z
			player_->Damage(enemy_->GetATK());

			// �_���[�W��󂯂����HP
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
		// �o�[���Ō�܂œ����Ă�Enter������Ȃ������ꍇ
		else if (!actionGauge_.IsMoving())
		{
			// �h�䎸�s�Ƃ��Ĉ���
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
	// �o�[���~�܂�����
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
		//if(�X�g�[���[���[�h�̎�)
		if (gameScene_.IsStory())
		{
			gameScene_.ChangePhase(GameScene::QUEST_PHASE::PHASE_STORY);

			// ���艹
			SoundManager::GetInstance().Play(SoundManager::SRC::SELECT_SE, Sound::TIMES::FORCE_ONCE);
		}
		else
		{
			// Wave 5��N���A������Q�[���N���A
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
	//�R�}���h�I��̕`�揈��
	DrawGraph(ARROW_POS_X + ARROW_OFFSET * selectedIndex_, ARROW_POS_Y, cursorImage_, true);
	DrawGraph(GameScene::COMMAND_ATTACK_X, GameScene::COMMAND_MENU_Y, attackImage_, true);
	DrawGraph(GameScene::COMMAND_TALK_X, GameScene::COMMAND_MENU_Y, talkImage_, true);
	DrawGraph(GameScene::COMMAND_ESCAPE_X, GameScene::COMMAND_MENU_Y, escapeImage_, true);
}

void BattlePhase::DrawPlayerTurn(void)
{
	//�v���C���[�̃^�[���̕`�揈��
	DrawString(100, 130, "�v���C���[�^�[��",0xFFFFFF);
	DrawFormatString(100, GameScene::COMMAND_MENU_Y, 0xFFFFFF, "CommandType: %d", commandType_);
}

void BattlePhase::DrawEnemyTurn(void)
{
	//�G�̃^�[���̕`�揈��
	DrawString(100, 130, "�G�̃^�[��", 0xFFFFFF);
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
	//���łɓG��|���Ă���ꍇ�̑����̏I������
	if (enemy_->IsDead())
	{
		gameScene_.RecordBattleResult(GameScene::BATTLE_RESULT::RESULT_ATTACK);
		battlePhase_ = BATTLE_END;

		SoundManager::GetInstance().Stop(SoundManager::SRC::GAME_BGM);
		return;
	}

	// �X�e�b�v���Ƃɏ�������S����
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
	// �X�e�b�v���Ƃɏ����𕪊�
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
		// �^�[�����o�߂��邲�Ƃɓ�������m�����ς��
		int runRate = RUN_SUCCESS_RATE + (battleTurnCnt_ * 5);

		// ������������i�������j
		if (runRate <= GetRand(99))
		{
			currentMessage_ = gameScene_.getMessage(enemy_->GetName(), "ON_ESCAPE");

			//�������ʂ� GameScene �ɋL�^�I
			gameScene_.RecordBattleResult(GameScene::BATTLE_RESULT::RESULT_ESCAPE);

			battlePhase_ = BATTLE_END;
		}
		// ���s��
		else
		{
			actionGauge_.Start(enemy_->GetGaugeGoodWidth(), enemy_->GetGaugeGreatWidth(), enemy_->GetGaugePerfectWidth());
			battlePhase_ = ENEMY_TURN;
		}

		// ���艹
		SoundManager::GetInstance().Play(SoundManager::SRC::SELECT_SE, Sound::TIMES::FORCE_ONCE);
	}
}

// �o�[�������Ă���Ԃ̏���
void BattlePhase::ProcessAttackGaugeInput(void)
{

	if (actionGauge_.IsMoving())
	{
		if (ins_.IsTrgDown(KEY_INPUT_RETURN))
		{
			// ����{�^���������ꂽ��o�[��~�߂ă_���[�W�v�Z
			GaugeState state = actionGauge_.PressButton();
			float rate = ActionGauge::GetResultRate(state);
			int power = player_->Attack(rate);
			enemy_->Damage(power);

			if (power > 0)
			{
				SoundManager::GetInstance().Play(SoundManager::SRC::ATTACK_SE, Sound::TIMES::FORCE_ONCE);
				DamageTalk(); // �_���[�W�ɉ������Z���t��Z�b�g
			}
			else
			{
				SoundManager::GetInstance().Play(SoundManager::SRC::MISS_SE, Sound::TIMES::FORCE_ONCE);
				currentMessage_ = "�U���̓~�X�����I";
			}

			// ���̃X�e�b�v�i���b�Z�[�W�m�F�҂��j�ֈڍs
			attackStep_ = AttackStep::WAIT_FOR_MESSAGE;
		}
		else
		{
			// �{�^����������Ă��Ȃ��Ԃ̓o�[��i�߂�
			float deltaTime = 1.0f / 60.0f;
			actionGauge_.Update(deltaTime);
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

		// ����̂��߂ɃX�e�b�v��ŏ��i�o�[�ړ��j�Ƀ��Z�b�g
		attackStep_ = AttackStep::MOVING_GAUGE;

		// �^�[����񂵂ēG�̃^�[���ֈڍs
		battleTurnCnt_++;
		actionGauge_.Start(enemy_->GetGaugeGoodWidth(), enemy_->GetGaugeGreatWidth(), enemy_->GetGaugePerfectWidth());
		battlePhase_ = ENEMY_TURN;
	}
}

// �b���R�}���h�̏���
void BattlePhase::ProcessTalkExecution(void)
{
	//�G��HP�����^���̏�ԂŘb���Ɛ�������񐔂�������
	if (enemy_->GetHP() == enemy_->GetMaxHP())
	{
		talkSuccessCount_++;

		// 4��ɒB�����ꍇ�͐�������I
		if (talkSuccessCount_ >= 4)
		{
			currentMessage_ = gameScene_.getMessage(enemy_->GetName(), "ON_TALK", talkSuccessCount_ - 1);
			gameScene_.RecordBattleResult(GameScene::BATTLE_RESULT::RESULT_TALK);
			battlePhase_ = BATTLE_END;
			return;
		}
		else
		{
			// CSV����Y���񐔂̃Z���t��擾���ĉ�ʂɃZ�b�g
			currentMessage_ = gameScene_.getMessage(enemy_->GetName(), "ON_TALK", talkSuccessCount_ - 1);
		}
	}
	else
	{
		// �U������HP�������Ă����Ԃ̂Ƃ�
		currentMessage_ = "����͌x�����Ęb�𕷂��Ă���Ȃ��c�c�I";
	}

	SoundManager::GetInstance().Play(SoundManager::SRC::SELECT_SE, Sound::TIMES::FORCE_ONCE);

	// ���̃X�e�b�v�i�Z���t�m�F�҂��j�ֈڍs
	talkStep_ = TalkStep::WAIT_FOR_MESSAGE;
}

// �b���R�}���h�̃Z���t�m�F��̏���
void BattlePhase::ProcessTalkMessageWait(void)
{
	if (ins_.IsTrgDown(KEY_INPUT_RETURN))
	{
		SoundManager::GetInstance().Play(SoundManager::SRC::SELECT_SE, Sound::TIMES::FORCE_ONCE);

		// ����̂��߂ɃX�e�b�v��ŏ��ɖ߂�
		talkStep_ = TalkStep::SELECT_TALK;

		// �^�[����i�߂ēG�̃^�[���ֈڍs
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
