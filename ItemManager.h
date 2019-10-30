//-------------------------------------------------
//	ItemManager.h.
//	10/26/2019.		created.	
//	10/26/2019.		modified.
//	by yangwenqi.
//-------------------------------------------------
#pragma once

#ifndef _ITEMMANAGER_H
#define _ITEMMANAGER_H

//-------------------------------------------------
//	headers...
//-------------------------------------------------
#include <d2d1.h>
#include <array>
#include <wincodec.h>

//-------------------------------------------------
namespace game
{
	//-------------------------------------------------
	//	type of content.
	//-------------------------------------------------
	enum class ItemType
	{
		IT_NONE = 0,//什么都没有
		IT_GOLD, //金子
		IT_STONE,//石头
	};

	//-------------------------------------------------
	class ItemManager 
	{
	public:
		ItemManager();
		~ItemManager();
		bool initialize(ID2D1HwndRenderTarget* renderTarget);//初始化
		void render(ID2D1HwndRenderTarget* renderTarget);  //渲染
		void update(ID2D1HwndRenderTarget* renderTarget); //更新
		ID2D1Bitmap* getItemBitmap(ItemType type);  //交互，图片的显示交给Avatar,
		//提供接口给外界，获得哪种类型
		ItemType isInsectWith(int x, int y);    //获取当前的点，看是否碰到什么东西

	private:
		IWICImagingFactory * _pWicImgFactory = nullptr;  
		ID2D1Bitmap*				_gold_bitmap = nullptr;  // 获取gold图片
		std::array<std::array<ItemType, 10>, 3> _items;  // 用数组来装金子，每行10个，有3行
	};

	//-------------------------------------------------
}

//-------------------------------------------------
#endif // !_ITEMMANAGER_H








