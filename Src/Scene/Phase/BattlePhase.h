#pragma once
#include <memory>
#include <string>
#include "../../Manager/InputManager.h"
#include"../../Object/ActionGauge.h"
#include "PhaseBase.h"

class GameScene;
class Player;

class BattlePhase : public PhaseBase
{
public:

	enum BATTLE_PHASE
	{
		COMMAND_SELECT,	//コマンド選択
		PLAYER_TURN,	//プレイヤーのターン
		ENEMY_TURN,		//敵のターン
		BATTLE_END,		//戦闘終了
	};

	enum COMMAND_TYPE
	{
		ATTACK,			//攻撃
		TALK,			//話す
		RUN,			//逃げる
		MAX				//最大値
	};

	static constexpr int RUN_SUCCESS_RATE = 0;	//逃げれる確率(%)

	int EnemyHP = 100;	//敵のHP

	BattlePhase(GameScene& gameScene);
	~BattlePhase();
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void Release(void)override;
private:

	InputManager& ins_ = InputManager::GetInstance();

	GameScene& gameScene_;			//親の情報を渡す
	std::unique_ptr<Player> player_;	//プレイヤーの情報を渡す
	ActionGauge actionGauge_;		//アクションゲージ

	std::string currentMessage_; // 現在表示するメッセージ

	BATTLE_PHASE battlePhase_;		//バトルフェーズの種類
	COMMAND_TYPE commandType_;		//コマンドの種類

	bool isBarActive_ = false;				//アクションゲージが動いているかどうか

	int battleTurnCnt_;				//バトルのターン数をカウント

	void UpdateCommandSelect(void);	//コマンド選択の更新処理
	void UpdatePlayerTurn(void);	//プレイヤーのターンの更新処理
	void UpdateEnemyTurn(void);		//敵のターンの更新処理
	void UpdateBattleEnd(void);		//戦闘終了の更新処理

	void DrawCommandSelect(void);	//コマンド選択の描画処理
	void DrawPlayerTrun(void);		//プレイヤーのターンの描画処理
	void DrawEnemyTrun(void);		//敵のターンの描画処理

	void ProsesSelectAttack(void);	//攻撃コマンドの処理
	void ProsesSelectTalk(void);	//話すコマンドの処理
	void ProsesSelectRun(void);		//逃げるコマンドの処理
};