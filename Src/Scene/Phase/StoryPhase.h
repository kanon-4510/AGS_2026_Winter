#pragma once
#include "PhaseBase.h"

class GameScene;

class StoryPhase : public PhaseBase
{
public:
	StoryPhase(GameScene& gameScene);
	~StoryPhase();
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void Release(void)override;
private:

	GameScene& gameScene_;			//e‚Ìî•ñ‚ğ“n‚·

};

