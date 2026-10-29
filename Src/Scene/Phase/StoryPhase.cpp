#include <DxLib.h>
#include <fstream>
#include "../../Utility/ASOUtility.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../GameScene.h"
#include "StoryPhase.h"

//シナリオCSVファイルのパス定義（.csvに対応）
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

	int currentWave = gameScene_.GetWaveCount(); // 現在のWave (1～5)

	//戦闘からの復帰時
	if (gameScene_.IsReturnFromBattle())
	{
		// 復帰フラグを下ろす
		gameScene_.SetReturnFromBattle(false);

		//ボス戦（Wave 5）終了後の判定ならエンディングCSVへ切り替え
		if (currentWave == 5)
		{
			GameScene::ENDING_TYPE ending = gameScene_.JudgeEndingType();
			switch (ending)
			{
			case GameScene::ENDING_TYPE::ENDING_NORMAL:
				LoadScenario(STORY_NORMAL_END); // ending_normal.csv
				break;
			case GameScene::ENDING_TYPE::ENDING_ESCAPE:
				LoadScenario(STORY_ESCAPE_END); // ending_escape.csv
				break;
			case GameScene::ENDING_TYPE::ENDING_TRUE:
				LoadScenario(STORY_TRUE_END);   // ending_true.csv
				break;
			}
			//エンディング専用のステートに切り替える
			storyPhase_ = ENDING;
			currentLineIndex_ = 0;

			return;
		}


		//修正箇所：まず現在の Wave の CSV をロードして会話リスト（dialogueList_）を構築する！
		SCENARIO_TYPE waveType = static_cast<SCENARIO_TYPE>(currentWave - 1);
		LoadScenario(waveType);

		//真ボス戦（Wave 6）からの復帰時はエンディングステートにする
		if (currentWave == 6)
		{
			storyPhase_ = ENDING;
		}

		// データが読み込まれた状態（dialogueList_ が入った状態）で分岐タグへジャンプ
		GameScene::BATTLE_RESULT result = gameScene_.GetLastBattleResult();
		OnReturnFromBattle(result);
	}
	//通常のシナリオ開始時（初回または次のWave進捗時）
	else
	{
		//Wave数に対応するシナリオ（STORY_WAVE_1 〜 STORY_WAVE_5）をロード
		SCENARIO_TYPE waveType = static_cast<SCENARIO_TYPE>(currentWave - 1);
		LoadScenario(waveType);
		currentLineIndex_ = 0;
	}
}

void StoryPhase::Update()
{
	// スペースキーで次の戦闘に進む(デバッグ)
	if (ins_.IsTrgDown(KEY_INPUT_SPACE))
	{
		gameScene_.NextWave();
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
		DrawBox(80, 350, 600, 350 + static_cast<int>(choiceList_.size()) * 40, GetColor(0, 0, 50), TRUE);

		for (size_t i = 0; i < choiceList_.size(); ++i)
		{
			int drawY = 370 + static_cast<int>(i) * 40;

			// カーソル（→）の描画
			if (i == selectCursor_)
			{
				DrawString(100, drawY, "→", 0xFFFF00);
			}

			// 選択肢テキストの描画
			DrawString(130, drawY, choiceList_[i].text.c_str(), 0xFFFFFF);
		}
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
		//空行やヘッダー行（名前,メッセージ... / 話者,メッセージ...）をスキップ
		if (line.empty() || line.find("名前,メッセージ") != std::string::npos || line.find("話者,メッセージ") != std::string::npos)
		{
			continue;
		}

		//Windows改行コード(\r)の除去
		if (!line.empty() && line.back() == '\r')
		{
			line.pop_back();
		}

		//AsoUtility::Split を使ってカンマで分割
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

void StoryPhase::OnReturnFromBattle(GameScene::BATTLE_RESULT result)
{
	std::string targetTag = "";
	switch (result)
	{
	case GameScene::BATTLE_RESULT::RESULT_ATTACK:
		targetTag = "BRANCH_ATTACK";
		break;
	case GameScene::BATTLE_RESULT::RESULT_TALK:
		targetTag = "BRANCH_TALK";
		break;
	case GameScene::BATTLE_RESULT::RESULT_ESCAPE:
		targetTag = "BRANCH_ESCAPE";
		break;
	}

	//CSV内のtargetTag が書かれている行までインデックスを移動
	for (size_t i = 0; i < dialogueList_.size(); ++i)
	{
		if (dialogueList_[i].eventTag == targetTag)
		{
			currentLineIndex_ = static_cast<int>(i);
			return;
		}
	}
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
				choiceList_.clear();
				int idx = currentLineIndex_ + 1;
				
				while (idx < dialogueList_.size() && !dialogueList_[idx].message.empty() && !dialogueList_[idx].eventTag.empty())
				{
					choiceList_.push_back({ dialogueList_[idx].message, dialogueList_[idx].eventTag });
					idx++;
				}
				selectCursor_ = 0;
				perPhase_ = storyPhase_; // 現在のフェーズを保持
				storyPhase_ = SELECT_ROUTE;
				return;
			}
			else if (tag == "START_BATTLE")
			{
				gameScene_.StartNextBattle();
				gameScene_.ChangePhase(GameScene::QUEST_PHASE::PHASE_BATTLE);
				return;
			}
			else if (tag == "JUMP_MESS")
			{
				std::string nextTag = dialogueList_[currentLineIndex_].eventTag;

				JumpToTag("MESS_POINT"); // 実際の合流地点のタグへジャンプ
				return;
			}
			//次のWaveに進む処理を追加！
			else if (tag == "GO_NEXT_WAVE")
			{
				//GameScene側のWaveカウントをインクリメント（1進める）
				gameScene_.NextWave();

				//復帰フラグが残っていれば確実にオフにする
				gameScene_.SetReturnFromBattle(false);

				//次のWaveのCSV（例: Wave 2）を読み込んで画面を初期化する
				Init();
				return;
			}
			else if(tag == "BAD_END")
			{
				// エンディングの最後に到達したらクリアシーンへ移行
				SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::OVER);
				return;
			}
			// 次の行へ進める
			currentLineIndex_++;

			//他の分岐タグを跨がないようにスキップガードをかける
			if (currentLineIndex_ < dialogueList_.size())
			{
				std::string nextTag = dialogueList_[currentLineIndex_].eventTag;

				// 分岐セリフの読了後に、別の分岐タグ（BRANCH_...）に遭遇したら GO_NEXT_WAVE 行まで飛ぶ
				if (nextTag == "SKIP_TEXT")
				{
					JumpToTag("GO_NEXT_WAVE");
					return;
				}
			}
		}
	}
}

