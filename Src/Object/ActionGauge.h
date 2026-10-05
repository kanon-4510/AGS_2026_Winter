#pragma once

#include "../Application.h"

struct GaugeParam {
    float greenWidth;  // GOOD幅 (例: 0.30f)
    float yellowWidth; // GREAT幅 (例: 0.20f)
    float redWidth;    // PERFECT幅 (例: 0.10f)
    float speed;       // バーの速度 (例: 1.0f)
};

//ゲージの状態
enum class GaugeState
{
	PERFECT, //100% (攻撃:100%ダメ / 防御:被ダメ0%)
	GREAT,   //50%  (攻撃:50%ダメ  / 防御:被ダメ50%)
	GOOD,    //10%  (攻撃:10%ダメ  / 防御:被ダメ90%)
	MISS     //0%   (攻撃:ミス     / 防御:被ダメ100%)
};

class ActionGauge
{
public:

    // UIレイアウト用の固定値（定数）
    static constexpr int GAUGE_X = Application::SCREEN_SIZE_X / 4; // 左端X座標
    static constexpr int GAUGE_Y = 380; // Y座標（固定）
    static constexpr int GAUGE_WIDTH = 600; // 全体の横幅
    static constexpr int GAUGE_HEIGHT = 150;  // 高さ（固定）

    // 基準となる判定幅 (全体の横幅に対する割合)
    static constexpr float BASE_PERFECT_WIDTH = 0.10f;
    static constexpr float BASE_GREAT_WIDTH = 0.40f;
    static constexpr float BASE_GOOD_WIDTH = 0.35f;

    ActionGauge();

    // 敵データから判定幅・スピードを受け取って起動
    void Start(float greenW, float yellowW, float redW, float speed = 1.0f);
    void Update(float deltaTime);
    void Draw();
    GaugeState PressButton();

    // 状態確認用
    bool IsMoving() const { return isMoving_; }   // バーが動いているか
    bool IsVisible() const { return isVisible_; } // 画面に表示されているか
    void Hide() { isVisible_ = false; isMoving_ = false; } // ゲージを完全に非表示にする

	void Reset();   //ゲージを初期状態に戻す

    static float GetResultRate(GaugeState state);

private:

    float barPosition_; //バーの現在位置 (0.0f ～ 1.0f)
    float barSpeed_;    //移動速度
    bool isVisible_;    //動作中フラグ
    bool isMoving_;     //バーを動かすかどうか（移動フラグ）

    // 各判定ゾーンの領域（開始・終了比率 0.0f ～ 1.0f）
    float greenStart_, greenEnd_;
    float yellowStart_, yellowEnd_;
    float redStart_, redEnd_;
};

