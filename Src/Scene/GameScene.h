#pragma once
#include <memory>
#include "../Manager/InputManager.h"
#include "SceneBase.h"
#include "../Object/PlayerStatus.h"
#include "Phase/PhaseBase.h"

class StoryPhase;
class BattlePhase;

class GameScene : public SceneBase
{
public:
	enum QUEST_PHASE 
	{
		PHASE_STORY,	//ストーリー
		PHASE_BATTLE,	//戦闘
		MAX				//最大値
	};

	//フォントサイズ
	static constexpr int STATUS_FONT_SIZE = 20;	//ステータスのフォントサイズ
	static constexpr int MENU_FONT_SIZE = 32;	//メニューのフォントサイズ

	//メッセージボックスの表示位置
	static constexpr int MESSAGE_BOX_X = 950;
	static constexpr int MESSAGE_BOX_Y = 50;

	//UI描画座標
	static constexpr int TURN_TEXT_X = 350;	//ターン数の表示位置X
	static constexpr int TURN_TEXT_Y = 30;	//ターン数の表示位置Y
	
	static constexpr int ROUTE_TEXT_X = 200;	//ルートの表示位置X
	static constexpr int ROUTE_TEXT_Y = 400;	//ルートの表示位置Y
	
	static constexpr int COMMAND_MENU_X = 200;	//コマンドメニューの表示位置X
	static constexpr int COMMAND_MENU_Y = 200;	//コマンドメニューの表示位置Y
	static constexpr int COMMAND_MENU_STEP_Y = 60;	//コマンドメニューの選択肢間隔Y
	
	static constexpr int PLAYER_IMG_X = 700;	//プレイヤーの画像表示位置X
	static constexpr int PLAYER_IMG_Y = 200;	//プレイヤーの画像表示位置Y


	GameScene(void);		//デフォルトコンストラクタ
	~GameScene(void);		//デストラクタ

	void Init(void) override;		//初期化処理
	void Update(void) override;		//更新処理
	void Draw(void) override;		//描画処理
	void Release(void) override;	//解放処理

	void ChangePhase(QUEST_PHASE phase);	//フェーズの変更

private:

	InputManager& ins_ = InputManager::GetInstance();
	std::unique_ptr<PhaseBase> phase_;
	std::unique_ptr<StoryPhase> storyPhase_;
	std::unique_ptr<BattlePhase> battlePhase_;

	QUEST_PHASE questPhase_;	//クエストのフェーズ
	int turn_;			//現在のターン数

	int playerImg_; //プレイヤーの画像ハンドル
	int stageImg_;  //ステージの画像ハンドル
	int messageBoxImg_; //メッセージボックスの画像ハンドル
};