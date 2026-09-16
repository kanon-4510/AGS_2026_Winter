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
	playerImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER).handleId_;
	stageImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::GAME_SCENE).handleId_;
	messageBoxImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::MESSAGE_BOX).handleId_;
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
}


//解放処理
void GameScene::Release(void)
{
	SoundManager::GetInstance().Stop(SoundManager::SRC::GAME_BGM);
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
}
