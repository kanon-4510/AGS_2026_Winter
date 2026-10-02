#pragma once
#include <string>
#include <vector>

class Player
{
public:

	constexpr static int FONT_SIZE = 20;			//fontサイズ

	constexpr static int STATUS_X = 450;			//ステータスの描画位置X
	constexpr static int STATUS_COLOR = 0xFFFFFF;	//ステータスの描画色

	constexpr static int STATUS_BONUS_X = 570;	//ステータスのボーナス分の描画位置X
	constexpr static int STATUS_BONUS_COLOR = 0x00FF00;	//ステータスのボーナス分の描画色

	constexpr static int NEED_EXP = 30;			//必要経験値
	constexpr static int RATE_BASE = 100;		//ステータスアップの確率の基準値
	constexpr static int SKILL_UP_RATE = 70;	//ステータスアップの確率

	constexpr static int PLAYER_POS_X = 900;	//プレイヤーの描画位置X
	constexpr static int PLAYER_POS_Y = 240;	//プレイヤーの描画位置Y

	constexpr static int ANIM_COUNT_ATTACK = 5;	//アニメーションのフレーム数
	constexpr static int ANIM_COUNT_DAMAGE = 10;	//アニメーションのフレーム数
	constexpr static int ANIM_MOVE_PIXELS = 3;	//動かすピクセル数

	// 技能計算の除数
	constexpr static int PHARMACY_DIVISOR = 7;       // 薬学ボーナス除数
	constexpr static int MARTIAL_ARTS_DIVISOR = 5;   // 武術ボーナス除数
	constexpr static int FAITH_DIVISOR = 15;         // 信仰ボーナス除数
	constexpr static int ARCHAEOLOGY_DIVISOR = 8;    // 考古学ボーナス除数
	constexpr static int ASTROLOGY_DIVISOR = 5;      // 占星術ボーナス除数

	// 定数設定（行間と各表示エリアの起点Y座標）
	static constexpr int BASE_Y = 600;             // 基礎ステータスの表示起点Y
	static constexpr int SKILL_BASE_Y = 310;       // 技能ステータスの表示起点Y
	static constexpr int LINE_HEIGHT = 30;         // 行間
	static constexpr int JOB_BONUS_OFFSET_X = 100; // 職業ボーナスの描画Xオフセット

	//プレイヤーのステータス
	static constexpr int MAX_HP = 100;        // ステータス名の描画X座標
	static constexpr int POWER = 10;        // ステータス名の描画X座標


	int maxHp_ = MAX_HP;
	int hp_ = maxHp_;
	int power_ = POWER;
	float defense_ = 0.0f;	//防御力
	
	int heal_ = 2;	//回復量

	Player();

	void Init();	//初期化処理

	//更新処理
	void Update();

	void Draw();//描画処理

	float Attack(float power);//攻撃処理
	float Defense(float defense);//防御処理

	//回復処理
	void Heal(int amount);
	void FullHeal();

	void Damage(int damage);//ダメージ処理
	void Death();	//死亡処理

	int GetMaxHp();//HPを外から参照できるようにする

private:
	
};