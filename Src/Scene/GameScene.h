#pragma once
#include <memory>
#include "../Manager/InputManager.h"
#include "SceneBase.h"
#include "Phase/PhaseBase.h"

class StoryPhase;
class BattlePhase;

class GameScene : public SceneBase
{
public:
	enum QUEST_PHASE 
	{
		PHASE_STORY,	//ストーリー
		PHASE_BATTLE,	//戦闘
		MAX				//最大値
	};

	// エンディングの種類
	enum ENDING_TYPE
	{
		ENDING_NORMAL,
		ENDING_ESCAPE,
		ENDING_TRUE
	};

	// 戦闘結果の定数
	enum BATTLE_RESULT
	{
		RESULT_ATTACK, // 撃破
		RESULT_TALK,   // 話して解決
		RESULT_ESCAPE  // 逃走
	};

	//フォントサイズ
	static constexpr int STATUS_FONT_SIZE = 20;	//ステータスのフォントサイズ
	static constexpr int MENU_FONT_SIZE = 32;	//メニューのフォントサイズ

	//メッセージボックスの表示位置
	static constexpr int MESSAGE_BOX_X = 950;
	static constexpr int MESSAGE_BOX_Y = 50;

	//UI描画座標
	static constexpr int TURN_TEXT_X = 350;	//ターン数の表示位置X
	static constexpr int TURN_TEXT_Y = 30;	//ターン数の表示位置Y
	
	static constexpr int ROUTE_TEXT_X = 200;	//ルートの表示位置X
	static constexpr int ROUTE_TEXT_Y = 400;	//ルートの表示位置Y
	
	static constexpr int COMMAND_ATTACK_X = 315;	//コマンドメニューの表示位置X
	static constexpr int COMMAND_TALK_X = 540;		//コマンドメニューの表示位置X
	static constexpr int COMMAND_ESCAPE_X = 765;	//コマンドメニューの表示位置X
	static constexpr int COMMAND_MENU_Y = 620;		//コマンドメニューの表示位置Y
	static constexpr int COMMAND_MENU_STEP_Y = 60;	//コマンドメニューの選択肢間隔Y
	
	static constexpr int PLAYER_IMG_X = 700;	//プレイヤーの画像表示位置X
	static constexpr int PLAYER_IMG_Y = 200;	//プレイヤーの画像表示位置Y


	GameScene(void);		//デフォルトコンストラクタ
	~GameScene(void);		//デストラクタ

	void Init(void) override;		//初期化処理
	void Update(void) override;		//更新処理
	void Draw(void) override;		//描画処理
	void Release(void) override;	//解放処理

	void ChangePhase(QUEST_PHASE phase);	//フェーズの変更

	void StartNextBattle();
	void RecordBattleResult(BATTLE_RESULT result);	//戦闘結果の記録

	
	ENDING_TYPE JudgeEndingType() const;	//エンディング判定

	//ゲッター・セッター
	int GetWaveCount(void) const;	//wavecount取得
	void SetCurrentWave(int wave);	//wavecount設定
	BATTLE_RESULT GetLastBattleResult() const { return lastBattleResult_; }
	bool IsReturnFromBattle() const { return isReturnFromBattle_; }
	void SetReturnFromBattle(bool flag) { isReturnFromBattle_ = flag; }

	// 戦績ゲッター
	int GetAttackWinCount() const { return battleCount_; }
	int GetTalkWinCount() const { return talkCount_; }
	int GetEscapeCount() const { return escapeCount_; }

	void NextWave(void);
	bool IsEnemyUsed(int index) const;
	void SetEnemyUsed(int index);

private:

	InputManager& ins_ = InputManager::GetInstance();
	std::unique_ptr<PhaseBase> phase_;
	std::unique_ptr<StoryPhase> storyPhase_;
	std::unique_ptr<BattlePhase> battlePhase_;

	QUEST_PHASE questPhase_;	//クエストのフェーズ

	int waveCount_ = 1;			//現在のターン数
	int battleCount_ = 0;		//敵を倒した回数
	int talkCount_ = 0;		//話して解決した回数
	int escapeCount_ = 0;		//逃げた回数
	
	BATTLE_RESULT lastBattleResult_; // 直前の戦闘結果
	bool isReturnFromBattle_;     // 戦闘からの復帰フラグ

	bool usedEnemies_[7] = {};

	bool hasStartedBattle_ = false;

	int playerImg_; //プレイヤーの画像ハンドル
	int stageImg_;  //ステージの画像ハンドル
	int messageBoxImg_; //メッセージボックスの画像ハンドル
};