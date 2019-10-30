//-------------------------------------------------
//	Avatar.h.
//	10/12/2019.		created.	
//	10/30/2019.		modified.
//	by yangwenqi
//-------------------------------------------------
#pragma once

#ifndef _AVATAR_H
#define _AVATAR_H

//-------------------------------------------------
//	headers...
//-------------------------------------------------
#include <d2d1.h>
#include "ISubject.h"
#include <map>
#include <list>
//-------------------------------------------------
//	incomplete types...
//-------------------------------------------------
struct IWICImagingFactory;

//-------------------------------------------------
namespace game
{
	//-------------------------------------------------
	enum class AvatarState
	{
		AS_Normal = 0,
		AS_Launched
	};
	//-------------------------------------------------
	class Listener_Remove
	{
	public:
		Listener_Remove(game::IListener* listener);
		bool operator()(game::IListener* listener);
	private:
		game::IListener* _listener;

	};
	//-------------------------------------------------
	class Avatar:public ISubject
	{
	public:
		Avatar();
		~Avatar();
		bool initialize(ID2D1HwndRenderTarget* renderTarget);
		void render(ID2D1HwndRenderTarget* renderTarget);
		void update(ID2D1HwndRenderTarget* renderTarget);
		void launch(D2D1_SIZE_F size);
		//-------------------------------------------------
		//inherited from ISubject.
		//-------------------------------------------------
		virtual void add_listener(MessageType mt, IListener* listener)final;
		virtual void remove_listener(MessageType mt, IListener* listener) final;
		virtual void notify(MessageType mt) final;

	private:
		void normal_state(D2D1_SIZE_F size);
		void launched_state(D2D1_SIZE_F size);
	private:
		/* for the format of image converting... */
		IWICImagingFactory * _pWicImgFactory = nullptr;
		ID2D1Bitmap*			_pBitmap = nullptr;
		ID2D1SolidColorBrush*   _pLineBrush = nullptr;
		int                     _avatar_x = 0; 
		/*true ->
		false  <-*/
		bool                   _bOrientation = true;
		float                  _fEndLineX = 0.0f;
		float                  _fEndLineY = 0.0f;
		float                  _fAngle = 0.0f;

		/*true ->
		false  <-*/
		bool                   _bAngleOrientation = true;

		const float				_fLineLength = 150.0f;  //长度
		const float				_MaxExtendLength = 300.0f; //最大伸展多长
		float					_extendLength = 0.0f;   //伸展的长度
		
		AvatarState				_avatar_state = AvatarState::AS_Normal;
		/* true: down
		false: up */
		bool					_bLineOrientation = true;  //伸缩的方向 向下
		bool                    _isComingBack = false;  //是否把东西带回来
		std::map<MessageType,std::list<IListener*>>_listeners;
		bool                    _is_inCrease=true;
	};

	//-------------------------------------------------
}

//-------------------------------------------------
#endif // !_AVATAR_H






