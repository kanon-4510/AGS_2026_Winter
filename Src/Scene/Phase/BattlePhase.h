#pragma once
#include <memory>
#include <string>
#include "../../Manager/InputManager.h"
#include"../../Object/ActionGauge.h"
#include "PhaseBase.h"

class GameScene;
class Player;
class Enemy;

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

	static constexpr int MAX_COMMANDS = 3;		//コマンドの最大数

	static constexpr int ARROW_POS_X = 295; //矢印の表示位置
	static constexpr int ARROW_OFFSET = 225; //矢印のオフセット

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
	std::unique_ptr<Enemy> enemy_;		//敵の情報を渡す
	ActionGauge actionGauge_;		//アクションゲージ

	std::string currentMessage_; // 現在表示するメッセージ

	BATTLE_PHASE battlePhase_;		//バトルフェーズの種類
	COMMAND_TYPE commandType_;		//コマンドの種類

	bool isBarActive_ = false;		//アクションゲージが動いているかどうか

	bool usedEnemies_[7];			//出現済みの敵を管理

	int battleTurnCnt_;				//バトルのターン数をカウント
	int talkSuccessCount_ = 0;		//現在の説得成功回数

	void SpawnEnemy();				// 敵の初期化処理

	int selectedIndex_ = 0;			//選択中のコマンドのインデックス

	int attackImage_;				//攻撃画像
	int talkImage_;					//会話画像
	int escapeImage_;				//逃走画像

	void UpdateCommandSelect(void);	//コマンド選択の更新処理
	void UpdatePlayerTurn(void);	//プレイヤーのターンの更新処理
	void UpdateEnemyTurn(void);		//敵のターンの更新処理
	void UpdateBattleEnd(void);		//戦闘終了の更新処理

	void DrawCommandSelect(void);	//コマンド選択の描画処理
	void DrawPlayerTurn(void);		//プレイヤーのターンの描画処理
	void DrawEnemyTurn(void);		//敵のターンの描画処理

	void ProsesSelectAttack(void);	//攻撃コマンドの処理
	void ProsesSelectTalk(void);	//話すコマンドの処理
	void ProsesSelectRun(void);		//逃げるコマンドの処理

};