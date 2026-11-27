#include <DxLib.h>
#include <fstream>
#include <sstream>
#include <algorithm>
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

	bool retFlag;
	CSVInit(retFlag);
	if (retFlag) return;

	for (int i = 0; i < 5; ++i)
	{
		usedEnemies_[i] = false;
	}

	if (SceneManager::GetInstance().IsStoryMode())
	{
		ChangePhase(QUEST_PHASE::PHASE_STORY);
	}
	else
	{
		ChangePhase(QUEST_PHASE::PHASE_BATTLE);
	}
}

void GameScene::CSVInit(bool& retFlag)
{
	retFlag = true;
	//Enemy.csvファイルを読み込む
	std::ifstream file("Data/CSV/Enemy.CSV");
	if (!file.is_open()) {
		return;
	}

	std::string line;
	// 1行目のヘッダー（名前,メッセージ,イベントタグ）をスキップ
	std::getline(file, line);

	while (std::getline(file, line)) {
		if (line.empty()) continue;

		std::stringstream ss(line);
		std::string name, msg, tag;

		if (std::getline(ss, name, ',') &&
			std::getline(ss, msg, ',') &&
			std::getline(ss, tag, ',')) {

			// ダブルクォーテーションの除去
			if (!msg.empty() && msg.front() == '"' && msg.back() == '"') {
				msg = msg.substr(1, msg.size() - 2);
			}

			// \n を実際の改行に変換
			msg = unescapeNewlines(msg);

			dialogues.push_back({ name, msg, tag });
		}
	}
	file.close();

	// 例：戦闘開始時のセリフを早速取得してみる
	//currentMessage = getMessage("カエル", "START_BATTLE");
	retFlag = false;
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
	
	//デバッグ情報の描画
	//DrawDebug();
}

//解放処理
void GameScene::Release(void)
{
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

	//フェーズ初期化（生成直後に必ず Init を呼ぶ）
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

GameScene::ENDING_TYPE GameScene::JudgeEndingType(void) const
{
	//逃走を３回以上かつWave5で逃げていれば逃走エンド
	if (escapeCount_ >= 3 && isBossDestroyed_ == false)
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
	if (waveCount_ < 6)
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

bool GameScene::IsStory() const
{
	return SceneManager::GetInstance().IsStoryMode();
}

// 条件に合うセリフをランダムで取得する関数
std::string GameScene::getMessage(const std::string& enemyName, const std::string& eventTag, int index) 
{
	std::vector<std::string> candidates;

	for (const auto& d : dialogues) {
		if (d.enemyName == enemyName && d.eventTag == eventTag) {
			candidates.push_back(d.message);
		}
	}

	//クラッシュ防止のため、見つからない場合は必ず文字列を返して抜ける
	if (candidates.empty()) {
		return "（未検出: " + enemyName + " / " + eventTag + "）";
	}

	//番号が指定されている場合（0, 1, 2...）は、その順番のセリフを返す
	if (index >= 0) {
		if (index < static_cast<int>(candidates.size())) {
			return candidates[index];
		}
		// 指定された番号がCSVの行数を超えていたら、最後の行を返す
		return candidates.back();
	}

	//番号が指定されていない場合（-1）は、これまで通りランダムで返す
	int randIndex = GetRand(static_cast<int>(candidates.size()) - 1);
	return candidates[randIndex];
}

// 文字列中の "\\n" を実際の改行文字 "\n" に置換する処理
std::string GameScene::unescapeNewlines(const std::string& str)
{
	std::string result = str;
	size_t pos = 0;
	while ((pos = result.find("\\n", pos)) != std::string::npos) {
		result.replace(pos, 2, "\n");
		pos += 1;
	}
	return result;
}

void GameScene::DrawDebug(void)
{
	DrawFormatString(0, 0, GetColor(255, 255, 255), "Battle Count: %d", battleCount_);
	DrawFormatString(0, 20, GetColor(255, 255, 255), "Talk Count: %d", talkCount_);
	DrawFormatString(0, 40, GetColor(255, 255, 255), "Escape Count: %d", escapeCount_);
}
