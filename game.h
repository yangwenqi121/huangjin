//------------------------------------------
//game.h   
//10/27/2019   created
//10/27/2019   modified
//10/27/2019   bu yangwenqi
//------------------------------------------
#include <Windows.h>
#include <memory>
#include "Avatar.h"
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <unordered_map>  //哈希map，数量有限
#include "ItemManager.h"
#include "ScoreManager.h"
//------------------------------------------
#pragma once
#ifndef _GAME_H
#define _GAME_H
//------------------------------------------
namespace game
{
	//------------------------------------------
	enum class  INPUTBUTTON
	{
		IB_Space=0,
	};
	//------------------------------------------
	//------------------------------------------
	/*全局对象 ,单例模式和外观模式*/
	class Game 
	{
	public:
		static Game * getSingleton(void); //构建静态共享类
		static void destroySingleton(void); //销毁
		bool initialize(HWND hWnd); //初始化
		LRESULT processEvent(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam); //事件
		bool frame(void);
		bool checkInput(INPUTBUTTON btn);
		std::shared_ptr<ItemManager>item_mgr(void);
	private:
		Game();
		~Game();
		Game(Game&)=delete;
		Game(Game&&) = delete;
		
	private:
		static Game* _game;//静态成员变量,本质的属于全局变量，作用域在类
		ID2D1Factory *_direct2dFactory = nullptr; /*工厂变量*/
		std::shared_ptr<Avatar>_avatar;  //独立拥有avatar
		ID2D1HwndRenderTarget * _renderTarget = nullptr;
		unsigned long _ticks = 0ul;
		std::shared_ptr<ItemManager>_item_manager;
		std::shared_ptr<ScoreMgr> _score_mgr;
		std::unordered_map<INPUTBUTTON, bool>_inputs;
	};


	//------------------------------------------
}


//------------------------------------------
#endif // !_GAME_H
