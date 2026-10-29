#pragma once
#include "PhaseBase.h"
#include "../GameScene.h"

class StoryPhase : public PhaseBase
{
public:

	//ストーリーの種類
	enum SCENARIO_TYPE
	{
		STORY_WAVE_1,
		STORY_WAVE_2,
		STORY_WAVE_3,
		STORY_WAVE_4,
		STORY_WAVE_5,
		STORY_TRUE_END,
		STORY_NORMAL_END,
		STORY_ESCAPE_END,
	};

	//ストーリーフェーズの種類
	enum STORY_PHASE
	{
		MAIN_STORY,		//メインストーリー
		SELECT_ROUTE,	//ルート選択
		ENDING,			//エンディング
		MAX,			//最大値
	};

	//ストーリーのデータ構造体
	struct DialogueData
	{
		std::string speaker;  // 話者
		std::string message;  // メッセージ
		std::string eventTag; // イベントタグ
	};

	struct ChoiceData
	{
		std::string text;       // 選択肢のテキスト
		std::string targetTag;  // 選んだときのジャンプ先タグ
	};

	static constexpr int MAX_SCENARIO_TYPE = 8;

	StoryPhase(GameScene& gameScene);
	~StoryPhase();
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void Release(void)override;

	bool LoadScenario(SCENARIO_TYPE type);

private:

	InputManager& ins_ = InputManager::GetInstance();

	GameScene& gameScene_;			//親の情報を渡す

	STORY_PHASE storyPhase_;		//ストーリーフェーズの種類
	STORY_PHASE perPhase_;			//現在のフェーズを保持する変数

	std::vector<DialogueData> dialogueList_;	//シナリオのデータを格納するリスト
	std::vector<ChoiceData> choiceList_;		//選択肢のリスト
	int currentLineIndex_ = 0;					//現在のセリフのインデックス
	int selectCursor_ = 0;      //現在のカーソル位置

	void OnReturnFromBattle(GameScene::BATTLE_RESULT result);

	void UpdateMainStory(void);		//メインストーリーの更新処理
	void UpdateSelectRoute(void);	//次に進む道を選択するための関数
	void UpdateEnding(void);		//エンディングの更新処理

	void JumpToTag(const std::string& targetTag);
};

