#pragma once
#include <memory>
#include <map>
#include <functional>
#include <string>
#include <vector>

class SceneManager;
class GameScene;
class Player;

class Enemy
{
public:

	// 敵の種類
	enum class TYPE
	{
		FROG,
		GOBLIN,
		REAPER,
		SLIME,
		DARKKNIGHT,
		BAT,
		DRAGON,
		DEVIL
	};

	// 敵の表示設定
	static constexpr int ENEMY_MAX_SIZE = 200;
	static constexpr int ENEMY_CENTER_X = 640;
	static constexpr int ENEMY_CENTER_Y = 220;

	// 敵情報の表示位置
	static constexpr int STATUS_X = 890;
	static constexpr int HP_STATUS_Y = 230;
	static constexpr int ATK_STATUS_Y = 250;

	// アニメーション設定
	static constexpr float ANIMATION_MOVE_TIME = 0.2f;
	static constexpr float ANIMATION_STOP_TIME = 0.2f;
	static constexpr float ANIMATION_MOVE_DISTANCE = 10.0f;
	static constexpr float ANIMATION_TOTAL_TIME = 1.0f;

	Enemy();
	~Enemy();

	// 初期化
	void Init(TYPE type);

	// 更新
	void Update();

	// 描画
	void Draw();

	// 処理
	void Release();

	// ダメージを受ける
	void Damage(int damage);

	// 死亡しているか
	bool IsDead() const;

	// HP取得
	int GetHP() const;

	// 最大HP取得
	int GetMaxHP() const;

	// ATK取得
	int GetATK() const;

	// 敵種類取得
	TYPE GetType() const;

	// ボスか
	bool IsBoss() const;

private:

	// 敵種類
	TYPE type_;

	// 現在HP
	int hp_;

	// 最大HP
	int maxHp_;

	// 攻撃力
	int atk_;

	// 生存しているか
	bool isAlive_;

	// アニメーション
	float animationTime_;

	// 仮表示用
	int x_;
	int y_;

	// 敵画像
	int imageHandle_;
};