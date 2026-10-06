#include <DxLib.h>
#include <cstdlib>
#include "../Common/Color.h"
#include "Enemy.h"

Enemy::Enemy()
	: type_(TYPE::FROG)
	, hp_(0)
	, maxHp_(0)
	, atk_(0)
	, isAlive_(false)
	, x_(0)
	, y_(0)
	, imageHandle_(-1)
	, animationTime_(0.0f)
{
}

Enemy::~Enemy()
{
	Release();
}

void Enemy::Init(TYPE type)
{
	type_ = type;

	// 敵ごとのステータス
	switch (type_)
	{
	case TYPE::FROG:
		maxHp_ = 20;
		atk_ = 10;
		imageHandle_ = LoadGraph("Data/Enemy/Frog.png");
		break;

	case TYPE::GOBLIN:
		maxHp_ = 50;
		atk_ = 15;
		imageHandle_ = LoadGraph("Data/Enemy/Goblin.png");
		break;

	case TYPE::REAPER:
		maxHp_ = 45;
		atk_ = 20;
		imageHandle_ = LoadGraph("Data/Enemy/Reaper.png");
		break;

	case TYPE::SLIME:
		maxHp_ = 100;
		atk_ = 10;
		imageHandle_ = LoadGraph("Data/Enemy/Slime.png");
		break;

	case TYPE::DARKKNIGHT:
		maxHp_ = 60;
		atk_ = 25;
		imageHandle_ = LoadGraph("Data/Enemy/Darkknight.png");
		break;

	case TYPE::BAT:
		maxHp_ = 100;
		atk_ = 25;
		imageHandle_ = LoadGraph("Data/Enemy/Bat.png");
		break;

	case TYPE::DRAGON:
		maxHp_ = 80;
		atk_ = 15;
		imageHandle_ = LoadGraph("Data/Enemy/Dragon.png");
		break;

	case TYPE::DEVIL:
		maxHp_ = 200;
		atk_ = 30;
		imageHandle_ = LoadGraph("Data/Enemy/Devil.png");
		break;
	}


	hp_ = maxHp_;
	animationTime_ = 0.0f;
	isAlive_ = true;
}

void Enemy::Update()
{
	if (!isAlive_)
	{
		return;
	}

	animationTime_ += 1.0f / 60.0f;

	if (animationTime_ >= ANIMATION_TOTAL_TIME)
	{
		animationTime_ -= ANIMATION_TOTAL_TIME;
	}
}

void Enemy::Draw()
{
	if (!isAlive_)
	{
		return;
	}

	if (imageHandle_ != -1)
	{
		int width;
		int height;

		GetGraphSize(
			imageHandle_,
			&width,
			&height
		);

		float scale = 1.0f;

		if (width > height)
		{
			scale =
				static_cast<float>(ENEMY_MAX_SIZE) / width;
		}
		else
		{
			scale =
				static_cast<float>(ENEMY_MAX_SIZE) / height;
		}

		int drawWidth =
			static_cast<int>(width * scale);

		int drawHeight =
			static_cast<int>(height * scale);

		// 上に移動する量
		int moveY = 0;

		if (animationTime_ < ANIMATION_MOVE_TIME)
		{
			// 元の位置から上
			float rate =
				animationTime_ / ANIMATION_MOVE_TIME;

			moveY =
				static_cast<int>(
					-ANIMATION_MOVE_DISTANCE * rate
					);
		}
		else if (animationTime_ <
			ANIMATION_MOVE_TIME + ANIMATION_STOP_TIME)
		{
			// 上で停止
			moveY =
				static_cast<int>(
					-ANIMATION_MOVE_DISTANCE
					);
		}
		else if (animationTime_ <
			ANIMATION_MOVE_TIME +
			ANIMATION_STOP_TIME +
			ANIMATION_MOVE_TIME)
		{
			// 上から元の位置
			float rate =
				(animationTime_ -
					ANIMATION_MOVE_TIME -
					ANIMATION_STOP_TIME)
				/ ANIMATION_MOVE_TIME;

			moveY =
				static_cast<int>(
					-ANIMATION_MOVE_DISTANCE *
					(1.0f - rate)
					);
		}
		else
		{
			// 元の位置で停止
			moveY = 0;
		}

		int drawCenterY =
			ENEMY_CENTER_Y + moveY;

		DrawExtendGraph(
			ENEMY_CENTER_X - drawWidth / 2,
			drawCenterY - drawHeight / 2,
			ENEMY_CENTER_X + drawWidth / 2,
			drawCenterY + drawHeight / 2,
			imageHandle_,
			TRUE
		);

		// HP
		DrawFormatString(
			STATUS_X,
			HP_STATUS_Y,
			GetColor(255, 255, 255),
			"HP : %d / %d",
			hp_,
			maxHp_
		);

		// ATK
		DrawFormatString(
			STATUS_X,
			ATK_STATUS_Y,
			GetColor(255, 255, 255),
			"ATK : %d",
			atk_
		);
	}
}

void Enemy::Release()
{
	if (imageHandle_ != -1)
	{
		DeleteGraph(imageHandle_);
		imageHandle_ = -1;
	}
}

void Enemy::Damage(int damage)
{
	if (!isAlive_)
	{
		return;
	}

	if (damage < 0)
	{
		damage = 0;
	}

	hp_ -= damage;

	if (hp_ <= 0)
	{
		hp_ = 0;
		isAlive_ = false;
	}
}

bool Enemy::IsDead() const
{
	return !isAlive_;
}

int Enemy::GetHP() const
{
	return hp_;
}

int Enemy::GetMaxHP() const
{
	return maxHp_;
}

int Enemy::GetATK() const
{
	return atk_;
}

Enemy::TYPE Enemy::GetType() const
{
	return type_;
}

bool Enemy::IsBoss() const
{
	return type_ == TYPE::DEVIL;
}