#pragma once
#include "PhaseBase.h"

class GameScene;

class BattlePhase : public PhaseBase
{
public:

	enum class BATTLE_STEP
	{
		COMMAND_SELECT,	//コマンド選択
		PLAYER_ATTACK,	//プレイヤー攻撃
		ENEMY_ATTACK,	//敵攻撃
		MAX
	};

	enum class COMMAND_SELECT
	{
		ATTACK,	//攻撃
		TALK,	//会話
		RUN,	//逃走
		MAX
	};

	BattlePhase(GameScene& gameScene);
	~BattlePhase();
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void Release(void)override;
private:

	GameScene& gameScene_;			//親の情報を渡す

	BATTLE_STEP battleStep_;		//バトルステップ
	COMMAND_SELECT commandSelect_;	//コマンド選択

	void TurnManager(void);		//ターン管理

	void CommandSelect(void);	//コマンド選択
	void PlayerAttack(void);	//プレイヤー攻撃
	void EnemyAttack(void);		//敵攻撃

	// 引数に vector を使うことで、3択でも4択でも対応可能にします
	void DrawCommandMenu(int x, int y, const std::vector<std::string>& labels, int cursor, int interval = 40);

	// DrawCommandMenuの数に合わせて選択をできるようにする
	void ProcessCommandMenuSelection(int& cursor, int maxItems);

};