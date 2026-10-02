#pragma once
#include "PhaseBase.h"

class GameScene;

class StoryPhase : public PhaseBase
{
public:

	//ストーリーフェーズの種類
	enum STORY_PHASE
	{
		MAIN_STORY,		//メインストーリー
		SELECT_ROUTE,	//ルート選択
		ENDING,			//エンディング
		MAX,			//最大値
	};

	//エンディングの種類
	enum ENDING_TYPE
	{
		STORY_WAVE_START,   //通常のWave開始時テキスト
		STORY_TRUE_END,     //トゥルーエンド
		STORY_NORMAL_END,   //ノーマルエンド
		STORY_ESCAPE_END,   //逃亡エンド
	};

	StoryPhase(GameScene& gameScene);
	~StoryPhase();
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void Release(void)override;
private:

	InputManager& ins_ = InputManager::GetInstance();

	GameScene& gameScene_;			//親の情報を渡す

	STORY_PHASE storyPhase_;		//ストーリーフェーズの種類
	ENDING_TYPE endingType_;			//エンディングの種類

	void UpdateMainStory(void);		//メインストーリーの更新処理
	void UpdateSelectRoute(void);	//次に進む道を選択するための関数
	void UpdateEnding(void);		//エンディングの更新処理

};

