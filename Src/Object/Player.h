#pragma once
#include <string>
#include <vector>

class Player
{
public:

	constexpr static int FONT_SIZE = 20;			//fontサイズ

	constexpr static int STATUS_X = 450;			//ステータスの描画位置X
	constexpr static int STATUS_COLOR = 0xFFFFFF;	//ステータスの描画色

	constexpr static int PLAYER_POS_X = 900;	//プレイヤーの描画位置X
	constexpr static int PLAYER_POS_Y = 240;	//プレイヤーの描画位置Y

	constexpr static int ANIM_COUNT_ATTACK = 5;	//アニメーションのフレーム数
	constexpr static int ANIM_COUNT_DAMAGE = 10;	//アニメーションのフレーム数
	constexpr static int ANIM_MOVE_PIXELS = 3;	//動かすピクセル数

	static constexpr int BASE_Y = 600;		//ステータスの描画位置Y

	//プレイヤーのステータス
	static constexpr int MAX_HP = 100;        // ステータス名の描画X座標
	static constexpr int POWER = 10;        // ステータス名の描画X座標

	//レベルアップ時のステータス上昇値
	static constexpr int HP_UP = 25;        //レベルアップ時のHP上昇値
	static constexpr int POWER_UP = 5;      //レベルアップ時の攻撃力上昇値

	Player();

	//初期化処理
	void Init();

	//更新処理
	void Update();

	//描画処理
	void Draw();

	float Attack(float power);//攻撃処理
	float Defense(float defense);//防御処理

	//回復処理
	void Heal();
	void FullHeal();

	void LevelUp();//レベルアップ処理

	void Damage(int damage);//ダメージ処理
	void Death();	//死亡処理

	int GetMaxHp();//HPを外から参照できるようにする

private:

	int maxHp_ = MAX_HP;	//最大HP
	int hp_ = maxHp_;		//現在のHP
	int power_ = POWER;		//攻撃力
	float defense_ = 0.0f;	//防御力

	float heal_ = 0.4f;			//回復量

};