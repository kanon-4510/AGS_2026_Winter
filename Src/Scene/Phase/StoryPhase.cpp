#include <DxLib.h>
#include "../../Manager/InputManager.h"
#include "../GameScene.h"
#include "StoryPhase.h"

StoryPhase::StoryPhase(GameScene& gameScene)
	: gameScene_(gameScene)
{
	storyPhase_ = MAIN_STORY;
	endingType_ = STORY_WAVE_START;
}

StoryPhase::~StoryPhase()
{
}

void StoryPhase::Init()
{
}

void StoryPhase::Update()
{
	switch (storyPhase_)
	{
	case StoryPhase::MAIN_STORY:
		UpdateMainStory();
		break;
	case StoryPhase::SELECT_ROUTE:
		UpdateSelectRoute();
		break;
	case StoryPhase::ENDING:
		UpdateEnding();
		break;
	default:
		break;
	}
}

void StoryPhase::Draw()
{
	DrawString(100, 100, "Story Phase", 0xFFFFFF);
	switch (storyPhase_)
	{
	case StoryPhase::MAIN_STORY:
		DrawFormatString(100, 150, 0xFFFFFF, "Story Phase: %d", storyPhase_);
		break;
	case StoryPhase::SELECT_ROUTE:
		DrawString(100, 150, "âEÇ©ç∂Ç©", 0xFF0000);
		break;
	case StoryPhase::ENDING:
		break;
	}
}

void StoryPhase::Release()
{
}

void StoryPhase::UpdateMainStory(void)
{
	if (ins_.IsTrgDown(KEY_INPUT_RETURN))
	{
		storyPhase_ = SELECT_ROUTE;
	}
}

void StoryPhase::UpdateSelectRoute(void)
{
	if (ins_.IsTrgDown(KEY_INPUT_LEFT)
		|| ins_.IsTrgDown(KEY_INPUT_RIGHT))
	{
		gameScene_.ChangePhase(GameScene::QUEST_PHASE::PHASE_BATTLE);
	}
}

void StoryPhase::UpdateEnding(void)
{
}
