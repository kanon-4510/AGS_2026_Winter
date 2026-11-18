#include "Player.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SoundManager.h"

Player::Player()
{
}

void Player::Init()
{
	hp_ = maxHp_;
	power_ = POWER;
	defense_ = 0.0f;
}

void Player::Update()
{
}

void Player::Draw()
{
	DrawFormatString(STATUS_X, STATUS_Y, STATUS_COLOR, "HP: %d", hp_);
	DrawFormatString(STATUS_X + STATUS_OFFSET, STATUS_Y, STATUS_COLOR, "ATK: %d", power_);
	//DrawFormatString(STATUS_X, BASE_Y+40, STATUS_COLOR, "DEF: %.1f", defense_);

	//HPバーの描画
	DrawHpBar(BAR_X, BAR_Y, BAR_WIDTH, BAR_HEIGHT);
}

float Player::Attack(float power)
{
	power_ = POWER * power;
	return power_;
}

float Player::Defense(float defense)
{
	//防御力0.0～1.0を受け取る
	return defense_ = defense;
}

void Player::Heal()
{
	//HPが最大値を超えないように40%回復する
	if(hp_ + (maxHp_ * heal_) > maxHp_)
	{
		hp_ = maxHp_;
	}
	else
	{
		hp_ += (maxHp_ * heal_);
	}
}

void Player::FullHeal()
{
	hp_ = GetMaxHp();
}

void Player::LevelUp()
{
	maxHp_ += HP_UP;
	hp_ = maxHp_;
	power_ += POWER_UP;
}

void Player::Damage(int damage)
{
	//防御力を考慮してダメージを計算
	hp_ -= damage - (damage * defense_);

	if (hp_ <= 0)
	{
		Death();
	}
}

void Player::Death()
{
	//クエストBGMを止める
	SoundManager::GetInstance().Stop(SoundManager::SRC::QUEST_BGM);

	SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::OVER);
}

int Player::GetMaxHp()
{
	return maxHp_;
}

int Player::GetHP()
{
	return hp_;
}

void Player::DrawHpBar(int x, int y, int width, int height)
{
	//HPバーの描画
	DrawBox(x, y, x + width, y + height, 0xFF0000, true); //赤い背景
	DrawBox(x, y, x + (width * hp_ / maxHp_), y + height, 0x00FF00, true); //緑のHPバー
}
