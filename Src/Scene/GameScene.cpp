#include <DxLib.h>
#include "../Application.h"
#include "../Common/Color.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/SoundManager.h"
#include "../Utility/AsoUtility.h"
#include "phase/PhaseBase.h"
#include "Phase/StoryPhase.h"
#include "Phase/BattlePhase.h"
#include "GameScene.h"

//デフォルトコンストラクタ
GameScene::GameScene(void)
:questPhase_(QUEST_PHASE::PHASE_STORY)
, waveCount_(1)
, hasStartedBattle_(false)
, lastBattleResult_(BATTLE_RESULT::RESULT_ATTACK)
, isReturnFromBattle_(false)
, battleCount_(0)
, talkCount_(0)
, escapeCount_(0)
{
	questPhase_ = QUEST_PHASE::PHASE_STORY;

	ChangePhase(QUEST_PHASE::PHASE_STORY);
}

//デストラクタ
GameScene::~GameScene(void)
{
}

//初期化処理
void GameScene::Init(void)
{
	questPhase_ = QUEST_PHASE::PHASE_STORY;
	waveCount_ = 1;
	hasStartedBattle_ = false;
	isReturnFromBattle_ = false;

	// カウント初期化
	battleCount_ = 0;
	talkCount_ = 0;
	escapeCount_ = 0;

	for (int i = 0; i < 5; ++i)
	{
		usedEnemies_[i] = false;
	}
	playerImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER).handleId_;
	stageImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::GAME_SCENE).handleId_;
	messageBoxImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::MESSAGE_BOX).handleId_;

	ChangePhase(QUEST_PHASE::PHASE_STORY);
}

//更新処理
void GameScene::Update(void)
{
	phase_->Update();
}

//描画処理
void GameScene::Draw(void)
{
	phase_->Draw();
	DrawFormatString(0, 0, GetColor(255, 255, 255), "Battle Count: %d", battleCount_);
	DrawFormatString(0, 20, GetColor(255, 255, 255), "Talk Count: %d", talkCount_);
	DrawFormatString(0, 40, GetColor(255, 255, 255), "Escape Count: %d", escapeCount_);
}

//解放処理
void GameScene::Release(void)
{
	SoundManager::GetInstance().Stop(SoundManager::SRC::GAME_BGM);
	phase_->Release();
}

void GameScene::ChangePhase(QUEST_PHASE phase)
{
	questPhase_ = phase;

	switch (questPhase_)
	{
	case QUEST_PHASE::PHASE_STORY:
		phase_ = std::make_unique<StoryPhase>(*this);
		break;
	case QUEST_PHASE::PHASE_BATTLE:
		phase_ = std::make_unique<BattlePhase>(*this);
		break;
	}

	// 追加: フェーズ初期化（生成直後に必ず Init を呼ぶ）
	if (phase_)
	{
		phase_->Init();
	}
}

int GameScene::GetWaveCount(void) const
{
	return waveCount_;
}

void GameScene::SetCurrentWave(int wave)
{
	waveCount_ = wave;
}

void GameScene::StartNextBattle(void)
{

	hasStartedBattle_ = true;
}

void GameScene::RecordBattleResult(BATTLE_RESULT result)
{
	lastBattleResult_ = result;
	isReturnFromBattle_ = true; // 戦闘復帰フラグON

	switch (result)
	{
	case BATTLE_RESULT::RESULT_ATTACK:
		battleCount_++;
		break;
	case BATTLE_RESULT::RESULT_TALK:
		talkCount_++;
		break;
	case BATTLE_RESULT::RESULT_ESCAPE:
		escapeCount_++;
		break;
	}
}

GameScene::ENDING_TYPE GameScene::JudgeEndingType() const
{
	//逃走が3回以上なら逃走エンド
	if (escapeCount_ >= 3)
	{
		return ENDING_TYPE::ENDING_ESCAPE;
	}

	//攻撃勝利が0回（一度も敵を倒していない）かつ「話す」解決が4回以上ならトゥルーエンド
	if (battleCount_ == 0 && talkCount_ >= 4)
	{
		return ENDING_TYPE::ENDING_TRUE;
	}

	//それ以外（倒して進んだ）はノーマルエンド
	return ENDING_TYPE::ENDING_NORMAL;
}


void GameScene::NextWave(void)
{
	if (waveCount_ < 5)
	{
		waveCount_++;
	}
}

bool GameScene::IsEnemyUsed(int index) const
{
	return usedEnemies_[index];
}

void GameScene::SetEnemyUsed(int index)
{
	usedEnemies_[index] = true;
}