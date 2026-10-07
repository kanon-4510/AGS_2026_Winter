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
	hp_ -= damage * defense_;

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
