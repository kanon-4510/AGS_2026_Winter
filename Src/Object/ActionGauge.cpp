#include <DxLib.h>
#include <cmath>
#include "../Common/Color.h"
#include "ActionGauge.h"

// 仮定数 (敵パラメータが未設定の場合に使用)　※後で消す
namespace DummyEnemyData {
	constexpr float PERFECT_FACTOR = 1.0f; // PERFECT判定の倍率
	constexpr float GREAT_FACTOR = 1.0f; // GREAT判定の倍率
	constexpr float GOOD_FACTOR = 1.0f; // GOOD判定の倍率
}

ActionGauge::ActionGauge()
    : barPosition_(0.0f)
    , barSpeed_(1.0f)
    , isActive_(false)
    , greenStart_(0.0f), greenEnd_(0.0f)
    , yellowStart_(0.0f), yellowEnd_(0.0f)
    , redStart_(0.0f), redEnd_(0.0f)
{
}

void ActionGauge::Start(float greenW, float yellowW, float redW, float speed)
{
    //float margin = 0.02f; // 余白比率

    //greenStart_ = margin;
    //greenEnd_ = greenStart_ + greenW;

    //yellowStart_ = greenEnd_ + margin;
    //yellowEnd_ = yellowStart_ + yellowW;

    //redStart_ = yellowEnd_ + margin;
    //redEnd_ = redStart_ + redW;

    //barPosition_ = 0.0f;
    //barSpeed_ = speed;
    //isActive_ = true;

    float margin = 0.05f; // 各ゾーン間の余白比率 (2%)

    // 最初のミスゾーンの大きさ（0.0f ～ 1.0f の比率）
    float initialMissWidth = 0.03f;

    // 1. 【緑 (GOOD)】 最初のミスゾーンの直後から配置
    greenStart_ = initialMissWidth;
    greenEnd_ = greenStart_ + greenW; // param.greenWidth ではなく greenW

    // 2. 【黄 (GREAT)】 緑の直後に配置
    yellowStart_ = greenEnd_ + margin;
    yellowEnd_ = yellowStart_ + yellowW; // param.yellowWidth ではなく yellowW

    // 3. 【赤 (PERFECT)】 黄の直後に配置（全体が右側にずれる）
    redStart_ = yellowEnd_ + margin;
    redEnd_ = redStart_ + redW; // param.redWidth ではなく redW

    barPosition_ = 0.0f;
    barSpeed_ = speed; // param.speed ではなく speed
    isActive_ = true;
}

void ActionGauge::Update(float deltaTime)
{
    if (!isActive_) return;

    barPosition_ += barSpeed_ * deltaTime;

    // 右端で折り返し
    if (barPosition_ >= 1.0f) {
        barPosition_ = 1.0f;
        barSpeed_ = -fabsf(barSpeed_);
    }
    // 左端まで戻ってきたら時間切れ (MISS)
    else if (barSpeed_ < 0.0f && barPosition_ <= 0.0f) {
        barPosition_ = 0.0f;
        isActive_ = false;
    }
}

void ActionGauge::Draw()
{
    // 色設定
    unsigned int colorBg = GetColor(0, 0, 0); // 枠内背景色（薄グレー）
    unsigned int colorBorder = GetColor(255, 255, 255);     // 外枠線（濃灰色）

    // --------------------------------------------------
    // 1. 【メッセージ枠（土台）】（常に描画）
    // --------------------------------------------------
    DrawBox(GAUGE_X, GAUGE_Y, GAUGE_X + GAUGE_WIDTH, GAUGE_Y + GAUGE_HEIGHT, colorBg, TRUE);
    DrawBox(GAUGE_X, GAUGE_Y, GAUGE_X + GAUGE_WIDTH, GAUGE_Y + GAUGE_HEIGHT, colorBorder, FALSE);
    DrawBox(GAUGE_X + 1, GAUGE_Y + 1, GAUGE_X + GAUGE_WIDTH - 1, GAUGE_Y + GAUGE_HEIGHT - 1, colorBorder, FALSE);

    // --------------------------------------------------
    // 2. 【判定ゾーン & バー】（isActive_ が true の時だけ上乗せ描画）
    // --------------------------------------------------
    if (isActive_) {
        unsigned int colorGreen = GetColor(80, 200, 80);   // 緑 (GOOD)
        unsigned int colorYellow = GetColor(255, 220, 0);   // 黄 (GREAT)
        unsigned int colorRed = GetColor(255, 60, 60);    // 赤 (PERFECT)

        // ゾーン描画処理
        auto DrawZone = [&](float startRatio, float endRatio, unsigned int color) {
            int left = GAUGE_X + static_cast<int>(startRatio * GAUGE_WIDTH);
            int right = GAUGE_X + static_cast<int>(endRatio * GAUGE_WIDTH);
            DrawBox(left, GAUGE_Y + 2, right, GAUGE_Y + GAUGE_HEIGHT - 2, color, TRUE);
            };

        DrawZone(greenStart_, greenEnd_, colorGreen);
        DrawZone(yellowStart_, yellowEnd_, colorYellow);
        DrawZone(redStart_, redEnd_, colorRed);

        // 動くバー（ポインタ）描画
        int currentX = GAUGE_X + static_cast<int>(barPosition_ * GAUGE_WIDTH);
        DrawLine(currentX, GAUGE_Y - 4, currentX, GAUGE_Y + GAUGE_HEIGHT + 4, GetColor(255, 255, 255), 4);
    }
}

GaugeState ActionGauge::PressButton()
{
    if (!isActive_) return GaugeState::MISS;

    isActive_ = false; // ボタン押下で即停止

    // 現在位置 (barPosition_) と各ゾーンの範囲を判定
    if (barPosition_ >= redStart_ && barPosition_ <= redEnd_) {
        return GaugeState::PERFECT;
    }
    if (barPosition_ >= yellowStart_ && barPosition_ <= yellowEnd_) {
        return GaugeState::GREAT;
    }
    if (barPosition_ >= greenStart_ && barPosition_ <= greenEnd_) {
        return GaugeState::GOOD;
    }

    return GaugeState::MISS;
}

float ActionGauge::GetResultRate(GaugeState state)
{
    switch (state) {
    case GaugeState::PERFECT: return 1.0f;
    case GaugeState::GREAT:   return 0.5f;
    case GaugeState::GOOD:    return 0.1f;
    case GaugeState::MISS:
    default:                  return 0.0f;
    }
}

void ActionGauge::Reset()
{
    barPosition_ = 0.0f;
    barSpeed_ = 1.0f;
    isActive_ = false;
}
