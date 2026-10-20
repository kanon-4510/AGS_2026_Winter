#include <DxLib.h>
#include <fstream>
#include "../../Utility/ASOUtility.h"
#include "../../Manager/InputManager.h"
#include "../GameScene.h"
#include "StoryPhase.h"

// シナリオCSVファイルのパス定義（.csvに対応）
const char* SCENARIO_FILE_PATHS[] = {
	"Data/CSV/story_wave1.csv",
	"Data/CSV/story_wave2.csv",
	"Data/CSV/story_wave3.csv",
	"Data/CSV/story_wave4.csv",
	"Data/CSV/story_wave5.csv",
	"Data/CSV/ending_true.csv",
	"Data/CSV/ending_normal.csv",
	"Data/CSV/ending_escape.csv"
};

StoryPhase::StoryPhase(GameScene& gameScene)
	: gameScene_(gameScene)
{
	storyPhase_ = MAIN_STORY;
	currentLineIndex_ = 0;
}

StoryPhase::~StoryPhase()
{
}

void StoryPhase::Init()
{
	storyPhase_ = MAIN_STORY;
	currentLineIndex_ = 0;

	// 最初（Wave 1）のシナリオをロード
	LoadScenario(STORY_WAVE_1);
}

void StoryPhase::Update()
{
	// スペースキーで次の戦闘に進む(デバッグ)
	if (ins_.IsTrgDown(KEY_INPUT_SPACE))
	{
		gameScene_.StartNextBattle();
		gameScene_.ChangePhase(GameScene::QUEST_PHASE::PHASE_BATTLE);
		return;
	}
	switch (storyPhase_)
	{
	case StoryPhase::MAIN_STORY:
		UpdateMainStory();
		break;
	case StoryPhase::SELECT_ROUTE:
		UpdateSelectRoute();
		break;
	case StoryPhase::ENDING:
		UpdateEnding();
		break;
	default:
		break;
	}
}

void StoryPhase::Draw()
{
	DrawString(100, 100, "Story Phase", 0xFFFFFF);

	// 読み込み済みデータの描画テスト
	if (currentLineIndex_ < dialogueList_.size())
	{
		const auto& data = dialogueList_[currentLineIndex_];

		// 話者の描画
		if (!data.speaker.empty())
		{
			DrawFormatString(100, 180, 0xFFFF00, "【%s】", data.speaker.c_str());
		}

		// メッセージの描画
		DrawString(100, 210, data.message.c_str(), 0xFFFFFF);

		// デバッグ用（イベントタグ表示）
		if (!data.eventTag.empty())
		{
			DrawFormatString(100, 250, 0x888888, "Tag: %s", data.eventTag.c_str());
		}
	}

	switch (storyPhase_)
	{
	case StoryPhase::MAIN_STORY:
		break;
	case StoryPhase::SELECT_ROUTE:
		break;
	case StoryPhase::ENDING:
		break;
	}
}

void StoryPhase::Release()
{
	dialogueList_.clear();
}

bool StoryPhase::LoadScenario(SCENARIO_TYPE type)
{
	dialogueList_.clear();
	currentLineIndex_ = 0;

	std::ifstream file(SCENARIO_FILE_PATHS[type]);
	if (!file.is_open())
	{
		OutputDebugStringA("エラー: シナリオCSVが開けませんでした -> ");
		OutputDebugStringA(SCENARIO_FILE_PATHS[type]);
		OutputDebugStringA("\n");
		return false;
	}

	std::string line;
	while (std::getline(file, line))
	{
		// 空行やヘッダー行（名前,メッセージ... / 話者,メッセージ...）をスキップ
		if (line.empty() || line.find("名前,メッセージ") != std::string::npos || line.find("話者,メッセージ") != std::string::npos)
		{
			continue;
		}

		// Windows改行コード(\r)の除去
		if (!line.empty() && line.back() == '\r')
		{
			line.pop_back();
		}

		// AsoUtility::Split を使ってカンマで分割
		std::vector<std::string> tokens = AsoUtility::Split(line, ',');

		DialogueData data;
		if (tokens.size() >= 1) data.speaker = tokens[0]; // 名前・話者
		if (tokens.size() >= 2) data.message = tokens[1]; // メッセージ
		if (tokens.size() >= 3) data.eventTag = tokens[2]; // イベントタグ

		dialogueList_.push_back(data);
	}

	file.close();
	return true;
}

void StoryPhase::UpdateMainStory(void)
{
	// Enterキーが押されたらテキストを送る
	if (ins_.IsTrgDown(KEY_INPUT_RETURN))
	{
		if (currentLineIndex_ < dialogueList_.size())
		{
			// イベントタグのチェック
			std::string tag = dialogueList_[currentLineIndex_].eventTag;

			if (tag == "SELECT_ROUTE")
			{
				storyPhase_ = SELECT_ROUTE;
				return;
			}
			else if (tag == "START_BATTLE")
			{
				gameScene_.StartNextBattle();
				gameScene_.ChangePhase(GameScene::QUEST_PHASE::PHASE_BATTLE);
				return;
			}

			// 次の行へ進める
			currentLineIndex_++;
		}
	}
}

void StoryPhase::UpdateSelectRoute(void)
{
	if (ins_.IsTrgDown(KEY_INPUT_RETURN))
	{
		storyPhase_ = MAIN_STORY;
		currentLineIndex_++;
	}
}

void StoryPhase::UpdateEnding(void)
{
}