void StoryPhase::UpdateSelectRoute(void)
{
	if (ins_.IsTrgDown(KEY_INPUT_UP))
	{
		selectCursor_ = (selectCursor_ - 1 + static_cast<int>(choiceList_.size())) % static_cast<int>(choiceList_.size());
	}
	else if (ins_.IsTrgDown(KEY_INPUT_DOWN))
	{
		selectCursor_ = (selectCursor_ + 1) % static_cast<int>(choiceList_.size());
	}

	if (ins_.IsTrgDown(KEY_INPUT_RETURN) && !choiceList_.empty())
	{
		std::string targetTag = choiceList_[selectCursor_].targetTag;
		if (targetTag == "CHOICE_LEFT")
		{
			targetTag = "CHOICE_TO_LEFT";
		}
		else if (targetTag == "CHOICE_RIGHT")
		{
			targetTag = "CHOICE_TO_RIGHT";
		}

		storyPhase_ = perPhase_;
		JumpToTag(targetTag);
	}
}

void StoryPhase::UpdateEnding(void)
{
	// Enterキーが押されたらエンディングのテキストを進める
	if (ins_.IsTrgDown(KEY_INPUT_RETURN))
	{
		if (currentLineIndex_ < dialogueList_.size())
		{
			std::string tag = dialogueList_[currentLineIndex_].eventTag;

			// エンディングの最後に到達したらクリアシーンへ移行
			if (tag == "END_GAME")
			{
				SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::CLEAR);
				return;
			}
			else if (tag == "SELECT_ROUTE")
			{
				choiceList_.clear();
				int idx = currentLineIndex_ + 1;

				while (idx < dialogueList_.size() && !dialogueList_[idx].message.empty() && !dialogueList_[idx].eventTag.empty())
				{
					choiceList_.push_back({ dialogueList_[idx].message, dialogueList_[idx].eventTag });
					idx++;
				}
				selectCursor_ = 0;
				perPhase_ = storyPhase_; // 現在のフェーズを保持
				storyPhase_ = SELECT_ROUTE;
				return;
			}
			else if (tag == "START_BATTLE")
			{
				gameScene_.NextWave();
				gameScene_.StartNextBattle();
				gameScene_.ChangePhase(GameScene::QUEST_PHASE::PHASE_BATTLE);
				return;
			}

			//次の行へ進める
			currentLineIndex_++;
		}
	}
}

// 目的のタグがある行までインデックスをスキップする補助関数
void StoryPhase::JumpToTag(const std::string& targetTag)
{
	for (size_t i = 0; i < dialogueList_.size(); ++i)
	{
		std::string tag = dialogueList_[i].eventTag;
		while (!tag.empty() && (tag.back() == '\r' || tag.back() == ' ' || tag.back() == '\n'))
		{
			tag.pop_back();
		}

		if (tag == targetTag)
		{
			currentLineIndex_ = static_cast<int>(i);
			return;
		}
	}
}